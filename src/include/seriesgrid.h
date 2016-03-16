/////////////////////////////////////////////////////////////////////////////
// Name:        seriesgrid.h
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     Sun 31 Jan 2016 15:36:22 AEDT
// RCS-ID:      
// Copyright:   Michael Rynn
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SERIESGRID_H_
#define _SERIESGRID_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/grid.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class SeriesGrid;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers

#define SYMBOL_SERIESGRID_STYLE wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL
#define SYMBOL_SERIESGRID_IDNAME ID_SERIES_GRID
#define SYMBOL_SERIESGRID_SIZE wxSize(200, 150)
#define SYMBOL_SERIESGRID_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SeriesGrid class declaration
 */

class SeriesGrid: public wxGrid
{    
    DECLARE_DYNAMIC_CLASS( SeriesGrid )
    DECLARE_EVENT_TABLE()
public:
    /// Constructors
    SeriesGrid();
    SeriesGrid(wxWindow* parent, wxWindowID id = ID_SERIES_GRID, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(200, 150), long style = wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = ID_SERIES_GRID, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(200, 150), long style = wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL);

    /// Destructor
    ~SeriesGrid();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    enum {
        ID_SERIES_GRID = 10001
    };
    
////@begin SeriesGrid event handler declarations

////@end SeriesGrid event handler declarations

////@begin SeriesGrid member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SeriesGrid member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SeriesGrid member variables
////@end SeriesGrid member variables
};

#endif
    // _SERIESGRID_H_
