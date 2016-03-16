/////////////////////////////////////////////////////////////////////////////
// Name:        countrylistdlg.h
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     Sat 06 Feb 2016 10:24:57 AEDT
// RCS-ID:      
// Copyright:   Michael Rynn
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _COUNTRYLISTDLG_H_
#define _COUNTRYLISTDLG_H_


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
#define SYMBOL_COUNTRYLISTDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_COUNTRYLISTDLG_TITLE _("CountryListDlg")
#define SYMBOL_COUNTRYLISTDLG_IDNAME ID_COUNTRYLISTDLG
#define SYMBOL_COUNTRYLISTDLG_SIZE wxSize(400, 300)
#define SYMBOL_COUNTRYLISTDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * CountryListDlg class declaration
 */

class CountryListDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( CountryListDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CountryListDlg();
    CountryListDlg( wxWindow* parent, wxWindowID id = SYMBOL_COUNTRYLISTDLG_IDNAME, const wxString& caption = SYMBOL_COUNTRYLISTDLG_TITLE, const wxPoint& pos = SYMBOL_COUNTRYLISTDLG_POSITION, const wxSize& size = SYMBOL_COUNTRYLISTDLG_SIZE, long style = SYMBOL_COUNTRYLISTDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_COUNTRYLISTDLG_IDNAME, const wxString& caption = SYMBOL_COUNTRYLISTDLG_TITLE, const wxPoint& pos = SYMBOL_COUNTRYLISTDLG_POSITION, const wxSize& size = SYMBOL_COUNTRYLISTDLG_SIZE, long style = SYMBOL_COUNTRYLISTDLG_STYLE );

    /// Destructor
    ~CountryListDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin CountryListDlg event handler declarations

////@end CountryListDlg event handler declarations

////@begin CountryListDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end CountryListDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin CountryListDlg member variables
    wxListBox* mCountry;
    /// Control identifiers
    enum {
        ID_COUNTRYLISTDLG = 10000,
        ID_COUNTRY_LIST = 10001
    };
////@end CountryListDlg member variables
};

#endif
    // _COUNTRYLISTDLG_H_
