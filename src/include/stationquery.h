/////////////////////////////////////////////////////////////////////////////
// Name:        stationquery.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Fri 15 Jan 2016 21:15:45 AEDT
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _STATIONQUERY_H_
#define _STATIONQUERY_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_STATIONQUERY_STYLE wxTAB_TRAVERSAL
#define SYMBOL_STATIONQUERY_TITLE _("StationQuery")
#define SYMBOL_STATIONQUERY_IDNAME ID_STATIONQUERY
#define SYMBOL_STATIONQUERY_SIZE wxDefaultSize
#define SYMBOL_STATIONQUERY_POSITION wxDefaultPosition
////@end control identifiers

#include <json/forwards.h>

/*!
 * StationQuery class declaration
 */

 class Database;
class StationSets;

class StationQuery: public wxPanel
{
    DECLARE_DYNAMIC_CLASS( StationQuery )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    StationQuery();
    StationQuery( wxWindow* parent, wxWindowID id = SYMBOL_STATIONQUERY_IDNAME, const wxPoint& pos = SYMBOL_STATIONQUERY_POSITION, const wxSize& size = SYMBOL_STATIONQUERY_SIZE, long style = SYMBOL_STATIONQUERY_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_STATIONQUERY_IDNAME, const wxPoint& pos = SYMBOL_STATIONQUERY_POSITION, const wxSize& size = SYMBOL_STATIONQUERY_SIZE, long style = SYMBOL_STATIONQUERY_STYLE );

    /// Destructor
    ~StationQuery();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin StationQuery event handler declarations

    /// wxEVT_DESTROY event handler for ID_STATIONQUERY
    void OnDestroy( wxWindowDestroyEvent& event );

    /// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_BOX_QTYPE
    void OnBoxQtypeSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_UP
    void OnBtnUpClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_DOWN
    void OnBtnDownClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_DELETE
    void OnBtnDeleteClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_COUNTRY_BTN
    void OnCountryBtnClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_VEGE
    void OnBtnVegeClick( wxCommandEvent& event );

////@end StationQuery event handler declarations

////@begin StationQuery member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end StationQuery member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin StationQuery member variables
    wxBoxSizer* mBossVSizer;
    wxChoice* opName;
    wxRadioBox* mBoxQType;
    wxButton* btnUp;
    wxButton* mBtnDown;
    wxButton* mBtnDelete;
    wxTextCtrl* editDescription;
    wxBoxSizer* mSizerStation;
    wxCheckBox* mBoxNameLike;
    wxTextCtrl* txtLike;
    wxBoxSizer* sizerLatBand;
    wxCheckBox* mBoxLatitude;
    wxTextCtrl* txtLatSouth;
    wxTextCtrl* txtLatNorth;
    wxBoxSizer* mSizerRandom;
    wxCheckBox* mBoxRandom;
    wxTextCtrl* txtRowCount;
    wxStaticText* staticRows;
    wxBoxSizer* mSizerCountry;
    wxCheckBox* mBoxCountry;
    wxTextCtrl* mCountryName;
    wxBoxSizer* mSizerVege;
    wxCheckBox* mBoxVege;
    wxTextCtrl* mTextVege;
    /// Control identifiers
    enum {
        ID_STATIONQUERY = -1,
        ID_CHOICE = 10000,
        ID_BOX_QTYPE = 10011,
        ID_BTN_UP = 10007,
        ID_BTN_DOWN = 10008,
        ID_BTN_DELETE = 10009,
        ID_TEXT_SET_NAME = 10002,
        ID_CHECK_LIKE = 10015,
        ID_NAME_LIKE = 10001,
        ID_BOX_LATITUDE = 10012,
        ID_SOUTH_LAT = 10013,
        ID_NORTH_LAT = 10014,
        ID_BOX_RANDOM = 10003,
        ID_RANDOM_COUNT = 10004,
        ID_BOX_COUNTRY = 10005,
        ID_COUNTRY_NAME = 10006,
        ID_COUNTRY_BTN = 10010,
        ID_BOX_VEGE = 10016,
        ID_TEXT_VEGE = 10017,
        ID_BTN_VEGE = 10018
    };
////@end StationQuery member variables
    bool  GetAsJSON(Json::Value& json);
    void  LoadFromJSON(const Json::Value& json);
    bool  GetSetName(std::string& value);

    void  SetQueryType(int sel);

    Database*      getDB();
    StationSets*   setsFrame;
    std::string    mSQL;
    wxFrame*       myFrame;

};


class JDoc {
public:
    static const std::string ID;
    static const std::string QSEQ;

    static const std::string NAME_LIKE;
    static const std::string DESCRIBE;
    static const std::string EFFECT;
    static const std::string LAT_BAND;
    static const std::string RANDOM;
    static const std::string COUNTRY;
    static const std::string VEGETATION;
    static const std::string SQL_OP;
    static const std::string QDISPLAY;
};

#endif
    // _STATIONQUERY_H_
