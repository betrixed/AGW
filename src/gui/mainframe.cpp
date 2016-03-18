/////////////////////////////////////////////////////////////////////////////
// Name:        mainframe.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     31/10/2013 06:55:37
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 31/10/2013 06:55:37

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "wx/imaglist.h"
#include "../include/luaedit.h"
////@end includes

#include "mainframe.h"
#include "loadshapedlg.h"
#include "database.hpp"
#include <wx/filedlg.h>
#include "wxCSV.hpp"
#include <wx/wfstream.h>
#include <wx/arrstr.h>
#include <wx/filename.h>
#include <vector>
#include <wx/txtstrm.h>
#include <wx/sstream.h>
#include <wx/progdlg.h>

////@begin XPM images
////@end XPM images

#include "selectseries.h"
#include "series.hpp"
#include "largeplot.h"
#include "plotxy.h"

#include "stationsets.h"
#include <sstream>
#include <exception>
#include "appdata.h"


#include <wx/aui/auibook.h>

enum {
    WORKER_EVENT = wxID_HIGHEST+1,
};

using namespace agw;
/*s
 * MainFrame type definition
 */

IMPLEMENT_CLASS( MainFrame, wxDocParentFrame )

//wxDECLARE_EVENT(wxEVT_GISS_THREAD_COMPLETED, wxThreadEvent);
//wxDECLARE_EVENT(wxEVT_GISS_THREAD_UPDATE, wxThreadEvent);

/*
 * MainFrame event table definition
 */

BEGIN_EVENT_TABLE( MainFrame, wxDocParentFrame )
    EVT_CLOSE(MainFrame::OnClose)

////@begin MainFrame event table entries
    EVT_WINDOW_DESTROY( MainFrame::OnDestroy )
    EVT_MENU( ID_NEW_SCRIPT, MainFrame::OnNewScriptClick )
    EVT_MENU( wxID_OPEN, MainFrame::OnOpenClick )
    EVT_MENU( ID_RUN_LUA, MainFrame::OnRunLuaClick )
    EVT_MENU( wxID_EXIT, MainFrame::OnExitClick )
    EVT_MENU( ID_VIEW_SERIES, MainFrame::OnViewSeriesClick )
    EVT_MENU( ID_MENUITEM, MainFrame::OnMenuitemClick )
    EVT_MENU( ID_BASELINE, MainFrame::GenBaseline )
    EVT_MENU( ID_IMPORT_GISS, MainFrame::OnImportGissClick )
    EVT_MENU( ID_GISS_STATION, MainFrame::OnGissImportStations )
    EVT_MENU( ID_IMPORT_SHAPE_FILE, MainFrame::OnImportShapeFileClick )
    EVT_MENU( ID_FIND_STATION, MainFrame::OnFindStationClick )
    EVT_MENU( ID_SETS, MainFrame::OnSetsClick )
    EVT_MENU( ID_LOG_VIEW, MainFrame::OnLogViewClick )
    EVT_MENU( ID_LUA_COMMAND, MainFrame::OnLuaCommandClick )
    EVT_MENU( ID_ABOUT, MainFrame::OnAboutClick )
    EVT_MENU( ID_OPEN_PLOTFILE, MainFrame::OnOpenPlotfileClick )
////@end MainFrame event table entries
    EVT_THREAD(WORKER_EVENT, MainFrame::OnWorkerEvent)
