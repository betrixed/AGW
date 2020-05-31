#ifndef _H_GLOBALTEMP
#define _H_GLOBALTEMP

#ifndef _H_AGW_FIRST
#include "agw.h"
#endif // _H_AGW_FIRST

#include "series.hpp"
#include "database.hpp"
#include <wx/thread.h>
#include "plotptr.h"
#include <deque>
#include <unordered_map>
#include <fstream>


class WorkThread;

namespace agw {

    // holder of read-only station data
    class TimeSeries {
    public:
        FloatSeries     temp_;
        DateYearMonth   dates_;

        TimeSeries();

        TimeSeries(const TimeSeries& c);
        const TimeSeries& operator=(const TimeSeries&);

        bool getMonthAverage(int yearStart, int yearEnd, std::vector<float>& monthAvg);
        // inclusive year numbers
        bool asYearAnomaly(const std::vector<float>& monthAvg, FloatSeries& rTemp, FloatSeries& rYear);
        // Don't average the months into year anomaly
        bool asMonthAnomaly(const std::vector<float>& monthAvg,  FloatSeries& rTemp, DateYearMonth& rYear);

                // initialize from query
        void initFromLocation(Database& db, const std::string& stationid, int yearStart = -1, int yearEnd = -1);

        void add(int year, int month, double value);

        bool averageDiff(const TimeSeries& mdata, int& commonYears, double& diff);
        void makeMonthAnomalyPlot(const std::vector<float>& monthAvg, const std::string& path );
        void makeYearAnomalyPlot(const std::vector<float>& monthAvg, const std::string& path );


    };

    typedef std::shared_ptr<TimeSeries> TSPtr;

    class MergeSeries {
    public:
        FloatSeries     weight_;
        TSPtr           cp_;
        unsigned int    mergeCount_; // how many merged here?

        // build initial array point by point


        // merge subsequent
        bool mergeDiff(const TimeSeries& mdata, const std::vector<float>& fw, double diff);

        // difference between this - that in average difference, common years


        // assumes other datapoints already exist
        void setWeight(double nWeight);


        void autoMerge(const MergeSeries& m);

        MergeSeries(const MergeSeries& ms);
        const MergeSeries& operator=(const MergeSeries& ms);
        MergeSeries();



    };
    typedef std::shared_ptr<MergeSeries> TrendPtr;



    // Holder of cached station data
    class MergeSite {
    public:
        std::string stationid;
        int         yearsCt_;
        TSPtr    data_;
        double      long_;
        double      lat_; // cache location of this
        MergeSite(const std::string& id);

    };
    typedef std::shared_ptr<MergeSite> SitePtr;

    class FoundSite {
    public:
        SitePtr site_;
        double  distance_;

        FoundSite() : distance_(0.0)
        {
        }

        FoundSite(SitePtr s, double d) : site_(s), distance_(d)
        {
        }
    };

    typedef std::unique_ptr<FoundSite> FoundPtr;

    class BigBox {
    public:
        int ix_;
        int rowix_;
        int sign_;
        double width_;
        double east_;
        double west_;

        std::vector<TrendPtr> sublist_;
        TrendPtr  data_; // area level merge
        BigBox(int index);
    };

    typedef std::shared_ptr<BigBox> AreaPtr;

    // basic regression selection using xvalue limits for ydata on xdata
    //PlotLayer_sptr regressRange(const Series& srcX, const Series& srcY, double xMin, double xMax);

    // jobs are an area to do
    // so list of jobs is a list of BigBox AreaPtr

    class GlobalTempArea;
    typedef std::shared_ptr<GlobalTempArea> GTAPtr;

    class AreaThread : public wxThread {
    public:
        AreaThread(GTAPtr& gta);
        virtual ExitCode Entry() wxOVERRIDE;
    protected:
        GTAPtr          gta_;
        Database        db_;

    };

    class GlobalTempArea {
        wxCriticalSection       critQ_;
        std::deque<AreaPtr>     jobList_;    // to be processed
        std::vector<AreaPtr>    areaList_;   // all of them
        std::unordered_map<std::string,SitePtr> site_cache_;
        TrendPtr                h_south;
        TrendPtr                h_north;
        TrendPtr                globe;
        unsigned int            doneCount_;
        SeriesPtr               gridLat_;
        double                  correlate_radius_;
        Database                db_;
        int                     minYears_;
        WorkThread*             worker_;

    public:
        int                     startYear_; // anomaly base line
        int                     endYear_;
        bool                    compensateBase_; // adjust as if 1951-1980 baseline

        enum {
            default_radiuskm = 1200,
            default_StartYear = 1951,
            default_minYears = 20,
        };
        GlobalTempArea(WorkThread* myworker);

        void openDB(const std::string& path);

        bool getSite(const std::string& site, SitePtr& setMe);
        bool getArea(AreaPtr& doArea);
        void fillCache();

        // thread-safe?
        double getBoxCenter(unsigned int bix);
        const std::string& getDBPath() const { return db_.path(); }
        double radius() const { return correlate_radius_; }
        int startYear() const { return startYear_; }
        int minYears() const { return minYears_; }
        void areaDone(int areaIndex);
        bool finishedAreas();
        void bigMerge();
    };



}; // namespace agw

#endif // _H_GLOBALTEMP
