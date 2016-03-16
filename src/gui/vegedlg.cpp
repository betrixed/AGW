/////////////////////////////////////////////////////////////////////////////
// Name:        vegedlg.cpp
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     Mon 08 Feb 2016 11:03:07 AEDT
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

#include "../include/vegedlg.h"

////@begin XPM images
////@end XPM images


/*
 * VegeListDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( VegeListDlg, wxDialog )


/*
 * VegeListDlg event table definition
 */

BEGIN_EVENT_TABLE( VegeListDlg, wxDialog )

////@begin VegeListDlg event table entries
////@end VegeListDlg event table entries

END_EVENT_TABLE()


/*
 * VegeListDlg constructors
 */

VegeListDlg::VegeListDlg()
{
    Init();
}

VegeListDlg::VegeListDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * VegeListDlg creator
 */

bool VegeListDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin VegeListDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end VegeListDlg creation
    return true;
}


/*
 * VegeListDlg destructor
 */

VegeListDlg::~VegeListDlg()
{
////@begin VegeListDlg destruction
////@end VegeListDlg destruction
}


/*
 * Member initialisation
 */

void VegeListDlg::Init()
{
////@begin VegeListDlg member initialisation
    mVegeList = NULL;
////@end VegeListDlg member initialisation
}


/*
 * Control creation for VegeListDlg
 */

void VegeListDlg::CreateControls()
{    
////@begin VegeListDlg content construction
    VegeListDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Select Vegetation Type"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mVegeListStrings;
    mVegeList = new wxListBox( itemDialog1, ID_VEGE_LIST, wxDefaultPosition, wxSize(-1, 280), mVegeListStrings, wxLB_SINGLE );
    itemBoxSizer2->Add(mVegeList, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer6 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton7);

    wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer6->AddButton(itemButton8);

    itemStdDialogButtonSizer6->Realize();

////@end VegeListDlg content construction
}


/*
 * Should we show tooltips?
 */

bool VegeListDlg::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap VegeListDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin VegeListDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end VegeListDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon VegeListDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin VegeListDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end VegeListDlg icon retrieval
}
