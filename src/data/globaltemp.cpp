#include "globaltemp.h"
#include "appdata.h"
#include "database.hpp"
#include <wx/log.h>
#include <cmath>
#include <algorithm>
#include "threadwork.hpp"
#include <wx/filename.h>

#include "plotlua.h"
#include "helper.h"
using namespace agw;

const double kmPerDegree = 111.3199;

double geo_distance(double lat1, double lat2, double long1, double long2)
{
    double degToRadians = M_PI / 180.0;

    lat1 = (90.0 - lat1) * degToRadians;
    lat2 = (90.0 - lat2) * degToRadians;
    double longdiff = (long1 - long2) * degToRadians;

    return std::acos( std::cos(lat1) * std::cos(lat2) + std::sin(lat1) * std::sin(lat2) * std::cos(longdiff)) * 6371;
}

TimeSeries::TimeSeries()
{
}

TimeSeries::TimeSeries(const TimeSeries& c)
{
    temp_ = c.temp_;
    dates_ = c.dates_;
}

const TimeSeries& TimeSeries::operator=(const TimeSeries& c)
{
    temp_ = c.temp_;
    dates_ = c.dates_;
    return *this;
}

void MergeSeries::autoMerge(const MergeSeries& m)
{
    int common;
    double diff;
    m.cp_->averageDiff(*(cp_), common, diff);
    mergeDiff(*(m.cp_), m.weight_.data(), diff);
}

MergeSeries::MergeSeries() : mergeCount_(0)
{
}

// copy
MergeSeries::MergeSeries(const MergeSeries& ms)
{
    this->MergeSeries::operator=(ms);
}

// copy
const MergeSeries&
MergeSeries::operator=(const MergeSeries& ms)
{
    cp_ = std::make_shared<TimeSeries>(*(ms.cp_.get()));
    weight_ = ms.weight_;
    mergeCount_ = ms.mergeCount_;
    return *this;
}
// month is zero-based
void TimeSeries::add(int year, int month, double value)
{
    int m1,y1, m2, y2;

    if (dates_.size() > 0)
    {
        dates_.getStartDate(m1,y1);
        dates_.getEndDate(m2,y2);
        int ix1 = y1*12+m1; // corresponds to 0 index
        int ix2 = y2*12+m2; // corresponds to n-1 index
        int newix = year*12+month;
        if (newix < ix1)
        {
            // awkward : ix1 - newix points to be prepended to data,
            dates_.setStartDate(month,year);
            temp_.prepend(nanFloat,ix1-newix);
        }
        else if (newix > ix2)
        {
            // easier : newix - ix2 points to be appended to data
            dates_.setEndDate(month,year);
            temp_.append(nanFloat, newix - ix2);
        }
        // insert index is newix - ix1
        const unsigned int index1 = newix - ix1;
        if(index1 >= temp_.size())
            raise(SIGABRT);
        temp_.set(index1, value);
    }
    else {
        // very first value only
        temp_.append(value);
        dates_.setStartDate(month,year);
        dates_.setEndDate(month,year);
    }
}

bool TimeSeries::getMonthAverage(int yearStart, int yearEnd, std::vector<float>& monthAvg)
{
    monthAvg.assign(12,nanFloat);
    std::vector<unsigned int> monthCount(12,0);
    int nMonth, nYear;
    dates_.getStartDate(nMonth, nYear);

    const unsigned int nOffset = nYear*12 + nMonth;
    const unsigned int nSize = temp_.size();

    for(int year = yearStart; year <= yearEnd; year++)
    {
        for (int month = 0; month < 12; month++)
        {
            unsigned int ix = year*12 + month;
            if (ix >= nOffset)
            {
                unsigned int pos = ix - nOffset;
                if (pos < nSize)
                {
                    double monthTemp = temp_[pos];
                    if (!std::isnan(monthTemp))
                    {
                        if (monthCount[month] == 0)
                        {
                            monthAvg[month] = monthTemp;

                        }
                        else {
                            monthAvg[month] +=  monthTemp;
                        }
                        monthCount[month] += 1;
                    }
                }
            }
        }
    }
    for (int month = 0; month < 12; month++)
    {
        auto ct = monthCount[month];
        if (ct == 0)
        {
            // really need all to work
            return false;
        }

        monthAvg[month] /= ct; // for later subtraction
    }
    return true;
}

