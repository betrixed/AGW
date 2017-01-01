/////////////////////////////////////////////////////////////////////////////
// Name:        axisdlg.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Mon 18 Apr 2016 17:44:42 AEST
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

#include "axisdlg.h"

#include "plotframe.h"
#include "plotlua.h"
#include "helper.h"

////@begin XPM images
////@end XPM images


using namespace agw;

/*
 * AxisDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AxisDlg, wxDialog )


/*
 * AxisDlg event table definition
 */

BEGIN_EVENT_TABLE( AxisDlg, wxDialog )

////@begin AxisDlg event table entries
    EVT_LISTBOX( ID_AXIS_ID, AxisDlg::OnAxisIdSelected )
    EVT_BUTTON( wxID_REVERT, AxisDlg::OnRevertClick )
    EVT_BUTTON( wxID_APPLY, AxisDlg::OnApplyClick )
////@end AxisDlg event table entries

END_EVENT_TABLE()


/*
 * AxisDlg constructors
 */

AxisDlg::AxisDlg()
{
    Init();
}

AxisDlg::AxisDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * AxisDlg creator
 */

bool AxisDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AxisDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end AxisDlg creation
    return true;
}


/*
 * AxisDlg destructor
 */

AxisDlg::~AxisDlg()
{
////@begin AxisDlg destruction
////@end AxisDlg destruction
}


/*
 * Member initialisation
 */

void AxisDlg::Init()
{
////@begin AxisDlg member initialisation
    mAxisId = NULL;
    mBtnRestore = NULL;
    mBtnApply = NULL;
    mBtnCancel = NULL;
    mButtonOK = NULL;
    mUnitsBook = NULL;
    mPageRender = NULL;
    mColorBack = NULL;
    mColorAxis = NULL;
    mColorRefLine = NULL;
    mColorGrid = NULL;
    mXReverse = NULL;
    mXAutolimits = NULL;
    mXAutoAxis = NULL;
    mXTextRotate = NULL;
    mXInside = NULL;
    mXLabelInside = NULL;
    mXBoxMajor = NULL;
    mXBoxSmall = NULL;
    mXBoxRef = NULL;
    mPageJSON = NULL;
    mJSON = NULL;
    mPageNumber = NULL;
    mXDivMajor = NULL;
    mXDivSmall = NULL;
    mXRefLine = NULL;
    mXMin = NULL;
    mXMax = NULL;
    mXListUnits = NULL;
    mPageDate = NULL;
    mDateFrom = NULL;
    mDateTo = NULL;
////@end AxisDlg member initialisation

}


/*
 * Control creation for AxisDlg
 */

