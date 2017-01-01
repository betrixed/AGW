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

    wxPanel* itemPanel29 = new wxPanel( GetBookCtrl(), ID_PLOT_COLOR, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel29->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxFlexGridSizer* itemFlexGridSizer30 = new wxFlexGridSizer(4, 2, 0, 0);
    itemPanel29->SetSizer(itemFlexGridSizer30);

    wxStaticText* itemStaticText31 = new wxStaticText( itemPanel29, wxID_STATIC, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer30->Add(itemStaticText31, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorBack = new wxColourPickerCtrl( itemPanel29, ID_COLOR_BACK, wxColour(255, 255, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer30->Add(mColorBack, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText33 = new wxStaticText( itemPanel29, wxID_STATIC, _("Axis"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer30->Add(itemStaticText33, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorAxis = new wxColourPickerCtrl( itemPanel29, ID_COLOR_AXIS, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer30->Add(mColorAxis, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText35 = new wxStaticText( itemPanel29, wxID_STATIC, _("Reference"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer30->Add(itemStaticText35, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorRefLine = new wxColourPickerCtrl( itemPanel29, ID_COLOR_REFLINE, wxColour(217, 125, 125), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer30->Add(mColorRefLine, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText37 = new wxStaticText( itemPanel29, wxID_STATIC, _("Grid"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer30->Add(itemStaticText37, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mColorGrid = new wxColourPickerCtrl( itemPanel29, ID_COLOUR_GRID, wxColour(248, 255, 194), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer30->Add(mColorGrid, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel29, _("Colors"));

    wxPanel* itemPanel39 = new wxPanel( GetBookCtrl(), ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel39->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer40 = new wxBoxSizer(wxVERTICAL);
    itemPanel39->SetSizer(itemBoxSizer40);

    wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer40->Add(itemBoxSizer41, 0, wxGROW|wxALL, 5);
    wxArrayString mLabelListStrings;
    mLabelList = new wxListBox( itemPanel39, ID_LABEL_LIST, wxDefaultPosition, wxDefaultSize, mLabelListStrings, wxLB_SINGLE );
    itemBoxSizer41->Add(mLabelList, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer41->Add(itemBoxSizer43, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mDeleteLabel = new wxButton( itemPanel39, ID_DELETE_LABEL, _("-"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer43->Add(mDeleteLabel, 0, wxGROW|wxALL, 5);

    mAddLabel = new wxButton( itemPanel39, ID_ADD_LABEL, _("+"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer43->Add(mAddLabel, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer46Static = new wxStaticBox(itemPanel39, wxID_ANY, _("Label Settings"));
    wxStaticBoxSizer* itemStaticBoxSizer46 = new wxStaticBoxSizer(itemStaticBoxSizer46Static, wxHORIZONTAL);
    itemBoxSizer40->Add(itemStaticBoxSizer46, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer46->Add(itemBoxSizer47, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText48 = new wxStaticText( itemStaticBoxSizer46->GetStaticBox(), wxID_STATIC, _("Rotate clockwise"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer47->Add(itemStaticText48, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxArrayString mRotateListStrings;
    mRotateListStrings.Add(_("0"));
    mRotateListStrings.Add(_("90"));
    mRotateListStrings.Add(_("-90"));
    mRotateListStrings.Add(_("180"));
    mRotateList = new wxListBox( itemStaticBoxSizer46->GetStaticBox(), ID_ROTATE_LIST, wxDefaultPosition, wxSize(-1, 110), mRotateListStrings, wxLB_SINGLE );
    itemBoxSizer47->Add(mRotateList, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer50 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer46->Add(itemBoxSizer50, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText51 = new wxStaticText( itemStaticBoxSizer46->GetStaticBox(), wxID_STATIC, _("Attach near"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer50->Add(itemStaticText51, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

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
    mAttachToList = new wxListBox( itemStaticBoxSizer46->GetStaticBox(), ID_ATTACH_LIST, wxDefaultPosition, wxSize(180, -1), mAttachToListStrings, wxLB_SINGLE );
    itemBoxSizer50->Add(mAttachToList, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer46->Add(itemBoxSizer53, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticText* itemStaticText54 = new wxStaticText( itemStaticBoxSizer46->GetStaticBox(), wxID_STATIC, _("Offset by"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer53->Add(itemStaticText54, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer53->Add(itemBoxSizer55, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText56 = new wxStaticText( itemStaticBoxSizer46->GetStaticBox(), wxID_STATIC, _("X"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer55->Add(itemStaticText56, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mXLabel = new wxTextCtrl( itemStaticBoxSizer46->GetStaticBox(), ID_XLABEL_OFFSET, _("0"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer55->Add(mXLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer58 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer53->Add(itemBoxSizer58, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText59 = new wxStaticText( itemStaticBoxSizer46->GetStaticBox(), wxID_STATIC, _("Y"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer58->Add(itemStaticText59, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYLabel = new wxTextCtrl( itemStaticBoxSizer46->GetStaticBox(), ID_YLABEL_OFFSET, _("0"), wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer58->Add(mYLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer61 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer46->Add(itemBoxSizer61, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticText* itemStaticText62 = new wxStaticText( itemStaticBoxSizer46->GetStaticBox(), wxID_STATIC, _("Font"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer61->Add(itemStaticText62, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mFont = new wxFontPickerCtrl( itemStaticBoxSizer46->GetStaticBox(), ID_FONTCTRL, wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial")), wxDefaultPosition, wxDefaultSize, wxFNTP_DEFAULT_STYLE|wxFNTP_FONTDESC_AS_LABEL|wxFNTP_USEFONT_FOR_LABEL );
    itemBoxSizer61->Add(mFont, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer64 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer40->Add(itemBoxSizer64, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer64->Add(itemBoxSizer65, 1, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText66 = new wxStaticText( itemPanel39, wxID_STATIC, _("Text Lines"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer65->Add(itemStaticText66, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mLabelText = new wxTextCtrl( itemPanel39, ID_LABEL_CONTENT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxSIMPLE_BORDER );
    itemBoxSizer65->Add(mLabelText, 0, wxGROW|wxALL, 5);

    GetBookCtrl()->AddPage(itemPanel39, _("Labels"));

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


