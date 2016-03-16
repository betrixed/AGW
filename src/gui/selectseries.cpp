/////////////////////////////////////////////////////////////////////////////
// Name:        selectseries.cpp
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     20/09/2015 18:29:22
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

#include "selectseries.h"

////@begin XPM images
////@end XPM images


/*
 * SelectSeries type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SelectSeries, wxDialog )


/*
 * SelectSeries event table definition
 */

BEGIN_EVENT_TABLE( SelectSeries, wxDialog )

////@begin SelectSeries event table entries
////@end SelectSeries event table entries

END_EVENT_TABLE()


/*
 * SelectSeries constructors
 */

SelectSeries::SelectSeries()
{
    Init();
}

SelectSeries::SelectSeries( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * SelectSeries creator
 */

bool SelectSeries::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SelectSeries creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end SelectSeries creation
    return true;
}


/*
 * SelectSeries destructor
 */

SelectSeries::~SelectSeries()
{
////@begin SelectSeries destruction
////@end SelectSeries destruction
}


/*
 * Member initialisation
 */

void SelectSeries::Init()
{
////@begin SelectSeries member initialisation
    listSeries_ = NULL;
    btnOK_ = NULL;
    btnCancel_ = NULL;
////@end SelectSeries member initialisation
}


/*
 * Control creation for SelectSeries
 */

void SelectSeries::CreateControls()
{    
////@begin SelectSeries content construction
    SelectSeries* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxLEFT|wxRIGHT|wxTOP, 2);

    wxArrayString listSeries_Strings;
    listSeries_ = new wxCheckListBox( itemDialog1, ID_CHECKLISTBOX, wxDefaultPosition, wxDefaultSize, listSeries_Strings, wxLB_SINGLE );
    itemBoxSizer3->Add(listSeries_, 1, wxGROW|wxALL, 5);

    wxTextCtrl* itemTextCtrl5 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemBoxSizer2->Add(itemTextCtrl5, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemBoxSizer6->Add(100, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnOK_ = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(btnOK_, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer6->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnCancel_ = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(btnCancel_, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer6->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SelectSeries content construction
}


/*
 * Should we show tooltips?
 */

bool SelectSeries::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap SelectSeries::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SelectSeries bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SelectSeries bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon SelectSeries::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SelectSeries icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SelectSeries icon retrieval
}
