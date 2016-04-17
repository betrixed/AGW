/////////////////////////////////////////////////////////////////////////////
// Name:        areaavgtempdlg.h
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     Sun 17 Apr 2016 21:39:05 AEST
// RCS-ID:      
// Copyright:   Michael Rynn
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _AREAAVGTEMPDLG_H_
#define _AREAAVGTEMPDLG_H_


/*!
 * Includes
 */

////@begin includes
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
#define SYMBOL_AREAAVGTEMPDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_AREAAVGTEMPDLG_TITLE _("Area Weighted Global Temperature Trend")
#define SYMBOL_AREAAVGTEMPDLG_IDNAME ID_AREAAVGTEMPDLG
#define SYMBOL_AREAAVGTEMPDLG_SIZE wxSize(400, 300)
#define SYMBOL_AREAAVGTEMPDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * AreaAvgTempDlg class declaration
 */

class AreaAvgTempDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( AreaAvgTempDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AreaAvgTempDlg();
    AreaAvgTempDlg( wxWindow* parent, wxWindowID id = SYMBOL_AREAAVGTEMPDLG_IDNAME, const wxString& caption = SYMBOL_AREAAVGTEMPDLG_TITLE, const wxPoint& pos = SYMBOL_AREAAVGTEMPDLG_POSITION, const wxSize& size = SYMBOL_AREAAVGTEMPDLG_SIZE, long style = SYMBOL_AREAAVGTEMPDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_AREAAVGTEMPDLG_IDNAME, const wxString& caption = SYMBOL_AREAAVGTEMPDLG_TITLE, const wxPoint& pos = SYMBOL_AREAAVGTEMPDLG_POSITION, const wxSize& size = SYMBOL_AREAAVGTEMPDLG_SIZE, long style = SYMBOL_AREAAVGTEMPDLG_STYLE );

    /// Destructor
    ~AreaAvgTempDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin AreaAvgTempDlg event handler declarations

////@end AreaAvgTempDlg event handler declarations

////@begin AreaAvgTempDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AreaAvgTempDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AreaAvgTempDlg member variables
    wxTextCtrl* mBaseFrom;
    wxTextCtrl* mBaseTo;
    wxCheckBox* mBoxAdjust;
    wxTextCtrl* mRange;
    wxTextCtrl* mFileBase;
    /// Control identifiers
    enum {
        ID_AREAAVGTEMPDLG = 10000,
        ID_BASE_FROM = 10001,
        ID_BASE_TO = 10002,
        ID_BOX_ADJUST = 10003,
        ID_RANGE = 10004,
        ID_FILE_BASE = 10005
    };
////@end AreaAvgTempDlg member variables
};

#endif
    // _AREAAVGTEMPDLG_H_
