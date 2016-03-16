/////////////////////////////////////////////////////////////////////////////
// Name:        plotproperty.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Sat 23 Jan 2016 22:21:53 AEDT
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
#include "wx/bookctrl.h"
////@end includes

#include "plotproperty.h"
#include "helper.h"
#include "json/json.h"
#include "plotxy.h"
////@begin XPM images
////@end XPM images

#include <sstream>

using namespace agw;
/*
 * PlotProperty type definition
 */

IMPLEMENT_DYNAMIC_CLASS( PlotProperty, wxPropertySheetDialog )


/*
 * PlotProperty event table definition
 */

BEGIN_EVENT_TABLE( PlotProperty, wxPropertySheetDialog )

////@begin PlotProperty event table entries
    EVT_CHECKBOX( ID_XBOX_AUTOLIMITS, PlotProperty::OnXboxAutolimitsClick )
    EVT_CHECKBOX( ID_YBOX_AUTOLIMITS, PlotProperty::OnYboxAutolimitsClick )
    EVT_LISTBOX( ID_LABEL_LIST, PlotProperty::OnLabelListSelected )
    EVT_BUTTON( ID_DELETE_LABEL, PlotProperty::OnDeleteLabelClick )
    EVT_BUTTON( ID_ADD_LABEL, PlotProperty::OnAddLabelClick )
    EVT_LISTBOX( ID_ATTACH_LIST, PlotProperty::OnAttachListSelected )
////@end PlotProperty event table entries

END_EVENT_TABLE()


/*
 * PlotProperty constructors
 */

PlotProperty::PlotProperty()
{
    Init();
}

PlotProperty::PlotProperty( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * PlotProperty creator
 */

bool PlotProperty::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin PlotProperty creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    SetSheetStyle(wxPROPSHEET_DEFAULT);
    wxPropertySheetDialog::Create( parent, id, caption, pos, size, style );

    CreateButtons(wxOK|wxCANCEL|wxHELP);
    CreateControls();
    LayoutDialog();
    Centre();
////@end PlotProperty creation
    return true;
}


/*
 * PlotProperty destructor
 */

PlotProperty::~PlotProperty()
{
////@begin PlotProperty destruction
////@end PlotProperty destruction
}


/*
 * Member initialisation
 */

void PlotProperty::Init()
{
////@begin PlotProperty member initialisation
    mLFrame = NULL;
    mTFrame = NULL;
    mRFrame = NULL;
    mBFrame = NULL;
    mBoxFixed = NULL;
    mXSize = NULL;
    mYSize = NULL;
    mXMin = NULL;
    mXMax = NULL;
    mXReverse = NULL;
    mXAutolimits = NULL;
    mXDivMajor = NULL;
    mXBoxMajor = NULL;
    mXDivSmall = NULL;
    mXBoxSmall = NULL;
    mXRefLine = NULL;
    mXBoxRef = NULL;
    mXAutoAxis = NULL;
    mXTextRotate = NULL;
    mXInside = NULL;
    mXLabelInside = NULL;
    mXListUnits = NULL;
    mYMin = NULL;
    mYMax = NULL;
    mYReverse = NULL;
    mYAutolimits = NULL;
    mYDivMajor = NULL;
    mYBoxMajor = NULL;
    mYDivSmall = NULL;
    mYBoxSmall = NULL;
    mYRefLine = NULL;
    mYBoxRef = NULL;
    mYAutoAxis = NULL;
    mYTextRotate = NULL;
    mYInside = NULL;
    mYLabelInside = NULL;
    mYListUnits = NULL;
    mColorBack = NULL;
    mColorAxis = NULL;
    mColorRefLine = NULL;
    mColorGrid = NULL;
    mLabelList = NULL;
    mDeleteLabel = NULL;
    mAddLabel = NULL;
    mRotateList = NULL;
    mAttachToList = NULL;
    mXLabel = NULL;
    mYLabel = NULL;
    mFont = NULL;
    mLabelText = NULL;
////@end PlotProperty member initialisation

    labelStore = Json::Value(Json::arrayValue);
    //wxLogMessage("0 Json labelStore ct = %d ", (int) labelStore.size());
    currentTextSelect = -1;
    blockUpdate = false;
}


/*
 * Control creation for PlotProperty
 */

