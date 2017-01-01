/////////////////////////////////////////////////////////////////////////////
// Name:        trendfitdlg.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Tue 19 Apr 2016 12:41:19 AEST
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

#include "trendfitdlg.h"
#include "helper.h"

////@begin XPM images
////@end XPM images


/*
 * TrendFitDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( TrendFitDlg, wxDialog )


/*
 * TrendFitDlg event table definition
 */

BEGIN_EVENT_TABLE( TrendFitDlg, wxDialog )

////@begin TrendFitDlg event table entries
    EVT_LISTBOX( ID_YLIST, TrendFitDlg::OnYlistSelected )
    EVT_LISTBOX( ID_XLIST, TrendFitDlg::OnXlistSelected )
////@end TrendFitDlg event table entries

END_EVENT_TABLE()

using namespace agw;
/*
 * TrendFitDlg constructors
 */

TrendFitDlg::TrendFitDlg()
{
    Init();
}

TrendFitDlg::TrendFitDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * TrendFitDlg creator
 */

bool TrendFitDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin TrendFitDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end TrendFitDlg creation
    return true;
}


/*
 * TrendFitDlg destructor
 */

TrendFitDlg::~TrendFitDlg()
{
////@begin TrendFitDlg destruction
////@end TrendFitDlg destruction
}


/*
 * Member initialisation
 */

void TrendFitDlg::Init()
{
////@begin TrendFitDlg member initialisation
    mYList = NULL;
    mYMin = NULL;
    mYMax = NULL;
    mYUnits = NULL;
    mXList = NULL;
    mXMin = NULL;
    mXMax = NULL;
    mXUnits = NULL;
    mFitName = NULL;
////@end TrendFitDlg member initialisation
}


/*
 * Control creation for TrendFitDlg
 */

