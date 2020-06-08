/////////////////////////////////////////////////////////////////////////////
// Name:        stationsets.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     12/26/2015 8:04:14 PM
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
#include "wx/imaglist.h"
////@end includes

#include <wx/regex.h>

#include "stationsets.h"
#include <string>
#include <sstream>
#include <random>

#include "newsubset.h"
#include "stationquery.h"
#include "plotxy.h"
#include "plotlua.h"
#include "mainframe.h"
#include "plotframe.h"
#include "helper.h"
#include "json/json.h"
#include "appdata.h"
#include "mapframe.h"
#include "mapwindow.h"
#include "plotlua.h"


////@begin XPM images
////@end XPM images


#include "stationdailydata.h"

/*
 * StationSets type definition
 */

IMPLEMENT_CLASS( StationSets, wxFrame )


/*
 * StationSets event table definition
 */

BEGIN_EVENT_TABLE( StationSets, wxFrame )

////@begin StationSets event table entries
    EVT_MENU( ID_QUERY_NEW, StationSets::OnQueryNewClick )
    EVT_MENU( ID_QUERY_DELETE, StationSets::OnQueryDeleteClick )
    EVT_MENU( ID_SET_MAP, StationSets::OnSetMapClick )
    EVT_LISTBOX( SET_LIST, StationSets::OnSetListSelected )
    EVT_BUTTON( ID_RUN_QUERY, StationSets::OnRunQueryClick )
    EVT_BUTTON( ID_ADD_QUERY, StationSets::OnAddQueryClick )
    EVT_BUTTON( ID_GO_DERIVE, StationSets::OnGoDeriveClick )
    EVT_RADIOBOX( ID_SEL_BASE, StationSets::OnSelBaseSelected )
    EVT_BUTTON( ID_PLUS_YEAR, StationSets::OnPlusYearClick )
    EVT_BUTTON( ID_MINUS_YEAR, StationSets::OnMinusYearClick )
    EVT_BUTTON( ID_PLOT_YEARS, StationSets::OnPlotYearsClick )
    EVT_BUTTON( ID_TIME_SERIES, StationSets::OnTimeSeriesClick )
    EVT_BUTTON( ID_STATION_DATA, StationSets::OnStationDataClick )
////@end StationSets event table entries

END_EVENT_TABLE()


/*
 * StationSets constructors
 */
using namespace agw;

enum { AVG_REAL_TEMP = 1};

StationSets::StationSets()
{
    Init();
}

StationSets::StationSets( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * StationSets creator
 */

bool StationSets::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin StationSets creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end StationSets creation
    return true;
}


/*
 * StationSets destructor
 */

StationSets::~StationSets()
{
////@begin StationSets destruction
////@end StationSets destruction
}


/*
 * Member initialisation
 */

void StationSets::Init()
{
////@begin StationSets member initialisation
    menuDerive = NULL;
    listSet = NULL;
    mRrun = NULL;
    mBook_ = NULL;
    txtSetXML = NULL;
    editSetName = NULL;
    mQueryList = NULL;
    mVSizer = NULL;
    pageDerive = NULL;
    mMeasure = NULL;
    mBaseStart = NULL;
    mBaseEnd = NULL;
    mBoxFit = NULL;
    mFitFrom = NULL;
    mFitTo = NULL;
    mBoxYears = NULL;
    mYearFrom = NULL;
    mYearTo = NULL;
    mMeasure2 = NULL;
    radboxReal = NULL;
    boxBaseDiff = NULL;
    mBaseStart2 = NULL;
    mBaseEnd2 = NULL;
    mYearToAdd = NULL;
    mYearList = NULL;
    mTAVG = NULL;
    mMMAX = NULL;
    mTMAX = NULL;
    mTMIN = NULL;
    mGrid_ = NULL;
////@end StationSets member initialisation
}


/*
 * Control creation for StationSets
 */

