/////////////////////////////////////////////////////////////////////////////
// Name:        largeplot.h
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     02/11/2013 00:04:06
// RCS-ID:      
// Copyright:   Michael Rynn
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _LARGEPLOT_H_
#define _LARGEPLOT_H_


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
class PlotXY;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_LARGEPLOT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_LARGEPLOT_TITLE _("Large Plot")
#define SYMBOL_LARGEPLOT_IDNAME ID_LARGEPLOT
#define SYMBOL_LARGEPLOT_SIZE wxSize(400, 300)
#define SYMBOL_LARGEPLOT_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * LargePlot class declaration
 */

class LargePlot: public wxFrame
{    
    DECLARE_CLASS( LargePlot )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    LargePlot();
    LargePlot( wxWindow* parent, wxWindowID id = SYMBOL_LARGEPLOT_IDNAME, const wxString& caption = SYMBOL_LARGEPLOT_TITLE, const wxPoint& pos = SYMBOL_LARGEPLOT_POSITION, const wxSize& size = SYMBOL_LARGEPLOT_SIZE, long style = SYMBOL_LARGEPLOT_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_LARGEPLOT_IDNAME, const wxString& caption = SYMBOL_LARGEPLOT_TITLE, const wxPoint& pos = SYMBOL_LARGEPLOT_POSITION, const wxSize& size = SYMBOL_LARGEPLOT_SIZE, long style = SYMBOL_LARGEPLOT_STYLE );

    /// Destructor
    ~LargePlot();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin LargePlot event handler declarations

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_OPTIONS
    void OnOptionsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MONTH_AVERAGE
    void OnMonthAverageClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_EXPORT
    void OnMenuExportClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_EXPORT_IMAGE
    void OnExportImageClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_RAINFALL_TOTAL
    void OnRainfallTotalClick( wxCommandEvent& event );

////@end LargePlot event handler declarations

////@begin LargePlot member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end LargePlot member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin LargePlot member variables
    PlotXY* plot_;
    /// Control identifiers
    enum {
        ID_LARGEPLOT = 10008,
        ID_OPTIONS = 10009,
        ID_MONTH_AVERAGE = 10003,
        ID_MENU_EXPORT = 10022,
        ID_EXPORT_IMAGE = 10023,
        ID_RAINFALL_TOTAL = 10024
    };
////@end LargePlot member variables



    
};

#endif
    // _LARGEPLOT_H_
