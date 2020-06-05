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
#include <wx/utils.h>
#include <wx/txtstrm.h>

#include "appdata.h"
#include "testwindowapp.h"
#include "mainframe.h"
#include "loadshapedlg.h"
#include "helper.h"

#include <string>
#include <sstream>
#include <memory>
#include <fstream>
#include <iostream>
#include <exception>

#include "plotxy.h"
#include "plotframe.h"
#include "series.hpp"
#include "luaedit.h"
#include "json/json.h"

const wxString  STD_PLOT_FILES(_("Plot files (*.plot;*.PLOT)|*.plot;*.PLOT"));
const wxString  STD_SCRIPT_FILES(_("Script files (*.lua;*.LUA)|*.lua;*.LUA"));

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


void AppData::readOSInfo()
{
    OSDescription_ = wxGetOsDescription().ToStdString();;

    OSId_ = wxGetOsVersion(&vMajor_, &vMinor_);

    is64Bit_ = wxIsPlatform64Bit();

    isLittleEndian_ = true; //assumed //wxIsPlotformLittleEndian();

    userName_ = wxGetUserName().ToStdString();
    if (userName_.size() == 0)
        userName_ = "?";
    userId_ = wxGetUserId().ToStdString();

    homeDir_ = wxGetHomeDir().ToStdString();

    host_ = wxGetFullHostName().ToStdString();
    wxLogMessage("Checking Environment . . ." );
    wxLogMessage("Compiled with %s",wxVERSION_STRING);

    wxLogMessage("OS is %s", OSDescription_.c_str());
    wxLogMessage("User name is %s", userName_.c_str());
    wxLogMessage("User Id is %s", userId_.c_str());
    wxLogMessage("Home Dir is %s", homeDir_.c_str());
    wxLogMessage("Host name is %s", host_.c_str());


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
    std::ofstream outjson(jpath);

    outjson << user_;
}

std::string AppData::getUserConfigPath()
{
    wxFileName userPath;

    userPath.Assign(this->userDataDir_, _T("user.json"));
    wxString jpath = userPath.GetFullPath();

    return jpath.ToStdString();
}

bool AppData::userValue(const std::string& key, std::string& value) const
{
   if (user_.isMember(key)) {
    value = user_[key].asString();
    return true;
   }
   return false;
}

void AppData::setUserValue(const std::string& key, const std::string& value)
{
    user_[key] = value;
}

void AppData::readUserConfig()
{
    std::string jpath = getUserConfigPath();
    wxLogMessage("user config path: %s", jpath.c_str() );

    if (!wxFileExists(jpath))
    {
        user_ = Json::Value(Json::objectValue);
        user_["db-file"] = "climate.sqlite3";
        saveUserConfig();
        return;
    }

    std::ifstream ifs(jpath);

    ifs >> user_;

}


SeriesPtr AppData::getGlobal(const char* s)
{
    auto mit = global_.find(std::string(s));
    if (mit != global_.end())
    {
        return (*mit).second;
    }
    SeriesPtr sp;
    if (!root_.isMember("global"))
    {
        return sp;
    }

    Json::Value g = root_["global"];
    if (g.isObject() && g.isMember(s))
    {
        Json::Value js = g[s];
        if (js.isObject() && js.isMember("jype"))
        {
            sp = readJSONSeries(js);
            if (sp != nullptr)
            {
                std::pair<std::string, SeriesPtr> ins(s, sp);
                global_.insert(ins);
            }
        }
    }
    return sp;
}

void AppData::readConfig()
{
    wxFileName configPath;
    configPath.Assign(appDataDir_, _T("config.json"));
    std::string jsonPath = configPath.GetFullPath().ToStdString();

    wxLogMessage("JSON path %s",jsonPath.c_str() );

    std::ifstream injson(jsonPath);

    injson >> root_;

}
void AppData::readScript(const std::string& path)
{
    wxFileInputStream input(path);
    wxTextInputStream text(input, wxT("\x09"), wxConvUTF8 );

    wxFileName fn;

    wxString wpath = path;
    fn.Assign(wpath);
    std::string tabname = fn.GetName().ToStdString();

    LuaEdit* led = this->mainFrame()->createLuaEdit(tabname);
    auto ctrl = led->luaEdit_;

    while(input.IsOk() && !input.Eof() )
    {
      wxString line=text.ReadLine();
      line << "\n";
      ctrl->AppendText(line);

    }

    led->SetFilePath(wpath);
}

