/////////////////////////////////////////////////////////////////////////////
// Name:        linestylecombo.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Thu 17 Mar 2016 08:16:46 AEDT
// RCS-ID:
// Copyright:   Michael Rynn
// Licence:
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "../include/linestylecombo.h"

////@begin XPM images
////@end XPM images


/*
 * LineStyleCombo type definition
 */

IMPLEMENT_DYNAMIC_CLASS( LineStyleCombo, wxOwnerDrawnComboBox )


/*
 * LineStyleCombo event table definition
 */

BEGIN_EVENT_TABLE( LineStyleCombo, wxOwnerDrawnComboBox )

////@begin LineStyleCombo event table entries
    EVT_COMBOBOX( ID_OWNERDRAWNCOMBOCTRL, LineStyleCombo::OnOwnerdrawncomboctrlSelected )
////@end LineStyleCombo event table entries

END_EVENT_TABLE()


/*
 * LineStyleCombo constructors
 */

LineStyleCombo::LineStyleCombo()
{
    Init();
}

LineStyleCombo::LineStyleCombo(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size,const wxArrayString& choices, long style, const wxValidator& validator)
{
    Init();
    Create(parent, id, value, pos, size, choices, style, validator);
}


/*
 * LineStyleCombo creator
 */

bool LineStyleCombo::Create(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, const wxArrayString& choices, long style, const wxValidator& validator)
{
////@begin LineStyleCombo creation
    wxOwnerDrawnComboBox::Create(parent, id, value, pos, size, choices, style, validator);
    CreateControls();
////@end LineStyleCombo creation
    return true;
}


/*
 * LineStyleCombo destructor
 */

LineStyleCombo::~LineStyleCombo()
{
////@begin LineStyleCombo destruction
////@end LineStyleCombo destruction
}


/*
 * Member initialisation
 */

void LineStyleCombo::Init()
{
////@begin LineStyleCombo member initialisation
////@end LineStyleCombo member initialisation
}


/*
 * Control creation for LineStyleCombo
 */

void LineStyleCombo::CreateControls()
{
////@begin LineStyleCombo content construction
////@end LineStyleCombo content construction
}


/*
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_OWNERDRAWNCOMBOCTRL
 */

void LineStyleCombo::OnOwnerdrawncomboctrlSelected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_OWNERDRAWNCOMBOCTRL in LineStyleCombo.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_OWNERDRAWNCOMBOCTRL in LineStyleCombo.
}


/*
 * Draws the given item
 */

void LineStyleCombo::OnDrawItem(wxDC& dc, const wxRect& rect, int item, int flags) const
{
    // TODO: implement me
    wxOwnerDrawnComboBox::OnDrawItem(dc, rect, item, flags);
}

/*
 * Draws the given item
 */

void LineStyleCombo::OnDrawBackground(wxDC& dc, const wxRect& rect, int item, int flags) const
{
    // TODO: implement me
    wxOwnerDrawnComboBox::OnDrawBackground(dc, rect, item, flags);
}

/*
 * Measures the given item's height
 */

wxCoord LineStyleCombo::OnMeasureItem(size_t WXUNUSED(item)) const
{
    // TODO: implement me
    return -1;
}

/*
 * Measures the given item's width
 */

wxCoord LineStyleCombo::OnMeasureItemWidth(size_t WXUNUSED(item)) const
{
    // TODO: implement me
    return -1;
}
