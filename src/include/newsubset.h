/////////////////////////////////////////////////////////////////////////////
// Name:        newsubset.h
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     12/26/2015 8:48:22 PM
// RCS-ID:      
// Copyright:   Michael Rynn
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _NEWSUBSET_H_
#define _NEWSUBSET_H_


/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class StationQuery;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_NEW_SUBSET 12300
#define ID_FOREIGN 12400
#define SYMBOL_STATIONSETDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_STATIONSETDIALOG_TITLE _("Create a subset of stations")
#define SYMBOL_STATIONSETDIALOG_IDNAME ID_NEW_SUBSET
#define SYMBOL_STATIONSETDIALOG_SIZE wxSize(400, 280)
#define SYMBOL_STATIONSETDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * StationSetDialog class declaration
 */

class StationSetDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( StationSetDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    StationSetDialog();
    StationSetDialog( wxWindow* parent, wxWindowID id = SYMBOL_STATIONSETDIALOG_IDNAME, const wxString& caption = SYMBOL_STATIONSETDIALOG_TITLE, const wxPoint& pos = SYMBOL_STATIONSETDIALOG_POSITION, const wxSize& size = SYMBOL_STATIONSETDIALOG_SIZE, long style = SYMBOL_STATIONSETDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_STATIONSETDIALOG_IDNAME, const wxString& caption = SYMBOL_STATIONSETDIALOG_TITLE, const wxPoint& pos = SYMBOL_STATIONSETDIALOG_POSITION, const wxSize& size = SYMBOL_STATIONSETDIALOG_SIZE, long style = SYMBOL_STATIONSETDIALOG_STYLE );

    /// Destructor
    ~StationSetDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin StationSetDialog event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_NEW_SUBSET
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_DESTROY event handler for ID_NEW_SUBSET
    void OnDestroy( wxWindowDestroyEvent& event );

////@end StationSetDialog event handler declarations

////@begin StationSetDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end StationSetDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin StationSetDialog member variables
    StationQuery* mStationPanel;
////@end StationSetDialog member variables
};

#endif
    // _NEWSUBSET_H_
