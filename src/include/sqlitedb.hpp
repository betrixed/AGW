#ifndef _H_SQLITE_DB
#define _H_SQLITE_DB
/*
#ifdef SPATIALITE_AMALGAMATION
    #include <spatialite/sqlite3.h>
#else
    #include <sqlite3.h>
#endif


#ifndef SPATIALITE_EXTENSION
    #include <spatialite.h>
#endif
*/

#include "geometry.h"
#include <string>
#include <wx/string.h>
#include <wx/datetime.h>
#include <cstdint>
#include <sstream>

#ifndef _SQLITE3_H_
#include "sqlite3.h"
#endif

typedef sqlite3_int64 DBRowId;

std::string trim(const std::string &s);

class SqliteDB;

class LoadShapeCmd {
public:
    std::string filepath_;
    std::string newtable_;
    std::string geocolumn_;
    std::string charset_;
    int  srid_;
    bool rtreeIndex_;
    bool compress_;
    bool force2d_;

    // for extended call
    std::string pkey_;
    std::string geomtype_;

    int execute(SqliteDB& sdb);
    int execute_ex(SqliteDB& sdb);
    int execute_auto(SqliteDB& sdb);
};

class SqliteDB {
public:
    SqliteDB();
    virtual ~SqliteDB();
    bool tableExists(const std::string& table);
    virtual bool execute(const std::string& sql);
    void execute_or_throw(const std::string& sql);

    virtual bool open();
    void close();

    bool commit();
	bool rollback();
    bool begin();
    DBRowId lastRowId();

    bool allowExtensionLoad() const { return allowExtensions_;}
    void allowExtensionLoad(bool onoff);

    void load_extension(const char* libname);

    bool isOpen() const { return ( db_ != nullptr ); }

    void path(const std::string& filepath);
    const std::string& path() const
    {
        return filepath_;
    }

    sqlite3* handle() { return db_; }

    const std::string& getLastError() { return lastError_; }
    const std::string& getLastSQL() { return lastSQL_; }

    static bool isThreadSafe();
    static void appInitialize(int threadUsage = 1);
    static void appShutdown();
protected:
    std::string  filepath_;
    sqlite3*     db_;
    int code_;
    std::string  lastError_;
    std::string  lastSQL_;
    bool allowExtensions_;
    friend class Statement;
    friend class DBException;
    friend class LoadShapeCmd;

};

#include "msgexception.h"

class DBException : public MsgException {
public:
	DBException(const std::string& msg) : MsgException(msg)
	{
	}

	DBException(const SqliteDB& sdb)
	{
        std::stringstream err;

        err << "SqliteDB exception: " << sdb.lastError_ << std::endl;
        err << " for: " << sdb.lastSQL_;

        msg_ = err.str();
	}
};

class Statement {
public:
    Statement(SqliteDB& sdb, const std::string& sql);
    Statement(SqliteDB& sdb);

    virtual ~Statement();

    void prepare(const std::string& sql);
    bool bind(const std::string& text, int pos);
    bool bind(wxString& text, int pos);
    bool bind(const char* p, int pos);
    bool bind(char& one, int pos);
    // pointer must stick around for execute in bind_static

    /* bool bind_static(const char* s, size_t len, int pos);
    bool bind_static(const std::string& text, int pos);
    bool bind_static(char& one, int pos);*/

    bool bind_char(char one, int pos);
    bool bind(long value, int pos);
    bool bind(const wxDateTime& value, int pos);
    bool bind(double value, int pos);

    bool bindRowId(DBRowId value, int pos);

    const char* getText(int zpos);
    char   getChar(int zpos); // first character of a text type like CHAR(1)
    double getDouble(int zpos);
    int32_t getInt32(int zpos);
    int64_t getInt64(int zpos);
    void get(int zpos, std::string& value);
    void get(int zpos, double& value);
    void get(int zpos, DBRowId& rowid);
    void get(int zpos, char& c);
    void get(int zpos, int32_t& value);

    const void* getBlob(int zpos);
    int  getBlobBytes(int zpos);

    wxDateTime getDateTime(int zpos);
    DBRowId getRowId(int zpos);

    void finalize(); // start from scratch
    bool reset(); // ready go again
    bool clear(); // clear all bindings
    bool next(); // expect SQLITE_ROW
    bool execute();  // expect SQLITE_DONE
    bool execute_or_throw();  // expect SQLITE_DONE
private:
    SqliteDB& db_;
    sqlite3_stmt* h_;

    int code_;
};
#endif //_H_SQLITE_DB
