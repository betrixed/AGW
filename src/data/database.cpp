#include "database.hpp"
#include "wxCSV.hpp"

#include <wx/log.h>
#include <wx/filefn.h>
#include <cstdint>
#include <sstream>
#include <map>

/*
spatialite setup:

Before anything:

Select InitSpatialMetaData();

Setup country gis data:

.loadshp cntry00 country UTF-8

After load of giss climate data;

select AddGeometryColumn('gissloc','Geometry', 4326, 'POINT', 2);

create table stationcountry as select distinct L.codeId, C.LONG_NAME as country, L.Name as station
        from gissloc L join country C where Contains(C.Geometry, L.Geometry);

create table cntrylist as select distinct country from stationcountry;
*/
const std::string LocCreateSql =
"CREATE TABLE gissloc ("
    "id INTEGER PRIMARY KEY,"
    "stationid CHAR(11) NOT NULL UNIQUE,"
    "latit REAL,"
    "longt REAL,"
    "elevt REAL,"
    "name VARCHAR(50),"
    "startdate DATE,"
    "enddate DATE"
");";

const std::string AddXYSql = "select AddGeometryColumn('gissloc', 'Geometry', 4326, 'POINT', 'XY')";

const std::string SetCreateSql =
"CREATE TABLE stationset ("
    "id      INTEGER PRIMARY KEY,"
    "ss_name VARCHAR(50) NOT NULL UNIQUE,"
    "ss_count INTEGER NOT NULL,"
    "queryJSON  TEXT"
");";

const std::string SetBelongSql =
"CREATE TABLE memberstation ("
    "ss_id INTEGER NOT NULL,"
    "sid   INTEGER NOT NULL,"
    "PRIMARY KEY (ss_id,sid)"
");";

/** hold number of valid values for a year.
dataid allows link to gisstemp.
Alternate unique key is sid,year,measure.
 */
const std::string GissYearSQL  =
"CREATE TABLE gissyear ("
     "dataid    INTEGER PRIMARY KEY,"
     "sid       INTEGER NOT NULL,"
     "year      INTEGER NOT NULL,"
     "measure   INTEGER NOT NULL,"  // 0 - avg, 1 = min, 2 = max
     "valuesct  INTEGER NOT NULL,"  // a year might not (yet) have all months
     "UNIQUE(sid, year, measure)"
");";


const std::string MonthTempCreateSql =
"CREATE TABLE gisstemp ("
    "dataid    INTEGER NOT NULL,"
    "monthid   INTEGER NOT NULL,"
    "value     REAL,"
    "dmflag CHAR(1),"
    "qcflag CHAR(1),"
    "dsflag CHAR(1),"
    "PRIMARY KEY (dataid, monthid)"
");";

const std::string ZeroBaseSql =
"CREATE TABLE zerobase ("
    "sid         INTEGER NOT NULL,"
    "monthid     INTEGER NOT NULL," // 1-12 AVG,   13-24 MIN, 25-36 MAX
    "measure     INTEGER NOT NULL,"  // 0 - avg, 1 = min, 2 = max
    "average     REAL,"
    "valuesct    REAL,"
    "PRIMARY KEY (sid, monthid,measure)"
");";

const std::string DailyCreateSql =
"CREATE TABLE daily ("
    "sid     INTEGER NOT NULL,"
    "obsdate DATE NOT NULL,"
    "tmin    REAL NOT NULL,"
    "tmax    REAL NOT NULL,"
    "pptn    REAL NOT NULL,"
    "PRIMARY KEY(sid, obsdate)"
");";


static std::vector< std::string > sql_names =
                    {"gissloc", "addxyindex", "stationset", "memberstation", "gissyear" ,  "gisstemp",
                   "zerobase",  "daily", "add_geometry", "year_index" };

static std::map<std::string, std::string> db_named_sql =
   { {"gissloc",LocCreateSql},
   { "addxyindex", AddXYSql},
        {"stationset",SetCreateSql},
        {"memberstation",SetBelongSql},
        {"gissyear",GissYearSQL} ,
        {"gisstemp",MonthTempCreateSql},
                   {"zerobase",ZeroBaseSql},
                   {"daily",DailyCreateSql} };


Database::Database()
{

}

void Database::execNamedSQL(const std::string& name) {
    auto sql = db_named_sql[name];
    execute_or_throw(sql);
}

