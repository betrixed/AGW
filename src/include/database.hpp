#ifndef _H_DATABASE
#define _H_DATABASE

#include "sqlitedb.hpp"
#include "geometry.h"

#include <wx/arrstr.h>
#include <cstdint>
#include <vector>

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


/*
Only one elevation !
No quality classifiers.
*/
class Station4 {
public:
    std::string stationid; // original station code
    double  lat_;   //1
    double  long_;  //2
    double  elev_;
    std::string name_;


    int colCount() const { return 10; }

	static DBRowId MakeRowId(const std::string& stationId);
    void setId(const std::string& stationId);
	bool save(SqliteDB &sdb);
    bool set(Statement& s);
    static bool deleteSelf(Station4* prec);
};

enum MTEMP : uint32_t {
    TAVG = 0,
    TMIN = 1,
    TMAX = 2
};

class GissYear {
public:
    DBRowId     dataid;
    std::string stationid;
    uint32_t measure;
    uint32_t year;
    uint32_t valuesCt;

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
