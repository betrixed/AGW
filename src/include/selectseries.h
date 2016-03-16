/////////////////////////////////////////////////////////////////////////////
// Name:        selectseries.h
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     01/11/2013 19:49:15
// RCS-ID:      
// Copyright:   Michael Rynn
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTSERIES_H_
#define _SELECTSERIES_H_


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
#define SYMBOL_SELECTSERIES_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_SELECTSERIES_TITLE _("Select Series")
#define SYMBOL_SELECTSERIES_IDNAME ID_SELECTSERIES
#define SYMBOL_SELECTSERIES_SIZE wxSize(400, 300)
#define SYMBOL_SELECTSERIES_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SelectSeries class declaration
 */

class SelectSeries: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( SelectSeries )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SelectSeries();
    SelectSeries( wxWindow* parent, wxWindowID id = SYMBOL_SELECTSERIES_IDNAME, const wxString& caption = SYMBOL_SELECTSERIES_TITLE, const wxPoint& pos = SYMBOL_SELECTSERIES_POSITION, const wxSize& size = SYMBOL_SELECTSERIES_SIZE, long style = SYMBOL_SELECTSERIES_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SELECTSERIES_IDNAME, const wxString& caption = SYMBOL_SELECTSERIES_TITLE, const wxPoint& pos = SYMBOL_SELECTSERIES_POSITION, const wxSize& size = SYMBOL_SELECTSERIES_SIZE, long style = SYMBOL_SELECTSERIES_STYLE );

    /// Destructor
    ~SelectSeries();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin SelectSeries event handler declarations

////@end SelectSeries event handler declarations

////@begin SelectSeries member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SelectSeries member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SelectSeries member variables
    wxCheckListBox* listSeries_;
    wxButton* btnOK_;
    wxButton* btnCancel_;
    /// Control identifiers
    enum {
        ID_SELECTSERIES = 10005,
        ID_CHECKLISTBOX = 10006,
        ID_TEXTCTRL = 10007
    };
////@end SelectSeries member variables
};

#endif
    // _SELECTSERIES_H_
