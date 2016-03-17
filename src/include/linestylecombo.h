/////////////////////////////////////////////////////////////////////////////
// Name:        linestylecombo.h
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Wed 27 Jan 2016 12:25:35 AEDT
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _LINESTYLECOMBO_H_
#define _LINESTYLECOMBO_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/odcombo.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class LineStyleCombo;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_LINESTYLECOMBO_STYLE 0
#define SYMBOL_LINESTYLECOMBO_IDNAME ID_OWNERDRAWNCOMBOCTRL
#define SYMBOL_LINESTYLECOMBO_SIZE wxDefaultSize
#define SYMBOL_LINESTYLECOMBO_POSITION wxDefaultPosition
////@end control identifiers


class DataLayerDlg;
/*!
 * LineStyleCombo class declaration
 */

class LineStyleCombo: public wxOwnerDrawnComboBox
{
    DECLARE_DYNAMIC_CLASS( LineStyleCombo )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    LineStyleCombo();
    LineStyleCombo(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, const wxArrayString& choices, long style = 0, const wxValidator& validator = wxDefaultValidator);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, const wxArrayString& choices, long style = 0, const wxValidator& validator = wxDefaultValidator);

    /// Destructor
    ~LineStyleCombo();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin LineStyleCombo event handler declarations

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_OWNERDRAWNCOMBOCTRL
    void OnOwnerdrawncomboctrlSelected( wxCommandEvent& event );

////@end LineStyleCombo event handler declarations

////@begin LineStyleCombo member function declarations

////@end LineStyleCombo member function declarations

    /// Draws the given item
    virtual void OnDrawItem(wxDC& dc, const wxRect& rect, int item, int flags) const;

    /// Draws the given item's background
    virtual void OnDrawBackground(wxDC& dc, const wxRect& rect, int item, int flags) const;

    /// Measures the given item's height
    virtual wxCoord OnMeasureItem(size_t item) const;

    /// Measures the given item's width
    virtual wxCoord OnMeasureItemWidth(size_t item) const;

////@begin LineStyleCombo member variables
    /// Control identifiers
    enum {
        ID_OWNERDRAWNCOMBOCTRL = 10012
    };
////@end LineStyleCombo member variables


};

#endif
    // _LINESTYLECOMBO_H_
