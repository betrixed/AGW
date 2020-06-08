#include "series.hpp"

#include <math.h>
#include <vector>
#include <fstream>

#include "json/json.h"
#include "series_helper.h"
#include "helper.h"

#ifdef WIN32
    #ifndef NAN
        static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
        #define NAN (*(const float *) __nan)
    #endif
#endif

using namespace agw;

static void jsonReadInt(const Json::Value& json, const std::string& key, int& value)
{
    if (json.isMember(key))
        value = json[key].asInt();
    else
        value = 0;
}
static void jsonReadUint(const Json::Value& json, const std::string& key, uint& value)
{
    if (json.isMember(key))
        value = json[key].asUInt();
    else
        value = 0;
}
Series::Series() : pts_(0), dataMin_(nanDouble), dataMax_(nanDouble), units_(DEFAULT), altered_(false)
{
}

Series::~Series()
{
}

Series::Series(const Series& cs)
    : pts_(cs.pts_), dataMin_(cs.dataMin_), dataMax_(cs.dataMax_), units_(cs.units_), altered_(cs.altered_),
      ordered_(cs.ordered_), jype_(cs.jype_), label_(cs.label_)
{
}

const Series& Series::operator=(const Series& cs)
{
    pts_ = cs.pts_;
    dataMin_ = cs.dataMin_;
    dataMax_ = cs.dataMax_;
    units_ = cs.units_;
    altered_ = cs.altered_;
    ordered_ = cs.ordered_;
    jype_ = cs.jype_;
    label_ = cs.label_;
    return *this;
}


std::string Series::toJsonText()
{
    Json::Value json(Json::objectValue);

    this->SaveJSON(json);

    std::stringstream fout;

    fout << json;

    return fout.str();

}
int Series::saveToFile(const std::string& path)
{
    Json::Value json(Json::objectValue);

    this->SaveJSON(json);

    std::ofstream fout(path);

    fout << json;

    fout.flush();

    auto pos = fout.tellp();
    fout.close();

    return (int)pos;


}

size_t Series::getDataLimits(double& dmin, double& dmax)
{
    size_t ct = 0;
    if (pts_ > 0)
    {
        double value;

        size_t ix = 0;

        while(ix < pts_)
        {
            value = operator[](ix);
            if (std::isnan(value))
            {
                ix++;
                continue;
            }
            if (ct == 0)
            {
                dmin = value;
                dmax = value;
            }
            else {
                if (value > dmax)
                    dmax = value;
                else if (value < dmin)
                    dmin = value;
            }
            ix++;
            ct++;
        }
    }
    return ct;
}

FloatSeries::~FloatSeries()
{
}

FloatSeries::FloatSeries(const FloatSeries& cs) : Series(cs), data_(cs.data_)
{

}

const FloatSeries& FloatSeries::operator=(const FloatSeries& cs)
{
    Series::operator=(cs);
    data_ = cs.data_;
    return *this;
}

double FloatSeries::operator[](size_t ix) const
{
    return data_[ix];
}

void FloatSeries::prepend(float init_value, size_t n)
{
    data_.insert(data_.begin(), n, init_value);
    pts_ += n;
}
void FloatSeries::append(float init_value, size_t n)
{
    data_.insert(data_.end(), n, init_value);
    pts_ += n;
}

void FloatSeries::append(double value)
{
    data_.push_back(value);
    pts_ = data_.size();
}

void FloatSeries::addToValue(size_t ix, double value)
{
    data_[ix] += value;
    altered_ = true;
}

void FloatSeries::set(size_t ix, double value)
{
    data_[ix] = (float) value;
    altered_ = true;
}

void FloatSeries::move(std::vector<float>& f)
{
    data_ = std::move(f);
	pts_ = data_.size();
}


void FloatSeries::setSize(size_t pts)
{
    auto oldSize = data_.size();

    data_.resize(pts);
    if (oldSize > 0)
        for(size_t i = oldSize; i < pts; i++)
            data_[i] = nanFloat;
    pts_ = pts;
    altered_ = true;
}


