/////////////////////////////////////////////////////////////////////////////
// Name:        plotoptions.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     02/11/2013 11:52:48
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _PLOTOPTIONS_H_
#define _PLOTOPTIONS_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/gbsizer.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_PLOTOPTIONS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_PLOTOPTIONS_TITLE _("Plot Options")
#define SYMBOL_PLOTOPTIONS_IDNAME ID_PLOTOPTIONS
#define SYMBOL_PLOTOPTIONS_SIZE wxSize(400, 300)
#define SYMBOL_PLOTOPTIONS_POSITION wxDefaultPosition
////@end control identifiers

#include "plotxy.h"
#include "plotlua.h"
/*!
 * PlotOptions class declaration
 */

class PlotOptions: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( PlotOptions )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    PlotOptions();
    PlotOptions( wxWindow* parent, wxWindowID id = SYMBOL_PLOTOPTIONS_IDNAME, const wxString& caption = SYMBOL_PLOTOPTIONS_TITLE, const wxPoint& pos = SYMBOL_PLOTOPTIONS_POSITION, const wxSize& size = SYMBOL_PLOTOPTIONS_SIZE, long style = SYMBOL_PLOTOPTIONS_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PLOTOPTIONS_IDNAME, const wxString& caption = SYMBOL_PLOTOPTIONS_TITLE, const wxPoint& pos = SYMBOL_PLOTOPTIONS_POSITION, const wxSize& size = SYMBOL_PLOTOPTIONS_SIZE, long style = SYMBOL_PLOTOPTIONS_STYLE );

    /// Destructor
    ~PlotOptions();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin PlotOptions event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
    void OnApplyClick( wxCommandEvent& event );

////@end PlotOptions event handler declarations

////@begin PlotOptions member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end PlotOptions member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin PlotOptions member variables
    wxTextCtrl* editLeftAxisArea;
    wxTextCtrl* editPixelsHeight_;
    wxTextCtrl* editPixelsWidth_;
    wxCheckBox* checkYAxisMajorGrid;
    wxCheckBox* checkYAxisMinorTicks;
    wxButton* btnOK_;
    /// Control identifiers
    enum {
        ID_PLOTOPTIONS = 10010,
        ID_TEXTCTRL7 = 10020,
        ID_TEXTCTRL8 = 10021,
        ID_TEXTCTRL1 = 10011,
        ID_TEXTCTRL2 = 10012,
        ID_CHECKBOX = 10018,
        ID_CHECKBOX1 = 10019
    };
////@end PlotOptions member variables

    void SetFromPlot();
    int oldHPixels_;
    int oldVPixels_;
    int oldLeftAxis_;
    int oldBottomAxis_;

    PlotXY* plotWindow_;
    agw::PlotLua_sptr plot_;
};

#endif
    // _PLOTOPTIONS_H_
