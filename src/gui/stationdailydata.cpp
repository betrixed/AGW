/////////////////////////////////////////////////////////////////////////////
// Name:        stationdailydata.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Tue 02 Jun 2020 13:27:30 AEST
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

#include "stationdailydata.h"

////@begin XPM images
////@end XPM images


/*
 * StationDailyData type definition
 */

IMPLEMENT_CLASS( StationDailyData, wxFrame )


/*
 * StationDailyData event table definition
 */

BEGIN_EVENT_TABLE( StationDailyData, wxFrame )

////@begin StationDailyData event table entries
////@end StationDailyData event table entries

END_EVENT_TABLE()

#include "appdata.h"
/*
 * StationDailyData constructors
 */

StationDailyData::StationDailyData()
{
    Init();
}

StationDailyData::StationDailyData( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * StationDailyData creator
 */

bool StationDailyData::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin StationDailyData creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end StationDailyData creation
    return true;
}


/*
 * StationDailyData destructor
 */

StationDailyData::~StationDailyData()
{
////@begin StationDailyData destruction
////@end StationDailyData destruction
}


/*
 * Member initialisation
 */

void StationDailyData::Init()
{
////@begin StationDailyData member initialisation
    btnSpinShow = NULL;
    txtDailyData = NULL;
////@end StationDailyData member initialisation
}


/*
 * Control creation for StationDailyData
 */

void StationDailyData::CreateControls()
{
////@begin StationDailyData content construction
    StationDailyData* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer1);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer1->Add(itemBoxSizer2, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText3 = new wxStaticText( itemFrame1, wxID_STATIC, _("Year"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl4 = new wxTextCtrl( itemFrame1, ID_SHOW_YEAR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemTextCtrl4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemFrame1, wxID_STATIC, _("Month"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice6Strings;
    itemChoice6Strings.Add(_("1"));
    itemChoice6Strings.Add(_("2"));
    itemChoice6Strings.Add(_("3"));
    itemChoice6Strings.Add(_("4"));
    itemChoice6Strings.Add(_("5"));
    itemChoice6Strings.Add(_("6"));
    itemChoice6Strings.Add(_("7"));
    itemChoice6Strings.Add(_("8"));
    itemChoice6Strings.Add(_("9"));
    itemChoice6Strings.Add(_("10"));
    itemChoice6Strings.Add(_("11"));
    itemChoice6Strings.Add(_("12"));
    wxChoice* itemChoice6 = new wxChoice( itemFrame1, ID_SHOW_MONTH, wxDefaultPosition, wxDefaultSize, itemChoice6Strings, 0 );
    itemChoice6->SetStringSelection(_("1"));
    itemBoxSizer2->Add(itemChoice6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemFrame1, ID_SHOW_NOW, _("Show"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnSpinShow = new wxSpinButton( itemFrame1, ID_SPINSHOW, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL );
    btnSpinShow->SetRange(0, 100);
    btnSpinShow->SetValue(0);
    itemBoxSizer2->Add(btnSpinShow, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtDailyData = new wxTextCtrl( itemFrame1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxHSCROLL|wxSUNKEN_BORDER );
    itemBoxSizer1->Add(txtDailyData, 1, wxGROW|wxALL, 5);

////@end StationDailyData content construction
}


/*
 * Should we show tooltips?
 */

bool StationDailyData::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap StationDailyData::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin StationDailyData bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end StationDailyData bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon StationDailyData::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin StationDailyData icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end StationDailyData icon retrieval
}

void StationDailyData::SetStationId(const wxString& name)
{
    stationId = name;
    auto app = AppData::instance();

    auto path = app.get("ghcn-daily");

    wxString filename = path  + "/" + stationId + ".csv";


}