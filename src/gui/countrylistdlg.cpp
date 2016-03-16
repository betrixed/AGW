/////////////////////////////////////////////////////////////////////////////
// Name:        countrylistdlg.cpp
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     Sat 06 Feb 2016 10:24:57 AEDT
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

#include "countrylistdlg.h"

////@begin XPM images
////@end XPM images


/*
 * CountryListDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CountryListDlg, wxDialog )


/*
 * CountryListDlg event table definition
 */

BEGIN_EVENT_TABLE( CountryListDlg, wxDialog )

////@begin CountryListDlg event table entries
////@end CountryListDlg event table entries

END_EVENT_TABLE()


/*
 * CountryListDlg constructors
 */

CountryListDlg::CountryListDlg()
{
    Init();
}

CountryListDlg::CountryListDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * CountryListDlg creator
 */

bool CountryListDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin CountryListDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end CountryListDlg creation
    return true;
}


/*
 * CountryListDlg destructor
 */

CountryListDlg::~CountryListDlg()
{
////@begin CountryListDlg destruction
////@end CountryListDlg destruction
}


/*
 * Member initialisation
 */

void CountryListDlg::Init()
{
////@begin CountryListDlg member initialisation
    mCountry = NULL;
////@end CountryListDlg member initialisation
}


/*
 * Control creation for CountryListDlg
 */

void CountryListDlg::CreateControls()
{    
////@begin CountryListDlg content construction
    CountryListDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Select a Country Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mCountryStrings;
    mCountry = new wxListBox( itemDialog1, ID_COUNTRY_LIST, wxDefaultPosition, wxSize(-1, 280), mCountryStrings, wxLB_SINGLE );
    itemBoxSizer2->Add(mCountry, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer6 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton7);

    wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton8);

    itemStdDialogButtonSizer6->Realize();

////@end CountryListDlg content construction
}


/*
 * Should we show tooltips?
 */

bool CountryListDlg::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap CountryListDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin CountryListDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end CountryListDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon CountryListDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin CountryListDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end CountryListDlg icon retrieval
}