void StationSets::CreateControls()
{
////@begin StationSets content construction
    StationSets* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_QUERY_NEW, _("New Query"), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(ID_QUERY_DELETE, _("Delete Station Set"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("Operations"));
    menuDerive = new wxMenu;
    menuDerive->Append(ID_SET_MAP, _("Map"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(menuDerive, _("View"));
    itemFrame1->SetMenuBar(menuBar);

    wxSplitterWindow* itemSplitterWindow9 = new wxSplitterWindow( itemFrame1, ID_SPLIT_SETS, wxDefaultPosition, wxSize(150, 100), wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow9->SetMinimumPaneSize(0);
    itemSplitterWindow9->SetSashGravity(0.2);

    wxPanel* itemPanel10 = new wxPanel( itemSplitterWindow9, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel10->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
    itemPanel10->SetSizer(itemBoxSizer11);

    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel10, wxID_H_SET_LIST, _("Station Sets"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer11->Add(itemStaticText12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxArrayString listSetStrings;
    listSet = new wxListBox( itemPanel10, SET_LIST, wxDefaultPosition, wxSize(200, -1), listSetStrings, wxLB_SINGLE|wxNO_BORDER );
    itemBoxSizer11->Add(listSet, 1, wxGROW|wxALL, 5);

    mRrun = new wxButton( itemPanel10, ID_RUN_QUERY, _("Update"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer11->Add(mRrun, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mBook_ = new wxNotebook( itemSplitterWindow9, ID_SET_BOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    txtSetXML = new wxTextCtrl( mBook_, ID_SET_XML, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );

    mBook_->AddPage(txtSetXML, _("JSON"));

    wxPanel* itemPanel17 = new wxPanel( mBook_, ID_PANEL_NOTE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel17->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxVERTICAL);
    itemPanel17->SetSizer(itemBoxSizer18);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer18->Add(itemBoxSizer19, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText20 = new wxStaticText( itemPanel17, wxID_SET_NAME_LABEL, _("Set Name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemStaticText20, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    editSetName = new wxTextCtrl( itemPanel17, ID_EDIT_SET_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(editSetName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton22 = new wxButton( itemPanel17, ID_ADD_QUERY, _("+"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemButton22, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    mQueryList = new wxScrolledWindow( itemPanel17, ID_QUERY_LIST, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL );
    itemBoxSizer18->Add(mQueryList, 1, wxGROW, 0);
    mQueryList->SetScrollbars(1, 1, 0, 0);
    mVSizer = new wxBoxSizer(wxVERTICAL);
    mQueryList->SetSizer(mVSizer);

    mQueryList->SetMinSize(wxDefaultSize);

    mBook_->AddPage(itemPanel17, _("Query"));

    pageDerive = new wxPanel( mBook_, ID_DERIVE_TREND, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    pageDerive->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxVERTICAL);
    pageDerive->SetSizer(itemBoxSizer26);

    wxStaticBox* itemStaticBoxSizer27Static = new wxStaticBox(pageDerive, wxID_ANY, _("Measure"));
    wxStaticBoxSizer* itemStaticBoxSizer27 = new wxStaticBoxSizer(itemStaticBoxSizer27Static, wxHORIZONTAL);
    itemBoxSizer26->Add(itemStaticBoxSizer27, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText28 = new wxStaticText( itemStaticBoxSizer27->GetStaticBox(), wxID_STATIC, _("Measurement"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer27->Add(itemStaticText28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mMeasureStrings;
    mMeasureStrings.Add(_("TAVG"));
    mMeasureStrings.Add(_("TMIN"));
    mMeasureStrings.Add(_("TMAX"));
    mMeasureStrings.Add(_("MMIN"));
    mMeasureStrings.Add(_("MMAX"));
    mMeasure = new wxChoice( itemStaticBoxSizer27->GetStaticBox(), ID_MEASURE, wxDefaultPosition, wxDefaultSize, mMeasureStrings, 0 );
    mMeasure->SetStringSelection(_("TAVG"));
    itemStaticBoxSizer27->Add(mMeasure, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer30Static = new wxStaticBox(pageDerive, wxID_ANY, _("Anomaly baseline year range (essential)"));
    wxStaticBoxSizer* itemStaticBoxSizer30 = new wxStaticBoxSizer(itemStaticBoxSizer30Static, wxHORIZONTAL);
    itemBoxSizer26->Add(itemStaticBoxSizer30, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText31 = new wxStaticText( itemStaticBoxSizer30->GetStaticBox(), wxID_STATIC, _("Start year"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer30->Add(itemStaticText31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBaseStart = new wxTextCtrl( itemStaticBoxSizer30->GetStaticBox(), ID_BASE_START, _("1950"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer30->Add(mBaseStart, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText33 = new wxStaticText( itemStaticBoxSizer30->GetStaticBox(), wxID_STATIC, _("to End year"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer30->Add(itemStaticText33, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBaseEnd = new wxTextCtrl( itemStaticBoxSizer30->GetStaticBox(), ID_BASE_END, _("1980"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer30->Add(mBaseEnd, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer35Static = new wxStaticBox(pageDerive, wxID_ANY, _("Linear Regression"));
    wxStaticBoxSizer* itemStaticBoxSizer35 = new wxStaticBoxSizer(itemStaticBoxSizer35Static, wxHORIZONTAL);
    itemStaticBoxSizer35Static->SetForegroundColour(wxColour(0, 0, 0));
    itemBoxSizer26->Add(itemStaticBoxSizer35, 0, wxGROW|wxALL, 5);
    mBoxFit = new wxCheckBox( itemStaticBoxSizer35->GetStaticBox(), ID_BOX_LINEFIT, _("Fit years"), wxDefaultPosition, wxDefaultSize, 0 );
    mBoxFit->SetValue(false);
    itemStaticBoxSizer35->Add(mBoxFit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mFitFrom = new wxTextCtrl( itemStaticBoxSizer35->GetStaticBox(), ID_FIT_FROM, _("1970"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer35->Add(mFitFrom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxStaticText* itemStaticText38 = new wxStaticText( itemStaticBoxSizer35->GetStaticBox(), wxID_STATIC, _("to"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer35->Add(itemStaticText38, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    mFitTo = new wxTextCtrl( itemStaticBoxSizer35->GetStaticBox(), ID_FIT_TO, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer35->Add(mFitTo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxStaticBox* itemStaticBoxSizer40Static = new wxStaticBox(pageDerive, wxID_ANY, _("Filter result year range"));
    wxStaticBoxSizer* itemStaticBoxSizer40 = new wxStaticBoxSizer(itemStaticBoxSizer40Static, wxHORIZONTAL);
    itemBoxSizer26->Add(itemStaticBoxSizer40, 0, wxGROW|wxALL, 5);
    mBoxYears = new wxCheckBox( itemStaticBoxSizer40->GetStaticBox(), ID_BOX_YEAR_INCLUDE, _("Only include years"), wxDefaultPosition, wxDefaultSize, 0 );
    mBoxYears->SetValue(false);
    itemStaticBoxSizer40->Add(mBoxYears, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYearFrom = new wxTextCtrl( itemStaticBoxSizer40->GetStaticBox(), ID_FILTER_FROM, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer40->Add(mYearFrom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText43 = new wxStaticText( itemStaticBoxSizer40->GetStaticBox(), wxID_STATIC, _("to"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer40->Add(itemStaticText43, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYearTo = new wxTextCtrl( itemStaticBoxSizer40->GetStaticBox(), ID_FILTER_TO, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer40->Add(mYearTo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer26->Add(itemBoxSizer45, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton46 = new wxButton( pageDerive, ID_GO_DERIVE, _("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer45->Add(itemButton46, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBook_->AddPage(pageDerive, _("Trend"));

    wxPanel* itemPanel47 = new wxPanel( mBook_, ID_TAB_YEAR, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel47->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxVERTICAL);
    itemPanel47->SetSizer(itemBoxSizer48);

    wxStaticBox* itemStaticBoxSizer49Static = new wxStaticBox(itemPanel47, wxID_ANY, _("Measure"));
    wxStaticBoxSizer* itemStaticBoxSizer49 = new wxStaticBoxSizer(itemStaticBoxSizer49Static, wxHORIZONTAL);
    itemBoxSizer48->Add(itemStaticBoxSizer49, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText50 = new wxStaticText( itemStaticBoxSizer49->GetStaticBox(), wxID_STATIC, _("Measurement"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer49->Add(itemStaticText50, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mMeasure2Strings;
    mMeasure2Strings.Add(_("TAVG"));
    mMeasure2Strings.Add(_("TMIN"));
    mMeasure2Strings.Add(_("TMAX"));
    mMeasure2Strings.Add(_("MMIN"));
    mMeasure2Strings.Add(_("MMAX"));
    mMeasure2 = new wxChoice( itemStaticBoxSizer49->GetStaticBox(), ID_CHOICE_M2, wxDefaultPosition, wxDefaultSize, mMeasure2Strings, 0 );
    mMeasure2->SetStringSelection(_("TAVG"));
    itemStaticBoxSizer49->Add(mMeasure2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString radboxRealStrings;
    radboxRealStrings.Add(_("&Anomoly"));
    radboxRealStrings.Add(_("&Real"));
    radboxReal = new wxRadioBox( itemStaticBoxSizer49->GetStaticBox(), ID_SEL_BASE, _("Anomoly"), wxDefaultPosition, wxDefaultSize, radboxRealStrings, 1, wxRA_SPECIFY_ROWS );
    radboxReal->SetSelection(0);
    itemStaticBoxSizer49->Add(radboxReal, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    boxBaseDiff = new wxStaticBox(itemPanel47, wxID_BASE_DIFF, _("Anomaly baseline year range"));
    wxStaticBoxSizer* itemStaticBoxSizer53 = new wxStaticBoxSizer(boxBaseDiff, wxHORIZONTAL);
    itemBoxSizer48->Add(itemStaticBoxSizer53, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText54 = new wxStaticText( itemStaticBoxSizer53->GetStaticBox(), wxID_STATIC, _("Start year"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer53->Add(itemStaticText54, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBaseStart2 = new wxTextCtrl( itemStaticBoxSizer53->GetStaticBox(), ID_BASE_START2, _("1950"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer53->Add(mBaseStart2, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText56 = new wxStaticText( itemStaticBoxSizer53->GetStaticBox(), wxID_STATIC, _("to End year"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer53->Add(itemStaticText56, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBaseEnd2 = new wxTextCtrl( itemStaticBoxSizer53->GetStaticBox(), ID_BASE_END2, _("1980"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer53->Add(mBaseEnd2, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer58Static = new wxStaticBox(itemPanel47, wxID_ANY, _("Year ranges"));
    wxStaticBoxSizer* itemStaticBoxSizer58 = new wxStaticBoxSizer(itemStaticBoxSizer58Static, wxHORIZONTAL);
    itemBoxSizer48->Add(itemStaticBoxSizer58, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer58->Add(itemBoxSizer1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText2 = new wxStaticText( itemStaticBoxSizer58->GetStaticBox(), wxID_STATIC, _("Range YYYY-YYYY"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer1->Add(itemStaticText2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mYearToAdd = new wxTextCtrl( itemStaticBoxSizer58->GetStaticBox(), ID_YEAR_TO_ADD, _("2010-2019"), wxDefaultPosition, wxSize(150, -1), 0 );
    itemBoxSizer1->Add(mYearToAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton61 = new wxButton( itemStaticBoxSizer58->GetStaticBox(), ID_PLUS_YEAR, _("+"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemStaticBoxSizer58->Add(itemButton61, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mYearListStrings;
    mYearList = new wxListBox( itemStaticBoxSizer58->GetStaticBox(), ID_LIST_YEARS, wxDefaultPosition, wxSize(150, -1), mYearListStrings, wxLB_SINGLE );
    itemStaticBoxSizer58->Add(mYearList, 0, wxGROW|wxALL, 5);

    wxButton* itemButton63 = new wxButton( itemStaticBoxSizer58->GetStaticBox(), ID_MINUS_YEAR, _("-"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemStaticBoxSizer58->Add(itemButton63, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer64 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer48->Add(itemBoxSizer64, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton65 = new wxButton( itemPanel47, ID_PLOT_YEARS, _("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer64->Add(itemButton65, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBook_->AddPage(itemPanel47, _("Year"));

    wxPanel* itemPanel66 = new wxPanel( mBook_, ID_MEMBER_GRID, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel66->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer67 = new wxBoxSizer(wxVERTICAL);
    itemPanel66->SetSizer(itemBoxSizer67);

    wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer67->Add(itemBoxSizer68, 0, wxGROW|wxALL, 5);
    wxGridSizer* itemGridSizer1 = new wxGridSizer(2, 3, 0, 0);
    itemBoxSizer68->Add(itemGridSizer1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton2 = new wxButton( itemPanel66, ID_TIME_SERIES, _("Plot Time Series"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemGridSizer1->Add(itemButton2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mTAVG = new wxCheckBox( itemPanel66, ID_BOX_TAVG, _("TAVG"), wxDefaultPosition, wxDefaultSize, 0 );
    mTAVG->SetValue(false);
    itemGridSizer1->Add(mTAVG, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mMMAX = new wxCheckBox( itemPanel66, ID_BOX_MMAX, _("MMAX"), wxDefaultPosition, wxDefaultSize, 0 );
    mMMAX->SetValue(false);
    itemGridSizer1->Add(mMMAX, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton5 = new wxButton( itemPanel66, ID_STATION_DATA, _("Station Daily Data"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer1->Add(itemButton5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mTMAX = new wxCheckBox( itemPanel66, ID_BOX_TMAX, _("TMAX"), wxDefaultPosition, wxDefaultSize, 0 );
    mTMAX->SetValue(false);
    itemGridSizer1->Add(mTMAX, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mTMIN = new wxCheckBox( itemPanel66, ID_BOX_TMIN, _("TMIN"), wxDefaultPosition, wxDefaultSize, 0 );
    mTMIN->SetValue(false);
    itemGridSizer1->Add(mTMIN, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer67->Add(itemBoxSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mGrid_ = new wxGrid( itemPanel66, ID_SET_GRID, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    mGrid_->SetDefaultColSize(50);
    mGrid_->SetDefaultRowSize(25);
    mGrid_->SetColLabelSize(25);
    mGrid_->SetRowLabelSize(50);
    mGrid_->CreateGrid(3, 1, wxGrid::wxGridSelectCells);
    itemBoxSizer67->Add(mGrid_, 1, wxGROW|wxALL, 0);

    mBook_->AddPage(itemPanel66, _("Stations"));

    itemSplitterWindow9->SplitVertically(itemPanel10, mBook_, 150);

////@end StationSets content construction
}


/*
 * Should we show tooltips?
 */

bool StationSets::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap StationSets::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin StationSets bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end StationSets bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon StationSets::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin StationSets icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end StationSets icon retrieval
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEW_SET
 */


bool StationSets::SetExists(const std::string& name)
{
    std::string sql = "SELECT count(*) from stationset"
                " WHERE ss_name = ?";

    Statement qry(db_, sql);
    qry.bind(name,1);
    if (qry.next())
    {
        int records = 0;

        qry.get(0,records);

        if (records > 0)
        {
            wxLogMessage("Set name %s already exists", name.c_str());
            return true;
        }
    }
    return false;
}
void StationSets::OnQueryNewClick( wxCommandEvent& event )
{
    event.Skip();
    /*auto setdef = new StationSetDialog(this);

    if (setdef->ShowModal() == wxID_OK)
    {
        // see if set exists
        std::string value1, value2, setName;

        auto editPanel = setdef->mStationPanel;


        auto wstr = editPanel->txtSetName->GetValue();
        wstr.Trim();

        if (wstr.Length() > 0)
        {
            setName = wstr.ToStdString();
            if (SetExists(setName))
                return;
        }

        // generate SQL for query and add
        std::stringstream buildsql;

         buildsql <<  "SELECT * from gissloc WHERE ";

        int ct = 0;

        if (editPanel->mBoxNameLike->IsChecked())
        {
            wstr = editPanel->txtLike->GetValue();
            wstr.Trim();
            if (wstr.Length() > 0)
            {
                ct++;
                buildsql << " Name like '" << wstr.c_str() << "'";
            }
        }
        wxString json;

        editPanel->GetAsJSON(json);

        if (ct > 0)
        {
            // create the set record
            std::string sql = "insert into stationset (setName, queryJSON) values (?,?)";
            Statement iq (db_, sql);
            iq.bind( setName, 1);
            iq.bind( (const char*)json.c_str(), 2);
            iq.execute();
        }

    }
    */
}

void StationSets::ReadSetNames()
{
    std::string sql = "select ss_name from stationset order by ss_name";

    if (!db_.isOpen())
    {
        db_.path(ap_->getDBPath());
        db_.open();
    }
    Statement iq (db_, sql);

    wxArrayString items;

    listSet->Clear();
    while (iq.next())
    {
       wxString result =  iq.getText(0);
        items.Add(result);
    }
    if (items.size() > 0)
        listSet->Insert(items,0);
}

void StationSets::SelectSetName(const wxString& value, bool readMembers)
{
    std::string sql = "select queryJSON from stationset where ss_name = ?";

    Statement iq (db_, sql);

    iq.bind(value, 1);

    if (iq.next())
    {
        std::string jsonstr = iq.getText(0);

        Json::Reader jread;
        Json::Value  jDoc;
        jread.parse(jsonstr,jDoc);


        std::string setName = jDoc[JDoc::ID].asString();

        this->editSetName->SetValue(setName);

        this->selectedSet_ = setName;

        txtSetXML->SetValue(jsonstr);

        this->ClearPanels();
        if (jDoc.isMember(JDoc::QSEQ))
        {
            const Json::Value& qseq = jDoc[JDoc::QSEQ];
            if (qseq.isArray())
            {
                int qct = qseq.size();
                for(int i = 0; i < qct; i++)
                {
                    const Json::Value& q = qseq[i];
                    // append new station query to list
                    StationQuery* sq = new StationQuery(mQueryList);
                    sq->setsFrame = this;

                    wxBoxSizer* hold = new wxBoxSizer(wxHORIZONTAL);
                    hold->Add(sq,1, wxALIGN_CENTER_VERTICAL, 5);
                    mVSizer->Add(hold,0,wxGROW,0);
                    sq->Show();
                    sq->LoadFromJSON(q);
                }
            }
        }
        this->packQueryList();
    }
    if (readMembers)
        RefreshMembers();
}
/*
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for SET_LIST
 */

void StationSets::OnSetListSelected( wxCommandEvent& event )
{
    // Before editing this code, remove the block markers.
    event.Skip();
    wxWakeUpIdle();
    int ix = listSet->GetSelection();
    if (ix == wxNOT_FOUND)
    {
        return;
    }

    wxString value = listSet->GetString(ix);
    SelectSetName(value);


}


std::string space2underscore(std::string text) {
    for(std::string::iterator it = text.begin(); it != text.end(); ++it) {
        if(isspace(*it)) {
            *it = '_';
        }
    }
    return text;
}

bool StationSets::GetSetName(std::string& name)
{
    wxString setName = editSetName->GetValue();
    setName.Trim();

    // replace whitespace with _

    name = setName.ToStdString();

    name = space2underscore(name);
    return (name.size() > 0);
}
// show ts the current members for selected set
void StationSets::RefreshMembers()
{


    std::string sql = "select A.* from gissloc A, memberstation B, stationset S"
                        " where A.id = B.sid and B.ss_id = S.id and S.ss_name = ?"
                        " order by A.name"
                        ;

    Statement qy(db_, sql);

    qy.bind(selectedSet_, 1);

    LocTablePtr model = std::make_shared<LocTable>();

    while(qy.next())
    {
        Station4* loc = new Station4();
        loc->set(qy);
        model->addRecord(loc);
    }
    mGrid_->BeginBatch();
    mGrid_->SetTable(model.get()); // take ownership is false
    mGrid_->AutoSizeColumns(false);
    mGrid_->SetColLabelSize(50);
    mGrid_->EndBatch();
    mGrid_->ForceRefresh();

    this->locTable_ = model; // save shared pointer
    wxLogMessage("'%s' has %d stations", selectedSet_.c_str(), model->GetNumberRows());

}




void StationSets::DeleteSet(const wxString& setName)
{
    StationSet ss_rec;

    if (!ss_rec.loadByName(db_,setName.ToStdString()))
    {
         wxLogMessage("Station set not found: %s",setName.c_str());
         return;
    }
    try {
        ss_rec.deleteSelf(db_);
    }
    catch(DBException &ex)
    {
        wxLogMessage("Sqlite3 exception: %s",ex.msg());
    }

}

/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_QUERY_DELETE
 */

void StationSets::OnQueryDeleteClick( wxCommandEvent& event )
{
    event.Skip();
    int ix = listSet->GetSelection();
    if (ix == wxNOT_FOUND)
    {
        return;
    }
    wxString value = listSet->GetString(ix);
    value.Trim();
    if (value.size() > 0)
    {
        DeleteSet(value);
    }
    listSet->Delete(ix);
}

void StationSets::DropSetTable(const std::string& tableName)
{
    std::string sql = "DROP TABLE IF EXISTS " + tableName;
    try {
        db_.execute_or_throw(sql);
    }
    catch(DBException& ex)
    {
        wxLogMessage("Sqlite3 exception: %s",ex.msg());
    }
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUN_QUERY
 */

void StationSets::OnRunQueryClick( wxCommandEvent& event )
{
    event.Skip();
    wxWakeUpIdle();
    // Convert the Sequence specifications to JSON.
    // Save as JSON into the "SET" record
    // Read and parse the saved JSON record, create the SQL
    // Run the sequence to find the members of the "SET".

    std::string setName;

    if (!GetSetName(setName))
    {
        wxLogMessage("Empty name for SET");
        return;

    }
    Json::Value root;

    if (!UIToJSON(root))
    {
        wxLogMessage("Error in conversion to JSON");
        return;
    }

    std::stringstream     js;
    js << root;
    std::string json = js.str();
    wxLogMessage("JSON = %s", json.c_str());

    StationSet ss_rec;

    try {
        if (!ss_rec.loadByName(db_,setName)) {
            ss_rec.setName(setName);
            ss_rec.setJSON(json);
            ss_rec.create(db_);
        }
        else {
            ss_rec.setJSON(json);
            ss_rec.deleteMembers(db_);
            ss_rec.save(db_);
        }
        wxLogMessage("Saved Set %s as JSON", setName.c_str());
    }
    catch(DBException& ex)
    {
        wxLogMessage("Station Set save fail: %s",ex.msg());
    }

    ReadSetNames();
    SelectSetName(setName, false);

    std::string theSet = setName;
    //std::string set_table = "set_id_" + std::string.to_string(ss_rec.id_);

    std::string sql = MakeSQL(); // the SQL to create the station_ids

    //wxLogMessage("Running SQL : %s" , sql.c_str());

    //DropSetTable(set_table);
    //DeleteMembers(); // Backup members first?, or execute to a temporary table?
    int results = 0;
    try {
        Statement qct(db_,"select count(distinct(id)) " + sql);
        if (qct.next())
        {
            qct.get(0,results);
            wxLogMessage("There are %d distinct rows", results);
        }
    }
    catch(DBException& ex)
    {
        wxLogMessage("Sqlite3 exception: %s",ex.msg());
    }
    if (results > 0)
    {

        std::stringstream moveSQL;

        moveSQL << "insert into memberstation (ss_id, sid) ";
        moveSQL << "SELECT distinct " << ss_rec.getRowId() << ", id " << sql;
        sql = moveSQL.str();

        try {
            db_.execute_or_throw(sql);
        }
        catch(DBException& ex)
        {
            wxLogMessage("Sqlite3 exception: %s",ex.msg());
        }
        this->RefreshMembers();
    }
    else {
        wxLogMessage("No stations fit criteria %s", sql.c_str());
    }
}
/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_QUERY
 */

void StationSets::OnAddQueryClick( wxCommandEvent& event )
{

    event.Skip();
    StationQuery* sq = new StationQuery(mQueryList);
    sq->setsFrame = this;

    wxBoxSizer* hold = new wxBoxSizer(wxHORIZONTAL);
    hold->Add(sq,1, wxALIGN_CENTER_VERTICAL, 5);
    mVSizer->Add(hold,0,wxGROW,0);
    sq->Show();
    this->packQueryList();



}
// need to remove reference by sizer and remove wxStaticLine
void StationSets::Removing(wxWindow* q)
{

    StationQuery* sq = dynamic_cast<StationQuery*>(q);
    if (!sq)
        return;

    sq->Hide();


    wxSizer* hs = sq->GetSizer();
    if (hs)
    {
        mVSizer->Detach(hs);
    }
    mQueryList->RemoveChild(sq);

    //mQueryList->RemoveChild(q);

    sq->Destroy();
    packQueryList();
}

void StationSets::MoveUp(StationQuery* q)
{
    wxSizerItem* item = mVSizer->GetItem(q);
    wxSizerItemList& itemList = mVSizer->GetChildren();
    wxSizerItemList::const_iterator prev = itemList.end();

    for ( auto it = itemList.begin();it != itemList.end(); it++)
    {

        wxSizerItem* test = (*it);
        if (test == item)
        {
            if (prev != itemList.end())
            {
                wxSizerItem* ptest = (*prev);
                if (ptest->IsWindow())
                {
                    test->AssignWindow(ptest->GetWindow());
                    ptest->AssignWindow(q);
                }
            }
            break;
        }
        prev = it;
    }

    mVSizer->Layout();
    this->Fit();
}
void StationSets::MoveDown(StationQuery* q)
{
    wxSizerItem* item = mVSizer->GetItem(q);
    wxSizerItemList& itemList = mVSizer->GetChildren();

    for ( auto it = itemList.begin();it != itemList.end(); it++)
    {

        wxSizerItem* test = (*it);
        if (test == item)
        {
            it++;
            if (it != itemList.end())
            {
                wxSizerItem* ptest = (*it);
                if (ptest->IsWindow())
                {
                    test->AssignWindow(ptest->GetWindow());
                    ptest->AssignWindow(q);
                }
            }
            break;
        }
    }

    mQueryList->Layout();
    this->Fit();


}

static StationQuery* getHSizeWindow(wxSizer* hs)
{
    auto wlist = hs->GetChildren();
    auto it = wlist.begin();
    auto fin = wlist.end();

    for(; it != fin; it++)
    {
        wxSizerItem* item = *it;
        wxWindow* w = item->GetWindow();
        if (w != nullptr)
        {
            StationQuery* result = dynamic_cast<StationQuery*>( w );
            if (result)
                return result;
        }
    }
    return nullptr;
}


void StationSets::ClearPanels()
{
    auto wlist = mVSizer->GetChildren();
    wxWindowList close_list;

    for( wxSizerItemList::iterator it = wlist.begin(); it != wlist.end(); ++it)
    {
        wxSizerItem* item = (*it);

        wxSizer* hs = item->GetSizer();
        if (hs == nullptr)
            continue;


        StationQuery* sq = getHSizeWindow(hs);

        if (sq != nullptr)
        {
            close_list.Append(sq);
        }
    }
    wxLogMessage("Clear Panels");
    for(auto node = close_list.GetFirst(); node; node = node->GetNext())
    {
        auto current = (wxWindow*) node->GetData();
        this->Removing(current);
    }
}


std::string StationSets::MakeSQL()
{
    std::stringstream sql;
    std::stringstream where;
    std::string whereClause;

    /* return incomplete SQL, without extraction fields
    */
    sql << " from gissloc ";

    auto wlist = mVSizer->GetChildren();
    int cct = 0;

    for( wxSizerItemList::iterator it = wlist.begin(); it != wlist.end(); ++it)
    {
        wxSizerItem* item = (*it);
        wxSizer* hs = item->GetSizer();
        if (hs == nullptr)
            continue;

        StationQuery* sq = getHSizeWindow(hs);
        if (sq == nullptr)
            continue;

        Json::Value json(Json::objectValue);
        if (!sq->GetAsJSON(json))
            continue;

        sq->LoadFromJSON(json);

        std::string condition = sq->mSQL;

        bool opAdd = (sq->opName->GetSelection() == 0) ? true : false;

        condition = trim(condition);

        if (condition.size() > 0)
        {
            if (cct > 0)
                where << " OR ";
            cct++;
            if (!opAdd)
            {
                where << " AND NOT ";
            }
            where << "(" << condition << ")";
        }
    }
    whereClause = where.str();
    if (whereClause.size() > 0)
    {
        sql << "WHERE (" << whereClause << ")";
    }
    return sql.str();

}
bool StationSets::UIToJSON(Json::Value& root)
{
    std::string setName;

    if (!GetSetName(setName))
        return false;

    root[JDoc::ID] = setName;

    Json::Value qseq(Json::arrayValue);

    auto wlist = mVSizer->GetChildren();
    for( wxSizerItemList::iterator it = wlist.begin(); it != wlist.end(); ++it)
    {
        wxSizerItem* item = (*it);

        wxSizer* hs = item->GetSizer();
        if (hs == nullptr)
            continue;

        StationQuery* sq = getHSizeWindow(hs);

        if (sq != nullptr)
        {
            Json::Value json(Json::objectValue);
            sq->GetAsJSON(json);
            qseq.append(json);
        }
    }
    root[JDoc::QSEQ] = qseq;

    return true;
}

int GetLongValue(wxTextCtrl* ctl)
{
    wxString txt = ctl->GetValue();
    txt.Trim();

    if (txt.size() == 0)
    {
        return -1;
    }
    long test = -1;

    if ( txt.ToLong(&test ) )
    {
        return (int) test;
    }
    return -1;
}

int GetYearRange(const wxString& txt, long& start, long& end)
{
    wxString value;
    wxRegEx range;

    bool isgood = range.Compile( "(\\d+)\\s*\\-\\s*(\\d+)",wxRE_ADVANCED);

    if ( isgood && range.Matches(txt)) {
        auto ix = range.GetMatchCount();
        if (ix == 0) {
            return 0;
        }
        if (ix > 1) {
            value = range.GetMatch(txt, 1);
            if (value.ToLong(&start)) {
                    if (ix == 2) {
                        end = start;
                        return 1;
                    }
            }
        }
        if (ix > 2) {
            value = range.GetMatch(txt, 2);
            if (value.ToLong(&end)) {
                return 2;
            }
        }
    }
    return 0;
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_GO_DERIVE
 */

void StationSets::OnGoDeriveClick( wxCommandEvent& event )
{
    std::string setName;

    if (!GetSetName(setName))
        return;
    event.Skip(false);

    // for anomaly (done for each month at eachlocation)
    // need a base year range (inclusive), and a measurement type
    // all anomalies then averaged over a year for the location set

    StationSet sset;

    if (!sset.loadByName(db_,setName)) {
        wxLogMessage("Missing station set name %s", setName.c_str());
        return;
    }
    std::string sql =
    " SELECT AVG(T.value - A.base) as anomoly, Y.year as year"
    "   FROM gissyear Y, gisstemp T,"
        " ( SELECT B.sid, R.monthid, AVG(R.value) as base"
            " FROM gisstemp R, "
             "gissyear B join memberstation S on S.sid = B.sid and S.ss_id = ?"
            " WHERE B.year >= ? and B.year <= ? "
            " and R.dataid = B.dataid and B.measure = ?"
            " group by B.sid, R.monthid ) A"
    " WHERE Y.sid = A.sid and A.monthid = T.monthid"
    " and Y.measure = ?"
    " and Y.dataid = T.dataid";

    std::string groupBy =  " GROUP by Y.year ORDER BY year";

    long baseYearFrom = GetLongValue(this->mBaseStart);
    long baseYearTo = GetLongValue(this->mBaseEnd);

    if ((baseYearFrom <= 0) || (baseYearTo <= 0) || (baseYearTo - baseYearFrom < 1))
    {
        wxLogMessage("Invalid Base Year Range");
        return;
    }



    bool wantRegression = this->mBoxFit->GetValue();

    double yearFrom = GetLongValue(this->mFitFrom);
    double yearTo = GetLongValue(this->mFitTo);
    if (wantRegression && (yearFrom >= 0))
    {
        if (yearTo > 0)
        {
            if (yearTo < yearFrom)
            {
                wxLogMessage("Fit years range error");
                return;
            }
        }
    }

    bool includeRange = this->mBoxYears->GetValue();

    if (includeRange)
    {
        std::stringstream yearfilter;

        long includeFrom = GetLongValue(this->mYearFrom);
        long includeTo = GetLongValue(this->mYearTo);

        if (includeFrom > 0)
        {
            yearfilter << " and Y.YEAR >= " << includeFrom;
        }
        if (includeTo >= 0)
        {
            if (includeTo < includeFrom)
            {
                wxLogMessage("Include years range error");
                return;
            }
            yearfilter << " and Y.YEAR <= " << includeTo;
        }
        sql += yearfilter.str();
    }
    sql += groupBy;

    int choice = mMeasure->GetSelection();
    if (choice == wxNOT_FOUND)
        choice = 0;

    MTEMP measure = (MTEMP) choice;
    std::string label = GissYear::measureStr(measure);

    int32_t minYear, maxYear;
    SeriesPtr ydata;
    SeriesPtr xdata;

    double anomaly;
    int32_t year;
    //auto mainf = (MainFrame*)this->GetParent();
    int ct = 0;

    try {
        Statement qy(db_, sql);
        qy.bindRowId(sset.getRowId(), 1);
        qy.bind(baseYearFrom,2);
        qy.bind(baseYearTo,3);
        qy.bind((long)measure, 4);
        qy.bind((long) measure, 5);

        while (qy.next())
        {
            ct++;
            qy.get(0, anomaly);
            qy.get(1, year);

            if (ydata == nullptr)
            {
                minYear = year;
                maxYear = year;
            }
            else {
                if (year > maxYear)
                    maxYear = year;
                else if (year < minYear)
                    minYear = year;
            }

            if (ydata == nullptr)
            {
                wxLogMessage("New Plot %s", label.c_str());
                ydata = std::make_shared<FloatSeries>();

                ydata->setLabel(label);

                xdata = std::make_shared<FloatSeries>();
                xdata->setLabel("year");

            }
            ydata->append(anomaly);
            xdata->append(year);
        }
        wxLogMessage("Anomaly rows = %d", ct);
    }
    catch(DBException& ex)
    {
        wxLogMessage("Anomaly Plot: %s", ex.msg());
        return;
    }


    if (ct > 0)
    {
        auto plua = PlotLua::create_sptr();

        agw::PlotLayer_sptr sp = std::make_shared<DataLayer>();

        DataLayer* play = static_cast<DataLayer*>(sp.get());

        play->label_ = label;

        play->ydata_ = ydata;
        play->xdata_ = xdata;

        play->symbolShape_ = PlotShape::SQUARE;
        play->symbolSize_ = 2;
        play->symbolFill_ = wxColor("yellow");

        plua->addLayer(sp);
        if (wantRegression)
        {
            PlotLayer_sptr fit = std::make_shared<LineFit>();

            LineFit* line = static_cast<LineFit*>(fit.get());

            line->label_ = "Line Fit";
            line->lineStyle_ = penStyleToIndex(wxPENSTYLE_SOLID);
            line->lineWidth_ = 3;

            Series& srcX = *(xdata);
            Series& srcY = *(ydata);
            if (yearFrom >= 0)
            {
                // create temporary filtered series
                DoubleSeries lineX, lineY;
                for(auto srcIX = 0; srcIX < (int)srcX.size(); srcIX++)
                {
                    double xval = srcX[srcIX];
                    if (xval >=  yearFrom)
                    {
                        if (yearTo >= 0)
                        {
                            if (xval > yearTo)
                                continue;
                        }
                        lineX.append(xval);
                        lineY.append(srcY[srcIX]);
                    }
                }
                // need at least 3 points for meaningful regression
                if (lineX.size() > 2)
                {
                    line->regression(lineX, lineY);
                    plua->addLayer(fit);
                }
            }
            else {
                // regression over all the data!
                line->regression(srcX, srcY);
                plua->addLayer(fit);
            }

            line->legendText_ = line->resultAsText();

        }
        plua->addLegend();
        plua->ylabel("Deg. C");
        plua->xlabel("Year");
        plua->title("Year average anomaly");
        plua->showPlot(plua,true);
    }
    wxLogMessage("Plot Created");
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PLUS_YEAR
 */

void StationSets::OnPlusYearClick( wxCommandEvent& event )
{
    event.Skip(false);
    long year1 = 0;
    long year2 = 0;
    auto value = mYearToAdd->GetValue();
    value.Trim();
    int ct = GetYearRange(value, year1, year2);
    if (ct > 0)
    {
        wxString yearStr;
        yearStr << year1 << "-" << year2;
        if (mYearList->FindString(yearStr) == wxNOT_FOUND)
        {
            mYearList->AppendAndEnsureVisible(yearStr);
        }
    }
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MINUS_YEAR
 */

void StationSets::OnMinusYearClick( wxCommandEvent& event )
{
    event.Skip(false);
    int sel = mYearList->GetSelection();
    if (sel != wxNOT_FOUND)
    {
        mYearList->Delete(sel);
    }
}



/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
 */

void StationSets::OnPlotYearsClick( wxCommandEvent& event )
{

    event.Skip(false);

    std::string setName;

    if (!GetSetName(setName))
        return;

    StationSet sset;
    if (!sset.loadByName(db_,setName)) {
        return;
    }
    int choice = mMeasure2->GetSelection();

    if (choice == wxNOT_FOUND)
        choice = 0;

    MTEMP measure = (MTEMP) choice;
    auto useReal = radboxReal->GetSelection();

    std::vector<float> months = {1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.};
    std::vector<PlotLayer_sptr>  years;
    std::string sql;
    std::string dropcommand;

    SeriesPtr xdata = std::make_shared<FloatSeries>(months,SeriesUnit::DATE_MONTH_NUM, "Month");
    SeriesPtr ydata, yerror;

    if (useReal != AVG_REAL_TEMP) {


        long baseYearFrom = GetLongValue(this->mBaseStart2);
        long baseYearTo = GetLongValue(this->mBaseEnd2);

        if ((baseYearFrom <= 0) || (baseYearTo <= 0) || (baseYearTo - baseYearFrom < 1))
        {
            wxLogMessage("Invalid Base Year Range");
            return;
        }

        if ( mYearList->GetCount() <= 0)
            return;
        // make temporary anomaly base table name
        std::string base_name;
        {

            std::stringstream ss;
            ss << "base_" << measure << "_" << baseYearFrom << "_" << baseYearTo;
            base_name = ss.str();
        }
        // Drop existing anomaly table
        dropcommand = "drop table if exists " + base_name;
        db_.execute(dropcommand);

        // if going to use every station for the query, there is no point in joining
        // to gissloc or memberstation to exlude any station, and the join will be too big anyway.

        // detect ALL condition
        // create temporary anomaly base table
        std::string select_sql = "CREATE TABLE " + base_name +
            " AS SELECT B.sid, R.monthid, AVG(R.value) as base"
            " FROM gisstemp R, "
            " gissyear B join memberstation S on S.sid = B.sid and S.ss_id = ?"
            " WHERE B.year >= ? and B.year <= ? "
            " and R.dataid = B.dataid and B.measure = ?"
            " group by B.sid, R.monthid";
        {
            Statement q_base(db_, select_sql);
             q_base.bindRowId(sset.getRowId(),1);
            q_base.bind(baseYearFrom,2);
            q_base.bind(baseYearTo,3);
            q_base.bind((long)measure, 4);

            q_base.execute();
        }
        sql =
             "SELECT  T.value-A.base as value, T.monthid as month, Y.year"
            " FROM gissyear Y, gisstemp T,"
                + base_name + " A "
            " WHERE Y.sid = A.sid and A.monthid = T.monthid"
            " and Y.measure = ?"
            " and Y.dataid = T.dataid"
            " and Y.year >= ? and Y.year <= ?"
             "  ORDER BY  year, month";
    } //
    else {
        // use Real temperatures , not difference from baseline
        sql =
             "SELECT T.value, T.monthid as month, Y.year"
            " FROM gisstemp T,"
            " gissyear Y join memberstation S on S.sid = Y.sid and S.ss_id = ?"
            " WHERE Y.measure = ?"
            " and Y.dataid = T.dataid"
            " and Y.year >= ? and Y.year <= ?"
             " ORDER BY  year, month";
    }

    // For each year range, get the average difference, and standard deviation to base
    long fromYear, toYear;
    wxString range;

    for(int i = 0; i < (int)mYearList->GetCount(); i++)
    {

        range = mYearList->GetString(i);
        GetYearRange(range, fromYear, toYear);

        Statement q2(db_, sql);
        int bindIndex = 1;
        if (useReal == AVG_REAL_TEMP) {
            q2.bindRowId(sset.getRowId(),bindIndex);
            bindIndex++;
        }
        q2.bind((long)measure, bindIndex);
        bindIndex++;
        q2.bind(fromYear,bindIndex);
        bindIndex++;
        q2.bind(toYear,bindIndex);

        // Get a result for each month
        PlotLayer_sptr yearplot = std::make_shared<DataLayer>();
        DataLayer* play = static_cast<DataLayer*>(yearplot.get());

        ydata = std::make_shared<FloatSeries>();
        yerror = std::make_shared<FloatSeries>();

        std::string yearStr = range.ToStdString();
        ydata->setLabel(yearStr);
        ydata->setSize(12);
        yerror->setSize(12);
        yerror->setLabel(yearStr + " error");
        for (int i = 0; i < 12; i++)
            ydata->set(i, nanFloat);

        /* matrix of 12 columns * years */
        /* each column a vector */
        typedef  std::vector<double> YearRow;
        std::vector<YearRow> results ;
        unsigned long nyears = toYear - fromYear + 1;
        results.resize(nyears);
        for (unsigned long ix = 0 ; ix < nyears; ix++)
        {
            results[ix].resize(12,nanFloat);
        }

        while (q2.next())
        {
            double tvalue;
            int32_t tyear, tmonth;

            q2.get(0, tvalue);
            q2.get(1, tmonth);
            q2.get(2, tyear);

            results[tyear - fromYear][tmonth-1] = tvalue;
        }
        RunningStat rstat;
        for(unsigned long mid = 0; mid < 12; mid++) {

            rstat.Clear();
            for(unsigned long yid = 0; yid < nyears; yid++)
            {
                double X = results[yid][mid];
                if (!std::isnan(X))
                    rstat.Push(X);
            }
            ydata->set(mid, rstat.Mean());
            yerror->set(mid, rstat.StandardDeviation());
        }


        play->ydata_ = ydata;
        play->xdata_ = xdata;
        play->errorbar_ = yerror;
        play->xNudge_ = (i % 5 - 2);
        play->label_ = yearStr;
        play->legendText_  = yearStr;
        years.push_back(yearplot);

    } // loop of year range list
        // clean up
    if (useReal != AVG_REAL_TEMP)
        db_.execute(dropcommand);

    /*
    catch(DBException& ex)
    {
        wxLogMessage("Anomaly Plot: %s", ex.msg());
        return;
    }
    */
    if (years.size() > 0)
    {
        PlotLua_sptr pl = PlotLua::create_sptr();

        pl->world_.xScale_.units(SeriesUnit::DATE_MONTH_NUM);

        for( uint ct = 0; ct < years.size(); ct++)
        {
            PlotLayer_sptr pp = years[ct];
            pl->addLayer(pp);
        }
        pl->showPlot(pl, true);
    }


}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_TIME_SERIES
 */

void StationSets::OnTimeSeriesClick( wxCommandEvent& event )
{
    event.Skip();
    auto rows = mGrid_->GetSelectedRows();
    // select each row as a time series plot

    /*PlotXY* thePlot;
    PlotFrame* pf;
    */

    LocTable* base = dynamic_cast<LocTable*>(mGrid_->GetTable());
    // first of all , generate FloatSeries for all the data, for each TMIN,TMAX,TAVG selected

    std::vector<SeriesPtr>  timeData;
    std::vector<uint>       zeroMonth;

    // using same statement with different bindings for each query
    Statement qtemp(db_, "select B.year, A.monthid, A.value from gissyear B"
                            " join gisstemp A on A.dataid = B.dataid"
                            " where B.sid = ? and B.measure = ?"
                            " order by year, monthid");

    std::vector<std::uint32_t> measures;

    if (mTAVG->IsChecked())
        measures.push_back(TAVG);
    if (mTMIN->IsChecked())
        measures.push_back(TMIN);
    if (mTMAX->IsChecked())
        measures.push_back(TMAX);
    if (mMMAX->IsChecked())
        measures.push_back(MMAX);
    uint minMonth = UINT_MAX;
    uint maxMonth = 0;

    try {
        std::stringstream allss;

        for(uint i = 0; i < rows.GetCount(); i++)
        {
            Station4* loc = base->getRecord(rows[i]);
            auto mit = measures.begin();
            auto fin = measures.end();
            for( ; mit != fin; mit++)
            {
                auto theMeasure = (*mit);
                qtemp.bindRowId(loc->id_, 1);
                qtemp.bind((long)theMeasure,2);

                SeriesPtr data = std::make_shared<FloatSeries>();
                timeData.push_back(data);
                FloatSeries& fref = *(static_cast<FloatSeries*> (data.get()));
                std::stringstream ss;
                ss << GissYear::measureStr(MTEMP(theMeasure)) << " " << loc->name_;
                data->setLabel(ss.str());
                allss << ss.str() << " ";
                uint zeroMonthIndex = 0;
                uint lastGoodIndex = 0;
                uint currentIndex = 0;
                for(uint ct = 0; qtemp.next(); ct++)
                {
                    int yearNum = qtemp.getInt32(0);
                    int MonthNum  = qtemp.getInt32(1);
                    double value = qtemp.getDouble(2);
                    uint monthIndex = yearNum * 12 + MonthNum;
                    if(monthIndex > maxMonth)
                        maxMonth = monthIndex;
                    if(monthIndex < minMonth)
                        minMonth = monthIndex;
                    if (ct == 0)
                    {
                        // this will be the minimum monthIndex, and so the zero offset for this
                        zeroMonthIndex = monthIndex;
                        zeroMonth.push_back(zeroMonthIndex); // save it for later
                        fref.append(value);
                    }
                    else {


                        currentIndex = monthIndex - zeroMonthIndex;
                        if (currentIndex >= fref.size())
                            fref.setSize(currentIndex+1);

                        if (currentIndex > lastGoodIndex+1)
                        {
                            for(size_t ix = lastGoodIndex+1; ix < currentIndex; ix++)
                                fref.set(ix, nanDouble);
                        }
                        fref.set(currentIndex,value);
                        lastGoodIndex = currentIndex;
                    }

                }
                // finished query rows, reset for new bindings
                qtemp.reset();
            }
        } // end loop measures
        if (zeroMonth.size() == 0) {
            wxLogMessage("No data records found for any %s", allss.str());
            return;
        }
    } // end try
    catch(DBException &ex)
    {
        wxLogMessage("Error: %s", ex.msg());
        return;
    }
    // Now collected all the time Data, and the minMonth and maxMonth over the entire collection
    // Create the xdata SeriesPtr for them all, a DateYearMonth virtual series,
    // and convert each ydata SeriesPtr into a OffsetFloatSeries, to allow for different data time spans.

    std::vector<SeriesPtr> offsetTimeData;

    {
        auto it = timeData.begin();
        auto fin = timeData.end();

        for( uint ct = 0; it != fin; it++, ct++)
        {
            SeriesPtr p = *it;

            FloatSeries* orig = static_cast<FloatSeries*> (p.get());
            uint firstMonth = zeroMonth[ct];

            uint frontOffset = firstMonth - minMonth;
            uint lastMonth = firstMonth + orig->size() - 1;
            uint tail = maxMonth - lastMonth;
            SeriesPtr rep = std::make_shared<OffsetFloatSeries>(*orig, frontOffset, tail );
            rep->setLabel(orig->getLabel());
            offsetTimeData.push_back(rep);
        }
    }
    {
        // make all the plot xdata, ydata
        auto it = offsetTimeData.begin();
        auto fin = offsetTimeData.end();
        /*
        pf = new PlotFrame(ap_->mainFrame());
        thePlot = new PlotXY(pf);
        pf->setPlot(thePlot);
        */

        SeriesPtr xdata = std::make_shared<DateYearMonth>(minMonth/12, minMonth%12, maxMonth/12, maxMonth%12);

        PlotLua_sptr pl = PlotLua::create_sptr();


        pl->world_.xScale_.units(xdata->units());
        for( uint ct = 0; it != fin; it++, ct++)
        {
            Series* sp = it->get();

            PlotLayer_sptr pp = std::make_shared<DataLayer>();
            DataLayer* dlay = static_cast<DataLayer*>(pp.get());

            dlay->symbolShape_ = PlotShape::CIRCLE;
            dlay->symbolFill_ = wxColor("yellow");
            dlay->lineStyle_ = penStyleToIndex(wxPENSTYLE_SOLID);
            dlay->lineColor_ = wxColor("green");
            dlay->xdata_ = xdata;
            dlay->ydata_ = (*it);
            dlay->label_ = sp->getLabel();
            dlay->legendText_ = dlay->label_;

            pl->addLayer(pp);

        }
        pl->showPlot(pl, true);
    }


}

void StationSets::packQueryList()
{
    mQueryList->Layout();
    wxSize newSize = mQueryList->GetBestVirtualSize();
    mQueryList->SetVirtualSize(newSize);
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_SET_MAP
 */

void StationSets::OnSetMapClick( wxCommandEvent& event )
{
    event.Skip(false);

    MapFrame* mf = new MapFrame(ap_->mainFrame());
    MapWindow* w = mf->mMap;
    std::string title;
    this->GetSetName(title);

    mf->SetTitle(title);

    w->map_c = this->ap_->getCountryMap();
    w->ap_ = this->ap_;
    w->stations_ = this->locTable_;

    mf->Show();
}


/*
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_SEL_BASE
 */

void StationSets::OnSelBaseSelected( wxCommandEvent& event )
{
    //auto value = radboxReal->GetSelection();
    event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STATION_DATA
 */

void StationSets::OnStationDataClick( wxCommandEvent& event )
{

    event.Skip();

    auto rows = mGrid_->GetSelectedRows();
    if (rows.GetCount() > 0) {
        LocTable* base = dynamic_cast<LocTable*>(mGrid_->GetTable());
        Station4* loc = base->getRecord(rows[0]);

        wxString fullpath = StationDailyData::FullPath(loc->stationid);

        if (!fullpath.IsEmpty()) {
            if (!wxFileExists(fullpath)) {
                wxLogMessage(wxString::Format("File not found %s", fullpath));
                return;
            }
            auto mf = ap_->mainFrame();
            auto setview = new StationDailyData(mf);
            setview->ap_ = ap_;

            setview->SetStationId(loc->stationid, fullpath);
            setview->Show();
        }


    }

}

