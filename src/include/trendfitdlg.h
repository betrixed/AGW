/////////////////////////////////////////////////////////////////////////////
// Name:        trendfitdlg.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Tue 19 Apr 2016 12:41:19 AEST
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _TRENDFITDLG_H_
#define _TRENDFITDLG_H_


/*!
 * Includes
 */

////@begin includes
////@end includes

#include "series.hpp"
#include "plotlua.h"
/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_TRENDFITDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_TRENDFITDLG_TITLE _("Least Squares Trend Fit")
#define SYMBOL_TRENDFITDLG_IDNAME ID_TRENDFITDLG
#define SYMBOL_TRENDFITDLG_SIZE wxSize(400, 300)
#define SYMBOL_TRENDFITDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * TrendFitDlg class declaration
 */

class TrendFitDlg: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( TrendFitDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    TrendFitDlg();
    TrendFitDlg( wxWindow* parent, wxWindowID id = SYMBOL_TRENDFITDLG_IDNAME, const wxString& caption = SYMBOL_TRENDFITDLG_TITLE, const wxPoint& pos = SYMBOL_TRENDFITDLG_POSITION, const wxSize& size = SYMBOL_TRENDFITDLG_SIZE, long style = SYMBOL_TRENDFITDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TRENDFITDLG_IDNAME, const wxString& caption = SYMBOL_TRENDFITDLG_TITLE, const wxPoint& pos = SYMBOL_TRENDFITDLG_POSITION, const wxSize& size = SYMBOL_TRENDFITDLG_SIZE, long style = SYMBOL_TRENDFITDLG_STYLE );

    /// Destructor
    ~TrendFitDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin TrendFitDlg event handler declarations

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_YLIST
    void OnYlistSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_XLIST
    void OnXlistSelected( wxCommandEvent& event );

////@end TrendFitDlg event handler declarations

////@begin TrendFitDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end TrendFitDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin TrendFitDlg member variables
    wxListBox* mYList;
    wxTextCtrl* mYMin;
    wxTextCtrl* mYMax;
    wxChoice* mYUnits;
    wxListBox* mXList;
    wxTextCtrl* mXMin;
    wxTextCtrl* mXMax;
    wxChoice* mXUnits;
    wxTextCtrl* mFitName;
    /// Control identifiers
    enum {
        ID_TRENDFITDLG = 10000,
        ID_YLIST = 10001,
        ID_YMIN = 10003,
        ID_YMAX = 10004,
        ID_YUNITS = 10009,
        ID_XLIST = 10002,
        ID_XMIN = 10005,
        ID_XMAX = 10006,
        ID_XUNITS = 10008,
        ID_FIT_NAME = 10007
    };
////@end TrendFitDlg member variables


    std::vector<agw::SeriesPtr>  series_;

    void doTrendFit(agw::PlotLua_sptr plp);

    private:
        void doListSelect(wxListBox* mList, wxTextCtrl* txtMin, wxTextCtrl* txtMax, wxChoice* chUnits);

};

#endif
    // _TRENDFITDLG_H_
