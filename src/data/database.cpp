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
    "codeId INTEGER PRIMARY KEY," // code is index as a decimal 11 digits
    "Latitude REAL,"
    "Longitude REAL,"
    "ElevationStation REAL,"
	"ElevationGrid REAL,"
    "Name VARCHAR(50),"
    "PopSize INTEGER,"
    "PopClass CHAR(1),"
    "PopLights CHAR(1),"
    "Vegetation VARCHAR(50)"
");";

const std::string SetCreateSql =
"CREATE TABLE stationset ("
    "setName VARCHAR(50) PRIMARY KEY,"
    "queryJSON  TEXT"
");";

const std::string SetBelongSql =
"CREATE TABLE memberstation ("
    "setName VARCHAR(50),"
    "codeId INTEGER,"
    "PRIMARY KEY (setName,codeId)"
");";

/* Anomaly base is the average value over about 30 years 1951-1980 at
   each location at each month. It thus loses the "year" specificity, but retains the month index */


const std::string GissYearSQL  =
"CREATE TABLE gissyear ("
     "codeId INTEGER PRIMARY KEY," // the combined primary key bit pattern is stationID, year, measure,
     "locId INTEGER, " // foreign key into gisloc
     "year   INTEGER,"
	 "measure CHAR(4) NOT NULL,"
	 "valuesCt INTEGER NOT NULL"
");";

const std::string GissYearIndex = "create index yearloc_ix on gissyear (locId, year);";

const std::string MonthTempCreateSql =
"CREATE TABLE gisstemp ("
    "codeId INTEGER," // the code is stationID, year, measure, also a foreign key into gissyear
    "monthId INTEGER,"
    "Value REAL,"
    "DMFlag CHAR(1),"
    "QCFlag CHAR(1),"
    "DSFlag CHAR(1),"
    "PRIMARY KEY (codeId, monthId)"
");";

const std::string ZeroBaseSql =
"CREATE TABLE zerobase ("
    "locId      INTEGER NOT NULL," // foreign key into gisloc
    "measure    CHAR(4) NOT NULL,"
    "monthId    INTEGER NOT NULL,"
    "average    REAL,"
    "years      REAL,"
    "PRIMARY KEY (locId,measure,monthId)"
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


const std::string ImportFileCreateSql =
"CREATE TABLE import_file ("
    "id integer PRIMARY KEY AUTOINCREMENT,"
    "filename text NOT NULL,"
    "statistic text NOT NULL,"
    "startdate date,"
    "enddate date,"
    "stationid integer"
");";


const std::string MaxTempCreateSql =
"CREATE TABLE max_temp ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "seriesId INTEGER NOT NULL,"
    "obsdate DATE NOT NULL,"
    "value   REAL NOT NULL"
");";

const std::string MinTempCreateSql =
"CREATE TABLE min_temp ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "seriesId INTEGER NOT NULL,"
    "obsdate DATE NOT NULL,"
    "value   REAL NOT NULL"
");";

const std::string RainfallCreateSql =
"CREATE TABLE rainfall ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "seriesId INTEGER NOT NULL,"
    "obsdate DATE NOT NULL,"
    "value   REAL NOT NULL"
");";

const std::string InsertImportFileSql =
"INSERT INTO import_file (filename, statistic)"
"VALUES (?, ?);";

const std::string UpdateImportFileSql =
"UPDATE import_file set startDate = ?, endDate = ?, stationId = ? where id = ?;";

const std::string InsertMaxTempSql =
"INSERT INTO max_temp (seriesId, OBSDATE, VALUE) VALUES (? , ?, ?);";

const std::string InsertMinTempSql =
"INSERT INTO min_temp (seriesId, OBSDATE, VALUE) VALUES (? , ?, ?);";


const std::string InsertRainfallSql =
"INSERT INTO rainfall (seriesId, OBSDATE, VALUE) VALUES (? , ?, ?);";

bool Database::importMaxTemp(wxCSV& rdr, const std::string& importName)
{
    wxArrayString row;
    bool firstData = true;
    long stationId;
    long year, month, day, accum;
    double temp;
    long ct = 0;
    wxString quality;
    wxDateTime startDate, endDate;
    sqlite3_int64         importId;
    this->begin();

    {
        Statement t(*this, InsertImportFileSql);
        t.bind(importName,1);
        t.bind("max_temp",2);
        t.execute();
        importId = this->lastRowId();
    }
    while(rdr.next(row))
    {
        if ((row[5]!=wxEmptyString) && (row[6]!=wxEmptyString) && (row[7]!=wxEmptyString))
        {
            auto dateOK = row[2].ToLong(&year);
            if (dateOK)
                dateOK = row[3].ToLong(&month);
            if (dateOK)
                dateOK = row[4].ToLong(&day);

            row[5].ToDouble(&temp);
            row[6].ToLong(&accum);
            quality = row[7];



            if (accum==1 && dateOK)
            {
                wxDateTime obsdate(day, (wxDateTime::Month) (month-1), year);
                if (firstData)
                {
                    firstData = false;
                    row[1].ToLong(&stationId);
                    startDate = obsdate;
                    endDate = obsdate;
                }
                else {
                    endDate = obsdate;
                }
                Statement s(*this,InsertMaxTempSql);
                s.bindRowId(importId,1);
                s.bind(obsdate,2);
                s.bind(temp,3);
                s.execute();
                ct += 1;
            }
        }
    }
    wxLogMessage("imported %d rows", ct);
    if (ct > 0)
    {
        Statement t(*this, UpdateImportFileSql);
        t.bind(startDate,1);
        t.bind(endDate,2);
        t.bindRowId(stationId,3);
        t.bindRowId(importId,4);
        t.execute();
    }
    return this->commit();
}