double DoubleSeries::operator[](size_t ix) const
{
    return data_[ix];
}

void DoubleSeries::set(size_t ix, double value)
{
    data_[ix] = value;
    altered_ = true;
}

void DoubleSeries::move(std::vector<double>& f)
{
    data_ = std::move(f);
	pts_ = data_.size();
}
void DoubleSeries::copy(const std::vector<double>& f)
{
    data_ = f;
	pts_ = data_.size();
}

void DoubleSeries::append(double value)
{
    data_.push_back(value);
    pts_ = data_.size();
}
void DoubleSeries::setSize(size_t pts)
{
    auto oldSize = data_.size();

    data_.resize(pts);
    if (oldSize > 0)
        for(size_t i = oldSize; i < pts; i++)
            data_[i] = nanDouble;
    pts_ = pts;
    altered_ = true;
}


DoubleSeries::~DoubleSeries()
{
}

void NormalStats::calc(const Series& x)
{
    double sum1 = 0.0;
    double value;
    auto nlimit = x.size();
    size_t ct = 0;
    RunningStat rstat;

    for (size_t i = 0 ; i < nlimit ; i++)
	{
        value = x[i];
        if (std::isnan(value))
        {
            continue;
        }
        else {
            rstat.Push(value);
        }
        if (ct==0)
        {
            minval_ =  value;
            maxval_ =  value;
        }
        else if (value < minval_)
            minval_ = value;
        else if (value > maxval_)
            maxval_ = value;
        ct++;
	}
	if (ct > 0)
	{
		this->mean_ = rstat.Mean() ;
        this->stddev_ = rstat.StandardDeviation();
	}
	else {
		this->mean_ = 0.0;
		this->stddev_ = 0.0;
	}
    valid_ = (ct > 0);
}

void RCorrelate::calc(const Series& x, const Series& y)
{
    auto pts = x.size();
    if ( pts < 2 || (pts != y.size()))
    {
        rval_ = 0.0;
        return;
    }

    xs.calc(x);
    ys.calc(y);
    double rsum = 0.0;
    for(size_t i = 0; i < pts; i++)
    {
        rsum += (x[i] - xs.mean_) * ( y[i] - ys.mean_ );
    }
    rval_ = rsum / ( xs.stddev_ * ys.stddev_ * (pts-1));

}

std::string agw::unitAsString(SeriesUnit u)
{
    switch(u)
    {

        case SeriesUnit::DATE_JULIAN:
            return "Julian";
        case SeriesUnit::DATE_JULIAN_MOD:
            return "Modified Julian";
        case SeriesUnit::DATE_MONTH_NUM:
            return "Month(1-12)";
        case SeriesUnit::DATE_YEAR_MONTH:
            return "Year-Month-MJDN";
        case SeriesUnit::DEFAULT:
        default:
            return "Default";
    }
}
SeriesUnit agw::toSeriesUnit(const std::string& s)
{
    if (s == "Default")
    {
        return  SeriesUnit::DEFAULT;
    }
    else if (s == "Month(1-12)")
    {
        return SeriesUnit::DATE_MONTH_NUM;
    }
    else if (s == "Modified Julian")
    {
        return SeriesUnit::DATE_JULIAN_MOD;
    }
    else if (s == "Julian")
    {
        return SeriesUnit::DATE_JULIAN;
    }
    else if (s == "Year-Month-MJDN")
    {
        return SeriesUnit::DATE_YEAR_MONTH;
    }
    else {
        return  SeriesUnit::DEFAULT;
    }
}

void Series::ReadJSON(const Json::Value& json)
{
    if (json.isMember("label"))
        this->label_ = json["label"].asString();
    if (json.isMember("unit"))
    {
        auto temp = json["unit"].asString();
        this->units_ = toSeriesUnit(temp);
    }
}

void Series::SaveJSON(Json::Value& json)
{
    json["jype"] = get_jype();
    json["label"] = label_;
    json["unit"] = unitAsString(units());
}

void LineSeries::SaveJSON(Json::Value& json)
{
    Series::SaveJSON(json);
    json["n"] = (Json::UInt) pts_;
    json["v1"] = v1_;
    json["v2"] = v2_;
}


