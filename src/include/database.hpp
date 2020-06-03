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
    DBRowId id_; // primary key
    std::string stationid; // original station code
    double  lat_;   //1
    double  long_;  //2
    double  elev_;
    std::string name_;
    wxDateTime  startDate_;
    wxDateTime  endDate_;

    int colCount() const { return 8; }

    Station4() : id_(0), lat_(0.0), long_(0.0), elev_(0.0)
    {
    }
	static DBRowId GetPrimaryKey(SqliteDB& sdb, const std::string& stationId);
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

class StationSet {
    DBRowId      id_;
    int32_t     ss_count_;
    std::string  ss_name_;
    std::string  queryJSON_;
public:
    StationSet() : id_(0), ss_count_(0)
    {
    }

    bool loadByName(SqliteDB& sdb, const std::string& name);
    void set(Statement& s);
    void create(SqliteDB& sdb);
    void save(SqliteDB& sdb);
    void deleteSelf(SqliteDB& sdb);
    void deleteMembers(SqliteDB& sdb);

    void setName(const std::string& s) { ss_name_ = s;}
    void setJSON(const std::string& s) { queryJSON_ = s;}
    void setCount(int32_t ct) { ss_count_ = ct;}

    const std::string& getName() const { return ss_name_; }
    const std::string& getJSON() const { return queryJSON_; }
    DBRowId getRowId() const { return id_; }
    int getCount() const { return ss_count_; }
};
class GissYear {
public:
    DBRowId     dataid;
    DBRowId     sid;
    int32_t    measure;
    int32_t    year;
    int32_t    valuesct;

	GissYear() : dataid(0), sid(0), measure(0), year(0), valuesct(0)
	{
	}
	bool loadById(SqliteDB &db, DBRowId id);
	bool loadByStation(SqliteDB &db, const std::string& id, uint32_t year, uint32_t measure);
	bool save(SqliteDB &db);
    bool updateValuesCt(SqliteDB &db, DBRowId id, uint32_t ct);
    static std::string measureStr(MTEMP val);
};

class MonthTemp {
public:
    DBRowId dataid;
    int    monthid;
    double value;
    char   dmflag;
    char    qcflag;
    char    dsflag;

    MonthTemp() : dataid(0), monthid(0), value(0.0), dmflag(0), qcflag(0), dsflag(0)
        {

        }
	bool load(SqliteDB &db);
	bool save(SqliteDB &db);
    bool update(SqliteDB &db);

};

class wxCSV;


class Database : public SqliteDB {
public:
    Database();
    void init();

    //bool hasImportFile(const std::string& filepath);
    //bool getSeriesNames(std::vector<std::string>& values);
    //bool getSeriesData(long rowid, const std::string& tableName, std::vector<float>& data, std::vector<double>& xdata);

    void generateBaseline();

    void execNamedSQL(const std::string& name);
    void ensureTableExists(const std::string& name);

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