bool TimeSeries::asYearAnomaly(const std::vector<float>& monthAvg, FloatSeries& rTemp, FloatSeries& rYear)
{
    int nMonth, nYear;
    dates_.getStartDate(nMonth, nYear);

    const unsigned int nOffset = nYear*12 + nMonth;
    int zMonth, zYear;
    dates_.getEndDate(zMonth, zYear);
    const unsigned int zOffset = zYear*12 + zMonth;

    int currentYear = nYear;
    double yearSum = 0.0;
    double monthCt = 0.0;
    rYear.clear();
    rTemp.clear();

    for(unsigned int ix = nOffset; ix <= zOffset; ix++)
    {
        int year = int(ix / 12);
        unsigned int tix = (ix - nOffset);
        if (year != currentYear)
        {
            rYear.append(currentYear);
            rTemp.append(yearSum / monthCt);
            yearSum = 0.0;
            monthCt = 0.0;
            currentYear = year;
        }
        double value = temp_[tix];
        if (!std::isnan(value))
        {
            int monthIX = ix % 12;
            yearSum += (value - monthAvg[monthIX]);
            monthCt += 1.0;
        }
    }
    if (monthCt > 0.0)
    {
        rYear.append(currentYear);
        rTemp.append(yearSum / monthCt);
    }
    return (rYear.size() > 0);
    // now only the monthly anomaly temperatures are left, but a yearly anomaly (average) is required
    // compress each years anomaly as an average of the available months, into the results array.


}


bool TimeSeries::asMonthAnomaly(const std::vector<float>& monthAvg, FloatSeries& rTemp, DateYearMonth& rDates)
{
    int nMonth, nYear;
    dates_.getStartDate(nMonth, nYear);
    const unsigned int nOffset = nYear*12 + nMonth;

    int zMonth, zYear;
    dates_.getEndDate(zMonth, zYear);
    const unsigned int zOffset = zYear*12 + zMonth;

    rDates.setStartDate(nOffset%12, nOffset/12);
    rDates.setEndDate(zOffset%12, zOffset/12);
    rTemp.assign(dates_.size(), nanFloat);

    for(unsigned int ix = nOffset; ix <= zOffset; ix++)
    {
        unsigned int tix = (ix - nOffset);
        double value = temp_[tix];
        if (!std::isnan(value))
        {
            int monthIX = ix % 12;
            rTemp.set(tix,value - monthAvg[monthIX]);
        }
    }
    return (rDates.size() > 0);


}

