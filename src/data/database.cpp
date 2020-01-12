#include "database.hpp"
#include "wxCSV.hpp"

#include <wx/log.h>
#include <wx/filefn.h>
#include <cstdint>
#include <sstream>


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
    "stationid CHAR(11) NOT NULL,"
    "latit REAL,"
    "longt REAL,"
    "elevt REAL,"
    "Name VARCHAR(50),"
    "PRIMARY KEY (stationid)"
");";
const std::string SetCreateSql =
"CREATE TABLE stationset ("
    "setid VARCHAR(50) PRIMARY KEY,"
    "queryJSON  TEXT"
");";

const std::string SetBelongSql =
"CREATE TABLE memberstation ("
    "stationid CHAR(11) NOT NULL,"
    "setid VARCHAR(50),"
    "PRIMARY KEY (stationid,setid)"
");";

/** hold number of valid values for a year */
const std::string GissYearSQL  =
"CREATE TABLE gissyear ("
     "dataid    INTEGER AUTO_INCREMENT NOT NULL,"
     "stationid CHAR(11) NOT NULL,"
     "year      INTEGER NOT NULL,"
     "measure   INTEGER NOT NULL,"  // 0 - avg, 1 = min, 2 = max
     "valuect   INTEGER NOT NULL,"
     "PRIMARY KEY (dataid)"
");";

const std::string GissYearIndex = "create index data_ix on gissyear (stationid, year, measure);";

const std::string MonthTempCreateSql =
"CREATE TABLE gisstemp ("
    "dataid    INTEGER NOT NULL,"
    "month     INTEGER NOT NULL,"
    "value     REAL,"
    "DMFlag CHAR(1),"
    "QCFlag CHAR(1),"
    "DSFlag CHAR(1),"
    "PRIMARY KEY (dataid, monthId)"
");";

const std::string ZeroBaseSql =
"CREATE TABLE zerobase ("
    "stationid CHAR(11) NOT NULL,"
    "measure  INTEGER NOT NULL,"  // 0 - avg, 1 = min, 2 = max
    "month     INTEGER NOT NULL," // 1-12 AVG,   13-24 MIN, 25-36 MAX
    "average    REAL,"
    "valuect    REAL,"
    "PRIMARY KEY (stationid,month)"
");";

const std::string StationCreateSql =
"CREATE TABLE station ("
    "id INTEGER PRIMARY KEY,"
    "name  TEXT NOT NULL,"
    "lat   REAL,"
    "long  REAL,"
    "altit REAL,"
    "startdate DATE,"
    "enddate DATE"
");";

Database::Database()
{
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
				execute(LocCreateSql);
				execute("select AddGeometryColumn('gissloc', 'Geometry', 4326, 'POINT', 'XY')");
				execute(GissYearSQL);
				execute(GissYearIndex);
				execute(MonthTempCreateSql);
				if (commit())
					wxLogMessage("GISS Tables Created");
			}
			catch(DBException* ex)
			{
				rollback();
				wxLogMessage("DBException: %s", ex->msg());
			}
        }
        if (!tableExists("stationset"))
        {
            execute(SetCreateSql);
            execute(SetBelongSql);
            wxLogMessage("Station Sets installed");
        }

    }
    else {
        wxLogMessage("Failed to open %s", filepath_.c_str());
    }
}

bool
Database::getGissLocRowId(const std::string& locCode, DBRowId& id)
{
    Statement s(*this, "SELECT LocId FROM gissloc WHERE LocCode = ?");
    s.bind(locCode,1);
    bool result = s.next();
    if (!result)
    {
        Statement ins(*this,"INSERT INTO gissloc (LocCode) VALUES (?)");
        ins.bind(locCode,1);
        if (ins.execute())
        {
            id = this->lastRowId();
            return true;
        }
    }else{
        id = s.getRowId(0);
        return true;
    }
    return false;
}

static DBRowId BaseStationID(const std::string& stationId, uint32_t year, MTEMP measure)
{ /* StationID is 27 bits : Country-10 + WMO-17
	 Lower is 30 bits : Station-Other-ID-10 + Year-14 + Measure-2 + Month-4 */
    uint32_t ctCode;
    uint32_t whoCode;
    uint32_t noCode;
    uint64_t rowId;

    std::istringstream buf(stationId.substr(0,3));
    buf >> ctCode;
    buf.clear();

    buf.str(stationId.substr(3,5));
    buf >> whoCode;
    buf.clear();

    buf.str(stationId.substr(8,3));
    buf >> noCode;

    // make the low 32 bits
    noCode = (noCode << 14) + (year & 0x03FFF);
    noCode = ((noCode << 2) + (measure & 0x03)) << 4;
    rowId = ((uint64_t)((ctCode << 17) + whoCode) << 32) + noCode;
    return rowId;

}


