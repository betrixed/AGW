/////////////////////////////////////////////////////////////////////////////
// Name:        stationsets.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     12/26/2015 8:04:14 PM
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _STATIONSETS_H_
#define _STATIONSETS_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/splitter.h"
#include "wx/notebook.h"
#include "wx/grid.h"
////@end includes

#include "stationtable.h"
/*!
 * Forward declarations
 */

////@begin forward declarations
class wxMenu;
class wxNotebook;
class wxBoxSizer;
class wxGrid;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_STATIONSETS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_STATIONSETS_TITLE _("StationSets")
#define SYMBOL_STATIONSETS_IDNAME ID_STATIONSETS
#define SYMBOL_STATIONSETS_SIZE wxSize(700, 500)
#define SYMBOL_STATIONSETS_POSITION wxDefaultPosition
////@end control identifiers

#include "database.hpp"
#include <jsoncpp/json/forwards.h>
/*!
 * StationSets class declaration
 */

 class StationQuery;
class AppData;
class LocTable;

typedef std::shared_ptr<LocTable> LocTablePtr;

class StationSets: public wxFrame
{
    DECLARE_CLASS( StationSets )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    StationSets();
    StationSets( wxWindow* parent, wxWindowID id = SYMBOL_STATIONSETS_IDNAME, const wxString& caption = SYMBOL_STATIONSETS_TITLE, const wxPoint& pos = SYMBOL_STATIONSETS_POSITION, const wxSize& size = SYMBOL_STATIONSETS_SIZE, long style = SYMBOL_STATIONSETS_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_STATIONSETS_IDNAME, const wxString& caption = SYMBOL_STATIONSETS_TITLE, const wxPoint& pos = SYMBOL_STATIONSETS_POSITION, const wxSize& size = SYMBOL_STATIONSETS_SIZE, long style = SYMBOL_STATIONSETS_STYLE );

    /// Destructor
    ~StationSets();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin StationSets event handler declarations

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_QUERY_NEW
    void OnQueryNewClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_QUERY_DELETE
    void OnQueryDeleteClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_SET_MAP
    void OnSetMapClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for SET_LIST
    void OnSetListSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUN_QUERY
    void OnRunQueryClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_QUERY
    void OnAddQueryClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_RADIOBOX
    void OnRadioboxSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_GO_DERIVE
    void OnGoDeriveClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_SEL_BASE
    void OnSelBaseSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PLUS_YEAR
    void OnPlusYearClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MINUS_YEAR
    void OnMinusYearClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PLOT_YEARS
    void OnPlotYearsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_TIME_SERIES
    void OnTimeSeriesClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STATION_DATA
    void OnStationDataClick( wxCommandEvent& event );

////@end StationSets event handler declarations

////@begin StationSets member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end StationSets member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin StationSets member variables
    wxMenu* menuDerive;
    wxListBox* listSet;
    wxButton* mRrun;
    wxNotebook* mBook_;
    wxTextCtrl* txtSetXML;
    wxTextCtrl* editSetName;
    wxScrolledWindow* mQueryList;
    wxBoxSizer* mVSizer;
    wxPanel* pageDerive;
    wxChoice* mMeasure;
    wxRadioBox* radboxAnomaly;
    wxTextCtrl* mBaseStart;
    wxTextCtrl* mBaseEnd;
    wxCheckBox* mBoxFit;
    wxTextCtrl* mFitFrom;
    wxTextCtrl* mFitTo;
    wxCheckBox* mBoxYears;
    wxTextCtrl* mYearFrom;
    wxTextCtrl* mYearTo;
    wxChoice* mMeasure2;
    wxRadioBox* radboxReal;
    wxStaticBox* boxBaseDiff;
    wxTextCtrl* mBaseStart2;
    wxTextCtrl* mBaseEnd2;
    wxTextCtrl* mYearToAdd;
    wxListBox* mYearList;
    wxCheckBox* mTAVG;
    wxCheckBox* mMMAX;
    wxCheckBox* mTMAX;
    wxCheckBox* mTMIN;
    wxGrid* mGrid_;
    /// Control identifiers
    enum {
        ID_STATIONSETS = 10000,
        ID_QUERY_NEW = 10030,
        ID_QUERY_DELETE = 10033,
        ID_SET_MAP = 10036,
        ID_SPLIT_SETS = 10007,
        ID_PANEL = 10001,
        wxID_H_SET_LIST = 10035,
        SET_LIST = 10002,
        ID_RUN_QUERY = 10003,
        ID_SET_BOOK = 10006,
        ID_SET_XML = 10005,
        ID_PANEL_NOTE = 10004,
        wxID_SET_NAME_LABEL = 10034,
        ID_EDIT_SET_NAME = 10009,
        ID_ADD_QUERY = 10010,
        ID_QUERY_LIST = 10008,
        ID_DERIVE_TREND = 10011,
        ID_MEASURE = 10021,
        ID_RADIOBOX = 10045,
        ID_BASE_START = 10019,
        ID_BASE_END = 10020,
        ID_BOX_LINEFIT = 10015,
        ID_FIT_FROM = 10012,
        ID_FIT_TO = 10014,
        ID_BOX_YEAR_INCLUDE = 10018,
        ID_FILTER_FROM = 10016,
        ID_FILTER_TO = 10017,
        ID_GO_DERIVE = 10013,
        ID_TAB_YEAR = 10022,
        ID_CHOICE_M2 = 10023,
        ID_SEL_BASE = 10041,
        wxID_BASE_DIFF = 10043,
        ID_BASE_START2 = 10024,
        ID_BASE_END2 = 10025,
        ID_YEAR_TO_ADD = 10026,
        ID_PLUS_YEAR = 10027,
        ID_LIST_YEARS = 10028,
        ID_MINUS_YEAR = 10029,
        ID_PLOT_YEARS = 10031,
        ID_MEMBER_GRID = 10032,
        ID_TIME_SERIES = 10037,
        ID_BOX_TAVG = 10038,
        ID_BOX_MMAX = 10044,
        ID_STATION_DATA = 10042,
        ID_BOX_TMAX = 10040,
        ID_BOX_TMIN = 10039,
        ID_SET_GRID = 10002
    };
////@end StationSets member variables
    bool SetExists(const std::string& name);
    void ReadSetNames();
    void RefreshMembers();

    bool GetSetName(std::string& name);
    void Removing(wxWindow* q);

    void DeleteSet(const wxString& setName);
    void ClearPanels();

    friend class StationQuery;

    void MoveUp(StationQuery* q);
    void MoveDown(StationQuery* q);

    bool UIToJSON(Json::Value& root);
    void SelectSetName(const wxString& setName,bool readMembers = true);
    void DropSetTable(const std::string& tableName);
    void packQueryList();

    std::string MakeSQL();

    Database        db_;
    std::string    selectedSet_;
    AppData*        ap_;
    LocTablePtr     locTable_; // set of stations selected
};

#endif
    // _STATIONSETS_H_