void MergeSeries::setWeight(double nWeight)
{
    TimeSeries& cs = *(cp_.get());
    auto n = cs.temp_.size();
    if (n > 0)
        weight_.assign(n, nWeight);
}
// year range inclusive if set
void TimeSeries::initFromLocation(Database& db, DBRowId locId,
                    int yearStart, int yearEnd)
{

   std::stringstream yss;

   yss << "select Y.Year, T.MonthId, T.Value"
        " from gisstemp T, gissyear Y"
        " where Y.codeId = T.codeId"
        " and Y.locId = ?"
        " and Y.measure = 'TAVG'";

    if (yearStart >= 0)
    {
        yss << " and Y.year >= ?";
    }
    if (yearEnd >= 0)
    {
        yss << " and Y.year <= ?";
    }
    yss << " order by Year, MonthId";

    Statement td(db, yss.str());

    td.bindRowId(locId,1);
    if (yearStart >= 0)
    {
        td.bind(long(yearStart),2);
    }
    if (yearEnd >= 0)
    {
        td.bind(long(yearEnd),3);
    }

    while(td.next())
    {
        int year = td.getInt32(0);
        int month = td.getInt32(1);
        double value = td.getDouble(2);

        // insert into merge data, padding with nan, or expanding date range as necessary
        // database month starts with 1, enum month starts with 0
        this->add(year,month-1, value);
    }
}
// combine each temperature series using balance of weighting
bool MergeSeries::mergeDiff(const TimeSeries& mdata, const std::vector<float>& fw, double diff)
{
    int nMonth, nYear;
    int dateStartIX2, dateStartIX1;
    int dateEndIX2, dateEndIX1;
    TimeSeries& cs = *(cp_.get());

    cs.dates_.getStartDate(nMonth, nYear);
    dateStartIX1 = nYear*12 + nMonth;

    mdata.dates_.getStartDate(nMonth, nYear);
    dateStartIX2 = nYear*12 + nMonth;


    if (dateStartIX2 < dateStartIX1)
    {// prepending mdata into this data.  If mdata starts early, prepend  so both start at same month.
        unsigned int n = dateStartIX1 - dateStartIX2;
        cs.temp_.prepend(nanFloat,n);
        weight_.prepend(nanFloat,n);
        cs.dates_.setStartDate(nMonth,nYear);
        dateStartIX1 = dateStartIX2;
    }

    // merging data

    cs.dates_.getEndDate(nMonth, nYear);
    dateEndIX1 = nYear*12 + nMonth;

    mdata.dates_.getEndDate(nMonth, nYear);
    dateEndIX2 = nYear*12 + nMonth;

    if (dateEndIX2 > dateEndIX1)
    {// appending mdata into this data, extend this array
        unsigned int n = dateEndIX2-dateEndIX1;
        cs.temp_.append(nanFloat,n);
        weight_.append(nanFloat,n);
        cs.dates_.setEndDate(nMonth,nYear);
        dateEndIX1 = dateEndIX2;
    }

    // now mdata fits somewhere within this data range, maybe all of it.
    // To merge , have to take all points from mdata, and adjust corresponding points of this

    const unsigned int commonStart = (dateStartIX2 > dateStartIX1) ? dateStartIX2 : dateStartIX1;
    const unsigned int commonEnd = (dateEndIX2 < dateEndIX1) ? dateEndIX2 : dateEndIX1;

    if (commonEnd < commonStart)
    {
        return false;
    }
    mergeCount_++;

    const unsigned int temp_size1 = cs.temp_.size();
    const unsigned int temp_size2 = mdata.temp_.size();
    if( commonStart - dateStartIX1 >= temp_size1 || commonEnd - dateStartIX1 >= temp_size1)
        raise(SIGABRT);
    if( commonStart - dateStartIX2 >= temp_size2 || commonEnd - dateStartIX2 >= temp_size2)
        raise(SIGABRT);
    for(unsigned int cix = commonStart; cix <= commonEnd; cix++)
    {
        const unsigned int index1 = cix - dateStartIX1; // temp_ indexes are from 0
        const unsigned int index2 = cix - dateStartIX2;

        double v_1 = cs.temp_[index1];
        double v_2 = mdata.temp_[index2];

        if (!std::isnan(v_2)) // can only merge real data
        {
            double w2 = fw[index2];

            if (std::isnan(v_1)) // first value for this month, has no wieght either
            {
                v_1 = ( v_2 - diff);
                weight_.set(index1, w2);
            }
            else
            {   // two values to arbitrate
                double w1 = weight_[index1];
                double wsum = w1 + w2;
                v_1 = (w1*v_1 + w2 * (v_2 - diff)) / wsum;
                weight_.set(index1, wsum);
            }
            cs.temp_.set(index1, v_1);
        }
    }
    return true;
}
// compare for overlap in date extremes, average common difference
bool TimeSeries::averageDiff(const TimeSeries& mdata, int& commonYears, double& diff)
{
    int nMonth, nYear;
    int dateStartIX2, dateStartIX1;
    int dateEndIX2, dateEndIX1;

    dates_.getStartDate(nMonth, nYear);
    dateStartIX1 = nYear*12 + nMonth;

    mdata.dates_.getStartDate(nMonth, nYear);
    dateStartIX2 = nYear*12 + nMonth;

    const unsigned int commonStart = (dateStartIX2 > dateStartIX1) ? dateStartIX2 : dateStartIX1;

    dates_.getEndDate(nMonth, nYear);
    dateEndIX1 = nYear*12 + nMonth;

    mdata.dates_.getEndDate(nMonth, nYear);
    dateEndIX2 = nYear*12 + nMonth;

    const unsigned int commonEnd = (dateEndIX2 > dateEndIX1) ? dateEndIX1 : dateEndIX2;

    if (commonEnd < commonStart)
    {
        commonYears = 0;
        return false;
    }
    const unsigned int temp_size1 = temp_.size();
    const unsigned int temp_size2 = mdata.temp_.size();
    if( commonStart - dateStartIX1 >= temp_size1 || commonEnd - dateStartIX1 >= temp_size1)
        raise(SIGABRT);
    if( commonStart - dateStartIX2 >= temp_size2 || commonEnd - dateStartIX2 >= temp_size2)
        raise(SIGABRT);

    double vsum = 0.0;
    int    vcount = 0;

    for(unsigned int cix = commonStart; cix <= commonEnd; cix++)
    {
        const unsigned int index1 = cix - dateStartIX1;
        const unsigned int index2 = cix - dateStartIX2;

        double v_1 = temp_[index1];
        double v_2 = mdata.temp_[index2];
        if (!std::isnan(v_1) && !std::isnan(v_2))
        {
            vsum += (v_1 - v_2);
            vcount++;
        }
    }
    if (vcount > 0)
    {
        commonYears = vcount/12;
        diff = vsum / vcount;
        return true;
    }
    return false;
}

bool getSite(DBRowId site, SitePtr& setMe);

MergeSite::MergeSite(DBRowId id) : codeId_(id), yearsCt_(0)
{
}



bool compareYears(FoundPtr const& s1, FoundPtr const&  s2)
{
    return (s1->site_->yearsCt_ < s2->site_->yearsCt_);
}