void LineSeries::ReadJSON(const Json::Value& json)
{
    Series::ReadJSON(json);
    int n;
    if (readInt(json,"n",n))
    {
        if (n <= 1)
            n = 2;
        pts_ = n;
    }
    readDouble(json,"v1",v1_);
    readDouble(json,"v2",v2_);
    calc();
}



void DoubleSeries::ReadJSON(const Json::Value& json)
{
    Series::ReadJSON(json);
    if (json.isMember("items"))
    {
        const Json::Value& points = json["items"];
        size_t ct = points.size();
        this->data_.resize(ct);
        this->pts_ = ct;
        for(Json::Value::ArrayIndex ix = 0; ix < ct; ix++)
        {
            if (points[ix].isNull())
                data_[ix] = nanFloat; // avoid converting null to 0.0
            else
                data_[ix] = (points[ix]).asDouble();
        }
    }
}

void DoubleSeries::SaveJSON(Json::Value& json)
{
    Series::SaveJSON(json);
    Json::Value points(Json::arrayValue);
    uint ix = 0;
    for(Data::const_iterator i = data_.begin(); i != data_.end(); i++, ix++ )
    {
        points[ix] = *i;
    }
    json["items"] = points;
}

void FloatSeries::ReadJSON(const Json::Value& json)
{
    Series::ReadJSON(json);
    if (json.isMember("items"))
    {
        const Json::Value& points = json["items"];
        size_t ct = points.size();
        this->data_.resize(ct);
        this->pts_ = ct;
        for(Json::Value::ArrayIndex ix = 0; ix < ct; ix++)
        {
            if (points[ix].isNull())
                data_[ix] = nanFloat;
            else
                data_[ix] = points[ix].asFloat();
        }
    }
}

void FloatSeries::SaveJSON(Json::Value& json)
{
    Series::SaveJSON(json);

    Json::Value points(Json::arrayValue);
    uint ix = 0;
    for(Data::const_iterator i = data_.begin(); i != data_.end(); i++, ix++ )
    {
        points[ix] = *i;
    }
    json["items"] = points;
}

std::string NormalStats::toString()
{
    if (valid_)
    {
        std::stringstream sout;

        sout << "stats(size=> " << n_ << ", min=> " << this->minval_ << ", max=> " << this->maxval_;
        sout << "mean=> " << mean_ << ", sdev=> " << stddev_  << ")";
        return sout.str();
    }
    else {
        return "stats(invalid)";
    }
}

void NormalStats::calc(const std::vector<double> &data)
{
    double sum1 = 0.0;
    double value;
    auto fwd = data.begin();
	auto fend = data.end();
	size_t ct = 0;

	for(; fwd != fend; fwd++)
	{
        value = *fwd;
        if (std::isnan(value))
            continue;

        if (ct==0)
        {
            minval_ =  value;
            maxval_ =   value;
        }
        else if (value < minval_)
            minval_ = value;
        else if (value > maxval_)
            maxval_ = value;
        ct++;
        sum1 += value;
	}
	n_ = ct;
	if (ct > 0)
	{
		this->mean_ = sum1 / ct;

		if (ct > 1)
		{
			double sum2 = 0;
			double sum3 = 0;
			for(fwd = data.begin(); fwd != fend; fwd++)
			{
                value = *fwd;
                if (std::isnan(value))
                    continue;
				auto x = (value - this->mean_);
				sum2 += x*x;
				sum3 += x;
			}
			this->stddev_ = sqrt((sum2 - sum3*sum3/ct)/(ct-1));
		}
		else {
			this->stddev_ = 0;
		}
	}
	else {
		this->mean_ = 0.0;
		this->stddev_ = 0.0;
	}
    valid_ = (ct > 0);

}