END_EVENT_TABLE()


 class ImportThread : public wxThread
 {
 public:
    ImportThread(MainFrame *handler,
            const std::string& dbPath,
            const std::string& importPath)
        : wxThread(), m_pHandler(handler), dbPath_(dbPath), importPath_(importPath)
        {
        }

    virtual ~ImportThread()
    {
        wxCriticalSectionLocker enter(m_pHandler->threadCS_);
    // the thread is being destroyed; make sure not to leave dangling pointers around
        m_pHandler->mThread_ = nullptr;
    }

 protected:
    // send a number from 0 to 100: -1 means cancel.  100 means complete.

    void UpdateProgress(int value)
    {
        wxThreadEvent event( wxEVT_THREAD, WORKER_EVENT );
        event.SetInt(value); // reached maximum
        wxQueueEvent( m_pHandler, event.Clone() );
    }

    MainFrame *m_pHandler;
    std::string dbPath_;
    std::string importPath_;
 };

 class IdleTimer : public wxTimer
{
protected:
    MainFrame* notify_;
public:
    IdleTimer(MainFrame* owner);
    virtual ~IdleTimer();
    virtual void Notify();
};

 class NewDBThread : public ImportThread {
  public:
    // importPath is location of country shapefile
    NewDBThread(MainFrame *handler,
            const std::string& dbPath,
            const std::string& importPath)
            : ImportThread(handler, dbPath, importPath)
        {
        }

     virtual ExitCode Entry() wxOVERRIDE;
 };

 class ImportGissThread : public ImportThread {
 public:
    ImportGissThread(MainFrame *handler,
            const std::string& dbPath,
            const std::string& importPath)
            : ImportThread(handler, dbPath, importPath)
        {
        }

     virtual ExitCode Entry() wxOVERRIDE;
 };

 class ImportStationThread : public ImportThread {
 public:
    ImportStationThread(MainFrame *handler,
            const std::string& dbPath,
            const std::string& importPath)
        : ImportThread(handler, dbPath, importPath)
        {
        }

    virtual ExitCode Entry() wxOVERRIDE;
 };

wxThread::ExitCode NewDBThread::Entry()
{

    wxLogMessage("Thread Entry");
    if (this->TestDestroy())
        return (wxThread::ExitCode)0;
    Database sdb;
    UpdateProgress(5);
    if (this->TestDestroy())
        return (wxThread::ExitCode)0;
    try {
        sdb.path(dbPath_);
        sdb.open();
        Database::AutoClose dbscope(sdb);
        sdb.allowExtensionLoad(true);

        sdb.execute_or_throw("Select InitSpatialMetaData()"); // this will take nearly all of the time
        sdb.allowExtensionLoad(false);
        wxLogMessage("spatialite init completed");
    }
    catch(DBException &ex)
    {
        wxLogMessage("Exception in spatialite create init : %s", ex.msg());
    }

    UpdateProgress(100);
    UpdateProgress(-1);
    return (wxThread::ExitCode)0;
}

wxThread::ExitCode ImportGissThread::Entry()
 {

    wxFileInputStream input_stream(importPath_);

    double fileLength = input_stream.GetLength();
    if (!input_stream.IsOk())
    {
        return (wxThread::ExitCode)0;
    }
    Database sdb_;
    sdb_.path(dbPath_);
    sdb_.open();

    Database::AutoClose onScopeExit(sdb_);
    int updateCt = 0;
    try {
        wxTextInputStream   text(input_stream, ',');

        bool hasNext = true;


        int lineCount = 0; // commit load indicator
        int maxLine = 0;

        while(hasNext && !this->TestDestroy())
        {
            //DBRowId stationId;

            auto chars = text.ReadLine();
            int  lineSize = chars.size();
            if (maxLine < lineSize)
                maxLine = lineSize;

            hasNext = (chars.size() > 114);
            if (hasNext)
            {

                auto stationCode = chars.substr(0,11).ToStdString();

                auto yearText = chars.substr(11,4);
                long yearNum;
                yearText.ToLong(&yearNum);

                auto measure = chars.substr(15,4);
                MTEMP startIX = TAVG;
                if (measure=="TMIN")
                {
                    startIX = TMIN;
                }
                else if (measure=="TMAX")
                {
                    startIX = TMAX;
                }
                int validCount = 0;
                MonthTemp  mt;

                try {
                    // make foreigns key for station-year
                    GissYear yr;
                    yr.setId(stationCode, yearNum, startIX); // station, year, measure
                    yr.locId = GissLocation::MakeRowId(stationCode);
                    yr.year = yearNum;

                    if (lineCount == 0)
                    {

                        double percentDone = (input_stream.TellI() * 100.0) / fileLength;
                        UpdateProgress(wxRound(percentDone));
                        //m_pHandler->C allAfter(&MainFrame::ImportProgress,m_pHandler,percentDone);
                        sdb_.begin();
                    }
                    lineCount++;
                    for(int i = 0; i < 12; i++)
                    {

                        auto offset = 19 + i * 8;
                        auto valueStr = chars.substr(offset,5);
                        if (valueStr != "-9999")
                        {
                            validCount++;
                            offset += 5;
                            mt.dmFlag = chars.at(offset);
                            mt.qcFlag = chars.at(offset+1);
                            mt.dsFlag = chars.at(offset+2);
                            if (valueStr.ToDouble(&mt.value))
                            {
                                mt.value /= 100.0;
                                mt.setId(stationCode,yearNum,startIX,i+1);
                                mt.save(sdb_);
                            }
                        }
                    }
                    if (validCount > 0)
                    {

                        yr.valuesCt = validCount;
                        yr.measure = measure;
                        yr.year = yearNum;
                        yr.save(sdb_);
                        updateCt++;
                    }
                    if (lineCount >= 2000)
                    {

                        sdb_.commit();

                        lineCount = 0;
                    }
                }
                catch(std::exception& ex)
                {
                    sdb_.rollback();
                    lineCount = 0;
                    return (wxThread::ExitCode)-1;
                }
                // save the year record as well

            }
        }
        UpdateProgress(100);

        if (lineCount > 0)
        {
            sdb_.commit();
            lineCount = 0;

        }
        wxLogMessage("maxLine length = %d", maxLine);
    }
    catch(DBException& ex)
    {
        wxLogMessage("Sqlite3 Exception %s", ex.msg());
    }

    UpdateProgress(-1);

    wxLogMessage("Updated %d records", updateCt);
    return (wxThread::ExitCode)0;
 }