void Database::ensureTableExists(const std::string& name) {
    if (!tableExists(name)) {
        execNamedSQL(name);
    }
}
void Database::init()
{
    if (this->isOpen())
    {
        wxLogMessage("Opened %s", filepath_.c_str());

        if (!tableExists("gissloc"))
        {
			try {
				begin();
				execNamedSQL("gissloc");
				execNamedSQL("addxyindex");
				execNamedSQL("gissyear");
				execNamedSQL("gisstemp");
				if (commit())
					wxLogMessage("GISS Tables Created");
			}
			catch(DBException* ex)
			{
				rollback();
				wxLogMessage("DBException: %s", ex->msg());
			}
        }
        ensureTableExists("stationset");
        ensureTableExists("memberstation");
        wxLogMessage("Station Sets installed");

    }
    else {
        wxLogMessage("Failed to open %s", filepath_.c_str());
    }
}
/*
    DBRowId dataid;
    int    monthid;
    double value;
    char   dmflag;
    char    qcflag;
    char    dsflag;
*/

bool
MonthTemp::loadByMonth(SqliteDB &sdb, DBRowId qdataid, int32_t qmonthid)
{
    dataid = qdataid;
    monthid = qmonthid;
    return load(sdb);
}

// Load a single record from setId (if it exists)
bool MonthTemp::load(SqliteDB &sdb)
{
    Statement query(sdb, "select value,dmflag,qcflag,dsflag from gisstemp where dataid = ? and monthid = ?");
    query.bindRowId(dataid,1);
    query.bind((long)monthid,2);
    if (query.next())
    {
        query.get(0,this->value);
        query.get(1,this->dmflag);
        query.get(2,this->qcflag);
        query.get(3,this->dsflag);
        return true;
    }
    return false;
}


// save doesn't know if the record already exists or not and tries an insert or replace
bool MonthTemp::save(SqliteDB &sdb)
{
    Statement query(sdb, "insert or replace into gisstemp(dataid, monthid, value, dmflag,qcflag,dsflag) values (?,?,?,?,?,?)");

    query.bindRowId(this->dataid,1);
    query.bind((long)this->monthid,2);
    query.bind(this->value,3);
    query.bind(this->dmflag,4);
    query.bind(this->qcflag,5);
    query.bind(this->dsflag,6);

    return query.execute();

}
 // Update assumes record exists
bool MonthTemp::update(SqliteDB &sdb)
{
    Statement query(sdb, "update gisstemp set value = ?,dmflag = ?,qcfFlag = ?,dsflag = ? where dataid = ? and monthid = ?");
    query.bind(this->value,1);
    query.bind(this->dmflag,2);
    query.bind(this->qcflag,3);
    query.bind(this->dsflag,4);
    query.bindRowId(this->dataid,5);
    query.bindRowId(this->monthid,6);
    return query.execute();
}
/*
     "dataid    INTEGER PRIMARY KEY,"
     "sid       INTEGER NOT NULL,"
     "year      INTEGER NOT NULL,"
     "measure   INTEGER NOT NULL,"  // 0 - avg, 1 = min, 2 = max
     "valuesct  INTEGER NOT NULL,"  // a year might not (yet) have all months
     "UNIQUE(sid, year, measure)"
*/

// static
std::string  GissYear::measureStr(MTEMP val)
{
    switch(val) {
case TAVG:
        return std::string("Averaged Daily");
case TMIN:
    return std::string("Avg. Minimum Daily");
case TMAX:
    return std::string("Avg. Maximum Daily");
case MMAX:
    return std::string("Highest Maximum");
case MMIN:
    return std::string("Lowest Minimum");
    }
    return std::string("Invalid MTEMP for measureStr");
}

bool GissYear::loadByStation(SqliteDB& sdb, DBRowId qsid,
                             int32_t qyear, int32_t qmeasure) {
    Statement query(sdb, "select dataid, valuesct from gissyear"
                    " where sid=? and measure=? and year=?");
    query.bindRowId(qsid,1);
    query.bind((long)qmeasure,2);
    query.bind((long)qyear,3);
    if (query.next())
    {
        this->dataid = query.getRowId(0);
        this->valuesct = query.getInt32(1);

        this->year = qyear;
        this->measure = qmeasure;
        this->sid = qsid;
        return true;
    }
    return false;

}
// Load a single record from setId (if it exists)
bool GissYear::loadById(SqliteDB &sdb, DBRowId id)
{
    Statement query(sdb, "select sid, year,measure,valuesct from gissyear where dataid = ? ");
    query.bindRowId(id,1);

    if (query.execute())
    {
        dataid = id;
        query.get(0, this->sid);
        this->year = query.getInt32(1);
        this->measure = query.getInt32(2);
        this->valuesct = query.getInt32(3);
        return true;
    }
    return false;
}



