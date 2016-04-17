/////////////////////////////////////////////////////////////////////////////
// Name:        areaavgtempdlg.cpp
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     Sun 17 Apr 2016 21:39:05 AEST
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

#include "areaavgtempdlg.h"

////@begin XPM images
////@end XPM images


/*
 * AreaAvgTempDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AreaAvgTempDlg, wxDialog )


/*
 * AreaAvgTempDlg event table definition
 */

BEGIN_EVENT_TABLE( AreaAvgTempDlg, wxDialog )

////@begin AreaAvgTempDlg event table entries
////@end AreaAvgTempDlg event table entries

END_EVENT_TABLE()


/*
 * AreaAvgTempDlg constructors
 */

AreaAvgTempDlg::AreaAvgTempDlg()
{
    Init();
}

AreaAvgTempDlg::AreaAvgTempDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * AreaAvgTempDlg creator
 */

bool AreaAvgTempDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AreaAvgTempDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end AreaAvgTempDlg creation
    return true;
}


/*
 * AreaAvgTempDlg destructor
 */

AreaAvgTempDlg::~AreaAvgTempDlg()
{
////@begin AreaAvgTempDlg destruction
////@end AreaAvgTempDlg destruction
}


/*
 * Member initialisation
 */

void AreaAvgTempDlg::Init()
{
////@begin AreaAvgTempDlg member initialisation
    mBaseFrom = NULL;
    mBaseTo = NULL;
    mBoxAdjust = NULL;
    mRange = NULL;
    mFileBase = NULL;
////@end AreaAvgTempDlg member initialisation
}


/*
 * Control creation for AreaAvgTempDlg
 */

void AreaAvgTempDlg::CreateControls()
{    
////@begin AreaAvgTempDlg content construction
    AreaAvgTempDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Anomaly Base (inclusive) Years from "), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBaseFrom = new wxTextCtrl( itemDialog1, ID_BASE_FROM, _("1951"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(mBaseFrom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("to"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBaseTo = new wxTextCtrl( itemDialog1, ID_BASE_TO, _("1980"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(mBaseTo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mBoxAdjust = new wxCheckBox( itemDialog1, ID_BOX_ADJUST, _("If different to 1951-1980, compensate by average difference to this"), wxDefaultPosition, wxDefaultSize, 0 );
    mBoxAdjust->SetValue(true);
    itemBoxSizer8->Add(mBoxAdjust, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer10, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Range (km) of Station Influence"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mRange = new wxTextCtrl( itemDialog1, ID_RANGE, _("1200"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(mRange, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer13, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemDialog1, wxID_STATIC, _("Base File Name for plots"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemStaticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mFileBase = new wxTextCtrl( itemDialog1, ID_FILE_BASE, _("globaltemp"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(mFileBase, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer16, 0, wxGROW|wxALL, 5);

    itemBoxSizer16->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton18 = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(itemButton18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 15);

    itemBoxSizer16->Add(5, 5, 1, wxGROW|wxALL, 5);

    wxButton* itemButton20 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(itemButton20, 0, wxALIGN_CENTER_VERTICAL|wxALL, 15);

    itemBoxSizer16->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end AreaAvgTempDlg content construction
}


/*
 * Should we show tooltips?
 */

bool AreaAvgTempDlg::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap AreaAvgTempDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AreaAvgTempDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end AreaAvgTempDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon AreaAvgTempDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AreaAvgTempDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AreaAvgTempDlg icon retrieval
}