void MainFrame::OnClose(wxCloseEvent& event)
{

    {
        wxCriticalSectionLocker enter(threadCS_);

        if (mThread_)
        {
            if (mThread_->Delete() != wxTHREAD_NO_ERROR)
            {
                wxLogError("Cannot delete the thread!");
            }
        }
    }

    while(1)
    {
        {
        wxCriticalSectionLocker enter(threadCS_);
        if (!mThread_)
            break;
        }
        //wxThread::This()->Sleep(1);
    }
    if (event.GetEventObject() == this)
    {
        Destroy();
    }
}
void MainFrame::DoPauseThread()
{

}
/*
 * MainFrame constructors
 */

MainFrame::MainFrame( wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
    : wxDocParentFrame( manager, parent, id, caption, pos, size, style )
{
    Init();
    Create( manager, parent, id, caption, pos, size, style );
}


/*
 * MainFrame creator
 */

bool MainFrame::Create( wxDocManager *manager, wxFrame *parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MainFrame creation
    SetParent(parent);
    CreateControls();
    Centre();
////@end MainFrame creation

    return true;
}


/*
 * MainFrame destructor
 */

MainFrame::~MainFrame()
{
////@begin MainFrame destruction
////@end MainFrame destruction
}


/*
 * Member initialisation
 */

void MainFrame::Init()
{
    ap_ = nullptr;
    log_ = nullptr;
////@begin MainFrame member initialisation
    book_ = NULL;
    mLuaRun = NULL;
    mLuaLog = NULL;
    mEnter = NULL;
    btnRun = NULL;
    mImport = NULL;
    mHelp = NULL;
////@end MainFrame member initialisation

    timer_ = nullptr;
    timerState_ = -1;
}


/*
 * Control creation for MainFrame
 */

void MainFrame::CreateControls()
{
////@begin MainFrame content construction
    MainFrame* itemDocParentFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu11 = new wxMenu;
    itemMenu11->Append(ID_NEW_SCRIPT, _("&New Script"), wxEmptyString, wxITEM_NORMAL);
    itemMenu11->Append(wxID_OPEN, _("&Open"), wxEmptyString, wxITEM_NORMAL);
    itemMenu11->Append(ID_RUN_LUA, _("&Run"), wxEmptyString, wxITEM_NORMAL);
    itemMenu11->AppendSeparator();
    itemMenu11->Append(wxID_EXIT, _("E&xit"), wxEmptyString, wxITEM_NORMAL);
    itemMenu11->Append(ID_VIEW_SERIES, _("View Series"), wxEmptyString, wxITEM_NORMAL);
    wxMenu* itemMenu18 = new wxMenu;
    itemMenu18->Append(ID_MENUITEM, _("&Aussie CSV"), wxEmptyString, wxITEM_NORMAL);
    itemMenu18->Append(ID_BASELINE, _("Generate baseline"), wxEmptyString, wxITEM_NORMAL);
    itemMenu11->Append(ID_IMPORT_MENU, _("Setup"), itemMenu18);
    menuBar->Append(itemMenu11, _("&File"));
    mImport = new wxMenu;
    mImport->Append(ID_IMPORT_GISS, _("GISS Month Data"), wxEmptyString, wxITEM_NORMAL);
    mImport->Append(ID_GISS_STATION, _("GISS Station Data"), wxEmptyString, wxITEM_NORMAL);
    mImport->AppendSeparator();
    mImport->Append(ID_IMPORT_SHAPE_FILE, _("Shape File (.SHP)"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(mImport, _("Import"));
    wxMenu* itemMenu26 = new wxMenu;
    itemMenu26->Append(ID_FIND_STATION, _("Find Station"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu26, _("Search"));
    wxMenu* itemMenu28 = new wxMenu;
    itemMenu28->Append(ID_SETS, _("Station subsets"), wxEmptyString, wxITEM_NORMAL);
    itemMenu28->Append(ID_LOG_VIEW, _("Log"), wxEmptyString, wxITEM_NORMAL);
    itemMenu28->Append(ID_LUA_COMMAND, _("Lua Command"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu28, _("Window"));
    mHelp = new wxMenu;
    mHelp->Append(ID_ABOUT, _("About"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(mHelp, _("Help"));
    wxMenu* itemMenu34 = new wxMenu;
    itemMenu34->Append(ID_OPEN_PLOTFILE, _("Open Plot "), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu34, _("Plot"));
    itemDocParentFrame1->SetMenuBar(menuBar);

    book_ = new wxAuiNotebook( itemDocParentFrame1, ID_AUINOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE|wxAUI_NB_TOP );

    mLuaRun = new wxPanel( book_, ID_LUA_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    mLuaRun->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    mLuaRun->SetSizer(itemBoxSizer4);

    mLuaLog = new wxTextCtrl( mLuaRun, ID_LUA_LOG, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemBoxSizer4->Add(mLuaLog, 1, wxGROW, 0);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer6, 0, wxGROW|wxLEFT|wxRIGHT, 0);
    mEnter = new wxTextCtrl( mLuaRun, ID_ENTER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemBoxSizer6->Add(mEnter, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

    btnRun = new wxButton( mLuaRun, ID_RUN, _("Run"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer6->Add(btnRun, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    book_->AddPage(mLuaRun, _("Lua"), false);

    LuaEdit* itemPanel9 = new LuaEdit( book_, LuaEdit::ID_NEW_LUA_EDIT, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel9->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    book_->AddPage(itemPanel9, _("New"), false);

    // Connect events and objects
    itemDocParentFrame1->Connect(ID_MAINFRAME, wxEVT_DESTROY, wxWindowDestroyEventHandler(MainFrame::OnDestroy), NULL, this);
////@end MainFrame content construction

    log_ = new wxLogWindow(nullptr, "Log Window", true,false);
    auto frame = log_->GetFrame();
    frame->SetClientSize(wxSize(400,500));

    logOut_ = std::unique_ptr<wxStreamToTextRedirector>(new wxStreamToTextRedirector(mLuaLog , &std::cout));
    logError_ = std::unique_ptr<wxStreamToTextRedirector>(new wxStreamToTextRedirector(mLuaLog , &std::cerr));
}


/*
 * Should we show tooltips?
 */

bool MainFrame::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap MainFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MainFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end MainFrame bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon MainFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MainFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MainFrame icon retrieval
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void MainFrame::OnExitClick( wxCommandEvent& event )
{
    event.Skip();
    this->Close();
}


/*
 * wxEVT_DESTROY event handler for ID_MAINFRAME
 */

void MainFrame::OnDestroy( wxWindowDestroyEvent& event )
{
    event.Skip();

}



/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM
 */

void MainFrame::OnMenuitemClick( wxCommandEvent& event )
{

    event.Skip();
    wxString filepath;

    if (SelectImportFile(filepath))
    {
        ImportCSV(filepath);
    }

}
bool MainFrame::SelectImportFile(wxString& filepath)
{
    wxFileDialog   openFileDialog(this, _("Select CSV file"), "", "",
            "CSV files (*.csv)|*.csv", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return false; // the user

    filepath = openFileDialog.GetPath();
    return true;
}

static wxString getFullFileName(const wxString& path)
{
    wxFileName fn(path);
    return fn.GetFullName();
}
static void showRow(wxArrayString& row)
{
    wxString outs;
    for(size_t i = 0; i < row.size(); i++)
    {
        outs << "[";
        outs << row[i];
        outs << "],";
    }
    wxLogMessage("%s",outs);
}
 Database& MainFrame::getDB()
 {
    return this->ap_->getDB();
 }
 bool MainFrame::ImportCSV(const wxString& filepath)
 {
    Database& db = this->getDB();

    wxFileInputStream input_stream(filepath);
    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", filepath);
        return false;
    }
	std::string fileroot = getFullFileName(filepath).ToStdString();
    if (db.hasImportFile(fileroot))
    {
        wxLogError("Already have '%s'.", fileroot);
        return false;
    }

    wxTextInputStream   text(input_stream, ',');
    wxCSV rdr(&text, ',');
    wxArrayString row;
    if(rdr.next(row))
    {
        if (row.size() < 7)
        {
            wxLogError("Expected header row ");
            showRow(row);
            return false;
        }
        auto AllGood = true;
        if (row[0] != "Product code")
        {
            AllGood = false;
            wxLogError("Expected Product code");
        }
        else if (row[1] != "Bureau of Meteorology station number")
            AllGood = false;
        else if (row[2] != "Year")
            AllGood = false;
        else if (row[3] != "Month")
            AllGood = false;
        else if (row[4] != "Day")
            AllGood = false;
        if (AllGood)
        {
            wxString statsTable;
            auto StatisticName = row[5];

            if (StatisticName == "Maximum temperature (Degree C)")
                statsTable = "max_temp";
            else if (StatisticName == "Minimum temperature (Degree C)")
				statsTable = "min_temp";
            else if (StatisticName == "Rainfall amount (millimetres)")
                statsTable = "rainfall";
			else
			{
                wxLogError("Unknown statistic name '%s'", StatisticName);
                return false;
            }
            if (statsTable == "max_temp")
                return db.importMaxTemp(rdr, fileroot);
			else if (statsTable == "min_temp")
				return db.importMinTemp(rdr,fileroot);
            else if (statsTable == "rainfall")
                return db.importRainfall(rdr,fileroot);
            else {
                showRow(row);
                wxLogError("Unsuccessful import");
                return false;
            }
        }
    }
    return true;
 }


wxArrayString toWxList(std::vector<std::string>&   values)
{
    wxArrayString result;
    for(size_t i = 0; i < values.size(); i++)
        result.Add(values[i].c_str(),1);
    return result;
}

/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_VIEW_SERIES
 */

void MainFrame::OnViewSeriesClick( wxCommandEvent& event )
{
    event.Skip();
    SelectSeries ss(this);

    std::vector<std::string>   values;

    Database& db = getDB();

    db.getSeriesNames(values);
    wxArrayString itemSet = toWxList(values);
    ss.listSeries_->InsertItems(itemSet, 0);

    if (ss.ShowModal() == wxID_OK)
    {
        wxArrayInt  items;
        ss.listSeries_->GetCheckedItems(items);

        for(size_t i = 0; i < items.size(); i++)
        {
            auto itemNo = items[i];
            wxString s = ss.listSeries_->GetString(itemNo);
            wxStringInputStream sis(s);
            wxTextInputStream row(sis," ,");

            int rowid;
            row >> rowid;

            wxString fileName;
            wxString tableName;

            row >> fileName;
            row >> tableName;

            std::vector<float> fdata;
            std::vector<double> xdata;

            std::string tname = tableName.ToStdString();
            if (!db.getSeriesData(rowid, tname, fdata, xdata))
			{
				wxLogError("No records found");
				return;
			}

            std::stringstream ss;

            if (tname=="max_temp")
                ss << "Max. daily Temp (C) " << fileName.char_str();
            else if (tname == "min_temp")
                ss << "Min. daily Temp (C) " << fileName.char_str();
            else if (tname == "rainfall")
                ss << "Daily Rainfall (mm) " << fileName.char_str();


            SeriesPtr fs = std::make_shared<FloatSeries>(fdata,DEFAULT,ss.str());

            SeriesPtr ds = std::make_shared<DoubleSeries>(xdata,DATE_JULIAN_MOD,"Date");

			LargePlot* f = new LargePlot(this);
            tableName << " ";
            tableName << fileName;
            f->SetTitle(tableName);

			f->Show();
            auto xyplot = f->plot_;
            xyplot->SetVirtualSize(1000,500);

            PlotPtr sp = std::make_shared<DataLayer>();

			DataLayer* p = static_cast<DataLayer*>(sp.get());
			p->xdata_ = ds;
			p->ydata_ = fs;

            xyplot->AddData(sp);
            xyplot->Refresh();
        }



    }

}

bool MainFrame::SelectStationInfo(wxString& filepath)
{
    wxFileDialog   openFileDialog(this, _("Select INV file"), "", "",
            "INV files (*.inv)|*.inv", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return false; // the user

    filepath = openFileDialog.GetPath();
    return true;
}

bool MainFrame::SelectGISSFile(wxString& filepath)
{
    wxFileDialog   openFileDialog(this, _("Select DAT file"), "", "",
            "DAT files (*.dat)|*.dat", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return false; // the user

    filepath = openFileDialog.GetPath();
    return true;
}

/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMPORT_GISS
 */

void MainFrame::OnImportGissClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMPORT_GISS in MainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMPORT_GISS in MainFrame.
    wxString filepath;

    if (SelectGISSFile(filepath))
    {
        wxLogMessage("Selected %s",filepath);
        ImportGiss(filepath);
    }
}

void MainFrame::ModalProgressDlg(const wxString& title, const wxString& msg)
{
    dlgProgress_ = new wxProgressDialog
                        (
                         title,
                         msg,
                         100,
                         this,
                         //wxPD_CAN_ABORT |
                         wxPD_AUTO_HIDE |
                         wxPD_ELAPSED_TIME |
                         wxPD_ESTIMATED_TIME |
                         wxPD_REMAINING_TIME
                        );
    //dlgProgress_->Show();

}
 void MainFrame::ImportGiss(const wxString& filepath)
 {
    Database& db = getDB();

    //timer_ = new IdleTimer(this);
    //timer_->Start();

    auto importThread = new ImportGissThread(this, db.path(), filepath.ToStdString());

    if ( importThread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
        delete importThread;
        return;
    }

    this->ModalProgressDlg("Import Progress", "Temperature data into database");


    if ( importThread->Run() != wxTHREAD_NO_ERROR )
    {
        wxLogError("Can't run the thread!");
        delete importThread;
        if (dlgProgress_)
        {
            dlgProgress_->Destroy();
            dlgProgress_ = nullptr;
        }
    }
    // after the call to wxThread::Run(), the m_pThread pointer is "unsafe":
    // at any moment the thread may cease to exist (because it completes its work).
    // To avoid dangling pointers OnThreadExit() will set m_pThread
    // to NULL when the thread dies.

 }

void  MainFrame::StartDBProgress(const std::string& path)
{
    //timer_ = new IdleTimer(this);
    //timer_->Start();
    std::string emptyStr;

    ModalProgressDlg("Database Create", "Spatialite Database Setup for a few minutes");


    mThread_ = new NewDBThread(this, path, emptyStr);
    if ( mThread_->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
        delete mThread_;
        return;
    }

    if ( mThread_->Run() != wxTHREAD_NO_ERROR )
    {
        wxLogError("Can't run the thread!");
        delete mThread_;
        if (dlgProgress_)
        {
            dlgProgress_->Destroy();
            dlgProgress_ = nullptr;
        }
        return;
    }

}


void  MainFrame::EndDBProgress()
{
}


 void MainFrame::OnWorkerEvent(wxThreadEvent& event)
{
    event.Skip(false);

    if (dlgProgress_ == nullptr)
    {
        wxWakeUpIdle();
        return;
    }
    int n = event.GetInt();
    if ( n == -1 )
    {
        auto temp = dlgProgress_;
        dlgProgress_ = nullptr;
        temp->Destroy();

        // the dialog is aborted because the event came from another thread, so
        // we may need to wake up the main event loop for the dialog to be
        // really closed
        wxWakeUpIdle();
    }
    else
    {
        if ( !dlgProgress_->Update(n) )
        {
            /*wxCriticalSectionLocker lock(m_csCancelled);

            m_cancelled = true*/
        }
    }
}

 wxThread::ExitCode ImportStationThread::Entry()
 {

    Database sdb_;

    wxFileInputStream input_stream(importPath_);


    //double fileLength = input_stream.GetLength();
    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", importPath_);
        return 0;
    }
    double fileLength = input_stream.GetLength();

    wxTextInputStream   text(input_stream, ',');

    sdb_.path(dbPath_);
    sdb_.open();


    bool hasNext = true;

    int lineCount = 0; // commit load indicator
	int updateCt = 0;
    while(hasNext)
    {
        //DBRowId stationId;


        auto chars = text.ReadLine();

        hasNext = (chars.size() > 0);
        if (hasNext)
        {
            auto stationCode = chars.substr(0,11).ToStdString();

            auto latitudeStr = chars.substr(12,8);
			auto longitudeStr = chars.substr(21,9);
			auto elevationStr = chars.substr(31,6);
			auto nameStr = chars.substr(38,30);
			auto elevationGridStr = chars.substr(69,4);
			auto popClassStr = chars.substr(73,1);
			auto popSizeStr = chars.substr(74,5);
			auto popLightsStr = chars.substr(106,1);
			auto vegetationStr = chars.substr(90,16);


            vegetationStr.Trim(true);
            popSizeStr.Trim(true);

            //int validCount = 0;
			GissLocation	rec;

			rec.setId(stationCode);
			latitudeStr.ToDouble(&rec.lat_);
			longitudeStr.ToDouble(&rec.long_);
			elevationStr.ToDouble(&rec.elevation1_);
			elevationGridStr.ToDouble(&rec.elevation2_);
			rec.name_ = nameStr;
			long temp;

			popSizeStr.ToLong(&temp);
			rec.popSize_ = temp;
			rec.popClass_ = popClassStr.at(0);
			rec.popLights_ = popLightsStr.at(0);
			rec.vegetation_ = vegetationStr;


            try {
                if (lineCount == 0)
                {
                    double percentDone = (input_stream.TellI() * 100.0) / fileLength;
                    UpdateProgress(wxRound(percentDone));

                    sdb_.begin();
                }
                lineCount++;
				if (rec.save(sdb_))
					updateCt++;

                if (lineCount >= 500)
                {

                    sdb_.commit();

                    lineCount = 0;
                }
            }
            catch(std::exception& ex)
            {
                sdb_.rollback();
                lineCount = 0;
				wxLogMessage("Rollback ");
				return 0;
            }
            // save the year record as well

        }
    }
    UpdateProgress(100);

    if (lineCount > 0)
    {
        sdb_.commit();
        lineCount = 0;
    }
    wxLogMessage("Updated %d records", updateCt);

    if (updateCt > 0)
    {
        sdb_.execute_or_throw("update gissloc set Geometry=MakePoint(Longitude,Latitude,4326);");
        wxLogMessage("Geometry updated", updateCt);
    }
    sdb_.close();

    UpdateProgress(-1);

	wxLogMessage("Updated %d records", updateCt);

	return 0;
 }




/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_GISS_STATION
 */

void MainFrame::OnGissImportStations( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for ID_GISS_STATION in MainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for ID_GISS_STATION in MainFrame.
    wxString filepath;

    if (this->SelectStationInfo(filepath))
    {
        ImportStationInfo(filepath);
    }
}

void MainFrame::ImportStationInfo(const wxString& filepath)
{
    Database& db = getDB();

    //timer_ = new IdleTimer(this);
    //timer_->Start();

    auto importThread = new ImportStationThread(this, db.path(), filepath.ToStdString());

    if ( importThread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
        delete importThread;
        return;
    }
    this->ModalProgressDlg(wxT("Import Progress"),wxT("Importing Station data"));

    if ( importThread->Run() != wxTHREAD_NO_ERROR )
    {
        wxLogError("Can't run the thread!");
        delete importThread;
        if (dlgProgress_)
        {
            dlgProgress_->Destroy();
            dlgProgress_ = nullptr;
        }
    }
    // after the call to wxThread::Run(), the m_pThread pointer is "unsafe":
    // at any moment the thread may cease to exist (because it completes its work).
    // To avoid dangling pointers OnThreadExit() will set m_pThread
    // to NULL when the thread dies.

}

/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_FIND_STATION
 */

void MainFrame::OnFindStationClick( wxCommandEvent& event )
{
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_BASELINE
 */

void MainFrame::GenBaseline( wxCommandEvent& event )
{
    event.Skip();
    getDB().generateBaseline();
}

PlotXY*  MainFrame::NewPlot(const wxString& text)
{
    PlotXY* p = new PlotXY(book_);
    book_->AddPage(p, text);
    return p;
}



/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_SETS
 */

void MainFrame::OnSetsClick( wxCommandEvent& event )
{
    // show the existing  sets
    event.Skip();
    auto setview = new StationSets(this);
    setview->ap_ = this->ap_;

    setview->ReadSetNames();
    setview->Show();
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_LOG_VIEW
 */

void MainFrame::OnLogViewClick( wxCommandEvent& event )
{

    event.Skip();
    this->log_->Show();
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_ABOUT
 */

void MainFrame::OnAboutClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for ID_ABOUT in MainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for ID_ABOUT in MainFrame.
}

void  MainFrame::StartInitTimer()
{
    timer_ = new IdleTimer(this);

    timer_->Start(1000);
}

IdleTimer::IdleTimer(MainFrame* owner) : wxTimer(owner), notify_(owner)
{
}

IdleTimer::~IdleTimer()
{

}

void IdleTimer::Notify()
{
    notify_->OnTimerNotify();
}

void MainFrame::OnTimerNotify()
{
    if (timer_ )
    {
        timer_->Stop();
        if (timerState_ == -1)
        {
            timerState_ = 0;
            ap_->openDB();
            if (!ap_->getReady())
            {
                this->StartDBProgress(ap_->getDBPath());
                timer_->Start(1000);
                return;
            }
            timerState_ = 1; // open done fully

        }

        {
            wxCriticalSectionLocker enter(threadCS_);
            if (mThread_)
            {
                auto fileSize = wxFileName::GetSize(ap_->getDBPath());
                int pcDone = (int) (fileSize.ToDouble()*100.0/6000000.0);
                if (this->dlgProgress_)
                  this->dlgProgress_->Update(pcDone);
                timer_->Start(1000);
            }
            else {
                if (timerState_ == 0)
                {
                    timerState_ = 1;
                    ap_->callOpenDB();
                }
                delete timer_;
                timer_ = nullptr;
            }
        }
    }
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_IMPORT_SHAPE_FILE
 */

void MainFrame::OnImportShapeFileClick( wxCommandEvent& event )
{
    event.Skip(false);
    ap_->loadShapeFile();
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_OPEN_PLOTFILE
 */

void MainFrame::OnOpenPlotfileClick( wxCommandEvent& event )
{

    event.Skip(false);
    ap_->openPlotFile();

}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_LUA_COMMAND
 */

void MainFrame::OnLuaCommandClick( wxCommandEvent& event )
{
    event.Skip(false);

}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_SCRIPT
 */

void MainFrame::OnNewScriptClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_SCRIPT in MainFrame.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_SCRIPT in MainFrame.
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_RUN_LUA
 */

void MainFrame::OnRunLuaClick( wxCommandEvent& event )
{
    event.Skip();
    // get current tab for Lua Edit
    wxWindow* w = book_->GetCurrentPage();
    LuaEdit* edit = dynamic_cast<LuaEdit*>(w);
    if (edit != nullptr)
    {
        std::string text = edit->luaEdit_->GetValue().ToStdString();

        if (text.size() > 0)
        {
            ap_->runLuaScript(text);
        }
    }
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
 */

void MainFrame::OnOpenClick( wxCommandEvent& event )
{
    event.Skip();
    ap_->openScriptFile();
}

LuaEdit* MainFrame::createLuaEdit(const std::string& tabname)
{
    LuaEdit* led = new LuaEdit( book_, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    led->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    book_->AddPage(led, tabname, false);
    return led;
}
