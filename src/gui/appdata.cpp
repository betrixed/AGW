#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstopcd
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/stdpaths.h>

#include "appdata.h"
#include "testwindowapp.h"
#include "mainframe.h"
#include "loadshapedlg.h"
#include "helper.h"

#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <exception>

#include "plotxy.h"
#include "plotframe.h"
#include "series.hpp"

#include "json/json.h"

const wxString  STD_PLOT_FILES(_("Plot files (*.plot;*.PLOT)|*.plot;*.PLOT"));

#include "plotlua.h"
#include <spatialite.h>
#include "libshape.hpp"

using namespace agw;

Database& AppData::getDB()
{
    return db_;
}

AppData* AppData::gAppData = nullptr;

AppData::AppData()
{
    AppData::gAppData = this;
    ready_ = false;
}

AppData::~AppData()
{
    AppData::gAppData = nullptr;
}

void AppData::runLuaScript(const std::string& script)
{
    try {
        if (!engine_.IsReady())
        {
            engine_.Init();
        }

        fprintf(stdout,"%s",script.c_str());
        fflush(stdout);

        std::cout << ">>" << script << std::endl;

        engine_.Run(script.c_str(), script.size(), "chunk");
    }
    catch(std::exception& ex)
    {
        wxLogMessage("std::exception: %s", ex.what());
    }

}

void AppData::exit()
{
    db_.close();
    spatialite_shutdown();
    SqliteDB::appShutdown();
}

bool getJSONArrayString(wxArrayString& data, Json::Value& jarr)
{
    int setSize = jarr.size();
    for (int i = 0; i < setSize; i++)
    {
        std::string value = jarr[i].asString();
        wxLogMessage("Add %s", value.c_str());
        data.Add(value);
    }
    return (setSize > 0);
}

void AppData::saveUserConfig()
{
    std::string jpath = getUserConfigPath();

    std::ofstream os(jpath);

    os << user_ << std::endl;
}

std::string AppData::getUserConfigPath()
{
    wxFileName userPath;

    userPath.Assign(this->userDataDir_, _T("user.json"));
    wxString jpath = userPath.GetFullPath();

    return jpath.ToStdString();
}

void AppData::readUserConfig()
{
    std::string jpath = getUserConfigPath();

    wxLogMessage("JSON user config path: %s", jpath.c_str() );

    if (!wxFileExists(jpath))
    {
        user_ = Json::Value(Json::objectValue);
        user_["db-file"] = "climate.sqlite3";
        saveUserConfig();
        return;
    }

    std::ifstream ifs(jpath);

    Json::Reader jr;

    jr.parse(ifs,user_);

}
void AppData::readConfig()
{
    wxFileName configPath;
    configPath.Assign(appDataDir_, _T("config.json"));
    std::string jsonPath = configPath.GetFullPath().ToStdString();

    wxLogMessage("JSON path %s",jsonPath.c_str() );

    std::ifstream injson(jsonPath);

    Json::Reader    jsonrdr;

    jsonrdr.parse(injson, root_);

}

void AppData::readPlot(const std::string& path)
{

    PlotLuaPtr pp = std::make_shared<PlotLua>();

    PlotLua* pl = pp.get();

    pl->readFile(path);

    pl->showPlot(pp,true);
}

void AppData::lastPlotDir(const wxString& path)
{
    wxFileName check(path);
    lastPlotDir_ = check.GetPath();
}

void AppData::openPlotFile()
{
    std::string ipath;
    //ipath = "/home/michael/dev/AGW/AGW/test.plot";
    if (!getPlotFile(ipath))
        return;
    readPlot(ipath);
}

