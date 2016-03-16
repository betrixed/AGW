/////////////////////////////////////////////////////////////////////////////
// Name:        loadshapedlg.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Fri 22 Jan 2016 00:30:52 AEDT
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
#include "agw.h"
#include "loadshapedlg.h"

////@begin XPM images
////@end XPM images


#include "helper.h"
#include "sqlitedb.hpp"
/*
 * LoadShapeDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( LoadShapeDlg, wxDialog )


/*
 * LoadShapeDlg event table definition
 */

BEGIN_EVENT_TABLE( LoadShapeDlg, wxDialog )

////@begin LoadShapeDlg event table entries
////@end LoadShapeDlg event table entries

END_EVENT_TABLE()


/*
 * LoadShapeDlg constructors
 */

LoadShapeDlg::LoadShapeDlg()
{
    Init();
}

LoadShapeDlg::LoadShapeDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * LoadShapeDlg creator
 */

bool LoadShapeDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin LoadShapeDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end LoadShapeDlg creation
    return true;
}


/*
 * LoadShapeDlg destructor
 */

LoadShapeDlg::~LoadShapeDlg()
{
////@begin LoadShapeDlg destruction
////@end LoadShapeDlg destruction
}


/*
 * Member initialisation
 */

void LoadShapeDlg::Init()
{
////@begin LoadShapeDlg member initialisation
    mPathText = NULL;
    mTableText = NULL;
    mGeomText = NULL;
    mCompressCheck = NULL;
    mForce2DCheck = NULL;
    mRTreeCheck = NULL;
    mGeomRadio = NULL;
    mGeomSelect = NULL;
    mColumnRadio = NULL;
    mColumnSelect = NULL;
    mSRIDSpin = NULL;
    mCharsetSelect = NULL;
////@end LoadShapeDlg member initialisation
}


/*
 * Control creation for LoadShapeDlg
 */

