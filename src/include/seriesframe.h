/////////////////////////////////////////////////////////////////////////////
// Name:        seriesframe.h
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     Sun 31 Jan 2016 15:32:33 AEDT
// RCS-ID:      
// Copyright:   Michael Rynn
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SERIESFRAME_H_
#define _SERIESFRAME_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
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
#define SYMBOL_SERIESFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SERIESFRAME_TITLE _("SeriesFrame")
#define SYMBOL_SERIESFRAME_IDNAME ID_SERIESFRAME
#define SYMBOL_SERIESFRAME_SIZE wxSize(400, 300)
#define SYMBOL_SERIESFRAME_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SeriesFrame class declaration
 */

class SeriesFrame: public wxFrame
{    
    DECLARE_CLASS( SeriesFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SeriesFrame();
    SeriesFrame( wxWindow* parent, wxWindowID id = SYMBOL_SERIESFRAME_IDNAME, const wxString& caption = SYMBOL_SERIESFRAME_TITLE, const wxPoint& pos = SYMBOL_SERIESFRAME_POSITION, const wxSize& size = SYMBOL_SERIESFRAME_SIZE, long style = SYMBOL_SERIESFRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SERIESFRAME_IDNAME, const wxString& caption = SYMBOL_SERIESFRAME_TITLE, const wxPoint& pos = SYMBOL_SERIESFRAME_POSITION, const wxSize& size = SYMBOL_SERIESFRAME_SIZE, long style = SYMBOL_SERIESFRAME_STYLE );

    /// Destructor
    ~SeriesFrame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin SeriesFrame event handler declarations

////@end SeriesFrame event handler declarations

////@begin SeriesFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SeriesFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SeriesFrame member variables
    SeriesGrid* mGrid;
    /// Control identifiers
    enum {
        ID_SERIESFRAME = 10000
    };
////@end SeriesFrame member variables
};

#endif
    // _SERIESFRAME_H_