bool GissYear::create(SqliteDB &sdb)
{
     Statement query(sdb, "insert into gissyear(sid,year,measure,valuesct) values (?,?,?,?)");

    query.bindRowId( this->sid,1);
    query.bind((long) this->year,2);
    query.bind((long) this->measure,3);
    query.bind((long) this->valuesct,4);
    bool result = query.execute();
    if (result) {
        dataid = sdb.lastRowId();
    }
    return result;
}

bool GissYear::save(SqliteDB &sdb) {
    if (dataid == 0) {
        GissYear temp;
        if (!temp.loadByStation(sdb,sid,year,measure)) {
            return create(sdb);
        }
        else {
            dataid = temp.dataid;
        }
    }
    return update(sdb);
}
bool GissYear::update(SqliteDB &sdb)
{

    Statement query(sdb, "update gissyear set sid=?,year=?,measure=?,valuesct=? "
                    " where dataid = ?"
                    );

    query.bindRowId( this->sid,1);
    query.bind((long) this->year,2);
    query.bind((long) this->measure,3);
    query.bind((long) this->valuesct,4);
    query.bindRowId( this->dataid, 5);
    return query.execute();
}
 // Update assumes record exists
bool GissYear::updateValuesCt(SqliteDB &sdb, int32_t ct)
{
    Statement query(sdb, "update gissyear set valuesct = ? where dataid = ?");

    query.bind((long)ct,1);
    query.bindRowId(dataid,2);
    return query.execute();
}

bool GissLocStats::update(SqliteDB &sdb)
{
   Statement query(sdb, "SELECT MIN(year) as minYear, MAX(year) as maxYear, COUNT(distinct year) as numYears"
                        " FROM gissyear where sid = ?"
                        );
   query.bindRowId(this->locId,1);
   if (query.next())
   {
       this->minYear = query.getInt32(0);
       this->maxYear = query.getInt32(1);
       this->numYears = query.getInt32(2);
       return true;
   }
   return false;
}

void
StationSet::set(Statement& s) {
    this->id_ = s.getRowId(0);
    s.get(1, this->ss_name_);
    this->ss_count_ = s.getInt32(2);
    s.get(3, this->queryJSON_);
}

void
StationSet::create(SqliteDB& sdb)
{
    Statement query(sdb, "insert into stationset (ss_name,ss_count,queryJSON)"
                    " values(?,?,?)");
    query.bind(ss_name_,1);
    query.bind((long)ss_count_,2);
    query.bind(queryJSON_,3);
    query.execute_or_throw();
    id_ = sdb.lastRowId();
}

void
StationSet::save(SqliteDB& sdb)
{
    Statement query(sdb, "UPDATE stationset set ss_name = ?, ss_count = ?, queryJSON = ?"
                    " WHERE id = ?" );
    query.bind(ss_name_,1);
    query.bind((long)ss_count_,2);
    query.bind(queryJSON_,3);
    query.bindRowId(id_,4);
    query.execute_or_throw();
}

void
StationSet::deleteMembers(SqliteDB& sdb)
{
    Statement dq (sdb, "delete from memberstation where ss_id = ?");
    dq.bindRowId(id_, 1);
    dq.execute();
}

void StationSet::deleteSelf(SqliteDB& sdb)
{
    deleteMembers(sdb);
    Statement dq (sdb, "delete from stationset where id = ?");
    dq.bindRowId(id_, 1);
    id_ = 0;
    dq.execute_or_throw();
}

bool
StationSet::loadByName(SqliteDB& sdb, const std::string& name)
{
    Statement query(sdb, "select * from stationset where ss_name = ?");
    query.bind(name,1);
    if (query.next()) {
        this->set(query);
        return true;
    }
    return false;
}
/*

 "id INTEGER PRIMARY KEY,"
    "stationid CHAR(11) NOT NULL UNIQUE,"
    "latit REAL,"
    "longt REAL,"
    "elevt REAL,"
    "name VARCHAR(50),"
    "startdate DATE,"
    "enddate DATE"
    */
