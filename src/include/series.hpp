#ifndef _H_SERIES
#define _H_SERIES

#include "agw.h"

#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <wx/string.h>
#include "json/json.h"
#include <wx/datetime.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <exception>
#include <lua.hpp>


namespace agw {
    class SeriesException : public std::exception {

    };

    const double nanDouble = NAN;
    const float nanFloat =  NAN;

    enum SeriesUnit {
        DEFAULT,
        DATE_JULIAN,
        DATE_JULIAN_MOD,
        DATE_MONTH_NUM, // (1-12)
        DATE_YEAR_MONTH  // Year * 12 + month (0-11)
    };

    std::string unitAsString(SeriesUnit unit);
    SeriesUnit toSeriesUnit(const std::string& v);

    // base class
    class Series {
    public:
        Series();
        virtual ~Series();

        Series(const Series& cs);
        const Series& operator=(const Series& cs);

        virtual void SaveJSON(Json::Value& json);
        virtual void ReadJSON(const Json::Value& json);

        virtual double operator[] (size_t ix) const = 0;
        virtual void fromString(size_t ix, const std::string& s) { ignore(ix); ignore(s); }
        //virtual std::string toString(size_t ix){}
         virtual void append(double value) = 0;
         virtual void append(const std::string& s){ ignore(s); }

        virtual void set(size_t ix, double value) = 0;
        virtual void setSize(size_t pts) = 0;
        virtual void assign(size_t n, double value)
        {
            ignore(n);ignore(value);
        }
        // return number valid values (not number of slots)
        virtual size_t getDataLimits(double& dmin, double& dmax);

        size_t size() const{ return pts_; }

        void setLabel(const std::string& label)
        {
            label_ = label;
        }
        const std::string& getLabel()
        {
            return label_;
        }
        const std::string& get_jype() {
            return jype_;
        }
        bool isAltered() const { return altered_; }
        bool isOrdered() const { return ordered_; }

        SeriesUnit units() const { return units_; }
        void units(SeriesUnit value) { units_ = value; }

        size_t calcDataLimits()
        {
            return getDataLimits(dataMin_, dataMax_);
        }

        double dataMin() const { return dataMin_; }
        double dataMax() const { return dataMax_; }

        int saveToFile(const std::string& path); // return file size
        std::string toJsonText();

    protected:

        size_t      pts_;
        double      dataMin_;
        double      dataMax_;
        SeriesUnit	units_;
        bool        altered_;
        bool		ordered_;
        std::string jype_;
        std::string label_;

        friend class SeriesRay;

    };

    // vector of gregorian date time (32-bits) from boost gregorian
    class DateSeries : public Series {
        virtual void fromString(size_t ix,const std::string& s);
        virtual void append(const std::string& s);

        virtual std::string toString(size_t ix);
        virtual void setSize(size_t pts);
        void init();

    protected:
        std::vector<boost::gregorian::date>  data_;
    };

    typedef std::shared_ptr<Series> SeriesPtr;


    class FloatSeries : public Series {
        void init()
        {
            jype_ = "float";
        }
    public:
        typedef std::vector<float> Data;

        FloatSeries()
        {
            init();
        }

        const Data& data()  const { return data_; }
        FloatSeries(const FloatSeries& cs);
        const FloatSeries& operator=(const FloatSeries& cs);

        FloatSeries(std::vector<float>& f, SeriesUnit value, const std::string& label)
        {
            init();
            move(f);
            units_ = value;
            label_ = label;
        }
        virtual ~FloatSeries();


        void prepend(float init_value, size_t n);
        void append(float init_value, size_t n);

        void addToValue(size_t ix, double value);

        virtual double operator[] (size_t ix) const;
        virtual void set(size_t ix, double value);
        virtual void append(double value);
        virtual void setSize(size_t pts);
        virtual void assign(size_t n, double value)
        {
            data_.assign(n, value);
            pts_ = data_.size();
        }
        void clear()
        {
            data_.clear();
            pts_ = 0;
        }
        void move(std::vector<float>& f);