bool AppData::getPlotFile(std::string& ipath)
{
    wxString lastFolder = lastPlotDir();

    wxFileDialog* dlg = new wxFileDialog(this->mFrame, _("Open Plot File"), lastFolder, wxEmptyString,
                       STD_PLOT_FILES, wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (dlg->ShowModal() == wxID_CANCEL)
        return false;     // the user changed idea...

    wxString path = dlg->GetPath();

    this->lastPlotDir(path);

    ipath = path.ToStdString();

    dlg->Destroy();
    return true;

}

void AppData::loadShapeFile()
{
    static wxString lastDir = wxEmptyString;

    wxFileDialog openFileDialog(this->mFrame, _("Open Shape File"), lastDir, wxEmptyString,
                       _("Shape files (*.shp;*.SHP)|*.shp;*.SHP"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    wxFileName path = openFileDialog.GetPath();
    lastDir = path.GetPath();
    wxString baseName = path.GetName();
    wxFileName dbpath(lastDir, baseName);
    wxString dbf_path = dbpath.GetFullPath();

    std::string dbpath_c(std::move(dbf_path.ToStdString()));
    shp::ShapeDBF shapeDatabase(dbpath_c);
    const size_t fieldCt = shapeDatabase.nFields();

    if (fieldCt > 0)
    {
        std::unique_ptr<LoadShapeDlg> dlg(
            new LoadShapeDlg(this->mFrame,LoadShapeDlg::ID_LOADSHAPEDLG,
                SYMBOL_LOADSHAPEDLG_TITLE,
                SYMBOL_LOADSHAPEDLG_POSITION,
                SYMBOL_LOADSHAPEDLG_SIZE));


        for(size_t i = 0; i < fieldCt; i++)
        {
            std::string fspec = shapeDatabase.fieldInfo(i)->sqlField();
            dlg->dbFields.Add(fspec);
        }

        setEditCtrl(dlg.get(), LoadShapeDlg::ID_LDSHP_PATH, dbf_path);
        setEditCtrl(dlg.get(), LoadShapeDlg::ID_LDSHP_TABLE, baseName);
        setEditCtrl(dlg.get(), LoadShapeDlg::ID_LDSHP_COLUMN, _T("Geometry"));



        if (!getJSONArrayString(dlg->charsetTypes, root_["charset-list"]))
        {
            wxLogMessage("JSON parse charset-list failure");
        }
        if (!getJSONArrayString(dlg->geomTypes, root_["geom-type-list"]))
        {
            wxLogMessage("JSON parse geom-type-list failure");
        }
        wxListBox* box = getWindowByID<wxListBox>(dlg.get(), LoadShapeDlg::ID_LDSHP_CHARSET);
        if (box)
            initListBox(box, dlg->charsetTypes);

        int result = dlg->ShowModal();

        if (result != wxID_OK)
        {
            return;
        }
        LoadShapeCmd cmd;

        dlg->SaveIn(cmd);

        wxLogMessage("Attempt load shape file as %s", cmd.newtable_.c_str());
        try {
            int rowct = cmd.execute(db_);
            wxLogMessage("%d rows created ", rowct);
        }
        catch(DBException& ex)
        {
            wxLogMessage(ex.msg());
        }
    }
}


void AppData::createCountryTable()
{
    if (!db_.tableExists("country"))
    {


        LoadShapeCmd cmd;

        try {
            if (root_.isMember("country"))
            {

                const Json::Value& country = root_["country"];

                cmd.pkey_ = country["pkey"].asString();
                cmd.geocolumn_ = "Geometry";
                cmd.srid_ = country["srid"].asInt();
                cmd.newtable_ = "country";

                wxFileName spath;

                spath.AssignDir(appDataDir_);

                wxString mapdir = country["mapdir"].asString();
                spath.AppendDir(mapdir);
                spath.SetName(country["shapefile"].asString());

                cmd.filepath_ = spath.GetFullPath().ToStdString();

                wxLogMessage("dataDir %s , mapdir %s", appDataDir_.c_str(), mapdir.c_str());

                cmd.rtreeIndex_ = true;
                cmd.geomtype_ = "AUTO";

                int rowct = cmd.execute(db_);
                wxLogMessage("country table with %d rows", rowct);
            }
            else {
                std::stringstream ss;

                ss << root_;

                wxLogMessage("Configuation dump : %s", ss.str().c_str());
            }
        }
        catch(DBException& ex)
        {
            wxLogMessage("country table create fail: %s", ex.msg());
        }

    }
}
void AppData::createDB(std::string& mydb)
{

    // special init for spatialite extensions
    wxLogMessage("First time initialize for spatialite/sqlite3, please wait . . .");

// put up a modal dialog progress

}


void AppData::callOpenDB()
{
    try {
        db_.path(mydbpath_);
        db_.open();
        wxLogMessage("spatialite data open");
        createCountryTable();
        db_.init();
        ready_ = true;

    }
    catch(DBException &ex)
    {
        wxLogMessage("spatialite open : %s", ex.msg());
    }

}
void AppData::openDB()
{
    readUserConfig();
    readConfig();

    std::string dbfile = user_["db-file"].asString();

    wxFileName db_path;
    db_path.Assign(userDataDir_, dbfile);
    wxString dbfilepath = db_path.GetFullPath();
    spatialite_initialize ();
    mydbpath_ = dbfilepath.ToStdString();

    // normal opening but enable extension

    // setup SQLite
    bool canMultiThreadDB = SqliteDB::isThreadSafe();
    wxLogMessage("SQLITE Threadsafe = %d", (int) canMultiThreadDB);
    SqliteDB::appInitialize();

    wxLogMessage("Path to climate database: %s", mydbpath_.c_str());
    if (!wxFileExists(dbfilepath))
    {
        createDB(mydbpath_);
        // first time will run on a thread
        // callOpenDB() on thread finish
    }
    else {
        callOpenDB();
    }
}

CountryMapPtr AppData::getCountryMap()
{
    if (this->countryMap_ != nullptr)
        return this->countryMap_;

    this->countryMap_ = std::make_shared<CountryLayer>();

    try {

        // get the specific record Geometry data
        Statement gq(db_, "select Geometry, LONG_NAME from country");

        //gq.bindRowId(loc->id, 1);

        while (gq.next())
        {
            const void* data = gq.getBlob(0);
            int   dsize = gq.getBlobBytes(0);
            agw::GeomReader reader(data, dsize);
            int32_t srid;
            agw::GRect  bound;
            int32_t gm_class;
            agw::GPoint pt;
            agw::GMultiPoly mp;

            reader.readGType(srid,bound,gm_class);

            std::unique_ptr<CountryRecord> crec(new CountryRecord());

            gq.get(1,crec->name_);

            if (gm_class == agw::POINT)
            {
                reader.readPoint(pt);
            }
            else if (gm_class == agw::MPOLY)
            {
                reader.readMultiPoly(crec->area_);
                crec->area_.MBR = bound;
            }

            countryMap_->clist.push_back(std::move(crec));
        }

        wxLogMessage("Countries loaded");
    }
    catch(DBException &ex)
    {
        wxLogMessage("Countries Load error %s", ex.msg());
    }
    return this->countryMap_;
}


void AppData::initPaths(TestWindowApp *app, MainFrame* mf)
{
    mf->ap_ = this;


    this->theApp = app;
    this->mFrame = mf;

    auto stdpaths = wxStandardPaths::Get();
    appPath_ = stdpaths.GetExecutablePath();
    wxLogMessage("Executing %s", appPath_.c_str());
#if defined(__WXGTK__)
    prefix_= stdpaths.GetInstallPrefix();
    wxLogMessage("Prefix path %s", prefix_.c_str());
#endif
    
    userDataDir_ = stdpaths.GetUserDataDir();
    appDataDir_ = stdpaths.GetDataDir();
    startWorkDir_ = wxGetCwd();

    wxLogMessage("User data dir %s", userDataDir_.c_str());
    wxLogMessage("App data dir %s", appDataDir_.c_str());
    wxLogMessage("Start working dir %s", startWorkDir_.c_str());
    if (!wxDirExists(userDataDir_))
    {
        wxLogMessage("Make directory : %s",userDataDir_.c_str() );
        wxMkDir(userDataDir_, 0777);
    }
    if (!wxDirExists(appDataDir_))
    {
         wxLogMessage("Make directory : %s",appDataDir_.c_str() );
         wxMkDir(appDataDir_, 0777);
    }

    mf->log_->Show(false);

}

void AppData::registerView(SeriesFrame* sf)
{
    //auto it = views_.begin();
    //auto fin = views_.end();

}
void AppData::registerView(PlotFrame* pf)
{

}