bool Database::importRainfall(wxCSV& rdr, const std::string& importName)
{
   wxArrayString row;
    bool firstData = true;
    long stationId;
    long year, month, day, accum;
    double temp;
    long ct = 0;
    wxString quality;
    wxDateTime startDate, endDate;
    sqlite3_int64         importId;
    this->begin();

    {
        Statement t(*this, InsertImportFileSql);
        t.bind(importName,1);
        t.bind("rainfall",2);
        t.execute();
        importId = this->lastRowId();
    }
    while(rdr.next(row))
    {
        if ((row[5]!=wxEmptyString) && (row[6]!=wxEmptyString) && (row[7]!=wxEmptyString))
        {
            auto dateOK = row[2].ToLong(&year);
            if (dateOK)
                dateOK = row[3].ToLong(&month);
            if (dateOK)
                dateOK = row[4].ToLong(&day);

            row[5].ToDouble(&temp);
            row[6].ToLong(&accum);
            quality = row[7];



            if (accum==1 && dateOK)
            {
                wxDateTime obsdate(day, (wxDateTime::Month) (month-1), year);
                if (firstData)
                {
                    firstData = false;
                    row[1].ToLong(&stationId);
                    startDate = obsdate;
                    endDate = obsdate;
                }
                else {
                    endDate = obsdate;
                }
                Statement s(*this,InsertRainfallSql);
                s.bindRowId(importId,1);
                s.bind(obsdate,2);
                s.bind(temp,3);
                s.execute();
                ct += 1;
            }
        }
    }
    wxLogMessage("imported %d rows", ct);
    if (ct > 0)
    {
        Statement t(*this, UpdateImportFileSql);
        t.bind(startDate,1);
        t.bind(endDate,2);
        t.bindRowId(stationId,3);
        t.bindRowId(importId,4);
        t.execute();
    }
    return this->commit();
}

bool Database::importMinTemp(wxCSV& rdr, const std::string& importName)
{
    wxArrayString row;
    bool firstData = true;
    long stationId;
    long year, month, day, accum;
    double temp;
    long ct = 0;
    wxString quality;
    wxDateTime startDate, endDate;
    sqlite3_int64         importId;
    this->begin();

    {
        Statement t(*this, InsertImportFileSql);
        t.bind(importName,1);
        t.bind("min_temp",2);
        t.execute();
        importId = this->lastRowId();
    }
    while(rdr.next(row))
    {
        if ((row[5]!=wxEmptyString) && (row[6]!=wxEmptyString) && (row[7]!=wxEmptyString))
        {
            auto dateOK = row[2].ToLong(&year);
            if (dateOK)
                dateOK = row[3].ToLong(&month);
            if (dateOK)
                dateOK = row[4].ToLong(&day);

            row[5].ToDouble(&temp);
            row[6].ToLong(&accum);
            quality = row[7];



            if (accum==1 && dateOK)
            {
                wxDateTime obsdate(day, (wxDateTime::Month) (month-1), year);
                if (firstData)
                {
                    firstData = false;
                    row[1].ToLong(&stationId);
                    startDate = obsdate;
                    endDate = obsdate;
                }
                else {
                    endDate = obsdate;
                }
                Statement s(*this,InsertMinTempSql);
                s.bindRowId(importId,1);
                s.bind(obsdate,2);
                s.bind(temp,3);
                s.execute();
                ct += 1;
            }
        }
    }
    wxLogMessage("imported %d rows", ct);
    if (ct > 0)
    {
        Statement t(*this, UpdateImportFileSql);
        t.bind(startDate,1);
        t.bind(endDate,2);
        t.bindRowId(stationId,3);
        t.bindRowId(importId,4);
        t.execute();
    }
    return this->commit();
}

bool Database::getSeriesData(long rowid, const std::string& tableName, std::vector<float>& data, std::vector<double>& xdata)
{
    wxString sql;

    sql << "SELECT value, obsdate from ";
    sql << tableName;
    sql << " WHERE seriesId = ? order by obsdate";
    Statement t(*this, sql.ToStdString());
    t.bind(rowid, 1);

    while(t.next())
    {
        data.push_back(t.getDouble(0));
        xdata.push_back(t.getDouble(1));
    }
     return data.size() > 0;
}