std::string stationQuery(int startYear, int minYears)
{
    std::stringstream ss;
    ss << "select count(Y.year) as totalYears, Y.locId, S.Longitude, S.Latitude"
               " from gissyear Y join gissloc S on S.codeId = Y.locId"
               " where Y.measure = 'TAVG'"
           << " and Y.year >= " << startYear
           << " group by Y.locId, S.Latitude, S.Longitude"
           << " having totalYears >=  " <<  minYears
           << " order by totalYears desc";
    return ss.str();
}
std::string radiusQuery(double clong, double clat, double cradius)
{
    std::stringstream ss;
    ss << "select distinct A.codeID,"
          " ST_Distance(A.Geometry, MakePoint("
          << clong
          << ","
          << clat
          << ", 4326)) as SDistance"
            " from gissloc A where SDistance < "
          << cradius;

          return ss.str();
}

void TimeSeries::makeYearAnomalyPlot(const std::vector<float>& monthAvg, const std::string& path)
{
    SeriesPtr ydata = std::make_shared<FloatSeries>();
    SeriesPtr xdata = std::make_shared<FloatSeries>();

    FloatSeries& rTemp = (FloatSeries&)*(ydata.get());
    FloatSeries& rYear = (FloatSeries&)*(xdata.get());
    this->asYearAnomaly(monthAvg, rTemp, rYear);

    PlotLuaPtr plot = std::make_shared<PlotLua>();
    plot->addLayer(xdata, ydata);

    PlotPtr fit = regressRange(*xdata,*ydata,1975.0, nanDouble);
    plot->addLayer(fit);
    std::string apath = path + "_anom";

    wxFileName fn;
    fn.Assign(apath);
    fn.SetExt("plot");
    plot->saveToFile(fn.GetFullPath().ToStdString());
}

void TimeSeries::makeMonthAnomalyPlot(const std::vector<float>& monthAvg, const std::string& path)
{
    SeriesPtr ydata = std::make_shared<FloatSeries>();
    SeriesPtr xdata = std::make_shared<DateYearMonth>();
    FloatSeries& rTemp = (FloatSeries&)*(ydata.get());
    DateYearMonth& rDates = (DateYearMonth&)*(xdata.get());

    this->asMonthAnomaly(monthAvg, rTemp, rDates);
    PlotLuaPtr plot = std::make_shared<PlotLua>();
    plot->addLayer(xdata, ydata);

    //PlotPtr fit = regressRange(*xdata,*ydata, 1975.0, nanDouble);
    //plot->addLayer(fit);
    std::string apath = path + "_month";

    wxFileName fn;
    fn.Assign(apath);
    fn.SetExt("plot");
    plot->saveToFile(fn.GetFullPath().ToStdString());
}

