/////////////////////////////////////////////////////////////////////////////
// Name:        monthaverage.cpp
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     02/11/2013 13:25:39
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

#include "monthaverage.h"

////@begin XPM images
////@end XPM images


/*
 * MonthAverage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( MonthAverage, wxDialog )


/*
 * MonthAverage event table definition
 */

BEGIN_EVENT_TABLE( MonthAverage, wxDialog )

////@begin MonthAverage event table entries
////@end MonthAverage event table entries

END_EVENT_TABLE()


/*
 * MonthAverage constructors
 */

MonthAverage::MonthAverage()
{
    Init();
}

MonthAverage::MonthAverage( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * MonthAverage creator
 */

bool MonthAverage::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MonthAverage creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end MonthAverage creation
    return true;
}


/*
 * MonthAverage destructor
 */

MonthAverage::~MonthAverage()
{
////@begin MonthAverage destruction
////@end MonthAverage destruction
}


/*
 * Member initialisation
 */

void MonthAverage::Init()
{
////@begin MonthAverage member initialisation
    editFrom_ = NULL;
    editTo_ = NULL;
    editCompareFrom_ = NULL;
    editCompareTo_ = NULL;
////@end MonthAverage member initialisation
}


/*
 * Control creation for MonthAverage
 */

void MonthAverage::CreateControls()
{    
////@begin MonthAverage content construction
    MonthAverage* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxGridBagSizer* itemGridBagSizer3 = new wxGridBagSizer(0, 0);
    itemGridBagSizer3->SetEmptyCellSize(wxSize(10, 20));
    itemBoxSizer2->Add(itemGridBagSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Baseline from year"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(itemStaticText4, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    editFrom_ = new wxTextCtrl( itemDialog1, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(editFrom_, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Baseline to year"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(itemStaticText6, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    editTo_ = new wxTextCtrl( itemDialog1, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(editTo_, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Comparison from year"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(itemStaticText8, wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    editCompareFrom_ = new wxTextCtrl( itemDialog1, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(editCompareFrom_, wxGBPosition(2, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("Compare to year"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(itemStaticText10, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    editCompareTo_ = new wxTextCtrl( itemDialog1, ID_TEXTCTRL6, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(editCompareTo_, wxGBPosition(3, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton13 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton13->SetDefault();
    itemBoxSizer12->Add(itemButton13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer12->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton15 = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end MonthAverage content construction
}


/*
 * Should we show tooltips?
 */

bool MonthAverage::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap MonthAverage::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MonthAverage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end MonthAverage bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon MonthAverage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MonthAverage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MonthAverage icon retrieval
}

void MonthAverage::SetOptions(MonthAverageOptions& val)
{
    wxString s;
    s.Printf("%d",val.baseLineStartYear);
    editFrom_->SetValue(s);
    s.Printf("%d",val.baseLineEndYear);
    editTo_->SetValue(s);
    s.Printf("%d",val.compareStartYear);
    editCompareFrom_->SetValue(s);
    s.Printf("%d",val.compareEndYear);
    editCompareTo_->SetValue(s);
}

bool MonthAverage::UpdateOptions(MonthAverageOptions& val)
{
    wxString s;
    MonthAverageOptions test;
    
    try {
        s = editFrom_->GetValue();
        test.baseLineStartYear = wxAtoi(s);
        s = editTo_->GetValue();
        test.baseLineEndYear = wxAtoi(s);
        s = editCompareFrom_->GetValue();
        test.compareStartYear = wxAtoi(s);
        s = editCompareTo_->GetValue();
        test.compareEndYear = wxAtoi(s);
        val = test;
        return true;
    }
    catch(...)
    {
        return false;
    }
    
}