void AxisDlg::CreateControls()
{
////@begin AxisDlg content construction
    AxisDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxArrayString mAxisIdStrings;
    mAxisIdStrings.Add(_("Left Y Axis"));
    mAxisIdStrings.Add(_("Bottom X Axis"));
    mAxisId = new wxListBox( itemDialog1, ID_AXIS_ID, wxDefaultPosition, wxDefaultSize, mAxisIdStrings, wxLB_SINGLE );
    itemBoxSizer3->Add(mAxisId, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBtnRestore = new wxButton( itemDialog1, wxID_REVERT, _("Restore"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(mBtnRestore, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mBtnApply = new wxButton( itemDialog1, wxID_APPLY, _("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(mBtnApply, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBtnCancel = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer11->Add(mBtnCancel, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemBoxSizer11->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mButtonOK = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer11->Add(mButtonOK, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mUnitsBook = new wxNotebook( itemDialog1, ID_UNITS_NOTE, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    mPageRender = new wxPanel( mUnitsBook, ID_PAGE_RENDER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    mPageRender->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
    mPageRender->SetSizer(itemBoxSizer17);

    wxFlexGridSizer* itemFlexGridSizer18 = new wxFlexGridSizer(4, 2, 0, 0);
    itemBoxSizer17->Add(itemFlexGridSizer18, 1, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText19 = new wxStaticText( mPageRender, wxID_STATIC, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer18->Add(itemStaticText19, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorBack = new wxColourPickerCtrl( mPageRender, ID_COLOR_BACK, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer18->Add(mColorBack, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText21 = new wxStaticText( mPageRender, wxID_STATIC, _("Axis"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer18->Add(itemStaticText21, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorAxis = new wxColourPickerCtrl( mPageRender, ID_COLOR_AXIS, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer18->Add(mColorAxis, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText23 = new wxStaticText( mPageRender, wxID_STATIC, _("Reference"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer18->Add(itemStaticText23, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorRefLine = new wxColourPickerCtrl( mPageRender, ID_COLOR_REFLINE, wxColour(217, 125, 125), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer18->Add(mColorRefLine, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText25 = new wxStaticText( mPageRender, wxID_STATIC, _("Grid"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer18->Add(itemStaticText25, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorGrid = new wxColourPickerCtrl( mPageRender, ID_COLOUR_GRID, wxColour(248, 255, 194), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer18->Add(mColorGrid, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer18->AddGrowableCol(1);

    wxStaticBox* itemStaticBoxSizer27Static = new wxStaticBox(mPageRender, wxID_ANY, _("Ticks"));
    wxStaticBoxSizer* itemStaticBoxSizer27 = new wxStaticBoxSizer(itemStaticBoxSizer27Static, wxVERTICAL);
    itemBoxSizer17->Add(itemStaticBoxSizer27, 1, wxGROW|wxALL, 10);
    mXReverse = new wxCheckBox( itemStaticBoxSizer27->GetStaticBox(), ID_XBOX_REVERSE, _("Reverse direction"), wxDefaultPosition, wxDefaultSize, 0 );
    mXReverse->SetValue(false);
    itemStaticBoxSizer27->Add(mXReverse, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    mXAutolimits = new wxCheckBox( itemStaticBoxSizer27->GetStaticBox(), ID_XBOX_AUTOLIMITS, _("Automatic Ticks"), wxDefaultPosition, wxDefaultSize, 0 );
    mXAutolimits->SetValue(false);
    itemStaticBoxSizer27->Add(mXAutolimits, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    mXAutoAxis = new wxCheckBox( itemStaticBoxSizer27->GetStaticBox(), ID_XBOX_AUTOAXIS, _("Automatic Limits"), wxDefaultPosition, wxDefaultSize, 0 );
    mXAutoAxis->SetValue(false);
    itemStaticBoxSizer27->Add(mXAutoAxis, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    mXTextRotate = new wxCheckBox( itemStaticBoxSizer27->GetStaticBox(), ID_XBOX_ROTATE, _("Text rotate"), wxDefaultPosition, wxDefaultSize, 0 );
    mXTextRotate->SetValue(false);
    itemStaticBoxSizer27->Add(mXTextRotate, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    mXInside = new wxCheckBox( itemStaticBoxSizer27->GetStaticBox(), ID_XBOX_INSIDE, _("Tick inside"), wxDefaultPosition, wxDefaultSize, 0 );
    mXInside->SetValue(false);
    itemStaticBoxSizer27->Add(mXInside, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    mXLabelInside = new wxCheckBox( itemStaticBoxSizer27->GetStaticBox(), ID_LABEL_INSIDE, _("Label Inside"), wxDefaultPosition, wxDefaultSize, 0 );
    mXLabelInside->SetValue(false);
    itemStaticBoxSizer27->Add(mXLabelInside, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    mXBoxMajor = new wxCheckBox( itemStaticBoxSizer27->GetStaticBox(), ID_XBOX_MAJOR, _("Show Major Grid"), wxDefaultPosition, wxDefaultSize, 0 );
    mXBoxMajor->SetValue(false);
    itemStaticBoxSizer27->Add(mXBoxMajor, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    mXBoxSmall = new wxCheckBox( itemStaticBoxSizer27->GetStaticBox(), ID_XBOX_MINOR, _("Show Fine Grid"), wxDefaultPosition, wxDefaultSize, 0 );
    mXBoxSmall->SetValue(false);
    itemStaticBoxSizer27->Add(mXBoxSmall, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    mXBoxRef = new wxCheckBox( itemStaticBoxSizer27->GetStaticBox(), ID_XBOX_REFLINE, _("Show Reference Line"), wxDefaultPosition, wxDefaultSize, 0 );
    mXBoxRef->SetValue(false);
    itemStaticBoxSizer27->Add(mXBoxRef, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    mUnitsBook->AddPage(mPageRender, _("Render"));

    mPageJSON = new wxPanel( mUnitsBook, ID_PAGE_JSON, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    mPageJSON->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
    mPageJSON->SetSizer(itemBoxSizer38);

    mJSON = new wxTextCtrl( mPageJSON, ID_JSON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemBoxSizer38->Add(mJSON, 1, wxGROW|wxALL, 5);

    mUnitsBook->AddPage(mPageJSON, _("JSON"));

    mPageNumber = new wxPanel( mUnitsBook, ID_PAGE_NUMBER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    mPageNumber->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
    mPageNumber->SetSizer(itemBoxSizer41);

    itemBoxSizer41->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridBagSizer* itemGridBagSizer43 = new wxGridBagSizer(0, 0);
    itemGridBagSizer43->SetEmptyCellSize(wxSize(10, 20));
    itemBoxSizer41->Add(itemGridBagSizer43, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText44 = new wxStaticText( mPageNumber, wxID_STATIC, _("Major"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer43->Add(itemStaticText44, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXDivMajor = new wxTextCtrl( mPageNumber, ID_XDIV_MAJOR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer43->Add(mXDivMajor, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxStaticText* itemStaticText46 = new wxStaticText( mPageNumber, wxID_STATIC, _("minor"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer43->Add(itemStaticText46, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXDivSmall = new wxSpinCtrl( mPageNumber, ID_XSPIN_MINOR, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 1 );
    itemGridBagSizer43->Add(mXDivSmall, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxStaticText* itemStaticText48 = new wxStaticText( mPageNumber, wxID_STATIC, _("Reference"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer43->Add(itemStaticText48, wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXRefLine = new wxTextCtrl( mPageNumber, ID_XEDIT_REFLINE, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer43->Add(mXRefLine, wxGBPosition(2, 1), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxStaticText* itemStaticText50 = new wxStaticText( mPageNumber, wxID_STATIC, _("Minimum"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer43->Add(itemStaticText50, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXMin = new wxTextCtrl( mPageNumber, ID_XMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer43->Add(mXMin, wxGBPosition(3, 1), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    wxStaticText* itemStaticText52 = new wxStaticText( mPageNumber, wxID_STATIC, _("Maximum"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer43->Add(itemStaticText52, wxGBPosition(4, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXMax = new wxTextCtrl( mPageNumber, ID_XMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer43->Add(mXMax, wxGBPosition(4, 1), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    itemGridBagSizer43->AddGrowableCol(1);

    itemBoxSizer41->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer55Static = new wxStaticBox(mPageNumber, wxID_ANY, _("Units"));
    wxStaticBoxSizer* itemStaticBoxSizer55 = new wxStaticBoxSizer(itemStaticBoxSizer55Static, wxVERTICAL);
    itemBoxSizer41->Add(itemStaticBoxSizer55, 0, wxGROW|wxALL, 5);
    wxArrayString mXListUnitsStrings;
    mXListUnitsStrings.Add(_("Number"));
    mXListUnitsStrings.Add(_("Julian Date"));
    mXListUnitsStrings.Add(_("Modified Julian"));
    mXListUnitsStrings.Add(_("Month"));
    mXListUnitsStrings.Add(_("Year-Month"));
    mXListUnits = new wxListBox( itemStaticBoxSizer55->GetStaticBox(), ID_XLIST_UNITS, wxDefaultPosition, wxSize(-1, 120), mXListUnitsStrings, wxLB_SINGLE );
    mXListUnits->SetStringSelection(_("Number"));
    itemStaticBoxSizer55->Add(mXListUnits, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemBoxSizer41->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mUnitsBook->AddPage(mPageNumber, _("Number"));

    mPageDate = new wxPanel( mUnitsBook, ID_PAGE_DATE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    mPageDate->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxVERTICAL);
    mPageDate->SetSizer(itemBoxSizer59);

    wxBoxSizer* itemBoxSizer60 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer59->Add(itemBoxSizer60, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText61 = new wxStaticText( mPageDate, wxID_STATIC, _("From Date"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer60->Add(itemStaticText61, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mDateFrom = new wxDatePickerCtrl( mPageDate, ID_DATE_FROM, wxDateTime(), wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
    itemBoxSizer60->Add(mDateFrom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer63 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer59->Add(itemBoxSizer63, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText64 = new wxStaticText( mPageDate, wxID_STATIC, _("To Date"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer63->Add(itemStaticText64, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mDateTo = new wxDatePickerCtrl( mPageDate, ID_DATE_TO, wxDateTime(), wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
    itemBoxSizer63->Add(mDateTo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mUnitsBook->AddPage(mPageDate, _("Date"));

    itemBoxSizer2->Add(mUnitsBook, 1, wxGROW|wxALL, 5);

////@end AxisDlg content construction
}


/*
 * Should we show tooltips?
 */

bool AxisDlg::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap AxisDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AxisDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end AxisDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon AxisDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AxisDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AxisDlg icon retrieval
}

void AxisDlg::fromAxis(const agw::LinearScale& xaxis)
{
    wxString tempStr;
    std::string text;

    tempStr << xaxis.axisTicks_;
    mXDivMajor->SetValue(tempStr);

    mXBoxMajor->SetValue(xaxis.showMajorGrid_);
    mXDivSmall->SetValue(xaxis.axisDiv_);
    mXBoxSmall->SetValue(xaxis.showMinorTicks_);

    tempStr.Clear();
    tempStr << xaxis.refValue_;
    mXRefLine->SetValue(tempStr);
    mXBoxRef->SetValue(xaxis.refLine_);
    mXAutoAxis->SetValue(xaxis.autoTicks_);
    mXListUnits->SetSelection((int) xaxis.units_);
    switch(xaxis.units_)
    {
        case SeriesUnit::DATE_YEAR_MONTH:
            {
                // convert to format yyyy-mm, 1 - based month
                int year;
                wxDateTime::Month month;
                mjdn_to_year_month(xaxis.dataMax_, year, month);
                text = std::to_string(year) + "-" + std::to_string(int(month)+1);
                mXMax->SetValue(text);
                mjdn_to_year_month(xaxis.dataMin_, year, month);
                text = std::to_string(year) + "-" + std::to_string(int(month)+1);
                mXMin->SetValue(text);
            }
            break;
        case SeriesUnit::DATE_JULIAN_MOD:
            {
            wxDateTime tempDate(xaxis.dataMin_ + 2400000.5);
            mDateFrom->SetValue(tempDate);
            tempDate.Set(xaxis.dataMax_ + 2400000.5);
            mDateTo->SetValue(tempDate);
            }
            break;
        case SeriesUnit::DEFAULT:
        default:
            tempStr.Clear();
            tempStr << xaxis.dataMax_;
            mXMax->SetValue(tempStr);

            tempStr.Clear();
            tempStr << xaxis.dataMin_;
            mXMin->SetValue(tempStr);

            break;
    }

    mXReverse->SetValue(xaxis.direct_ == ScaleFlow::NEGATIVE);

    mXAutolimits->SetValue(xaxis.autoLimits_);
    mXTextRotate->SetValue(xaxis.tickRotate_ == TextRotate::CW_90);
    mXInside->SetValue(xaxis.insideTick_);
    mXLabelInside->SetValue(xaxis.insideLabel_);

    this->selectedAxis = (xaxis.coord_ == ScaleCoord::XCOORD) ? 1 : 0;
    this->allowAxisSelect = false;
    mAxisId->SetSelection(this->selectedAxis);
    this->allowAxisSelect = true;

}

void AxisDlg::setFromJson(const Json::Value& json)
{

}

void AxisDlg::setPlotFrame(PlotFrame* pf)
{
    this->plotFrame = pf;

    agw::PlotLuaPtr  plp = pf->thePlot->plotLua_;

    origXAxis = plp->world_.xScale_;
    origYAxis = plp->world_.yScale_;

    tempXAxis = origXAxis;
    tempYAxis = origYAxis;

    Json::Value  jcont(Json::objectValue);
    Json::Value  xaxis(Json::objectValue);
    Json::Value  yaxis(Json::objectValue);

    origXAxis.SaveJSON(xaxis);
    origYAxis.SaveJSON(yaxis);

    jcont["xaxis"] = xaxis;
    jcont["yaxis"] = yaxis;

    std::stringstream   jout;

    jout << jcont << std::endl;

    mJSON->SetValue(jout.str());


    fromAxis(origXAxis);
    this->Show();



}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_REVERT
 */

void AxisDlg::OnRevertClick( wxCommandEvent& event )
{
    event.Skip(true);
    agw::PlotLuaPtr  plp = plotFrame->thePlot->plotLua_;

     plp->world_.xScale_ = origXAxis;
     plp->world_.yScale_ = origYAxis;

     plotFrame->thePlot->Refresh();


}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
 */

void AxisDlg::OnApplyClick( wxCommandEvent& event )
{

    event.Skip(true);

    agw::PlotLuaPtr  plp = plotFrame->thePlot->plotLua_;

    if (mAxisId->GetSelection() == 1)
        toAxis(plp->world_.xScale_);
    else
        toAxis(plp->world_.yScale_);

    plotFrame->thePlot->Refresh();
}

void AxisDlg::toAxis(LinearScale& xaxis)
{
    wxString tempStr;

    tempStr = mXDivMajor->GetValue();
    tempStr.ToDouble(&xaxis.axisTicks_);

    xaxis.showMajorGrid_ = mXBoxMajor->IsChecked();
    xaxis.axisDiv_ = mXDivSmall->GetValue();
    xaxis.showMinorTicks_ = mXBoxSmall->IsChecked();

    tempStr = mXRefLine->GetValue();
    tempStr.ToDouble(&xaxis.refValue_);
    xaxis.refLine_ = mXBoxRef->IsChecked();
    xaxis.autoTicks_ = mXAutoAxis->IsChecked();
    xaxis.autoLimits_ = mXAutolimits->IsChecked();

    xaxis.units_ = (SeriesUnit) mXListUnits->GetSelection();


    switch(xaxis.units_)
    {
        case SeriesUnit::DATE_YEAR_MONTH:
            {
                int year;
                wxDateTime::Month month;
                std::string xmin_text = mXMin->GetValue().ToStdString();
                std::string xmax_text = mXMax->GetValue().ToStdString();

                // convert from format yyyy-mm, 1 - based month
                to_year_month(xmin_text, year, month);
                xaxis.dataMin_  = to_mjdn(year, month);
                to_year_month(xmax_text, year, month);
                xaxis.dataMax_ = to_mjdn(year, month);
            }
            break;
        case SeriesUnit::DATE_JULIAN_MOD:
            {
            wxDateTime tempDate = mDateFrom->GetValue();
            xaxis.dataMin_ = tempDate.GetModifiedJulianDayNumber();
            tempDate = mDateTo->GetValue();
            xaxis.dataMax_ = tempDate.GetModifiedJulianDayNumber();
            }
            break;
        case SeriesUnit::DEFAULT:
        default:
            tempStr = mXMax->GetValue();
            tempStr.ToDouble(&xaxis.dataMax_);

            tempStr = mXMin->GetValue();
            tempStr.ToDouble(&xaxis.dataMin_);
            break;
    }



    xaxis.direct_ = mXReverse->IsChecked() ? ScaleFlow::NEGATIVE : ScaleFlow::POSITIVE;

    xaxis.tickRotate_ = (mXTextRotate->IsChecked())?TextRotate::CW_90 : TextRotate::R_0;
    xaxis.insideTick_ = mXInside->IsChecked();
    xaxis.insideLabel_ = mXLabelInside->IsChecked();
}


/*
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_AXIS_ID
 */

void AxisDlg::OnAxisIdSelected( wxCommandEvent& event )
{
    event.Skip(true);
    if (!this->allowAxisSelect)
    {
        return;
    }
    int newSelection = mAxisId->GetSelection();

    if (newSelection != this->selectedAxis)
    {
        // TODO: Apply or park current selection
        if (this->selectedAxis == 1)
        {
            toAxis(tempXAxis);
        }
        else {
            toAxis(tempYAxis);
        }
        if (newSelection == 1)
        {
            fromAxis(tempXAxis);
        }
        else {
            fromAxis(tempYAxis);
        }
    }
}