void TempTrendThread::doGlobalTempTrend(Database& db, const std::string& path)
{
    std::vector<std::unique_ptr<FoundSite>>   localList;
    std::vector<AreaPtr>   areaList;
    std::vector<TSPtr>  merge_1;
    std::vector<TrendPtr>  merge_band;
    TrendPtr  amp;
    TrendPtr  h_south;
    TrendPtr  h_north;
    TrendPtr  globe;
    PlotLuaPtr plot;
    SeriesPtr  xdata;
    SeriesPtr  ydata;
    wxFileName fn;
    AreaPtr    boxp;
    SeriesPtr gridlat = AppData::instance().getGlobal("grid80");
    Series*   gridp = gridlat.get();
    std::vector<float> weights;

    enum {
        default_StartYear = 1880,
        default_minYears = 20,
    };

    const double default_radius = 1200.0;
    const double kmPerDegree = 111.3199;

    double correlate_radius = default_radius/kmPerDegree;
    int startYear = default_StartYear;
    int minYears = default_minYears;

    int yearsOverlap = 0;
    double diff = 0.0;

    //int countFound = 0;


    areaList.resize(80);

    // build a cache of stations where number of years is >= minYears

    std::unordered_map<DBRowId,SitePtr> site_cache;
    {
        wxLogMessage("Caching station data . . .");

        std::string qsql = stationQuery(startYear, minYears);
        Statement ystm(db, qsql);
        int rowct = 0;
        while(ystm.next())
        {
            int yearsCt = ystm.getInt32(0);
            DBRowId locId = ystm.getRowId(1);

            SitePtr site = std::make_shared<MergeSite>(locId);
            site->yearsCt_ = yearsCt;
            site->long_ = ystm.getDouble(2);
            site->lat_ = ystm.getDouble(3);

            TSPtr tp = std::make_shared<TimeSeries>();

            tp->initFromLocation(db, locId, startYear);

            site->data_ = tp;

            site_cache.insert(std::pair<DBRowId, SitePtr>(locId,site));
            rowct++;
            //this->UpdateProgress( int(100.0 * rowct / 10000.0));
        }
    }
    for(int bb = 0; bb < 80; bb++)
    {
        areaList[bb] = std::make_shared<BigBox>(bb+1);
    }
    try {
        wxLogMessage("Processing sub-boxes . . . ");
        for(int bb = 0; bb < 80; bb++)
        {
            boxp = areaList[bb];
            this->UpdateProgress( int(100.0 * bb / 80.0));
            wxLogMessage("Area %d", bb+1);
            BigBox* box = boxp.get();

            //int cacheHits = 0;

            for(int row = 0; row < 10; row++)
            {
                int ix = (box->rowix_ - 1) * 20 + row*2;
                double center_lat = gridp->operator[](ix)* box->sign_;

                for(int col = 0; col < 10; col++)
                {
                    TrendPtr mdatap = box->sublist_[col*10+row];

                    double mid_long = box->east_ + box->width_ * 3.0/20.0 * col;

                    std::string radQuerySql = radiusQuery(mid_long, center_lat,correlate_radius );

                    Statement stm(db, radQuerySql);
                    int rowcount = 0;
                    localList.clear();

                    while(stm.next())
                    {
                        DBRowId id = stm.getRowId(0);

                        auto ait = site_cache.find(id);
                        if (ait != site_cache.end())
                        {
                            SitePtr sp_1 = ait->second;
                            localList.emplace_back(new FoundSite(sp_1, stm.getDouble(1) * kmPerDegree));
                        }
                        rowcount++;
                    }
                    unsigned int localix = 0;
                    unsigned int nCount = localList.size();
                    if (nCount > 1)
                    {
                        std::sort(localList.begin(), localList.end(), compareYears);
                    }

                    if (localix < nCount)
                    {
                        FoundSite* fs = localList[localix].get();
                        localix++;
                        double weight = (correlate_radius - fs->distance_) / correlate_radius;

                        mdatap->cp_ = std::make_shared<TimeSeries>(*(fs->site_->data_));
                        mdatap->setWeight(weight);
                        mdatap->mergeCount_ = 1;
                    }
                    while (localix < nCount)
                    {
                        FoundSite* fs = localList[localix].get();
                        localix++;

                        double nWeight = (correlate_radius- fs->distance_) / correlate_radius;
                        TSPtr cp = fs->site_->data_;

                        unsigned int wsize = cp->dates_.size();

                        if (wsize == 0)
                            continue;

                        TimeSeries& mergeCS = *(mdatap->cp_);
                        if (cp->averageDiff(mergeCS, yearsOverlap, diff) && yearsOverlap > 20)
                        {
                            weights.assign(wsize, nWeight);
                            mdatap->mergeDiff(*cp, weights, diff);
                        }
                    }
                    //wxLogMessage("Merged trend: (%g,%g) Months %lu",mid_long,center_lat, mdata.dates_.size());
                    if (mdatap->mergeCount_ > 0)
                    {
                        merge_1.push_back(mdatap->cp_); // ready for full merge
                    }
                }
            }
            // go to combine all the sub-boxes into one area temp merge.
            amp = std::make_shared<MergeSeries>();
            boxp->data_ = amp;

            for(unsigned int mbox = 0; mbox < merge_1.size(); mbox++)
            {
                TSPtr tp3 = merge_1[mbox];

                if (mbox == 0) // direct copy
                {
                    amp->cp_ = std::make_shared<TimeSeries>(*tp3);
                    amp->setWeight(1.0);
                    amp->mergeCount_ = 1;
                }
                else {
                    TimeSeries& cmp = *(amp->cp_);
                    tp3->averageDiff(cmp, yearsOverlap, diff);
                    weights.assign(tp3->dates_.size(),1.0);
                    amp->mergeDiff(*tp3, weights, diff);
                }
            }
        }
        // Merge individual big boxes across latitude bands, weighted by number of sub-merges
        // There are 8 bands, so
        for(int band = 0; band < 8; band++)
        {
            merge_band.push_back(std::make_shared<MergeSeries>());
        }
        for(int bb = 0; bb < 80; bb++)
        {
            AreaPtr boxp = areaList[bb];
            // zero index into merge_band, 0-3, 4-7
            int bandix = (boxp->sign_ == 1) ? boxp->rowix_-1 : 8 - boxp->rowix_;

            TrendPtr plat = merge_band[bandix];
            TrendPtr dp = boxp->data_;

            if (plat->mergeCount_ == 0)
            {
                *(plat) = *dp;
                plat->mergeCount_ = 1;
                plat->setWeight(dp->mergeCount_/100.0);
            }
            else {
                dp->setWeight(dp->mergeCount_/100.0);
                plat->autoMerge(*dp);
            }
        }
        // keep the weights merge hemispheres.
        h_north = std::make_shared<MergeSeries>();

        for(int bb = 3; bb >= 0; bb--)
        {
            TrendPtr plat = merge_band[bb];
            if (h_north->mergeCount_ == 0)
            {
                *(h_north) = *plat;
                h_north->mergeCount_ = 1;
            }
            else {
                h_north->autoMerge(*plat);
            }
        }
        h_south = std::make_shared<MergeSeries>();
        for(int bb = 4; bb <= 7; bb++)
        {
            TrendPtr plat = merge_band[bb];
            if (h_south->mergeCount_ == 0)
            {
                *(h_south) = *plat;
                h_south->mergeCount_ = 1;
            }
            else {
                h_south->autoMerge(*plat);
            }
        }
        globe = std::make_shared<MergeSeries>();
        (*globe) = *h_north;
        globe->mergeCount_ = 1;
        globe->autoMerge(*h_south);

        plot = std::make_shared<PlotLua>();
        TimeSeries& gc = *(globe->cp_);
        ydata = std::make_shared<FloatSeries>(gc.temp_);
        xdata = std::make_shared<DateYearMonth>(gc.dates_);

        plot->addLayer(xdata, ydata);
        fn.Assign(path);
        fn.SetExt("plot");
        plot->saveToFile(fn.GetFullPath().ToStdString());

        std::vector<float> monthAvg;

        if (gc.getMonthAverage(1951, 1980,monthAvg))
        {
            gc.makeMonthAnomalyPlot(monthAvg,path);
            gc.makeYearAnomalyPlot(monthAvg,path);
        }

    }
    catch (DBException& x)
    {
        wxLogMessage("%s", x.what());
    }
}

