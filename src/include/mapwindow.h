/////////////////////////////////////////////////////////////////////////////
// Name:        mapwindow.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Thu 11 Feb 2016 20:16:52 AEDT
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAPWINDOW_H_
#define _MAPWINDOW_H_


/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class MapWindow;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers

#define SYMBOL_MAPWINDOW_STYLE wxSUNKEN_BORDER|wxFULL_REPAINT_ON_RESIZE|wxHSCROLL|wxVSCROLL
#define SYMBOL_MAPWINDOW_IDNAME ID_SCROLLEDWINDOW
#define SYMBOL_MAPWINDOW_SIZE wxSize(600, 300)
#define SYMBOL_MAPWINDOW_POSITION wxDefaultPosition
////@end control identifiers


#include "appdata.h"

class wxStatusBar;

/*!
 * MapWindow class declaration
 */

class MapWindow: public wxScrolledCanvas
{
    DECLARE_DYNAMIC_CLASS( MapWindow )
    DECLARE_EVENT_TABLE()

public:

    enum { 
        ID_SCROLLEDWINDOW = 10001 
    };

    /// Constructors
    MapWindow();
    MapWindow(wxWindow* parent, wxWindowID id = ID_SCROLLEDWINDOW, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(100, 100), long style = wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = ID_SCROLLEDWINDOW, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(100, 100), long style = wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL);

    /// Destructor
    ~MapWindow();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin MapWindow event handler declarations

    /// wxEVT_PAINT event handler for ID_SCROLLEDWINDOW
    void OnPaint( wxPaintEvent& event );

    /// wxEVT_LEFT_DOWN event handler for ID_SCROLLEDWINDOW
    void OnLeftDown( wxMouseEvent& event );

////@end MapWindow event handler declarations

////@begin MapWindow member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MapWindow member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MapWindow member variables
////@end MapWindow member variables

    void drawCountryLayer(wxDC& dc);
    void drawStationLayer(wxDC& dc);

    const GissLocation* findStation(const wxPoint& p);
    const CountryRecord* findCountry(const wxPoint& p);

    CountryMapPtr    map_c;
    LocTablePtr      stations_;
    AppData*        ap_;

    double          zoom_;
    double          top_;
    double          left_;

    double  xscale_;
    double  yscale_;
    double  xoffset_;
    double  yoffset_;
    int     mwidth_;
    int     mheight_;

    wxStatusBar*  status_;
};

#endif
    // _MAPWINDOW_H_
