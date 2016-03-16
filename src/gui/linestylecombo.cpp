/////////////////////////////////////////////////////////////////////////////
// Name:        linestylecombo.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Wed 27 Jan 2016 12:24:45 AEDT
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

#include "linestylecombo.h"

////@begin XPM images
////@end XPM images

#include "datalayerdlg.h"
#include "helper.h"

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

LineStyleCombo::LineStyleCombo(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, const wxArrayString& choices, long style, const wxValidator& validator)
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
 * Draws the given item
 */

void LineStyleCombo::OnDrawItem(wxDC& dc, const wxRect& rect, int item, int flags) const
{
    if ( item == wxNOT_FOUND )
            return;

        wxRect r(rect);
        r.Deflate(3);
        r.height -= 2;

        wxPenStyle penStyle = indexToPenStyle(item);


        wxPen pen( dc.GetTextForeground(), 3, penStyle );

        // Get text colour as pen colour
        dc.SetPen( pen );

        if ( !(flags & wxODCB_PAINTING_CONTROL) )
        {
            dc.DrawText(GetString( item ),
                        r.x + 3,
                        (r.y + 0) + ( (r.height/2) - dc.GetCharHeight() )/2
                       );

            dc.DrawLine( r.x+5, r.y+((r.height/4)*3), r.x+r.width - 5, r.y+((r.height/4)*3) );
        }
        else
        {
            dc.DrawLine( r.x+5, r.y+r.height/2, r.x+r.width - 5, r.y+r.height/2 );
        }
}

/*
 * Draws the given item
 */

void LineStyleCombo::OnDrawBackground(wxDC& dc, const wxRect& rect, int item, int flags) const
{
            // combo control itself, use the default rendering.
        if ( (flags & (wxODCB_PAINTING_CONTROL|wxODCB_PAINTING_SELECTED)) ||
             (item & 1) == 0 )
        {
            wxOwnerDrawnComboBox::OnDrawBackground(dc,rect,item,flags);
            return;        if ( (flags & (wxODCB_PAINTING_CONTROL|wxODCB_PAINTING_SELECTED)) ||
             (item & 1) == 0 )
        {
            wxOwnerDrawnComboBox::OnDrawBackground(dc,rect,item,flags);
            return;
        }

        // Otherwise, draw every other background with different colour.
        wxColour bgCol(240,240,250);
        dc.SetBrush(wxBrush(bgCol));
        dc.SetPen(wxPen(bgCol));
        dc.DrawRectangle(rect);
        }

        // Otherwise, draw every other background with different colour.
        wxColour bgCol(240,240,250);
        dc.SetBrush(wxBrush(bgCol));
        dc.SetPen(wxPen(bgCol));
        dc.DrawRectangle(rect);
}

/*
 * Measures the given item's height
 */

wxCoord LineStyleCombo::OnMeasureItem(size_t WXUNUSED(item)) const
{

    return 36;

}

/*
 * Measures the given item's width
 */

wxCoord LineStyleCombo::OnMeasureItemWidth(size_t WXUNUSED(item)) const
{
    // TODO: implement me
    return -1;
}


/*
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_OWNERDRAWNCOMBOCTRL
 */

void LineStyleCombo::OnOwnerdrawncomboctrlSelected( wxCommandEvent& event )
{
    event.Skip(false);
    DataLayerDlg* dlg = getParentByID<DataLayerDlg>(this,DataLayerDlg::ID_DATALAYERDLG);
    if (dlg)
    {
        dlg->mDisplay->Refresh();
    }
}