void TrendFitDlg::CreateControls()
{
////@begin TrendFitDlg content construction
    TrendFitDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Y-Values"));
    wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mYListStrings;
    mYList = new wxListBox( itemStaticBoxSizer4->GetStaticBox(), ID_YLIST, wxDefaultPosition, wxDefaultSize, mYListStrings, wxLB_SINGLE );
    itemStaticBoxSizer4->Add(mYList, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer6, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("Y Min"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYMin = new wxTextCtrl( itemStaticBoxSizer4->GetStaticBox(), ID_YMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(mYMin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("Y Max"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYMax = new wxTextCtrl( itemStaticBoxSizer4->GetStaticBox(), ID_YMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(mYMax, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxArrayString mYUnitsStrings;
    mYUnitsStrings.Add(_("number"));
    mYUnitsStrings.Add(_("year-month"));
    mYUnits = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_YUNITS, wxDefaultPosition, wxDefaultSize, mYUnitsStrings, 0 );
    mYUnits->SetStringSelection(_("number"));
    itemBoxSizer12->Add(mYUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer14Static = new wxStaticBox(itemDialog1, wxID_ANY, _("X-Values"));
    wxStaticBoxSizer* itemStaticBoxSizer14 = new wxStaticBoxSizer(itemStaticBoxSizer14Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer14, 0, wxGROW|wxALL, 5);

    wxArrayString mXListStrings;
    mXList = new wxListBox( itemStaticBoxSizer14->GetStaticBox(), ID_XLIST, wxDefaultPosition, wxDefaultSize, mXListStrings, wxLB_SINGLE );
    itemStaticBoxSizer14->Add(mXList, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer14->Add(itemBoxSizer16, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemStaticBoxSizer14->GetStaticBox(), wxID_STATIC, _("X Min"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(itemStaticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXMin = new wxTextCtrl( itemStaticBoxSizer14->GetStaticBox(), ID_XMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(mXMin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer14->Add(itemBoxSizer19, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText20 = new wxStaticText( itemStaticBoxSizer14->GetStaticBox(), wxID_STATIC, _("X Max"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemStaticText20, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXMax = new wxTextCtrl( itemStaticBoxSizer14->GetStaticBox(), ID_XMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(mXMax, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer14->Add(itemBoxSizer22, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxArrayString mXUnitsStrings;
    mXUnitsStrings.Add(_("number"));
    mXUnitsStrings.Add(_("year-month"));
    mXUnits = new wxChoice( itemStaticBoxSizer14->GetStaticBox(), ID_XUNITS, wxDefaultPosition, wxDefaultSize, mXUnitsStrings, 0 );
    mXUnits->SetStringSelection(_("number"));
    itemBoxSizer22->Add(mXUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer24, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText25 = new wxStaticText( itemDialog1, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer24->Add(itemStaticText25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mFitName = new wxTextCtrl( itemDialog1, ID_FIT_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer24->Add(mFitName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer27, 0, wxGROW|wxALL, 5);

    wxButton* itemButton28 = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer27->Add(itemButton28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer27->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton30 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer27->Add(itemButton30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end TrendFitDlg content construction
}


/*
 * Should we show tooltips?
 */

bool TrendFitDlg::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap TrendFitDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin TrendFitDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end TrendFitDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon TrendFitDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin TrendFitDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end TrendFitDlg icon retrieval
}





/*
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_XLIST
 */


void TrendFitDlg::doListSelect(wxListBox* mList, wxTextCtrl* txtMin, wxTextCtrl* txtMax, wxChoice* chUnits)
{
    int ix = mList->GetSelection();
    if (ix < 0 || ix >= series_.size())
        return;

    SeriesPtr sp = series_[ix];

    SeriesUnit su = sp->units();

   // wxLogMessage("Series Unit = %d", int(su));
    double dmin, dmax;
    std::string text;
    switch(su)
    {
        case SeriesUnit::DATE_YEAR_MONTH:
            {
                int year;
                wxDateTime::Month month;

                sp->getDataLimits(dmin, dmax);
                mjdn_to_year_month(dmax, year, month);
                text = std::to_string(year) + "-" + std::to_string(int(month)+1);
                txtMax->SetValue(text);
                mjdn_to_year_month(dmin, year, month);
                text = std::to_string(year) + "-" + std::to_string(int(month)+1);
                txtMin->SetValue(text);
                chUnits->SetSelection(1);

            }
            break;
        default:
            sp->getDataLimits(dmin, dmax);
            text = std::to_string(dmin);
            txtMin->SetValue(text);
            text = std::to_string(dmax);
            txtMax->SetValue(text);
            chUnits->SetSelection(0);
            break;

    }
}

void TrendFitDlg::OnXlistSelected( wxCommandEvent& event )
{
    event.Skip(true);
    doListSelect(mXList, mXMin, mXMax, mXUnits);

}
/*
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_YLIST
 */

void TrendFitDlg::OnYlistSelected( wxCommandEvent& event )
{
    event.Skip(true);
    doListSelect(mYList, mYMin, mYMax, mYUnits);
}

// parse yyyy-mm,  convert to zero based month


void TrendFitDlg::doTrendFit(agw::PlotLuaPtr plp)
{
    int xsel = mXList->GetSelection();
    int ysel = mYList->GetSelection();

    if (xsel != ysel && xsel >= 0 && ysel >= 0 && xsel < series_.size() && ysel < series_.size())
    {
        SeriesPtr yseries = series_[ysel];
        SeriesPtr xseries = series_[xsel];

        std::string xmin_text = mXMin->GetValue().ToStdString();
        std::string xmax_text = mXMax->GetValue().ToStdString();
        double dmin = 0;
        double dmax = 0;
        int year;
        wxDateTime::Month month;

        switch (xseries->units())
        {
        case SeriesUnit::DATE_YEAR_MONTH:
            {
                // parse year-month
                to_year_month(xmin_text, year, month);
                dmin = to_mjdn(year, month);
                to_year_month(xmax_text, year, month);
                dmax = to_mjdn(year, month);

            }
            break;
        case SeriesUnit::DEFAULT:
        default:
            dmin = std::stod(xmin_text);
            dmax = std::stod(xmax_text);
            break;
        }

        PlotPtr pp = regressRange(*(xseries.get()), *(yseries.get()), dmin, dmax);
        plp->addLayer(pp);
    }
    else {
        // error message ** TODO validate message
    }
}