void PlotProperty::CreateControls()
{
////@begin PlotProperty content construction
    PlotProperty* itemPropertySheetDialog1 = this;

    wxPanel* itemPanel2 = new wxPanel( GetBookCtrl(), ID_PP_MARGIN, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel2->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemPanel2->SetSizer(itemBoxSizer3);

    wxGridBagSizer* itemGridBagSizer4 = new wxGridBagSizer(0, 0);
    itemGridBagSizer4->SetEmptyCellSize(wxSize(10, 20));
    itemBoxSizer3->Add(itemGridBagSizer4, 1, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemGridBagSizer4->Add(itemBoxSizer5, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText6 = new wxStaticText( itemPanel2, wxID_STATIC, _("Left"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl7 = new wxTextCtrl( itemPanel2, ID_EDIT_LMARGIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemTextCtrl7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mLFrame = new wxCheckBox( itemPanel2, ID_BOX_LFRAME, _("Frame"), wxDefaultPosition, wxDefaultSize, 0 );
    mLFrame->SetValue(false);
    itemBoxSizer5->Add(mLFrame, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemGridBagSizer4->Add(itemBoxSizer9, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel2, wxID_STATIC, _("Top"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl11 = new wxTextCtrl( itemPanel2, ID_EDIT_TMARGIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemTextCtrl11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mTFrame = new wxCheckBox( itemPanel2, ID_BOX_TFRAME, _("Frame"), wxDefaultPosition, wxDefaultSize, 0 );
    mTFrame->SetValue(false);
    itemBoxSizer9->Add(mTFrame, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemGridBagSizer4->Add(itemBoxSizer13, wxGBPosition(1, 2), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel2, wxID_STATIC, _("Right"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemStaticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl15 = new wxTextCtrl( itemPanel2, ID_EDIT_RMARGIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemTextCtrl15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mRFrame = new wxCheckBox( itemPanel2, ID_BOX_RFRAME, _("Frame"), wxDefaultPosition, wxDefaultSize, 0 );
    mRFrame->SetValue(false);
    itemBoxSizer13->Add(mRFrame, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
    itemGridBagSizer4->Add(itemBoxSizer17, wxGBPosition(2, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel2, wxID_STATIC, _("Bottom"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer17->Add(itemStaticText18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl19 = new wxTextCtrl( itemPanel2, ID_EDIT_BMARGIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer17->Add(itemTextCtrl19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mBFrame = new wxCheckBox( itemPanel2, ID_BOX_BFRAME, _("Frame"), wxDefaultPosition, wxDefaultSize, 0 );
    mBFrame->SetValue(false);
    itemBoxSizer17->Add(mBFrame, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer21Static = new wxStaticBox(itemPanel2, wxID_ANY, _("Plot area"));
    wxStaticBoxSizer* itemStaticBoxSizer21 = new wxStaticBoxSizer(itemStaticBoxSizer21Static, wxVERTICAL);
    itemGridBagSizer4->Add(itemStaticBoxSizer21, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mBoxFixed = new wxCheckBox( itemStaticBoxSizer21->GetStaticBox(), ID_BOX_FIXED, _("Fixed"), wxDefaultPosition, wxDefaultSize, 0 );
    mBoxFixed->SetValue(false);
    itemStaticBoxSizer21->Add(mBoxFixed, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer21->Add(itemBoxSizer23, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText24 = new wxStaticText( itemStaticBoxSizer21->GetStaticBox(), wxID_STATIC, _("X - Size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(itemStaticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXSize = new wxTextCtrl( itemStaticBoxSizer21->GetStaticBox(), ID_XSIZE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(mXSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer21->Add(itemBoxSizer26, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText27 = new wxStaticText( itemStaticBoxSizer21->GetStaticBox(), wxID_STATIC, _("Y - Size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer26->Add(itemStaticText27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYSize = new wxTextCtrl( itemStaticBoxSizer21->GetStaticBox(), ID_YSIZE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer26->Add(mYSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemGridBagSizer4->AddGrowableRow(0);
    itemGridBagSizer4->AddGrowableRow(1);
    itemGridBagSizer4->AddGrowableRow(2);
    itemGridBagSizer4->AddGrowableCol(0);
    itemGridBagSizer4->AddGrowableCol(1);
    itemGridBagSizer4->AddGrowableCol(2);

    GetBookCtrl()->AddPage(itemPanel2, _("Size/Margins"));

    wxPanel* itemPanel29 = new wxPanel( GetBookCtrl(), ID_PP_XSCALE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel29->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel29->SetSizer(itemBoxSizer30);

    wxStaticBox* itemStaticBoxSizer31Static = new wxStaticBox(itemPanel29, wxID_ANY, _("Limits"));
    wxStaticBoxSizer* itemStaticBoxSizer31 = new wxStaticBoxSizer(itemStaticBoxSizer31Static, wxVERTICAL);
    itemBoxSizer30->Add(itemStaticBoxSizer31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer31->Add(itemBoxSizer32, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText33 = new wxStaticText( itemStaticBoxSizer31->GetStaticBox(), wxID_STATIC, _("Minimum"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer32->Add(itemStaticText33, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXMin = new wxTextCtrl( itemStaticBoxSizer31->GetStaticBox(), ID_XMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer32->Add(mXMin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer31->Add(itemBoxSizer35, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText36 = new wxStaticText( itemStaticBoxSizer31->GetStaticBox(), wxID_STATIC, _("Maximum"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer35->Add(itemStaticText36, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXMax = new wxTextCtrl( itemStaticBoxSizer31->GetStaticBox(), ID_XMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer35->Add(mXMax, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXReverse = new wxCheckBox( itemStaticBoxSizer31->GetStaticBox(), ID_XBOX_REVERSE, _("Reverse direction"), wxDefaultPosition, wxDefaultSize, 0 );
    mXReverse->SetValue(false);
    itemStaticBoxSizer31->Add(mXReverse, 0, wxALIGN_LEFT|wxALL, 5);

    mXAutolimits = new wxCheckBox( itemStaticBoxSizer31->GetStaticBox(), ID_XBOX_AUTOLIMITS, _("Automatic Limits"), wxDefaultPosition, wxDefaultSize, 0 );
    mXAutolimits->SetValue(false);
    itemStaticBoxSizer31->Add(mXAutolimits, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer40Static = new wxStaticBox(itemPanel29, wxID_ANY, _("Ticks"));
    wxStaticBoxSizer* itemStaticBoxSizer40 = new wxStaticBoxSizer(itemStaticBoxSizer40Static, wxVERTICAL);
    itemBoxSizer30->Add(itemStaticBoxSizer40, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxGridBagSizer* itemGridBagSizer41 = new wxGridBagSizer(0, 0);
    itemGridBagSizer41->SetEmptyCellSize(wxSize(10, 20));
    itemStaticBoxSizer40->Add(itemGridBagSizer41, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText42 = new wxStaticText( itemStaticBoxSizer40->GetStaticBox(), wxID_STATIC, _("Major"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer41->Add(itemStaticText42, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXDivMajor = new wxTextCtrl( itemStaticBoxSizer40->GetStaticBox(), ID_XDIV_MAJOR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer41->Add(mXDivMajor, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    mXBoxMajor = new wxCheckBox( itemStaticBoxSizer40->GetStaticBox(), ID_XBOX_MAJOR, _("Grid"), wxDefaultPosition, wxDefaultSize, 0 );
    mXBoxMajor->SetValue(false);
    itemGridBagSizer41->Add(mXBoxMajor, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText45 = new wxStaticText( itemStaticBoxSizer40->GetStaticBox(), wxID_STATIC, _("minor"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer41->Add(itemStaticText45, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXDivSmall = new wxSpinCtrl( itemStaticBoxSizer40->GetStaticBox(), ID_XSPIN_MINOR, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 1 );
    itemGridBagSizer41->Add(mXDivSmall, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    mXBoxSmall = new wxCheckBox( itemStaticBoxSizer40->GetStaticBox(), ID_XBOX_MINOR, _("grid"), wxDefaultPosition, wxDefaultSize, 0 );
    mXBoxSmall->SetValue(false);
    itemGridBagSizer41->Add(mXBoxSmall, wxGBPosition(1, 2), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText48 = new wxStaticText( itemStaticBoxSizer40->GetStaticBox(), wxID_STATIC, _("Reference"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer41->Add(itemStaticText48, wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXRefLine = new wxTextCtrl( itemStaticBoxSizer40->GetStaticBox(), ID_XEDIT_REFLINE, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer41->Add(mXRefLine, wxGBPosition(2, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    mXBoxRef = new wxCheckBox( itemStaticBoxSizer40->GetStaticBox(), ID_XBOX_REFLINE, _("Line"), wxDefaultPosition, wxDefaultSize, 0 );
    mXBoxRef->SetValue(false);
    itemGridBagSizer41->Add(mXBoxRef, wxGBPosition(2, 2), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemGridBagSizer41->AddGrowableCol(1);

    wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer40->Add(itemBoxSizer51, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    mXAutoAxis = new wxCheckBox( itemStaticBoxSizer40->GetStaticBox(), ID_XBOX_AUTOAXIS, _("Automatic Axis Fit"), wxDefaultPosition, wxDefaultSize, 0 );
    mXAutoAxis->SetValue(false);
    itemBoxSizer51->Add(mXAutoAxis, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer40->Add(itemBoxSizer53, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    mXTextRotate = new wxCheckBox( itemStaticBoxSizer40->GetStaticBox(), ID_XBOX_ROTATE, _("Text rotate"), wxDefaultPosition, wxDefaultSize, 0 );
    mXTextRotate->SetValue(false);
    itemBoxSizer53->Add(mXTextRotate, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer40->Add(itemBoxSizer55, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    mXInside = new wxCheckBox( itemStaticBoxSizer40->GetStaticBox(), ID_XBOX_INSIDE, _("Tick inside"), wxDefaultPosition, wxDefaultSize, 0 );
    mXInside->SetValue(false);
    itemBoxSizer55->Add(mXInside, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer40->Add(itemBoxSizer57, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    mXLabelInside = new wxCheckBox( itemStaticBoxSizer40->GetStaticBox(), ID_LABEL_INSIDE, _("Label Inside"), wxDefaultPosition, wxDefaultSize, 0 );
    mXLabelInside->SetValue(false);
    itemBoxSizer57->Add(mXLabelInside, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxStaticBox* itemStaticBoxSizer59Static = new wxStaticBox(itemPanel29, wxID_ANY, _("Units"));
    wxStaticBoxSizer* itemStaticBoxSizer59 = new wxStaticBoxSizer(itemStaticBoxSizer59Static, wxVERTICAL);
    itemBoxSizer30->Add(itemStaticBoxSizer59, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxArrayString mXListUnitsStrings;
    mXListUnitsStrings.Add(_("Number"));
    mXListUnitsStrings.Add(_("Julian Date"));
    mXListUnitsStrings.Add(_("Modified Julian"));
    mXListUnitsStrings.Add(_("Month"));
    mXListUnits = new wxListBox( itemStaticBoxSizer59->GetStaticBox(), ID_XLIST_UNITS, wxDefaultPosition, wxSize(-1, 120), mXListUnitsStrings, wxLB_SINGLE );
    mXListUnits->SetStringSelection(_("Number"));
    itemStaticBoxSizer59->Add(mXListUnits, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel29, _("X-Scale"));

    wxPanel* itemPanel61 = new wxPanel( GetBookCtrl(), ID_PP_YSCALE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel61->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel61->SetSizer(itemBoxSizer62);

    wxStaticBox* itemStaticBoxSizer63Static = new wxStaticBox(itemPanel61, wxID_ANY, _("Limits"));
    wxStaticBoxSizer* itemStaticBoxSizer63 = new wxStaticBoxSizer(itemStaticBoxSizer63Static, wxVERTICAL);
    itemBoxSizer62->Add(itemStaticBoxSizer63, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer64 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer63->Add(itemBoxSizer64, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText65 = new wxStaticText( itemStaticBoxSizer63->GetStaticBox(), wxID_STATIC, _("Minimum"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer64->Add(itemStaticText65, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYMin = new wxTextCtrl( itemStaticBoxSizer63->GetStaticBox(), ID_YMIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer64->Add(mYMin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer67 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer63->Add(itemBoxSizer67, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText68 = new wxStaticText( itemStaticBoxSizer63->GetStaticBox(), wxID_STATIC, _("Maximum"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer67->Add(itemStaticText68, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYMax = new wxTextCtrl( itemStaticBoxSizer63->GetStaticBox(), ID_YMAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer67->Add(mYMax, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYReverse = new wxCheckBox( itemStaticBoxSizer63->GetStaticBox(), ID_YBOX_REVERSE, _("Reverse direction"), wxDefaultPosition, wxDefaultSize, 0 );
    mYReverse->SetValue(false);
    itemStaticBoxSizer63->Add(mYReverse, 0, wxALIGN_LEFT|wxALL, 5);

    mYAutolimits = new wxCheckBox( itemStaticBoxSizer63->GetStaticBox(), ID_YBOX_AUTOLIMITS, _("Automatic Limits"), wxDefaultPosition, wxDefaultSize, 0 );
    mYAutolimits->SetValue(false);
    itemStaticBoxSizer63->Add(mYAutolimits, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer72Static = new wxStaticBox(itemPanel61, wxID_ANY, _("Ticks"));
    wxStaticBoxSizer* itemStaticBoxSizer72 = new wxStaticBoxSizer(itemStaticBoxSizer72Static, wxVERTICAL);
    itemBoxSizer62->Add(itemStaticBoxSizer72, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxGridBagSizer* itemGridBagSizer73 = new wxGridBagSizer(0, 0);
    itemGridBagSizer73->SetEmptyCellSize(wxSize(10, 20));
    itemStaticBoxSizer72->Add(itemGridBagSizer73, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText74 = new wxStaticText( itemStaticBoxSizer72->GetStaticBox(), wxID_STATIC, _("Major"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer73->Add(itemStaticText74, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYDivMajor = new wxTextCtrl( itemStaticBoxSizer72->GetStaticBox(), ID_YDIV_MAJOR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer73->Add(mYDivMajor, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    mYBoxMajor = new wxCheckBox( itemStaticBoxSizer72->GetStaticBox(), ID_YBOX_MAJOR, _("Grid"), wxDefaultPosition, wxDefaultSize, 0 );
    mYBoxMajor->SetValue(false);
    itemGridBagSizer73->Add(mYBoxMajor, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText77 = new wxStaticText( itemStaticBoxSizer72->GetStaticBox(), wxID_STATIC, _("minor"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer73->Add(itemStaticText77, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYDivSmall = new wxSpinCtrl( itemStaticBoxSizer72->GetStaticBox(), ID_YDIV_SMALL, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 1 );
    itemGridBagSizer73->Add(mYDivSmall, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    mYBoxSmall = new wxCheckBox( itemStaticBoxSizer72->GetStaticBox(), ID_YBOX_SMALL, _("grid"), wxDefaultPosition, wxDefaultSize, 0 );
    mYBoxSmall->SetValue(false);
    itemGridBagSizer73->Add(mYBoxSmall, wxGBPosition(1, 2), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText80 = new wxStaticText( itemStaticBoxSizer72->GetStaticBox(), wxID_STATIC, _("Reference"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer73->Add(itemStaticText80, wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYRefLine = new wxTextCtrl( itemStaticBoxSizer72->GetStaticBox(), ID_YREF_LINE, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer73->Add(mYRefLine, wxGBPosition(2, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 10);

    mYBoxRef = new wxCheckBox( itemStaticBoxSizer72->GetStaticBox(), ID_YBOX_REFLINE, _("Line"), wxDefaultPosition, wxDefaultSize, 0 );
    mYBoxRef->SetValue(false);
    itemGridBagSizer73->Add(mYBoxRef, wxGBPosition(2, 2), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer83 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer72->Add(itemBoxSizer83, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    mYAutoAxis = new wxCheckBox( itemStaticBoxSizer72->GetStaticBox(), ID_YBOX_AUTOTICK, _("Automatic Ticks"), wxDefaultPosition, wxDefaultSize, 0 );
    mYAutoAxis->SetValue(false);
    itemBoxSizer83->Add(mYAutoAxis, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer72->Add(itemBoxSizer85, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    mYTextRotate = new wxCheckBox( itemStaticBoxSizer72->GetStaticBox(), ID_YBOX_ROTATE, _("Text rotate"), wxDefaultPosition, wxDefaultSize, 0 );
    mYTextRotate->SetValue(false);
    itemBoxSizer85->Add(mYTextRotate, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer72->Add(itemBoxSizer87, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    mYInside = new wxCheckBox( itemStaticBoxSizer72->GetStaticBox(), ID_YBOX_INSDIE, _("Tick inside"), wxDefaultPosition, wxDefaultSize, 0 );
    mYInside->SetValue(false);
    itemBoxSizer87->Add(mYInside, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxBoxSizer* itemBoxSizer89 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer72->Add(itemBoxSizer89, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    mYLabelInside = new wxCheckBox( itemStaticBoxSizer72->GetStaticBox(), ID_YLABEL_INSIDE, _("Label Inside"), wxDefaultPosition, wxDefaultSize, 0 );
    mYLabelInside->SetValue(false);
    itemBoxSizer89->Add(mYLabelInside, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    wxStaticBox* itemStaticBoxSizer91Static = new wxStaticBox(itemPanel61, wxID_ANY, _("Units"));
    wxStaticBoxSizer* itemStaticBoxSizer91 = new wxStaticBoxSizer(itemStaticBoxSizer91Static, wxVERTICAL);
    itemBoxSizer62->Add(itemStaticBoxSizer91, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxArrayString mYListUnitsStrings;
    mYListUnitsStrings.Add(_("Number"));
    mYListUnitsStrings.Add(_("Julian Date"));
    mYListUnitsStrings.Add(_("Modified Julian"));
    mYListUnitsStrings.Add(_("Month"));
    mYListUnits = new wxListBox( itemStaticBoxSizer91->GetStaticBox(), ID_YLIST_UNITS, wxDefaultPosition, wxSize(-1, 120), mYListUnitsStrings, wxLB_SINGLE );
    mYListUnits->SetStringSelection(_("Number"));
    itemStaticBoxSizer91->Add(mYListUnits, 1, wxGROW|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel61, _("Y-Scale"));

    wxPanel* itemPanel93 = new wxPanel( GetBookCtrl(), ID_PLOT_COLOR, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel93->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxFlexGridSizer* itemFlexGridSizer94 = new wxFlexGridSizer(4, 2, 0, 0);
    itemPanel93->SetSizer(itemFlexGridSizer94);

    wxStaticText* itemStaticText95 = new wxStaticText( itemPanel93, wxID_STATIC, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer94->Add(itemStaticText95, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorBack = new wxColourPickerCtrl( itemPanel93, ID_COLOR_BACK, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer94->Add(mColorBack, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText97 = new wxStaticText( itemPanel93, wxID_STATIC, _("Axis"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer94->Add(itemStaticText97, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorAxis = new wxColourPickerCtrl( itemPanel93, ID_COLOR_AXIS, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer94->Add(mColorAxis, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText99 = new wxStaticText( itemPanel93, wxID_STATIC, _("Reference"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer94->Add(itemStaticText99, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorRefLine = new wxColourPickerCtrl( itemPanel93, ID_COLOR_REFLINE, wxColour(217, 125, 125), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer94->Add(mColorRefLine, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText101 = new wxStaticText( itemPanel93, wxID_STATIC, _("Grid"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer94->Add(itemStaticText101, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorGrid = new wxColourPickerCtrl( itemPanel93, ID_COLOUR_GRID, wxColour(248, 255, 194), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer94->Add(mColorGrid, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel93, _("Colors"));

    wxPanel* itemPanel103 = new wxPanel( GetBookCtrl(), ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel103->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer104 = new wxBoxSizer(wxVERTICAL);
    itemPanel103->SetSizer(itemBoxSizer104);

    wxBoxSizer* itemBoxSizer105 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer104->Add(itemBoxSizer105, 0, wxGROW|wxALL, 5);
    wxArrayString mLabelListStrings;
    mLabelList = new wxListBox( itemPanel103, ID_LABEL_LIST, wxDefaultPosition, wxDefaultSize, mLabelListStrings, wxLB_SINGLE );
    itemBoxSizer105->Add(mLabelList, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer107 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer105->Add(itemBoxSizer107, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mDeleteLabel = new wxButton( itemPanel103, ID_DELETE_LABEL, _("-"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer107->Add(mDeleteLabel, 0, wxGROW|wxALL, 5);

    mAddLabel = new wxButton( itemPanel103, ID_ADD_LABEL, _("+"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer107->Add(mAddLabel, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer110Static = new wxStaticBox(itemPanel103, wxID_ANY, _("Label Settings"));
    wxStaticBoxSizer* itemStaticBoxSizer110 = new wxStaticBoxSizer(itemStaticBoxSizer110Static, wxHORIZONTAL);
    itemBoxSizer104->Add(itemStaticBoxSizer110, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer111 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer110->Add(itemBoxSizer111, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText112 = new wxStaticText( itemStaticBoxSizer110->GetStaticBox(), wxID_STATIC, _("Rotate clockwise"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer111->Add(itemStaticText112, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxArrayString mRotateListStrings;
    mRotateListStrings.Add(_("0"));
    mRotateListStrings.Add(_("90"));
    mRotateListStrings.Add(_("-90"));
    mRotateListStrings.Add(_("180"));
    mRotateList = new wxListBox( itemStaticBoxSizer110->GetStaticBox(), ID_ROTATE_LIST, wxDefaultPosition, wxSize(-1, 110), mRotateListStrings, wxLB_SINGLE );
    itemBoxSizer111->Add(mRotateList, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer114 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer110->Add(itemBoxSizer114, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText115 = new wxStaticText( itemStaticBoxSizer110->GetStaticBox(), wxID_STATIC, _("Attach near"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer114->Add(itemStaticText115, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxArrayString mAttachToListStrings;
    mAttachToListStrings.Add(_("Mid-left border"));
    mAttachToListStrings.Add(_("Mid-top border"));
    mAttachToListStrings.Add(_("Mid-right border"));
    mAttachToListStrings.Add(_("Mid-bottom border"));
    mAttachToListStrings.Add(_("Top-left corner"));
    mAttachToListStrings.Add(_("Top-right corner"));
    mAttachToListStrings.Add(_("Bottom-right corner"));
    mAttachToListStrings.Add(_("Bottom-left corner"));
    mAttachToListStrings.Add(_("Middle"));
    mAttachToList = new wxListBox( itemStaticBoxSizer110->GetStaticBox(), ID_ATTACH_LIST, wxDefaultPosition, wxSize(180, -1), mAttachToListStrings, wxLB_SINGLE );
    itemBoxSizer114->Add(mAttachToList, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer117 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer110->Add(itemBoxSizer117, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticText* itemStaticText118 = new wxStaticText( itemStaticBoxSizer110->GetStaticBox(), wxID_STATIC, _("Offset by"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer117->Add(itemStaticText118, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer119 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer117->Add(itemBoxSizer119, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText120 = new wxStaticText( itemStaticBoxSizer110->GetStaticBox(), wxID_STATIC, _("X"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer119->Add(itemStaticText120, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXLabel = new wxTextCtrl( itemStaticBoxSizer110->GetStaticBox(), ID_XLABEL_OFFSET, _("0"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer119->Add(mXLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer122 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer117->Add(itemBoxSizer122, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText123 = new wxStaticText( itemStaticBoxSizer110->GetStaticBox(), wxID_STATIC, _("Y"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer122->Add(itemStaticText123, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYLabel = new wxTextCtrl( itemStaticBoxSizer110->GetStaticBox(), ID_YLABEL_OFFSET, _("0"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer122->Add(mYLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer125 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer110->Add(itemBoxSizer125, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticText* itemStaticText126 = new wxStaticText( itemStaticBoxSizer110->GetStaticBox(), wxID_STATIC, _("Font"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer125->Add(itemStaticText126, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mFont = new wxFontPickerCtrl( itemStaticBoxSizer110->GetStaticBox(), ID_FONTCTRL, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial")), wxDefaultPosition, wxDefaultSize, wxFNTP_DEFAULT_STYLE|wxFNTP_FONTDESC_AS_LABEL|wxFNTP_USEFONT_FOR_LABEL );
    itemBoxSizer125->Add(mFont, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer104->Add(itemBoxSizer128, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer129 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer128->Add(itemBoxSizer129, 1, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText130 = new wxStaticText( itemPanel103, wxID_STATIC, _("Text Lines"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer129->Add(itemStaticText130, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mLabelText = new wxTextCtrl( itemPanel103, ID_LABEL_CONTENT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxSIMPLE_BORDER );
    itemBoxSizer129->Add(mLabelText, 0, wxGROW|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel103, _("Labels"));

    // Connect events and objects
    mLabelText->Connect(ID_LABEL_CONTENT, wxEVT_KILL_FOCUS, wxFocusEventHandler(PlotProperty::OnKillFocus), NULL, this);
////@end PlotProperty content construction
}


/*
 * Should we show tooltips?
 */

bool PlotProperty::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap PlotProperty::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin PlotProperty bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end PlotProperty bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon PlotProperty::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin PlotProperty icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end PlotProperty icon retrieval
}


void PlotProperty::ReadJSON(const Json::Value& json)
{
    wxColor temp;

    readColor(json,"back-color",temp);
    mColorBack->SetColour(temp);

    readColor(json,"axis-color",temp);
    mColorAxis->SetColour(temp);

    readColor(json,"ref-color",temp);
    mColorRefLine->SetColour(temp);

    readColor(json,"grid-color",temp);
    mColorGrid->SetColour(temp);

    mLFrame->SetValue(json["left-frame"].asBool());
    mTFrame->SetValue(json["top-frame"].asBool());
    mRFrame->SetValue(json["right-frame"].asBool());
    mBFrame->SetValue(json["bottom-frame"].asBool());

    if (json.isMember("margins"))
    {
        const Json::Value& margins = json["margins"];

        setEditCtrl(this, ID_EDIT_LMARGIN, margins[0].asString());
        setEditCtrl(this, ID_EDIT_TMARGIN, margins[1].asString());
        setEditCtrl(this, ID_EDIT_RMARGIN, margins[2].asString());
        setEditCtrl(this, ID_EDIT_BMARGIN, margins[3].asString());
    }

    if (json.isMember("fixed-size"))
    {
        const Json::Value& fixed = json["fixed-size"];
        mXSize->SetValue(fixed[0].asString());
        mYSize->SetValue(fixed[1].asString());
    }

    if (json.isMember("is-fixed-size"))
        mBoxFixed->SetValue(json["is-fixed-size"].asBool());

    if (json.isMember("xaxis"))
    {
        LinearScale xaxis(ScaleCoord::XCOORD,ScaleFlow::POSITIVE);
        xaxis.ReadJSON(json["xaxis"]);
        this->ReadXAxis(xaxis);
    }

    if (json.isMember("yaxis"))
    {
        LinearScale yaxis(ScaleCoord::YCOORD,ScaleFlow::NEGATIVE);
        yaxis.ReadJSON(json["yaxis"]);
        this->ReadYAxis(yaxis);
    }

    if (json.isMember("labels"))
    {
        labelStore = json["labels"];
        uint ct = labelStore.size();
        //wxLogMessage("Read labelStore size = %d", (int) ct);
        for(uint i = 0; i < ct; i++)
        {
            const Json::Value& theLabel = labelStore[i];
            if (theLabel.isMember("text"))
            {
                const Json::Value& text = theLabel["text"];

                if (text.isArray())
                {
                    uint linect = text.size();
                    std::stringstream ss;

                    for(uint k = 0; k < linect; k++)
                    {
                        std::string line = text[k].asString();
                        if (k > 0)
                            ss << std::endl;
                        ss << line;
                    }
                    mLabelList->AppendAndEnsureVisible(ss.str());
                }
            }
        }
    }
    /*wxCheckBox* mXFixSize;
    wxCheckBox* mYFixSize;
    wxTextCtrl* mXSize;
    wxTextCtrl* mYSize;*/

}

bool setJSONInt(wxWindow* host, int ID, Json::Value& v)
{
    wxString temp;

    if (getEditCtrl(host,ID,temp))
    {
        long result;

        if (temp.ToLong(&result))
        {
            v = (int) result;
            return true;
        }
    }
    return false;
}

void PlotProperty::ReadXAxis(const LinearScale& xaxis)
{
    wxString tempStr;

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

    tempStr.Clear();
    tempStr << xaxis.dataMax_;
    mXMax->SetValue(tempStr);

    tempStr.Clear();
    tempStr << xaxis.dataMin_;
    mXMin->SetValue(tempStr);

    mXReverse->SetValue(xaxis.direct_ == ScaleFlow::NEGATIVE);

    mXAutolimits->SetValue(xaxis.autoLimits_);
    mXTextRotate->SetValue(xaxis.tickRotate_ == TextRotate::CW_90);
    mXInside->SetValue(xaxis.insideTick_);
    mXLabelInside->SetValue(xaxis.insideLabel_);

}

void PlotProperty::ReadYAxis(const LinearScale& yaxis)
{
    wxString tempStr;
    tempStr << yaxis.axisTicks_;
    mYDivMajor->SetValue(tempStr);

    mYBoxMajor->SetValue(yaxis.showMajorGrid_);
    mYDivSmall->SetValue(yaxis.axisDiv_);
    mYBoxSmall->SetValue(yaxis.showMinorTicks_);

    tempStr.Clear();
    tempStr << yaxis.refValue_;
    mYRefLine->SetValue(tempStr);
    mYBoxRef->SetValue(yaxis.refLine_);
    mYAutoAxis->SetValue(yaxis.autoTicks_);
    mYListUnits->SetSelection((int) yaxis.units_);

    tempStr.Clear();
    tempStr << yaxis.dataMax_;
    mYMax->SetValue(tempStr);

    tempStr.Clear();
    tempStr << yaxis.dataMin_;
    mYMin->SetValue(tempStr);

    mYReverse->SetValue(yaxis.direct_ == ScaleFlow::NEGATIVE);

    mYAutolimits->SetValue(yaxis.autoLimits_);
    mYTextRotate->SetValue(yaxis.tickRotate_ == TextRotate::R_0);
    mYInside->SetValue(yaxis.insideTick_);
    mYLabelInside->SetValue(yaxis.insideLabel_);

}


void PlotProperty::SaveXAxis(LinearScale& xaxis)
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

    tempStr = mXMax->GetValue();
    tempStr.ToDouble(&xaxis.dataMax_);

    tempStr = mXMin->GetValue();
    tempStr.ToDouble(&xaxis.dataMin_);

    xaxis.direct_ = mXReverse->IsChecked() ? ScaleFlow::NEGATIVE : ScaleFlow::POSITIVE;

    xaxis.tickRotate_ = (mXTextRotate->IsChecked())?TextRotate::CW_90 : TextRotate::R_0;
    xaxis.insideTick_ = mXInside->IsChecked();
    xaxis.insideLabel_ = mXLabelInside->IsChecked();
}

void PlotProperty::SaveYAxis(LinearScale& yaxis)
{
    wxString tempStr;

    tempStr = mYDivMajor->GetValue();
    tempStr.ToDouble(&yaxis.axisTicks_);

    yaxis.showMajorGrid_ = mYBoxMajor->IsChecked();
    yaxis.axisDiv_ = mYDivSmall->GetValue();
    yaxis.showMinorTicks_ = mYBoxSmall->IsChecked();

    tempStr = mYRefLine->GetValue();
    tempStr.ToDouble(&yaxis.refValue_);
    yaxis.refLine_ = mYBoxRef->IsChecked();
    yaxis.autoTicks_ = mYAutoAxis->IsChecked();
    yaxis.autoLimits_ = mYAutolimits->IsChecked();

    yaxis.units_ = (SeriesUnit) mYListUnits->GetSelection();

    tempStr = mYMax->GetValue();
    tempStr.ToDouble(&yaxis.dataMax_);

    tempStr = mYMin->GetValue();
    tempStr.ToDouble(&yaxis.dataMin_);

    yaxis.direct_ = mYReverse->IsChecked() ? ScaleFlow::NEGATIVE : ScaleFlow::POSITIVE;
    yaxis.tickRotate_ = (mYTextRotate->IsChecked())?TextRotate::R_0 : TextRotate::ACW_90;
    yaxis.insideTick_ = mYInside->IsChecked();
    yaxis.insideLabel_ = mYLabelInside->IsChecked();
}


void PlotProperty::SaveJSON(Json::Value& json)
{

    json["back-color"] = colorAsString(mColorBack->GetColour());

    json["axis-color"] = colorAsString(mColorAxis->GetColour());

    json["ref-color"] = colorAsString(mColorRefLine->GetColour());
    json["grid-color"] = colorAsString(mColorGrid->GetColour());

    json["left-frame"] = mLFrame->IsChecked();
    json["top-frame"] = mTFrame->IsChecked();
    json["right-frame"] = mRFrame->IsChecked();
    json["bottom-frame"] = mBFrame->IsChecked();

    {
        Json::Value& margins = json["margins"];

        setJSONInt(this, ID_EDIT_LMARGIN, margins[0]);
        setJSONInt(this, ID_EDIT_TMARGIN, margins[1]);
        setJSONInt(this, ID_EDIT_RMARGIN, margins[2]);
        setJSONInt(this, ID_EDIT_BMARGIN, margins[3]);
    }
    {
        Json::Value& fixed = json["fixed-size"];
        wxString temp = mXSize->GetValue();
        long itemp = 400;
        temp.ToLong(&itemp);
        fixed[0] = (int)itemp;

        temp = mYSize->GetValue();
        itemp = 320;
        temp.ToLong(&itemp);
        fixed[1] = (int)itemp;

        json["is-fixed-size"] = mBoxFixed->IsChecked();
    }
    {
        LinearScale xaxis(ScaleCoord::XCOORD,ScaleFlow::POSITIVE);
        SaveXAxis(xaxis);
        Json::Value jx(Json::objectValue);
        xaxis.SaveJSON(jx);
        json["xaxis"] = jx;
    }
    {
        LinearScale yaxis(ScaleCoord::YCOORD,ScaleFlow::NEGATIVE);
        SaveYAxis(yaxis);
        Json::Value jy(Json::objectValue);
        yaxis.SaveJSON(jy);
        json["yaxis"] = jy;
    }


}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETE_LABEL
 */

void PlotProperty::OnDeleteLabelClick( wxCommandEvent& event )
{
    event.Skip(false);
    int sel = mLabelList->GetSelection();
    if (sel == wxNOT_FOUND)
        return;
    mLabelList->Delete(sel);
    Json::Value temp;
    labelStore.removeIndex(sel,&temp);

}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADD_LABEL
 */

void PlotProperty::OnAddLabelClick( wxCommandEvent& event )
{
    // add content of text to list, and to multi-lines store
    event.Skip(false);
    blockUpdate = true;
    //wxLogMessage("Before SetValue NewText Json labelStore ct = %d ", (int) labelStore.size());
    wxString displayLine = mLabelText->GetValue();
    if (displayLine.size() == 0)
    {
        displayLine = "New Text";
        mLabelText->SetValue(displayLine);
    }

    //wxLogMessage("1 Json labelStore ct = %d ", (int) labelStore.size());

    Json::Value json(Json::objectValue);
    setLabelObject(json);
    labelStore.append(json);
    //wxLogMessage("2 Json labelStore ct = %d ", (int) labelStore.size());
    mLabelList->AppendAndEnsureVisible(displayLine);
    int sel = mLabelList->GetCount()-1;
    mLabelList->SetSelection(sel);
    changeSelection();

    blockUpdate = false;
    //wxLogMessage("3 Json labelStore ct = %d ", (int) labelStore.size());
}


/*
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LABEL_LIST
 */

void PlotProperty::OnLabelListSelected( wxCommandEvent& event )
{

    event.Skip(false);
    changeSelection();

}

static void dumpJson(const Json::Value& v)
{
    std::stringstream ss;

    ss << v;
    std::string look = ss.str();

    wxLogMessage("Dump**: %s", look.c_str());

}

void PlotProperty::changeSelection()
{
    if (currentTextSelect >= 0)
    {
        saveTextSelect();
        currentTextSelect = -1;
    }

    currentTextSelect = mLabelList->GetSelection();

    if (currentTextSelect == wxNOT_FOUND)
        return;

    const Json::Value& theLabel = labelStore[currentTextSelect];
    dumpJson(theLabel);
    // should have been created
    if (theLabel.isObject())
    {
        const Json::Value& text = theLabel["text"];
        std::string editText;

        if (text.isArray())
        {
            uint linect = text.size();
            std::stringstream ss;

            for(uint k = 0; k < linect; k++)
            {
                std::string line = text[k].asString();
                if (k > 0)
                    ss << std::endl;
                ss << line;
            }
            editText = ss.str();
            mLabelText->SetValue(editText);
            //wxLogMessage("Value set to %s", editText.c_str());
        }
        int select = 0;

        if (theLabel.isMember("font"))
        {
            const Json::Value& font = theLabel["font"];
            std::string faceName = font["name"].asString();
            int pointSize = font["size"].asInt();
            wxFont theFont(wxFontInfo(pointSize).FaceName(faceName));

            mFont->SetSelectedFont(theFont);
        }
        if (readInt(theLabel,"attach",select))
            mAttachToList->SetSelection(select);

        if (readInt(theLabel,"rotate",select))
            mRotateList->SetSelection(select);

        if (theLabel.isMember("offset"))
        {
            const Json::Value& offset = theLabel["offset"];

            if (offset.isArray())
            {
                int xoffset = offset[0].asInt();
                int yoffset = offset[1].asInt();

                mXLabel->SetValue(std::to_string(xoffset));
                mYLabel->SetValue(std::to_string(yoffset));
            }
        }
    }

}
void PlotProperty::setLabelObject(Json::Value& json, bool textOnly)
{
    std::stringstream displayLine;
    uint linect = mLabelText->GetNumberOfLines();
   //wxLogMessage("setLabelObject: Current select = %d ", (int) currentTextSelect);

    Json::Value text(Json::arrayValue);

    for(uint i = 0; i < linect; i++)
    {
        std::string line = mLabelText->GetLineText(i).ToStdString();
        text[i] = line;

        if (i > 0)
            displayLine << std::endl;
        displayLine << line;
    }
    json["text"] = text;

    if (textOnly)
        return;

    Json::Value font(Json::objectValue);

    wxFont theFont = mFont->GetSelectedFont();
    font["name"] = theFont.GetFaceName().ToStdString();
    font["size"] = theFont.GetPointSize();

    json["font"] = font;

    json["jype"] = "textbox";
    int select = mAttachToList->GetSelection();

    json["attach"] = (select >= 0) ? select : 0;

    select = mRotateList->GetSelection();
    json["rotate"] = (select >= 0) ? select : 0;
    wxLogMessage("Rotate = %d", select);
    Json::Value offset(Json::arrayValue);
    int xoffset = GetLongValue(mXLabel);
    int yoffset = GetLongValue(mYLabel);
    offset[0] = xoffset;
    offset[1] = yoffset;
    json["offset"] = offset;

}

void PlotProperty::saveTextSelect(bool textOnly)
{
    if (currentTextSelect < 0)
        return;

    //wxLogMessage("saveTextSelect: Current select = %d ", (int) currentTextSelect);

    Json::Value& theLabel = labelStore[currentTextSelect];

    setLabelObject(theLabel,textOnly);

    wxString displayLine = mLabelText->GetValue();
    if (displayLine.size() == 0)
        displayLine = "^(*|*)^";
    mLabelList->SetString(currentTextSelect, displayLine);


}

/*
 * wxEVT_KILL_FOCUS event handler for ID_LABEL_CONTENT
 */

void PlotProperty::OnKillFocus( wxFocusEvent& event )
{
    event.Skip(false);
    if (blockUpdate)
        return;

    int sel = mLabelList->GetSelection();

    if (sel < 0)
        return;

    saveTextSelect();
}


/*
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_ATTACH_LIST
 */

void PlotProperty::OnAttachListSelected( wxCommandEvent& event )
{

    event.Skip(false);
    mXLabel->SetValue("0");
    mYLabel->SetValue("0");

}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_XBOX_AUTOLIMITS
 */

void PlotProperty::OnXboxAutolimitsClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_XBOX_AUTOLIMITS in PlotProperty.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_XBOX_AUTOLIMITS in PlotProperty.
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_YBOX_AUTOLIMITS
 */

void PlotProperty::OnYboxAutolimitsClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_YBOX_AUTOLIMITS in PlotProperty.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_YBOX_AUTOLIMITS in PlotProperty.
}