BigBox::BigBox(int index) : rowix_(0), sign_(0)
{
    ix_ = index;
    if (ix_ <= 40)
    {
        if (ix_ > 24)
        {
            rowix_ = 4;
            sign_ = 1;
            width_ = 360.0 / 16.0;
            east_ = -180.0 + (ix_-25)*width_;
            west_ = -180.0 + (ix_-24)*width_;
        }
        else if (ix_ > 12)
        {
            rowix_ = 3;
            sign_ = 1;
            width_ = 360.0 / 12.0;
            east_ = -180.0 + (ix_-13)*width_;
            west_ = -180.0 + (ix_-12)*width_;
        }
        else if (ix_ > 4)
        {
            rowix_ = 2;
            sign_ = 1;
            width_ = 360.0 / 8.0;
            east_ = -180.0 + (ix_- 5)*width_;
            west_ = -180.0 + (ix_- 4)*width_;
        }
        else if (ix_ > 0) {
            rowix_ = 1;
            sign_ = 1;
            width_ = 360.0 / 4.0;
            east_ = -180.0 + (ix_- 1)*width_;
            west_ = -180.0 + ix_*width_;
        }
    }
    else {
        if (ix_ <= 56)
        {
            rowix_ = 4;
            sign_ = -1;
            width_ = 360.0 / 16.0;
            east_ = -180.0 + (ix_-41)*width_;
            west_ = -180.0 + (ix_-40)*width_;
        }
        else if (ix_ <= 68)
        {
            rowix_ = 3;
            sign_ = -1;
            width_ = 360.0 / 12.0;
            east_ = -180.0 + (ix_-57)*width_;
            west_ = -180.0 + (ix_-56)*width_;
        }
        else if (ix_ <= 76)
        {
            rowix_ = 2;
            sign_ = -1;
            width_ = 360.0 / 8.0;
            east_ = -180.0 + (ix_-69)*width_;
            west_ = -180.0 + (ix_-68)*width_;
        }
        else if (ix_ <= 80)
        {
            rowix_ = 1;
            sign_ = -1;
            width_ = 360.0 / 4.0;
            east_ = -180.0 + (ix_-77)*width_;
            west_ = -180.0 + (ix_-76)*width_;
        }
    }
    if (rowix_ > 0)
    {
        sublist_.resize(100);
        for(unsigned int i = 0; i < 100; i++)
        {
            sublist_[i] = std::make_shared<MergeSeries>();
        }
    }
}

PlotPtr agw::regressRange( Series const& srcX,  Series const& srcY, double xMin, double xMax)
{
    PlotPtr fit = std::make_shared<LineFit>();

    LineFit* line = static_cast<LineFit*>(fit.get());

    line->label_ = "Line Fit";
    line->lineStyle_ = penStyleToIndex(wxPENSTYLE_SOLID);
    line->lineWidth_ = 3;

        // create temporary filtered series
    DoubleSeries lineX, lineY;
    for(unsigned int srcIX = 0; srcIX < srcX.size(); srcIX++)
    {
        double xval = srcX[srcIX];

        if (std::isnan(xval))
            continue;
        if (xval <  xMin)
            continue;
        double  yval = srcY[srcIX];
        if (std::isnan(yval))
            continue;
        if (!std::isnan(xMax))
            if (xval > xMax)
                continue;

        lineX.append(xval);
        lineY.append(yval);
    }
        // need at least 3 points for meaningful regression
    if (lineX.size() > 2)
    {
        line->regression(lineX, lineY);
    }
    line->legendText_ = line->resultAsText();
    return fit;
}

