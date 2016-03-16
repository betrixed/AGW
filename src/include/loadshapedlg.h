/////////////////////////////////////////////////////////////////////////////
// Name:        loadshapedlg.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Fri 22 Jan 2016 00:30:52 AEDT
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _LOADSHAPEDLG_H_
#define _LOADSHAPEDLG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/spinctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_LOADSHAPEDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_LOADSHAPEDLG_TITLE _("Load Shape File")
#define SYMBOL_LOADSHAPEDLG_IDNAME ID_LOADSHAPEDLG
#define SYMBOL_LOADSHAPEDLG_SIZE wxDefaultSize
#define SYMBOL_LOADSHAPEDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * LoadShapeDlg class declaration
 */


class LoadShapeCmd;

class LoadShapeDlg: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( LoadShapeDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    LoadShapeDlg();
    LoadShapeDlg( wxWindow* parent, wxWindowID id = SYMBOL_LOADSHAPEDLG_IDNAME, const wxString& caption = SYMBOL_LOADSHAPEDLG_TITLE, const wxPoint& pos = SYMBOL_LOADSHAPEDLG_POSITION, const wxSize& size = SYMBOL_LOADSHAPEDLG_SIZE, long style = SYMBOL_LOADSHAPEDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_LOADSHAPEDLG_IDNAME, const wxString& caption = SYMBOL_LOADSHAPEDLG_TITLE, const wxPoint& pos = SYMBOL_LOADSHAPEDLG_POSITION, const wxSize& size = SYMBOL_LOADSHAPEDLG_SIZE, long style = SYMBOL_LOADSHAPEDLG_STYLE );

    /// Destructor
    ~LoadShapeDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin LoadShapeDlg event handler declarations

////@end LoadShapeDlg event handler declarations

////@begin LoadShapeDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end LoadShapeDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin LoadShapeDlg member variables
    wxTextCtrl* mPathText;
    wxTextCtrl* mTableText;
    wxTextCtrl* mGeomText;
    wxCheckBox* mCompressCheck;
    wxCheckBox* mForce2DCheck;
    wxCheckBox* mRTreeCheck;
    wxRadioBox* mGeomRadio;
    wxComboBox* mGeomSelect;
    wxRadioBox* mColumnRadio;
    wxComboBox* mColumnSelect;
    wxSpinCtrl* mSRIDSpin;
    wxListBox* mCharsetSelect;
    /// Control identifiers
    enum {
        ID_LOADSHAPEDLG = 10000,
        ID_LDSHP_PATH = 10038,
        ID_LDSHP_TABLE = 10002,
        ID_LDSHP_COLUMN = 10003,
        ID_LDSHP_COMPRESSED = 10006,
        ID_LDSHP_COERCE_2D = 10005,
        ID_LDSHP_RTREE = 10007,
        ID_LDSHP_USER_GTYPE = 10012,
        ID_LDSHP_GTYPE = 10013,
        ID_LDSHP_USER_PKEY = 10008,
        ID_LDSHP_PKCOL = 10009,
        ID_LDSHP_SRID = 10001,
        ID_LDSHP_CHARSET = 10011
    };
////@end LoadShapeDlg member variables

    wxArrayString dbFields;
    wxArrayString geomTypes;
    wxArrayString charsetTypes;

    void OnUserPKey(wxCommandEvent& event);
    void OnUserGType(wxCommandEvent& event);

    void SaveIn(LoadShapeCmd& cmd);

};

#endif
    // _LOADSHAPEDLG_H_