void LoadShapeDlg::CreateControls()
{
////@begin LoadShapeDlg content construction
    LoadShapeDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer4, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer5, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("&Path"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mPathText = new wxTextCtrl( itemDialog1, ID_LDSHP_PATH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    itemBoxSizer5->Add(mPathText, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer8, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer8->Add(itemBoxSizer9, 1, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("&Table name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_LEFT|wxALL, 5);

    mTableText = new wxTextCtrl( itemDialog1, ID_LDSHP_TABLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(mTableText, 1, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemDialog1, wxID_STATIC, _("&GeomColumn name"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALL, 5);

    mGeomText = new wxTextCtrl( itemDialog1, ID_LDSHP_COLUMN, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(mGeomText, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer14Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Geometry storage"));
    wxStaticBoxSizer* itemStaticBoxSizer14 = new wxStaticBoxSizer(itemStaticBoxSizer14Static, wxVERTICAL);
    itemBoxSizer8->Add(itemStaticBoxSizer14, 0, wxGROW|wxALL, 5);

    mCompressCheck = new wxCheckBox( itemStaticBoxSizer14->GetStaticBox(), ID_LDSHP_COMPRESSED, _("Apply geometry compression"), wxDefaultPosition, wxDefaultSize, 0 );
    mCompressCheck->SetValue(false);
    itemStaticBoxSizer14->Add(mCompressCheck, 0, wxALIGN_LEFT|wxALL, 5);

    mForce2DCheck = new wxCheckBox( itemStaticBoxSizer14->GetStaticBox(), ID_LDSHP_COERCE_2D, _("Coerce 2D geometries [x,y]"), wxDefaultPosition, wxDefaultSize, 0 );
    mForce2DCheck->SetValue(false);
    itemStaticBoxSizer14->Add(mForce2DCheck, 0, wxALIGN_LEFT|wxALL, 5);

    mRTreeCheck = new wxCheckBox( itemStaticBoxSizer14->GetStaticBox(), ID_LDSHP_RTREE, _("With Spatial Index (R*Tree)"), wxDefaultPosition, wxDefaultSize, 0 );
    mRTreeCheck->SetValue(false);
    itemStaticBoxSizer14->Add(mRTreeCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer18, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer19Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Geometry Type"));
    wxStaticBoxSizer* itemStaticBoxSizer19 = new wxStaticBoxSizer(itemStaticBoxSizer19Static, wxVERTICAL);
    itemBoxSizer18->Add(itemStaticBoxSizer19, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mGeomRadioStrings;
    mGeomRadioStrings.Add(_("Automatic"));
    mGeomRadioStrings.Add(_("User specified"));
    mGeomRadio = new wxRadioBox( itemStaticBoxSizer19->GetStaticBox(), ID_LDSHP_USER_GTYPE, _("&Mode"), wxDefaultPosition, wxDefaultSize, mGeomRadioStrings, 1, wxRA_SPECIFY_COLS );
    mGeomRadio->SetSelection(0);
    itemStaticBoxSizer19->Add(mGeomRadio, 0, wxGROW|wxALL, 5);

    wxArrayString mGeomSelectStrings;
    mGeomSelect = new wxComboBox( itemStaticBoxSizer19->GetStaticBox(), ID_LDSHP_GTYPE, wxEmptyString, wxDefaultPosition, wxDefaultSize, mGeomSelectStrings, wxCB_DROPDOWN );
    mGeomSelect->Enable(false);
    itemStaticBoxSizer19->Add(mGeomSelect, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer22Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Primary Key Column"));
    wxStaticBoxSizer* itemStaticBoxSizer22 = new wxStaticBoxSizer(itemStaticBoxSizer22Static, wxVERTICAL);
    itemBoxSizer18->Add(itemStaticBoxSizer22, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mColumnRadioStrings;
    mColumnRadioStrings.Add(_("Automatic"));
    mColumnRadioStrings.Add(_("User specified"));
    mColumnRadio = new wxRadioBox( itemStaticBoxSizer22->GetStaticBox(), ID_LDSHP_USER_PKEY, _("&Mode"), wxDefaultPosition, wxDefaultSize, mColumnRadioStrings, 1, wxRA_SPECIFY_COLS );
    mColumnRadio->SetSelection(0);
    itemStaticBoxSizer22->Add(mColumnRadio, 0, wxGROW|wxALL, 5);

    wxArrayString mColumnSelectStrings;
    mColumnSelect = new wxComboBox( itemStaticBoxSizer22->GetStaticBox(), ID_LDSHP_PKCOL, wxEmptyString, wxDefaultPosition, wxDefaultSize, mColumnSelectStrings, wxCB_DROPDOWN );
    mColumnSelect->Enable(false);
    itemStaticBoxSizer22->Add(mColumnSelect, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer25, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer25->Add(itemBoxSizer26, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText27 = new wxStaticText( itemDialog1, wxID_STATIC, _("&SRID"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer26->Add(itemStaticText27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mSRIDSpin = new wxSpinCtrl( itemDialog1, ID_LDSHP_SRID, wxT("4326"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -1, 10000, 4326 );
    itemBoxSizer26->Add(mSRIDSpin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer29Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Charset Encoding"));
    wxStaticBoxSizer* itemStaticBoxSizer29 = new wxStaticBoxSizer(itemStaticBoxSizer29Static, wxHORIZONTAL);
    itemStaticBoxSizer29Static->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Lucida Sans Unicode")));
    itemBoxSizer25->Add(itemStaticBoxSizer29, 1, wxGROW|wxALL, 5);

    wxArrayString mCharsetSelectStrings;
    mCharsetSelect = new wxListBox( itemStaticBoxSizer29->GetStaticBox(), ID_LDSHP_CHARSET, wxDefaultPosition, wxSize(300, -1), mCharsetSelectStrings, wxLB_SINGLE );
    mCharsetSelect->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Lucida Sans Unicode")));
    itemStaticBoxSizer29->Add(mCharsetSelect, 1, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer31 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer31, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton32 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer31->AddButton(itemButton32);

    wxButton* itemButton33 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer31->AddButton(itemButton33);

    itemStdDialogButtonSizer31->Realize();

////@end LoadShapeDlg content construction

    this->Bind(wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction) &LoadShapeDlg::OnUserPKey,
                this, ID_LDSHP_USER_PKEY);
    this->Bind(wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction) &LoadShapeDlg::OnUserGType,
                this, ID_LDSHP_USER_GTYPE);


}


/*
 * Should we show tooltips?
 */

bool LoadShapeDlg::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap LoadShapeDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin LoadShapeDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end LoadShapeDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon LoadShapeDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin LoadShapeDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end LoadShapeDlg icon retrieval
}




void LoadShapeDlg::OnUserGType(wxCommandEvent & WXUNUSED(event))
{
//
// the mode of user defined Geometry Type changed
//

  if (mGeomRadio->GetSelection() == 0)
  {
      // automatic
      mGeomSelect->Clear();
      mGeomSelect->Enable(false);
  }
  else {
      // user specified
      initComboBox(mGeomSelect, this->geomTypes);
  }
}

void LoadShapeDlg::OnUserPKey(wxCommandEvent & WXUNUSED(event))
{
//
// the mode of user defined Primary Key Column changed
//

  if (mColumnRadio->GetSelection() == 0)
    {
      // automatic
      mColumnSelect->Clear();
      mColumnSelect->Enable(false);
  }
  else {
      // user specified
        initComboBox(mColumnSelect, this->dbFields);
    }
}

void trimAfterSpace(std::string& s)
{
    size_t firstspace = s.find(" ");
    if (firstspace > 0)
        s = s.substr(0,firstspace);
}

void LoadShapeDlg::SaveIn(LoadShapeCmd& cmd)
{
    cmd.geocolumn_ = mGeomText->GetValue().ToStdString();
    cmd.newtable_ = mTableText->GetValue().ToStdString();

    int ix = mCharsetSelect->GetSelection();
    if (ix >= 0)
    {
        cmd.charset_ = mCharsetSelect->GetString(ix).ToStdString();
        trimAfterSpace(cmd.charset_);
    }
    else
        cmd.charset_ = "UTF-8";

    if (mGeomRadio->GetSelection() == 0)
        cmd.geomtype_ =  "AUTO";
    else {
        cmd.geomtype_ = mGeomSelect->GetValue().ToStdString();
        trimAfterSpace(cmd.geomtype_);
    }
    if (mColumnRadio->GetSelection() == 0)
    {
        cmd.pkey_ = "";
    }
    else {
        cmd.pkey_ = mColumnSelect->GetValue().ToStdString();
        trimAfterSpace(cmd.pkey_);
    }

    cmd.srid_ = mSRIDSpin->GetValue();
    cmd.force2d_ = mForce2DCheck->IsChecked();
    cmd.rtreeIndex_ = mRTreeCheck->IsChecked();
    cmd.compress_ = mCompressCheck->IsChecked();
    cmd.filepath_ = mPathText->GetValue().ToStdString();
}
