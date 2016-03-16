#ifndef _H_DATABASE
#define _H_DATABASE

#include "sqlitedb.hpp"
#include "geometry.h"

#include <wx/arrstr.h>
#include <cstdint>
#include <vector>
/*
    From ftp://ftp.ncdc.noaa.gov/pub/data/ghcn/v3/
    Another line format, this time for monthly averaged data
    101603550001878TAVG  959  1 1019  1 1179  1 1679  1 2049  1 2309  1 2559  1 2749  1 2389  1-9999 Q1 1439  1 1219  1
    Character position fields.

    ID 10160355000  1-11
    YEAR 1878       12-15
    ELEMENT TAVG    16-19
Repeat
    VALUE1  959     20-24
    DMFLAG1         25
    QCFLAG1         26
    DSFLAG1   1     27

   -9999 is missing value
   DSFLAG12         column 115

   so we have 12 values per  year
   ID digits 1-3=Country Code, digits 4-8 represent
           the WMO id if the station is a WMO station.  It is a WMO station if
           digits 9-11="000".
*/

/* The station code is 11 decimal digits,
   The 63 bit integer indexes of SQLITE have at up to 19 decimal digits.
   The range of years of meaningful temperature records starts about 1800
   Each Temperature record has a stationId (11),  year (4), month(2) and the type(1) of record (Avg, Min or Max)
   This makes 18 digits.  So to make a general index of each temperature record, need only concantenate these.
   17 digits if the Month index is 1-12 for average, 13-24 for min and 25-36 for max values.

   To check an index for a station, multiply code by 10^

   X = 2**bits
   Log(X) = bits*Log(2)
   bits = Log(X) / Log(2)

   As a binary system, for 9999 years, need 14 bits,
   for 36 temp values, need 6 bits,
   StationId 37 bits (11 decimal digits)
   Country code - 999,  10 bits
   WMO  or other station, 27 bits

   Upper-32 Country(10), WMO(17) - 27 bits
   Lower-32 Other(10), Year(14), Measure(2), Month(4)  -- 30 bits

   How about   Two 32-bit values -  StationID is 27 bits : Country-10 + WMO-17
								 -  Lower is 30 bits : Station-Other-ID-10 + Year-14 + Measure-2 + Month-4


               For a year record, Month = 0

			   Normally Station-Other-ID is zero - unless none WMO-stations

    For the month anomaly baseline index, this takes out the "year"  (makes it effectively year zero ! )
    Then can use the same tables, such that MonthTemp.yearId = 0.

    To calculate anomaly baseline for a month, first average all MonthTemp records where yearId between 1951 and 1980.
    A join with gissyear has to be done for each measurement type and locId


     - SELECT AVG(MT.Value) as value, COUNT(MT.VALUE) as valuesCt,
               Y.codeId, MT.monthId FROM gisstemp T, gissyear Y
       where Y.YEARID >= 1951 and Y.YEARID <= 1980
       and MT.codeId = Y.codeId
       group by Y.codeId, MT.monthId

        Store in MonthTemp as codeId, monthId = 0, value, flags=' ',

    INSERT into MonthTemp (codeId, monthId,
*/
// in memory instance of import data
class ImportRecord {
public:
    sqlite3_int64 id;
    std::string   filename_;
    std::string   tablename_;
    wxDateTime    startDate_;
    wxDateTime    endDate_;
    sqlite3_int64 stationId;
};

class GissLocStats {
public:
    sqlite3_int64 locId; //0
    int32_t       minYear;
    int32_t       maxYear;
    int32_t       numYears;

    GissLocStats(sqlite3_int64 id) :
        locId(id), minYear(0), maxYear(0), numYears(0)
    {
    }

    bool update(SqliteDB &sdb);
};

class CountryRecord {
public:
    std::string      name_;
    agw::GMultiPoly area_;
};


class CountryLayer {
public:
    std::vector<std::unique_ptr<CountryRecord>> clist;
};

class GissLocation {
public:
    sqlite3_int64 id; //0
    double  lat_;   //1
    double  long_;  //2
    double  elevation1_;
	double  elevation2_;
    std::string name_;
    int32_t popSize_;
    char popClass_;
    char popLights_;
    std::string vegetation_;

    int colCount() const { return 10; }

	static DBRowId MakeRowId(const std::string& stationId);
    void setId(const std::string& stationId);
	bool save(SqliteDB &sdb);
    bool set(Statement& s);
    static bool deleteSelf(GissLocation* prec);
};

enum MTEMP : uint32_t {
    TAVG = 0,
    TMIN = 1,
    TMAX = 2
};

class GissYear {
public:
    DBRowId id;
    DBRowId locId;

    uint32_t year;
    uint32_t valuesCt;
    std::string measure;

    static DBRowId MakeRowId(const std::string& stationId, uint32_t year, MTEMP measure);
    void setId(const std::string& stationId, uint32_t year, MTEMP measure);


	GissYear() : year(0), valuesCt(0)
	{
	}
	bool load(SqliteDB &db);
	bool save(SqliteDB &db);
    bool update(SqliteDB &db);

};

class MonthTemp {
public:
    DBRowId id;
    int    monthId;
    double value;
    char   dmFlag;
    char    qcFlag;
    char    dsFlag;

    void setId(const std::string& stationId, uint32_t year, MTEMP measure, int monthNum);
	bool load(SqliteDB &db);
	bool save(SqliteDB &db);
    bool update(SqliteDB &db);

};

class wxCSV;


class Database : public SqliteDB {
public:
    Database();
    void init();

    bool hasImportFile(const std::string& filepath);
    bool importMaxTemp(wxCSV& rdr, const std::string& fileroot);
	bool importMinTemp(wxCSV& rdr, const std::string& fileroot);
    bool importRainfall(wxCSV& rdr, const std::string& fileroot);
    bool getSeriesNames(std::vector<std::string>& values);
    bool getSeriesData(long rowid, const std::string& tableName, std::vector<float>& data, std::vector<double>& xdata);

    bool getGissLocRowId(const std::string& locCode, DBRowId& id);
    void generateBaseline();


    virtual bool open();

    void executeThrow(const std::string& sql)
    {
 		if (!this->SqliteDB::execute(sql))
		{
			throw new DBException(this->lastError_);
		}
    }

    class AutoClose {
    public:
        AutoClose(Database& db) : db_(db)
        {
        }

        ~AutoClose()
        {
            db_.close();
        }
        private:
            Database& db_;
    };
};
#endif
