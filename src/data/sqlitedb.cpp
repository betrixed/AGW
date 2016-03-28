
#include "sqlitedb.hpp"
#include <cctype>
#include <string>
#include <algorithm>
#include <memory>
#include <wx/log.h>

#include <spatialite.h>

std::string trim(const std::string &s)
{
   auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
   auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
   return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

int32_t Statement::getInt32(int zpos)
{
    return sqlite3_column_int(h_,zpos);
}

const void* Statement::getBlob(int zpos)
{
    return sqlite3_column_blob(h_,zpos);
}

int  Statement::getBlobBytes(int zpos)
{
    return sqlite3_column_bytes(h_,zpos);
}

char    Statement::getChar(int zpos)
{
    char* cp = (char*)sqlite3_column_text(h_,zpos);
    if (cp != nullptr)
    {
        return *cp;
    }
    else
        return (char)0;
}
double  Statement::getDouble(int zpos)
{
    return sqlite3_column_double(h_, zpos);
}

void Statement::get(int zpos, std::string& value)
{
    char* cp = (char*)sqlite3_column_text(h_,zpos);
    int len = sqlite3_column_bytes(h_,zpos);
    value.assign(cp,len);
}

void Statement::get(int zpos, double& value)
{
    value = sqlite3_column_double(h_, zpos);
}

void Statement::get(int zpos, DBRowId& rowid)
{
    rowid = sqlite3_column_int64(h_, zpos);
}

void Statement::get(int zpos, char& c)
{
    char* cp = (char*)sqlite3_column_text(h_,zpos);
    c = *cp;
}

void Statement::get(int zpos, int32_t& value)
{
    value = sqlite3_column_int(h_,zpos);
}

DBRowId  Statement::getRowId(int zpos)
{
    return sqlite3_column_int64(h_, zpos);
}

const char*  Statement::getText(int zpos)
{
    return (const char*) sqlite3_column_text(h_, zpos);
}

wxDateTime Statement::getDateTime(int zpos)
{
    auto value = sqlite3_column_double(h_, zpos);
    return wxDateTime( value + 2400000.5);
}

void Statement::finalize()
{
    if (h_ != nullptr)
    {
        sqlite3_finalize(h_);
        h_ = nullptr;
    }
}

Statement::~Statement()
{
    finalize();
}

void Statement::prepare(const std::string& sql)
{
    const char* pzTail = nullptr;
    if (h_ != nullptr)
        finalize();
    db_.lastSQL_ = sql;
    code_ = sqlite3_prepare_v2(db_.handle(), sql.c_str(), sql.size(), &h_, &pzTail);
    if (code_ != SQLITE_OK)
    {
        int errorCode = sqlite3_extended_errcode(db_.handle());
        db_.lastError_ = sqlite3_errstr(errorCode);
        throw DBException(db_);
    }
}

Statement::Statement(SqliteDB& sdb) : db_(sdb), h_(nullptr), code_(0)
{

}

Statement::Statement(SqliteDB& sdb, const std::string& sql) : db_(sdb), h_(nullptr), code_(0)
{
    prepare(sql);
}

bool Statement::execute()
{
    code_ = sqlite3_step(h_);
    if (code_ != SQLITE_DONE)
    {
        if (code_ != SQLITE_MISUSE)
        {
            const char* msg = sqlite3_errmsg(db_.handle());
            db_.lastError_ = msg;
        }
        else {
            db_.lastError_ = "Sqlite API misuse!";
        }
        return false;
    }
    return true;
}
bool Statement::execute_or_throw()
{
    code_ = sqlite3_step(h_);
    if (code_ != SQLITE_DONE)
    {
        if (code_ != SQLITE_MISUSE)
        {
            const char* msg = sqlite3_errmsg(db_.handle());
            db_.lastError_ = msg;


            throw DBException(db_ );
        }
        else {
            throw DBException("Sqlite API misuse!");
        }
        return false;
    }
    return true;
}

bool Statement::reset()
{
    code_ = sqlite3_reset(h_);
    return (code_ == SQLITE_OK);
}

bool Statement::clear()
{
    code_ = sqlite3_clear_bindings(h_);
    return (code_ == SQLITE_OK);
}

bool Statement::next()
{
    code_ = sqlite3_step(h_);
    return (code_ == SQLITE_ROW);
}

bool Statement::bind(double value, int pos)
{
    code_ = sqlite3_bind_double(h_, pos, value);
    return (code_ == SQLITE_OK);
}

bool Statement::bind(const std::string& text, int pos)
{
    code_ = sqlite3_bind_text(h_, pos, text.c_str(),text.size(), SQLITE_TRANSIENT);
    return (code_ == SQLITE_OK);
}

bool Statement::bind( wxString& text, int pos)
{
    auto buf =  text.utf8_str();
    code_ = sqlite3_bind_text(h_, pos, buf.data(), buf.length(), SQLITE_TRANSIENT);
    return (code_ == SQLITE_OK);
}

bool Statement::bind(const char* s, int pos)
{
    code_ = sqlite3_bind_text(h_, pos, s, strlen(s), SQLITE_STATIC);
    return (code_ == SQLITE_OK);
}
// may be better as int
bool Statement::bind(char& c, int pos)
{
    code_ = sqlite3_bind_text(h_, pos, &c, 1, SQLITE_STATIC);
    return (code_ == SQLITE_OK);
}
/*


bool Statement::bind_static(char& one, int pos)
{
    code_ = sqlite3_bind_text(h_, pos, &one, 1, SQLITE_STATIC);
    return (code_ == SQLITE_OK);
}
bool Statement::bind_static(const std::string& text, int pos)
{
    code_ = sqlite3_bind_text(h_, pos, text.c_str(),text.size(), SQLITE_STATIC);
    return (code_ == SQLITE_OK);
}

bool Statement::bind_char(char one, int pos)
{
    code_ = sqlite3_bind_text(h_, pos, &one, 1, SQLITE_TRANSIENT);
    return (code_ == SQLITE_OK);
}
*/
bool Statement::bindRowId(DBRowId value, int pos)
{
     code_ = sqlite3_bind_int64(h_, pos, value);
     return (code_ == SQLITE_OK);
}

bool Statement::bind(long value, int pos)
{
    code_ = sqlite3_bind_int(h_, pos, value);
    return (code_ == SQLITE_OK);
}

bool Statement::bind(const wxDateTime& value, int pos)
{
    // JDN - 2400000.5,  The MJD 0 is Nov 17, 1858.
    return bind(value.GetModifiedJulianDayNumber(), pos);
}

bool SqliteDB::tableExists(const std::string& table)
{
    Statement query(*this,"SELECT name FROM sqlite_master WHERE type='table' AND name=?");
    query.bind(table, 1);
    return query.next();
}

void
SqliteDB::close()
{
    if (db_ != nullptr)
    {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

void
SqliteDB::path(const std::string& filepath)
{
    this->close();
    filepath_ = filepath;
}

bool
SqliteDB::open()
{
    this->close();
    code_ = sqlite3_open(filepath_.c_str(), &db_);
    return (code_==SQLITE_OK);
}

SqliteDB::SqliteDB() : db_(nullptr),code_(0)
{
}

SqliteDB::~SqliteDB()
{
    this->close();
}



sqlite3_int64 SqliteDB::lastRowId()
{
    return sqlite3_last_insert_rowid(db_);
}
bool SqliteDB::begin()
{
    return this->execute("BEGIN TRANSACTION");
}

bool SqliteDB::commit()
{
    return this->execute("COMMIT");
}
bool SqliteDB::rollback()
{
    return this->execute("ROLLBACK");

}
void SqliteDB::execute_or_throw(const std::string& sql)
{
    char* errmsg = nullptr;
    code_ = sqlite3_exec(db_, sql.c_str(),  nullptr,  nullptr, &errmsg);
    if (errmsg != nullptr)
    {
        lastError_ = errmsg;
        sqlite3_free(errmsg);
        lastSQL_ = sql;
        throw DBException(*this);
    }
}

 bool SqliteDB::execute(const std::string& sql)
 {
    char* errmsg = nullptr;
    code_ = sqlite3_exec(db_, sql.c_str(),  nullptr,  nullptr, &errmsg);
    if (errmsg != nullptr)
    {
        lastError_ = errmsg;
        lastSQL_ = sql;
        sqlite3_free(errmsg);
        return false;
    }
    return true;
 }

 void SqliteDB::allowExtensionLoad(bool onoff)
 {
    code_ = sqlite3_enable_load_extension(db_, (onoff ? 1 : 0));
    this->allowExtensions_ = (onoff && (code_ == SQLITE_OK));
    if (onoff != this->allowExtensions_)
    {
        lastError_ = "Unable to set load_extension";

        throw DBException(*this);
    }

 }
 void SqliteDB::load_extension(const char* libname)
 {
    char* errmsg = nullptr;
    code_ = sqlite3_load_extension(db_, libname,  nullptr, &errmsg);
    if (code_ != SQLITE_OK)
    {
        lastSQL_ = libname;
        if (errmsg != nullptr)
        {
            lastError_ = errmsg;
            sqlite3_free(errmsg);
        }
        throw DBException(*this);
    }
 }


void SqliteDB::appInitialize(int threadUsage)
{
    int result = 0;
    if (threadUsage == 0)
    {
        result = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
        if (result != SQLITE_OK)
        {
            throw DBException("sqlite3_config");
        }
    }
    result = sqlite3_initialize();
    if (result != SQLITE_OK)
    {
        throw DBException("sqlite3_initialize");
    }
}

void SqliteDB::appShutdown()
{
   sqlite3_shutdown();
}


 bool SqliteDB::isThreadSafe()
 {
    return (sqlite3_threadsafe() > 0);
 }


int LoadShapeCmd::execute(SqliteDB& sdb)
{
    if (pkey_.size() > 0)
        return execute_ex(sdb);
    else
        return execute_auto(sdb);
}

int LoadShapeCmd::execute_auto(SqliteDB& sdb)
{
/*
int load_shapefile (sqlite3 * sqlite, char *shp_path,
					   char *table, char *charset, int srid,
					   char *column, int coerce2d,
					   int compressed, int verbose,
					   int spatial_index, int *rows,
					   char *err_msg);
*/
    std::unique_ptr<char> buf(new char[512]);
    int rowCt = 0;
    int result = load_shapefile(sdb.handle(), (char*)filepath_.c_str(), (char*)newtable_.c_str(),  (char*)charset_.c_str(),
                        (int)srid_, (char*)geocolumn_.c_str(),
                        force2d_, false, 0, rtreeIndex_, &rowCt, buf.get());
    if (result == 0)
    {
        sdb.lastError_ = buf.get();
        throw DBException(sdb);
    }
    return rowCt;
}
int LoadShapeCmd::execute_ex(SqliteDB& sdb)
{

    std::unique_ptr<char> buf(new char[512]);
    int rowCt = 0;
    int result = load_shapefile_ex(sdb.handle(), (char*)filepath_.c_str(), (char*)newtable_.c_str(),  (char*)charset_.c_str(),
                        (int)srid_, (char*)geocolumn_.c_str(),
                        (char*) geomtype_.c_str(), (char*) pkey_.c_str(),
                        force2d_, false, 0, rtreeIndex_, &rowCt, buf.get());
    if (result == 0)
    {
        sdb.lastError_ = buf.get();
        throw DBException(sdb);
    }
    return rowCt;
}


