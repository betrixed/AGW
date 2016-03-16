/////////////////////////////////////////////////////////////////////////////
// Name:        vegedlg.h
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     Mon 08 Feb 2016 11:03:07 AEDT
// RCS-ID:      
// Copyright:   Michael Rynn
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _VEGEDLG_H_
#define _VEGEDLG_H_


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
#define SYMBOL_VEGELISTDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_VEGELISTDLG_TITLE _("VegeListDlg")
#define SYMBOL_VEGELISTDLG_IDNAME ID_VEGELISTDLG
#define SYMBOL_VEGELISTDLG_SIZE wxSize(400, 300)
#define SYMBOL_VEGELISTDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * VegeListDlg class declaration
 */

class VegeListDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( VegeListDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    VegeListDlg();
    VegeListDlg( wxWindow* parent, wxWindowID id = SYMBOL_VEGELISTDLG_IDNAME, const wxString& caption = SYMBOL_VEGELISTDLG_TITLE, const wxPoint& pos = SYMBOL_VEGELISTDLG_POSITION, const wxSize& size = SYMBOL_VEGELISTDLG_SIZE, long style = SYMBOL_VEGELISTDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_VEGELISTDLG_IDNAME, const wxString& caption = SYMBOL_VEGELISTDLG_TITLE, const wxPoint& pos = SYMBOL_VEGELISTDLG_POSITION, const wxSize& size = SYMBOL_VEGELISTDLG_SIZE, long style = SYMBOL_VEGELISTDLG_STYLE );

    /// Destructor
    ~VegeListDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin VegeListDlg event handler declarations

////@end VegeListDlg event handler declarations

////@begin VegeListDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end VegeListDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin VegeListDlg member variables
    wxListBox* mVegeList;
    /// Control identifiers
    enum {
        ID_VEGELISTDLG = 10000,
        ID_VEGE_LIST = 10001
    };
////@end VegeListDlg member variables
};

#endif
    // _VEGEDLG_H_