bool Database::getSeriesNames(std::vector<std::string>& values)
{
    Statement s(*this, "SELECT * FROM import_file");

    size_t ct = 0;
    while(s.next())
    {
        std::stringstream outs;
        outs << s.getText(0);
        outs << ", ";
        outs << s.getText(1);
        outs << ", ";
        outs << s.getText(2);
        outs << ", ";
        outs << s.getDateTime(3).FormatISODate();
        outs << ", ";
        outs << s.getDateTime(4).FormatISODate();
        values.push_back(outs.str());
        ct += 1;
    }
    return ct > 0;
}


bool Database::hasImportFile(const std::string& filepath)
{
    Statement s(*this, "SELECT * FROM import_file WHERE filename=?");

    s.bind(filepath,1);
    return s.next();
}

Database::Database()
{
}

void Database::init()
{
    if (this->isOpen())
    {
        wxLogMessage("Opened %s", filepath_.c_str());
        if (!tableExists("import_file"))
        {
            execute(ImportFileCreateSql);
            execute(StationCreateSql);
            execute(MaxTempCreateSql);
            execute(MinTempCreateSql);
			execute(RainfallCreateSql);
            wxLogMessage("Tables Created");
        }
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

DBRowId
GissYear::MakeRowId(const std::string& stationId, uint32_t year, MTEMP measure)
{
    return BaseStationID(stationId, year, measure);
}

void GissYear::setId(const std::string& stationId, uint32_t year, MTEMP measure)
{
    this->id = BaseStationID(stationId, year, measure);
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
    Statement query(sdb, "select year,measure,valuesCt,locId from gissyear where codeId = ?");
    query.bindRowId(id,1);
    if (query.execute())
    {
        this->year = query.getInt32(0);
        this->measure = query.getText(1);
        this->valuesCt = query.getInt32(2);
        query.get(3,this->locId);
        return true;
    }
    return false;
}

// save doesn't know if the record already exists or not and tries an insert or replace
bool GissYear::save(SqliteDB &sdb)
{
    Statement query(sdb, "insert or replace into gissyear(codeId,year,measure,valuesCt,locId) values (?,?,?,?,?)");

    query.bindRowId(this->id,1);
    query.bind((long) this->year,2);
    query.bind(this->measure,3);
    query.bind((long)this->valuesCt,4);
    query.bindRowId(this->locId,5);
    return query.execute();

}
 // Update assumes record exists
bool GissYear::update(SqliteDB &sdb)
{
    Statement query(sdb, "update gissyear set year = ?, measure = ?, valuesCt = ?, locId = ? where codeId = ?");

    query.bind((long)this->year,1);
    query.bind(this->measure,2);
    query.bind((long)this->valuesCt,3);
    query.bindRowId(this->locId,4);
    query.bindRowId(this->id,5);
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

DBRowId GissLocation::MakeRowId(const std::string& stationId)
{
/* StationID is 27 bits : Country-10 + WMO-17
	 Lower is 30 bits : Station-Other-ID-10 + 20 bits of zero */
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
    noCode = (noCode << 20);
    rowId = ((uint64_t)((ctCode << 17) + whoCode) << 32) + noCode;
    return rowId;
}

void GissLocation::setId(const std::string& stationId)
{
	this->id = GissLocation::MakeRowId(stationId);
}

bool GissLocation::save(SqliteDB &sdb)
{

    std::stringstream ss;

    ss << "insert or replace into gissloc("
        "codeId,Latitude,Longitude,ElevationStation,ElevationGrid,Name,PopSize,PopClass,PopLights,Vegetation,Geometry"
		") values (?,?,?,?,?,?,?,?,?,?,"
		<< "MakePoint(" << this->long_ << "," << this->lat_ << ",4326)"
		<< ")";

    Statement query(sdb,ss.str());

    query.bindRowId(this->id,1);
	query.bind(this->lat_,2);
	query.bind(this->long_,3);
	query.bind(this->elevation1_,4);
	query.bind(this->elevation2_,5);
	query.bind(this->name_,6);
    query.bind((long) this->popSize_,7);
    query.bind((long) this->popClass_,8);
    query.bind((long) this->popLights_,9);
    query.bind(this->vegetation_,10);
    return query.execute();
}

bool GissLocation::deleteSelf(GissLocation* prec)
{
    delete prec;
    return true;
}
bool GissLocation::set(Statement& qy)
{
  qy.get(0,this->id);
  qy.get(1,this->lat_);
  qy.get(2,this->long_);
  qy.get(3,this->elevation1_);
  qy.get(4,this->elevation2_);
  qy.get(5,this->name_);
  qy.get(6,this->popSize_);
  qy.get(7,this->popClass_);
  qy.get(8,this->popLights_);
  qy.get(9,this->vegetation_);
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