        virtual void SaveJSON(Json::Value& json);
        virtual void ReadJSON(const Json::Value& json);
    protected:
        std::vector<float>  data_;

        friend class OffsetFloatSeries;
    };

    // there are 0 to n-1 valid stored points, but these are indexed using offset_ , and there are nanFloat
    // offset_ points before, and tail_ points after
    // total (virtual size) is offset_ + pts_ + tail_;, of which (0 to pts_-1) are stored, but may have missing values of nanFloat
    // only the [] and set functions use this.

    class OffsetFloatSeries : public FloatSeries {
        void init()
        {
            jype_ = "offset_float";
        }

    public:
        OffsetFloatSeries(uint offset, uint pts, uint tail) : offset_(offset), tail_(tail)
        {
            setSize(pts);
        }

        OffsetFloatSeries(FloatSeries& f, uint offset, uint tail) : FloatSeries(f.data_, f.units_, f.label_), offset_(offset), tail_(tail)
        {
        }
        virtual double operator[] (size_t ix) const
        {
            if (ix >= offset_)
            {
                auto vix = ix - offset_;
                if (vix < pts_)
                {
                    return data_[vix];
                }
            }
            return nanDouble;
        }
        void prepend(double init_value, size_t n)
        {
            data_.insert(data_.begin(), n, init_value);
            pts_ += n;
        }
        void append(double init_value, size_t n)
        {
            data_.insert(data_.end(), n, init_value);
            pts_ += n;
        }
        virtual void set(size_t ix, double value)
        {
            if (ix >= offset_)
            {
                auto vix = ix - offset_;
                if (vix < pts_)
                {
                    data_[vix] = value;
                }
            }
        }
        virtual void SaveJSON(Json::Value& json);
        virtual void ReadJSON(const Json::Value& json);

    protected:
        uint offset_;
        uint tail_;

    };

    // given a number of points, generate a line from equation of index value (from  0 to n-1
    // specify first and last value, eg 200 points, from -10.0 to 10.0
    class LineSeries : public Series {
    protected:

        void init()
        {
            jype_ = "line";
        }

        void calc()
        {
            scale_ = (v2_ - v1_) / (pts_ - 1);
            offset_ = v1_;
        }
    public:
        double v1_;
        double v2_;
        double scale_;
        double offset_;

        LineSeries()
        {
            init();
            set(2, 0.0, 1.0);

        }
        LineSeries(size_t n, double v1, double v2)
        {
            init();
            set(n,v1,v2);

        }
        // cannot set number of points to be 1
        void set(size_t n,double v1, double v2 )
        {
             if ( n <= 1)
                n = 2;

             pts_ = n;
             v1_ = v1;
             v2_ = v2;

             calc();
            // (0, v1_),   pts_-1, v2_)

        }
        // append has no meaning
        virtual void append(double value) { ignore(value);  }
        virtual double operator[] (size_t ix) const
        {
            return ix  * scale_ + offset_;
        }
        virtual void setSize(size_t pts)
        {
            if(pts<=1)
                pts=2;
            pts_ = pts;
            calc();
        }

        virtual void set(size_t ix, double value)
        {
            if (ix == 0)
            {
                v1_ = value;
            }
            else
            {
                v2_ = value;
                pts_ = ix + 1;
            }
            calc();
        }

        virtual void SaveJSON(Json::Value& json);
        virtual void ReadJSON(const Json::Value& json);

    };

    class DoubleSeries : public Series {
        void init()
        {
            jype_ = "double";
        }

    public:
        typedef std::vector<double> Data;


        DoubleSeries()
        {
           init();
        }
        // move data constructor
        DoubleSeries(std::vector<double>& f, SeriesUnit value, const std::string& label)
        {
            init();
            move(f);
            units_ = value;
            label_ = label;
        }
        virtual ~DoubleSeries();

