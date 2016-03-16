/////////////////////////////////////////////////////////////////////////////
// Name:        mapframe.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Thu 11 Feb 2016 20:13:08 AEDT
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAPFRAME_H_
#define _MAPFRAME_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/statusbr.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class MapWindow;
class wxStatusBar;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_MAPFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_MAPFRAME_TITLE _("MapFrame")
#define SYMBOL_MAPFRAME_IDNAME ID_MAPFRAME
#define SYMBOL_MAPFRAME_SIZE wxSize(400, 300)
#define SYMBOL_MAPFRAME_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * MapFrame class declaration
 */

class MapFrame: public wxFrame
{
    DECLARE_CLASS( MapFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MapFrame();
    MapFrame( wxWindow* parent, wxWindowID id = SYMBOL_MAPFRAME_IDNAME, const wxString& caption = SYMBOL_MAPFRAME_TITLE, const wxPoint& pos = SYMBOL_MAPFRAME_POSITION, const wxSize& size = SYMBOL_MAPFRAME_SIZE, long style = SYMBOL_MAPFRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MAPFRAME_IDNAME, const wxString& caption = SYMBOL_MAPFRAME_TITLE, const wxPoint& pos = SYMBOL_MAPFRAME_POSITION, const wxSize& size = SYMBOL_MAPFRAME_SIZE, long style = SYMBOL_MAPFRAME_STYLE );

    /// Destructor
    ~MapFrame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin MapFrame event handler declarations

////@end MapFrame event handler declarations

////@begin MapFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MapFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MapFrame member variables
    MapWindow* mMap;
    wxStatusBar* mStatus;
    /// Control identifiers
    enum {
        ID_MAPFRAME = 10000,
        ID_STATUSBAR = 10002
    };
////@end MapFrame member variables


};

#endif
    // _MAPFRAME_H_
