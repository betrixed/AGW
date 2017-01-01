/////////////////////////////////////////////////////////////////////////////
// Name:        plotframe.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Sat 23 Jan 2016 10:25:31 AEDT
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _PLOTFRAME_H_
#define _PLOTFRAME_H_


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
class wxMenu;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_PLOTFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PLOTFRAME_TITLE _("PlotFrame")
#define SYMBOL_PLOTFRAME_IDNAME ID_PLOTFRAME
#define SYMBOL_PLOTFRAME_SIZE wxSize(400, 300)
#define SYMBOL_PLOTFRAME_POSITION wxDefaultPosition
////@end control identifiers

class PlotXY;
class AppData;
class AxisDlg;

/*!
 * PlotFrame class declaration
 */

class PlotFrame: public wxFrame
{
    DECLARE_CLASS( PlotFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    PlotFrame();
    PlotFrame( wxWindow* parent, wxWindowID id = SYMBOL_PLOTFRAME_IDNAME, const wxString& caption = SYMBOL_PLOTFRAME_TITLE, const wxPoint& pos = SYMBOL_PLOTFRAME_POSITION, const wxSize& size = SYMBOL_PLOTFRAME_SIZE, long style = SYMBOL_PLOTFRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PLOTFRAME_IDNAME, const wxString& caption = SYMBOL_PLOTFRAME_TITLE, const wxPoint& pos = SYMBOL_PLOTFRAME_POSITION, const wxSize& size = SYMBOL_PLOTFRAME_SIZE, long style = SYMBOL_PLOTFRAME_STYLE );

    /// Destructor
    ~PlotFrame();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin PlotFrame event handler declarations

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_OPEN_MENU
    void OnOpenMenuClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_SAVE_MENU
    void OnSaveMenuClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_CLOSE
    void OnCloseClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_EXPORT_PNG
    void OnExportPngClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_PLOT_SETTINGS
    void OnPlotSettingsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_LAYER_MENU
    void OnLayerMenuClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_AXIS
    void OnMenuAxisClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_LEGEND
    void OnLegendClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_DATA_VIEW
    void OnDataViewClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TREND_FIT
    void OnTrendFitClick( wxCommandEvent& event );

////@end PlotFrame event handler declarations

////@begin PlotFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end PlotFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin PlotFrame member variables
    wxMenu* mFileMenu;
    wxMenu* mMenuView;
    /// Control identifiers
    enum {
        ID_PLOTFRAME = 10000,
        ID_OPEN_MENU = 10040,
        ID_SAVE_MENU = 10039,
        ID_CLOSE = 10041,
        ID_EXPORT_PNG = 10044,
        ID_PLOT_SETTINGS = 10042,
        ID_LAYER_MENU = 10045,
        ID_MENU_AXIS = 10061,
        ID_LEGEND = 10046,
        ID_DATA_VIEW = 10047,
        ID_TREND_FIT = 10062
    };
////@end PlotFrame member variables


    PlotXY*     thePlot;
    AppData*    theApp;
    AxisDlg*    axisDlg;

    void  setPlot(PlotXY* p);
    void AdjustFixed(bool toContent = false);
};

#endif
    // _PLOTFRAME_H_