wxThread::ExitCode  TempTrendThread::Entry()
{
    wxLogMessage("Thread Entry");
    Database sdb;
    sdb.path(dbPath_);
    sdb.open();
    Database::AutoClose dbscope(sdb);
    doGlobalTempTrend(sdb, filePath_);
    this->UpdateProgress( 100);
    UpdateProgress(-1);
    return (wxThread::ExitCode)0;
}



AreaThread::AreaThread(GlobalTempArea* gta) : gta_(gta)
{
}

bool GlobalTempArea::finishedAreas()
{
    wxCriticalSectionLocker lock(critQ_);

    return (doneCount_ == areaList_.size()) ? true : false;
}

void GlobalTempArea::areaDone(int areaIndex)
{
    wxCriticalSectionLocker lock(critQ_);

    doneCount_++;
}

bool GlobalTempArea::getArea(AreaPtr& doArea)
{
    wxCriticalSectionLocker lock(critQ_);

    if (jobList_.size() > 0)
    {
        doArea = jobList_.front();
        jobList_.pop_front();
        return true;
    }
    return false;
}

GlobalTempArea::GlobalTempArea()
{
    startYear_ = default_StartYear;
    minYears_ = default_minYears;
    correlate_radius_ = default_radiuskm/kmPerDegree;
    gridLat_ = AppData::instance().getGlobal("grid80");
}

bool GlobalTempArea::getSite(DBRowId site, SitePtr& setMe)
{
    auto zit = site_cache_.find(site);
    if (zit != site_cache_.end())
    {
        setMe = zit->second;
        return true;
    }
    return false;
}


wxThread::ExitCode
AreaThread::Entry()
{
    std::vector<std::unique_ptr<FoundSite>>   localList;
    std::vector<float> weights;
    std::vector<TSPtr>  merge_1;

    AreaPtr boxp;

    db_.path(gta_->getDBPath());
    db_.open();

    Database::AutoClose closeMe(db_);

    double correlate_radius = gta_->radius();
    int yearsOverlap;
    double diff;

    while(gta_->getArea(boxp))
    {
        BigBox* box = boxp.get();

        //int cacheHits = 0;
        for(int row = 0; row < 10; row++)
        {
            int ix = (box->rowix_ - 1) * 20 + row*2;
            double center_lat = gta_->getBoxCenter(ix) * box->sign_;

            for(int col = 0; col < 10; col++)
            {
                TrendPtr mdatap = box->sublist_[col*10+row];

                double mid_long = box->east_ + box->width_ * 3.0/20.0 * col;

                std::string radQuerySql = radiusQuery(mid_long, center_lat, correlate_radius );

                Statement stm(db_, radQuerySql);
                int rowcount = 0;
                localList.clear();

                while(stm.next())
                {
                    DBRowId id = stm.getRowId(0);
                    SitePtr  sp_1;
                    if (gta_->getSite(id, sp_1))
                    {
                         localList.emplace_back(new FoundSite(sp_1, stm.getDouble(1) * kmPerDegree));
                    }
                    rowcount++;
                }
                unsigned int localix = 0;
                unsigned int nCount = localList.size();
                if (nCount > 1)
                {
                    std::sort(localList.begin(), localList.end(), compareYears);
                }

                if (localix < nCount)
                {
                    FoundSite* fs = localList[localix].get();
                    localix++;
                    double weight = (correlate_radius - fs->distance_) / correlate_radius;

                    mdatap->cp_ = std::make_shared<TimeSeries>(*(fs->site_->data_));
                    mdatap->setWeight(weight);
                    mdatap->mergeCount_ = 1;
                }
                while (localix < nCount)
                {
                    FoundSite* fs = localList[localix].get();
                    localix++;

                    double nWeight = (correlate_radius- fs->distance_) / correlate_radius;
                    TSPtr cp = fs->site_->data_;

                    unsigned int wsize = cp->dates_.size();

                    if (wsize == 0)
                        continue;

                    TimeSeries& mergeCS = *(mdatap->cp_);
                    if (cp->averageDiff(mergeCS, yearsOverlap, diff) && yearsOverlap > 20)
                    {
                        weights.assign(wsize, nWeight);
                        mdatap->mergeDiff(*cp, weights, diff);
                    }
                }
                //wxLogMessage("Merged trend: (%g,%g) Months %lu",mid_long,center_lat, mdata.dates_.size());
                if (mdatap->mergeCount_ > 0)
                {
                    merge_1.push_back(mdatap->cp_); // ready for full merge
                }
            }
        }
        // go to combine all the sub-boxes into one area temp merge.
        TrendPtr amp = std::make_shared<MergeSeries>();
        boxp->data_ = amp;

        for(unsigned int mbox = 0; mbox < merge_1.size(); mbox++)
        {
            TSPtr tp3 = merge_1[mbox];

            if (mbox == 0) // direct copy
            {
                amp->cp_ = std::make_shared<TimeSeries>(*tp3);
                amp->setWeight(1.0);
                amp->mergeCount_ = 1;
            }
            else {
                TimeSeries& cmp = *(amp->cp_);
                tp3->averageDiff(cmp, yearsOverlap, diff);
                weights.assign(tp3->dates_.size(),1.0);
                amp->mergeDiff(*tp3, weights, diff);
            }
        }
    }
    gta_->areaDone(boxp->ix_);
    return wxThread::ExitCode(0);
}