void MonthTemp::setId(const std::string& stationId, uint32_t year, MTEMP measure, int monthNum)
{
    this->id = BaseStationID(stationId, year, measure);
    this->monthId = monthNum;
}

// Load a single record from setId (if it exists)
bool MonthTemp::load(SqliteDB &sdb)
{
    Statement query(sdb, "select Value,DMFlag,QCFlag,DSFlag from gisstemp where codeId = ? and monthId = ?");
    query.bindRowId(id,1);
    if (query.execute())
    {
        query.get(0,this->value);
        query.get(1,this->dmFlag);
        query.get(2,this->qcFlag);
        query.get(3,this->dsFlag);
        return true;
    }
    return false;
}


void GissYear::setId(const std::string& stationId, uint32_t year, MTEMP measure)
{
    this->stationid = stationid;
    this->measure = measure;
    this->year = year;
}

// save doesn't know if the record already exists or not and tries an insert or replace
bool MonthTemp::save(SqliteDB &sdb)
{
    Statement query(sdb, "insert or replace into gisstemp(codeId,Value,DMFlag,QCFlag,DSFlag,monthId) values (?,?,?,?,?,?)");

    query.bindRowId(this->id,1);
    query.bind(this->value,2);
    query.bind(this->dmFlag,3);
    query.bind(this->qcFlag,4);
    query.bind(this->dsFlag,5);
    query.bindRowId(this->monthId,6);
    return query.execute();

}
 // Update assumes record exists
bool MonthTemp::update(SqliteDB &sdb)
{
    Statement query(sdb, "update gisstemp set Value = ?,DMFlag = ?,QCFlag = ?,DSFlag = ? where codeId = ? and monthId = ?");
    query.bind(this->value,1);
    query.bind(this->dmFlag,2);
    query.bind(this->qcFlag,3);
    query.bind(this->dsFlag,4);
    query.bindRowId(this->id,5);
    query.bindRowId(this->monthId,6);
    return query.execute();
}

// Load a single record from setId (if it exists)
bool GissYear::load(SqliteDB &sdb)
{
    Statement query(sdb, "select year,measure,valuesCt from gissyear where stationid = ?");
    query.bind(stationid,1);
    if (query.execute())
    {
        this->year = query.getInt32(0);
        this->measure = query.getInt32(1);
        this->valuesCt = query.getInt32(2);
        return true;
    }
    return false;
}

// save doesn't know if the record already exists or not and tries an insert or replace
bool GissYear::save(SqliteDB &sdb)
{
    Statement query(sdb, "insert into gissyear(stationid,year,measure,valuesct) values (?,?,?,?,?)");

    query.bind(stationid,1);
    query.bind((long) this->year,2);
    query.bind((long) this->measure,3);
    query.bind((long) this->valuesCt,4);
    return query.execute();

}
 // Update assumes record exists
bool GissYear::update(SqliteDB &sdb)
{
    Statement query(sdb, "update gissyear set year = ?, measure = ?, valuesCt = ? where stationid = ?");

    query.bind((long)this->year,1);
    query.bind((long)this->measure,2);
    query.bind((long)this->valuesCt,3);
    query.bind(this->stationid,4);
    return query.execute();
}

bool GissLocStats::update(SqliteDB &sdb)
{
   Statement query(sdb, "SELECT MIN(year) as minYear, MAX(year) as maxYear, COUNT(distinct year) as numYears"
                        " FROM gissyear where locId = ?"
                        );
   query.bindRowId(this->locId,1);
   if (query.next())
   {
       query.get(0, this->minYear);
       query.get(1, this->maxYear);
       query.get(2, this->numYears);
       return true;
   }
   return false;
}


void Station4::setId(const std::string& stationId)
{
	this->stationid = stationid;
}

bool Station4::save(SqliteDB &sdb)
{

    std::stringstream ss;

    ss << "insert or replace into gissloc("
        "stationid, latit,longt,elevt,code,name, Geometry"
		") values (?,?,?,?,?,"
		<< "MakePoint(" << this->long_ << "," << this->lat_ << ",4326)"
		<< ")";

    Statement query(sdb,ss.str());
    query.bind(this->stationid,1);
	query.bind(this->lat_,2);
	query.bind(this->long_,3);
	query.bind(this->elev_,4);
	query.bind(this->name_,5);
    return query.execute();
}

bool Station4::deleteSelf(Station4* prec)
{
    delete prec;
    return true;
}
bool Station4::set(Statement& qy)
{
  qy.get(0,this->stationid);
  qy.get(1,this->lat_);
  qy.get(2,this->long_);
  qy.get(3,this->elev_);
  qy.get(5,this->name_);
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