void NormalStats::calc(const std::vector<float> &data)
{
    double sum1 = 0.0;
    double value;
    auto fwd = data.begin();
	auto fend = data.end();
	size_t ct = 0;

	for(; fwd != fend; fwd++)
	{
        value = *fwd;
        if (std::isnan(value))
            continue;

        if (ct==0)
        {
            minval_ =  value;
            maxval_ =   value;
        }
        else if (value < minval_)
            minval_ = value;
        else if (value > maxval_)
            maxval_ = value;
        ct++;
        sum1 += value;
	}
	n_ = ct;
	if (ct > 0)
	{
		this->mean_ = sum1 / ct;

		if (ct > 1)
		{
			double sum2 = 0;
			double sum3 = 0;
			for(fwd = data.begin(); fwd != fend; fwd++)
			{
                value = *fwd;
                if (std::isnan(value))
                    continue;
				auto x = (value - this->mean_);
				sum2 += x*x;
				sum3 += x;
			}
			this->stddev_ = sqrt((sum2 - sum3*sum3/ct)/(ct-1));
		}
		else {
			this->stddev_ = 0;
		}
	}
	else {
		this->mean_ = 0.0;
		this->stddev_ = 0.0;
	}
    valid_ = (ct > 0);

}

void OffsetFloatSeries::SaveJSON(Json::Value& json)
{
    Series::SaveJSON(json);
    json["offset"] = offset_;
    json["tail"] = tail_;
}
void OffsetFloatSeries::ReadJSON(const Json::Value& json)
{
    Series::ReadJSON(json);
    jsonReadUint(json,"offset",offset_);
    jsonReadUint(json,"tail",tail_);

}

void DateYearMonth::SaveJSON(Json::Value& json)
{
    Series::SaveJSON(json);

    json["year1"] = year1_;
    json["month1"] = month1_;
    json["year2"] = year2_;
    json["month2"] = month2_;

}


void DateYearMonth::ReadJSON(const Json::Value& json)
{
    Series::ReadJSON(json);

    jsonReadInt(json,"year1",year1_);
    jsonReadInt(json,"month1",month1_);
    jsonReadInt(json,"year2",year2_);
    jsonReadInt(json,"month2",month2_);
}

double DateYearMonth::operator[] (size_t ix) const
{
    int monthIndex =  year1_ * 12 + month1_ + ix;

    int yearNum = monthIndex/12;
    int monthNum = monthIndex % 12;

    wxDateTime fudge(1,wxDateTime::Month(monthNum), yearNum);
    return fudge.GetModifiedJulianDayNumber();
}
size_t DateYearMonth::getDataLimits(double& dmin, double& dmax)
{
    calcPts();
    dmin = (*this)[0];
    dmax = (*this)[pts_ - 1];
    return pts_;
}

DateYearMonth::DateYearMonth(const DateYearMonth& c)
            : Series(c),year1_(c.year1_), month1_(c.month1_), year2_(c.year2_), month2_(c.month2_)
{
}

const DateYearMonth&
DateYearMonth::operator=(const DateYearMonth&c)
{
    Series::operator=(c);
    year1_ = c.year1_;
    month1_ = c.month1_;
    year2_ = c.year2_;
    month2_ = c.month2_;
    return *this;
}


void DateYearMonth::calcPts()
{

    if (year2_ == 0 && month2_ == 0) {
        pts_ = 0;
        return;
    }
    int monthDiff = (month2_ - month1_);
    int yearDiff = (year2_ - year1_);
    if (monthDiff < 0)
    {
        wxASSERT_MSG(yearDiff > 0, "Year difference must be positive");
        yearDiff -= 1;
        monthDiff += 12;
    }
    pts_ = yearDiff*12 + monthDiff + 1;

}

void DateSeries::fromString(size_t ix,const std::string& s)
{
    data_[ix] = boost::gregorian::from_string(s);
}

void DateSeries::append(const std::string& s)
{
    data_.push_back(boost::gregorian::from_string(s));
}

std::string DateSeries::toString(size_t ix)
{
    return boost::gregorian::to_iso_string(data_[ix]);
}

void DateSeries::setSize(size_t pts)
{
    data_.resize(pts);
    pts_ = pts;
}

void DateSeries::init()
{
    jype_ = "date";
}