void AppData::readPlot(const std::string& path)
{
    PlotLua_sptr pp = PlotLua::create_sptr();

    PlotLua* pl = pp.get();

    pl->readFile(path);

    pl->showPlot(pp,true);
}
bool AppData::getLuaFileSave(wxString& ipath)
{
    std::unique_ptr<wxFileDialog> savedlg(new wxFileDialog(this->mFrame, wxString(_("Save as")), lastScriptDir_, wxEmptyString,
            STD_SCRIPT_FILES, wxFD_SAVE | wxFD_OVERWRITE_PROMPT));
    if (savedlg->ShowModal() == wxID_CANCEL)
        return false;

    wxString path = savedlg->GetPath();

    ensureExtension(path,"lua");

    ipath = path;

    this->lastScriptDir(path);
    return true;
}

void AppData::saveLuaScript(const wxString& script, const wxString& path)
{
    std::ofstream fout(path.ToStdString());

    fout << script.ToStdString();

    fout.close();
}

bool AppData::getPlotFileSave(wxString& ipath)
{
    std::unique_ptr<wxFileDialog> savedlg(new wxFileDialog(this->mFrame, wxString(_("Save as")), lastPlotDir_, wxEmptyString,
            STD_PLOT_FILES, wxFD_SAVE | wxFD_OVERWRITE_PROMPT));
    if (savedlg->ShowModal() == wxID_CANCEL)
        return false;

    wxString path = savedlg->GetPath();

    ensureExtension(path,"plot");

    ipath = path;

    this->lastPlotDir(path);
    return true;
}

void AppData::lastPlotDir(const wxString& path)
{
    wxFileName check(path);
    lastPlotDir_ = check.GetPathWithSep();
}
void AppData::lastScriptDir(const wxString& path)
{
    wxFileName check(path);
    lastScriptDir_ = check.GetPathWithSep();
}
void AppData::openPlotFile()
{
    std::string ipath;
    //ipath = "/home/michael/dev/AGW/AGW/test.plot";
    if (!getPlotFile(ipath))
        return;
    readPlot(ipath);
}

void AppData::openScriptFile()
{
    std::string ipath;
    //ipath = "/home/michael/dev/AGW/AGW/test.plot";
    if (!getScriptFile(ipath))
        return;
    readScript(ipath);
}

bool
AppData::luaFileChooser(
    std::string msg,
    std::string extension,
    std::string& retPath)
{
    wxString lastFolder = lastScriptDir();

    wxString prompt = wxString(msg);

    std::stringstream fileTypes;

    fileTypes << "For Script (*." << extension << ")|*." << extension;

    wxString fileKinds = wxString(fileTypes.str());

    wxFileDialog* dlg = new wxFileDialog(this->mFrame, prompt, lastFolder, wxEmptyString,
                       fileKinds, wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (dlg->ShowModal() == wxID_CANCEL)
    {
        retPath = "";
        return false;     // the user changed mind...
    }

    wxString path = dlg->GetPath();

    this->lastScriptDir(path);

    retPath = path.ToStdString();

    dlg->Destroy();
    return true;

}

bool AppData::getScriptFile(std::string& ipath)
{
    wxString lastFolder = lastScriptDir();

    wxFileDialog* dlg = new wxFileDialog(this->mFrame, _("Open Script File"), lastFolder, wxEmptyString,
                       STD_SCRIPT_FILES, wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (dlg->ShowModal() == wxID_CANCEL)
        return false;     // the user changed idea...

    wxString path = dlg->GetPath();

    this->lastScriptDir(path);

    ipath = path.ToStdString();

    dlg->Destroy();
    return true;

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

/*!
    \brief This should be called after creating AppData object.

    Establish the wxWidgets application object, and the main window.
    Get information about the execution environment, configuration, user,
    and current database location.

*/
void AppData::initPaths(TestWindowApp *app, MainFrame* mf)
{
    mf->ap_ = this;


    this->theApp = app;
    this->mFrame = mf;

    readOSInfo();


    switch(OSId_)
    {
    case wxOS_UNIX_LINUX:
    case wxOS_UNIX_FREEBSD:
    case wxOS_UNIX_OPENBSD:
    case wxOS_UNIX_NETBSD:
    case wxOS_MAC_OSX_DARWIN:
        setupUnixPaths();
        break;
    default:
        setupWindowsPaths();
        break;
    }


    mf->log_->Show(false);

}

void AppData::setupUnixPaths()
{
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

    wxFileName dataPath;

    dataPath.AssignDir(homeDir_);

    dataPath.AppendDir(".local");
    dataPath.AppendDir("share");
    dataPath.AppendDir("AGW");


    userDataDir_ = dataPath.GetFullPath();

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
}

void AppData::setupWindowsPaths()
{
}

void AppData::registerView(SeriesFrame* sf)
{
    //auto it = views_.begin();
    //auto fin = views_.end();

}
void AppData::registerView(PlotFrame* pf)
{

}

