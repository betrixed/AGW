/////////////////////////////////////////////////////////////////////////////
// Name:        stationdailydata.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Tue 02 Jun 2020 13:27:30 AEST
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _STATIONDAILYDATA_H_
#define _STATIONDAILYDATA_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/spinbutt.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinButton;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_STATIONDAILYDATA_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_STATIONDAILYDATA_TITLE _("Station Daily Data")
#define SYMBOL_STATIONDAILYDATA_IDNAME ID_STATIONDAILYDATA
#define SYMBOL_STATIONDAILYDATA_SIZE wxSize(400, 300)
#define SYMBOL_STATIONDAILYDATA_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * StationDailyData class declaration
 */

class StationDailyData: public wxFrame
{
    DECLARE_CLASS( StationDailyData )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    StationDailyData();
    StationDailyData( wxWindow* parent, wxWindowID id = SYMBOL_STATIONDAILYDATA_IDNAME, const wxString& caption = SYMBOL_STATIONDAILYDATA_TITLE, const wxPoint& pos = SYMBOL_STATIONDAILYDATA_POSITION, const wxSize& size = SYMBOL_STATIONDAILYDATA_SIZE, long style = SYMBOL_STATIONDAILYDATA_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_STATIONDAILYDATA_IDNAME, const wxString& caption = SYMBOL_STATIONDAILYDATA_TITLE, const wxPoint& pos = SYMBOL_STATIONDAILYDATA_POSITION, const wxSize& size = SYMBOL_STATIONDAILYDATA_SIZE, long style = SYMBOL_STATIONDAILYDATA_STYLE );

    /// Destructor
    ~StationDailyData();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin StationDailyData event handler declarations

////@end StationDailyData event handler declarations

////@begin StationDailyData member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end StationDailyData member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin StationDailyData member variables
    wxSpinButton* btnSpinShow;
    wxTextCtrl* txtDailyData;
    /// Control identifiers
    enum {
        ID_STATIONDAILYDATA = 10000,
        ID_SHOW_YEAR = 10001,
        ID_SHOW_MONTH = 10002,
        ID_SHOW_NOW = 10003,
        ID_SPINSHOW = 10004,
        ID_TEXTCTRL = 10005
    };
////@end StationDailyData member variables

    wxString stationId;

    void SetStationId(const wxString& name);

};

#endif
    // _STATIONDAILYDATA_H_