        std::vector<double>& data() { return data_; }
        virtual double operator[] (size_t ix) const;
        virtual void set(size_t ix, double value);
        virtual void append(double value);
        virtual void setSize(size_t pts);
        virtual void assign(size_t n, double value)
        {
            data_.assign(n, value);
        }
        void move(std::vector<double>& f);
        void copy(const std::vector<double>& f);

        virtual void SaveJSON(Json::Value& json);
        virtual void ReadJSON(const Json::Value& json);
    private:
        std::vector<double>  data_;
    };

    // A virtual time base with Year/Month steps, countains all values inbetween without storing them.
    // This ignores calendar breakdowns in the past. Both first and last endpoints are included, in the range
    // month values are an enumerator starting at zero for wxDateTime
    class DateYearMonth : public Series {

    protected:
        int year1_;
        int month1_;
        int year2_;
        int month2_;

        void calcPts();

        void init()
        {
            jype_ = "year-month-range";
            units_ = DATE_YEAR_MONTH;
        }
    public:
        DateYearMonth() : year1_(0), month1_(0), year2_(0), month2_(0)
        {
            init();
            calcPts();
        }

        DateYearMonth(int year1, int month1, int year2, int month2) :
            year1_(year1), month1_(month1), year2_(year2), month2_(month2)
        {
            init();
            calcPts();

        }

        DateYearMonth(const DateYearMonth&);
        const DateYearMonth& operator=(const DateYearMonth&);

        void setStartDate(int month, int year) { month1_ = month; year1_ = year; calcPts(); }
        void setEndDate(int month, int year) { month2_ = month; year2_ = year; calcPts(); }
        void getStartDate(int& month, int& year) const {month = month1_; year = year1_;}
        void getEndDate(int& month, int& year) const {month = month2_; year = year2_;}

        DateYearMonth(const wxDateTime& firstMonth, const wxDateTime& lastMonth)
        {
            init();
            year1_ = firstMonth.GetYear();
            month1_ = firstMonth.GetMonth();

            year2_ = lastMonth.GetYear();
            month2_ = lastMonth.GetMonth();

            calcPts();
        }

        // return YEAR*12 + MONTH (0-11) ie number of months
        virtual double operator[] (size_t ix) const;
        virtual size_t getDataLimits(double& dmin, double& dmax);

        virtual void set(size_t ix, double value) { ignore(ix);  ignore(value); }
        virtual void setSize(size_t pts) { ignore(pts); }

        virtual void append(double value) { ignore(value); }

        virtual void SaveJSON(Json::Value& json);
        virtual void ReadJSON(const Json::Value& json);


    };


    struct NormalStats {
        double mean_;
        double stddev_;
        size_t n_;
        double minval_;
        double maxval_;
        bool   valid_;


        std::string toString();

        NormalStats(double m = 0.0, double s = 0.0, size_t n = 0) : mean_(m), stddev_(s), n_(n)
            ,minval_(0.0), maxval_(0.0), valid_(false)
        {
        }

        ~NormalStats(){}

        void calc(const std::vector<double>& data);
        void calc(const Series& x);
        static void setup_require(lua_State* L);
        static int   init_lib(lua_State* L);
        static int   toString(lua_State* L);
        static int   NewStats(lua_State* L);
        static int   lMin(lua_State* L);
        static int   lMax(lua_State* L);
        static int   lSize(lua_State* L);
        static int   lAvg(lua_State* L);
        static int   lStddev(lua_State* L);
        static int   lCalc(lua_State* L);

    };
    struct RCorrelate {
        NormalStats xs;
        NormalStats ys;
        double rval_;

        RCorrelate()  : xs(), ys(), rval_(0.0)
        {
        }
        void calc(const Series& x, const Series& y);
    };

}; // namespace agw

#endif