bool
Station4::loadByCode(SqliteDB& sdb, const std::string& qstationid) {
    Statement query(sdb, "select id, latit, longt, elevt, "
            " name, startdate, enddate from gissloc where stationid = ?");
    query.bind(qstationid,1);
    if (query.next()) {
        this->stationid = qstationid;
        id_ = query.getRowId(0);
        query.get(1,lat_);
        query.get(2,long_);
        query.get(3,elev_);
        query.get(4,name_);
        startDate_ = query.getDateTime(5);
        endDate_ = query.getDateTime(6);
        return true;
    }
    return false;
}

 DBRowId
 Station4::GetPrimaryKey(SqliteDB& sdb, const std::string& stationId)
 {
    DBRowId result;

    Statement query(sdb, "select id from gissloc where stationid = ?");
    query.bind(stationId,1);
    if (query.next()) {
        query.get(0,result);
        return result;
    }
    std::stringstream ss;
    ss << "insert into gissloc (stationid) values (?)";
    Statement nrow(sdb,ss.str());
    nrow.bind(stationId,1);
    nrow.execute_or_throw();
    return sdb.lastRowId();
 }

void Station4::setId(const std::string& stationId)
{
	this->stationid = stationid;
}

bool Station4::save(SqliteDB &sdb)
{
    if (this->id_ == 0) {
        this->id_ = Station4::GetPrimaryKey(sdb, this->stationid);
    }
    std::stringstream ss;

    ss << "update gissloc set stationid = ? , latit = ?, longt = ?, elevt = ?,"
        " name = ?, startdate = ?, enddate = ?,";
    //ss << " Geometry = MakePoint(" << this->long_ << "," << this->lat_ << ",4326)";
    ss << " Geometry = MakePoint(?,?,4326)";
    ss << " where id = ? ";

//		") values (?,?,?,?,?,"
//		<< "MakePoint(" << this->long_ << "," << this->lat_ << ",4326)"
//		<< ")";

    Statement query(sdb,ss.str());
    query.bind(this->stationid,1);
	query.bind(this->lat_,2);
	query.bind(this->long_,3);
	query.bind(this->elev_,4);
	query.bind(this->name_,5);
	query.bind(this->startDate_,6);
	query.bind(this->endDate_,7);
	query.bind(this->long_,8);
	query.bind(this->lat_,9);
	query.bindRowId(this->id_,10);

    return query.execute();
}

bool Station4::deleteSelf(Station4* prec)
{
    delete prec;
    return true;
}
bool Station4::set(Statement& qy)
{
  qy.get(0,this->id_);
  qy.get(1,this->stationid);
  qy.get(2,this->lat_);
  qy.get(3,this->long_);
  qy.get(4,this->elev_);
  qy.get(5,this->name_);
  this->startDate_ = qy.getDateTime(6);
  this->endDate_ = qy.getDateTime(7);
  return true;
}


void Database::generateBaseline()
{
    try {
        if (tableExists("zerobase"))
        {
            begin();
            execute("drop table zerobase");
            if (commit())
                wxLogMessage("zerobase Dropped");
        }

        if (!tableExists("zerobase"))
        {
            begin();
            execute(ZeroBaseSql);
            if (commit())
                wxLogMessage("zerobase Created");
        }
        // zero base is an average for each month, from start year to finish year.
		// to make use of it, for anomaly calcultion , use the difference.

        std::string sql =
        "insert into zerobase (locId, measure, monthId, average, years)"
        "  SELECT Y.locId, Y.measure, T.monthId,"
        "   AVG(T.Value) as average, COUNT(T.VALUE) as years "
        "   FROM gisstemp T, gissyear Y "
        "  WHERE Y.YEAR >= 1951 and Y.YEAR <= 1980 "
        "   and T.codeId = Y.codeId "
        " group by Y.locId, Y.measure, T.monthId ";

         begin();
         execute(sql);
        if (commit())
                wxLogMessage("zerobase populated");
    }
    catch(DBException* ex)
    {
        rollback();
        wxLogMessage("DBException: %s", ex->msg());
    }
}

bool Database::open()
{
    if (SqliteDB::open())
    {
        allowExtensionLoad(true);
        execute_or_throw("Select load_extension('mod_spatialite')");
        allowExtensionLoad(false);
    }
    return true;
}