void GlobalTempArea::fillCache()
{
    wxLogMessage("Caching station data . . .");

    std::string qsql = stationQuery(startYear_, minYears_);
    Statement ystm(db_, qsql);
    int rowct = 0;
    while(ystm.next())
    {
        int yearsCt = ystm.getInt32(0);
        DBRowId locId = ystm.getRowId(1);

        SitePtr site = std::make_shared<MergeSite>(locId);
        site->yearsCt_ = yearsCt;
        site->long_ = ystm.getDouble(2);
        site->lat_ = ystm.getDouble(3);

        TSPtr tp = std::make_shared<TimeSeries>();

        tp->initFromLocation(db_, locId, startYear_);

        site->data_ = tp;

        site_cache_.insert(std::pair<DBRowId, SitePtr>(locId,site));
        rowct++;
        //this->UpdateProgress( int(100.0 * rowct / 10000.0));
    }
    wxLogMessage("Cached %d sites . . .", rowct);
    areaList_.resize(80);
    for(int bb = 0; bb < 80; bb++)
    {
        areaList_[bb] = std::make_shared<BigBox>(bb+1);
    }

}

void GlobalTempArea::bigMerge()
{
    PlotLuaPtr plot;
    SeriesPtr  xdata;
    SeriesPtr  ydata;
    wxFileName fn;
    std::vector<TrendPtr>  merge_band;

    for(int band = 0; band < 8; band++)
    {
        merge_band.push_back(std::make_shared<MergeSeries>());
    }
    for(int bb = 0; bb < 80; bb++)
    {
        AreaPtr boxp = areaList_[bb];
        // zero index into merge_band, 0-3, 4-7
        int bandix = (boxp->sign_ == 1) ? boxp->rowix_-1 : 8 - boxp->rowix_;

        TrendPtr plat = merge_band[bandix];
        TrendPtr dp = boxp->data_;

        if (plat->mergeCount_ == 0)
        {
            *(plat) = *dp;
            plat->mergeCount_ = 1;
            plat->setWeight(dp->mergeCount_/100.0);
        }
        else {
            dp->setWeight(dp->mergeCount_/100.0);
            plat->autoMerge(*dp);
        }
    }
    // keep the weights merge hemispheres.
    h_north = std::make_shared<MergeSeries>();

    for(int bb = 3; bb >= 0; bb--)
    {
        TrendPtr plat = merge_band[bb];
        if (h_north->mergeCount_ == 0)
        {
            *(h_north) = *plat;
            h_north->mergeCount_ = 1;
        }
        else {
            h_north->autoMerge(*plat);
        }
    }
    h_south = std::make_shared<MergeSeries>();
    for(int bb = 4; bb <= 7; bb++)
    {
        TrendPtr plat = merge_band[bb];
        if (h_south->mergeCount_ == 0)
        {
            *(h_south) = *plat;
            h_south->mergeCount_ = 1;
        }
        else {
            h_south->autoMerge(*plat);
        }
    }
    globe = std::make_shared<MergeSeries>();
    (*globe) = *h_north;
    globe->mergeCount_ = 1;
    globe->autoMerge(*h_south);

    plot = std::make_shared<PlotLua>();
    TimeSeries& gc = *(globe->cp_);
    ydata = std::make_shared<FloatSeries>(gc.temp_);
    xdata = std::make_shared<DateYearMonth>(gc.dates_);

    plot->addLayer(xdata, ydata);
    fn.Assign(path_);
    fn.SetExt("plot");
    plot->saveToFile(fn.GetFullPath().ToStdString());

    std::vector<float> monthAvg;

    if (gc.getMonthAverage(1951, 1980,monthAvg))
    {
        gc.makeMonthAnomalyPlot(monthAvg,path_);
        gc.makeYearAnomalyPlot(monthAvg,path_);
    }
}


double
GlobalTempArea::getBoxCenter(unsigned int bix)
{
    return gridLat_->operator[](bix);
}

