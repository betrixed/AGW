/////////////////////////////////////////////////////////////////////////////
// Name:        stationquery.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Fri 15 Jan 2016 21:15:45 AEDT
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

#include "../include/stationquery.h"

////@begin XPM images
////@end XPM images

#include "json/json.h"


#include "stationsets.h"
#include "countrylistdlg.h"
#include "vegedlg.h"
#include "helper.h"
/*
 * StationQuery type definition
 */

IMPLEMENT_DYNAMIC_CLASS( StationQuery, wxPanel )


/*
 * StationQuery event table definition
 */

BEGIN_EVENT_TABLE( StationQuery, wxPanel )

////@begin StationQuery event table entries
    EVT_WINDOW_DESTROY( StationQuery::OnDestroy )
    EVT_RADIOBOX( ID_BOX_QTYPE, StationQuery::OnBoxQtypeSelected )
    EVT_BUTTON( ID_BTN_UP, StationQuery::OnBtnUpClick )
    EVT_BUTTON( ID_BTN_DOWN, StationQuery::OnBtnDownClick )
    EVT_BUTTON( ID_BTN_DELETE, StationQuery::OnBtnDeleteClick )
    EVT_BUTTON( ID_COUNTRY_BTN, StationQuery::OnCountryBtnClick )
////@end StationQuery event table entries

END_EVENT_TABLE()


/*
 * StationQuery constructors
 */

StationQuery::StationQuery()
{
    Init();
}

StationQuery::StationQuery( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}


/*
 * StationQuery creator
 */

bool StationQuery::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin StationQuery creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end StationQuery creation

    // find the StationSets parent


    wxWindow* test = parent;
    while( StationSets::ID_STATIONSETS != test->GetId())
    {
        test = test->GetParent();
        if (test == nullptr)
            break;
    }
    if (test != nullptr)
    {
        this->myFrame = dynamic_cast<StationSets*>(test);
    }
    else
        this->myFrame = nullptr;

    wxLogMessage("Query Panel %d created at %p",GetId(), (void*)this);
    return true;
}


/*
 * StationQuery destructor
 */

StationQuery::~StationQuery()
{
////@begin StationQuery destruction
////@end StationQuery destruction
    wxLogMessage("~StationQuery()");
}


/*
 * Member initialisation
 */

void StationQuery::Init()
{
////@begin StationQuery member initialisation
    mBossVSizer = NULL;
    opName = NULL;
    mBoxQType = NULL;
    btnUp = NULL;
    mBtnDown = NULL;
    mBtnDelete = NULL;
    editDescription = NULL;
    mSizerStation = NULL;
    mBoxNameLike = NULL;
    txtLike = NULL;
    sizerLatBand = NULL;
    mBoxLatitude = NULL;
    txtLatSouth = NULL;
    txtLatNorth = NULL;
    mSizerRandom = NULL;
    mBoxRandom = NULL;
    txtRowCount = NULL;
    staticRows = NULL;
    mSizerCountry = NULL;
    mBoxCountry = NULL;
    mCountryName = NULL;
////@end StationQuery member initialisation

    myFrame = nullptr;
}


/*
 * Control creation for StationQuery
 */

void StationQuery::CreateControls()
{
////@begin StationQuery content construction
    StationQuery* itemPanel1 = this;

    mBossVSizer = new wxBoxSizer(wxVERTICAL);
    itemPanel1->SetSizer(mBossVSizer);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    mBossVSizer->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemPanel1, wxID_STATIC, _("Op"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString opNameStrings;
    opNameStrings.Add(_("Or"));
    opNameStrings.Add(_("And Not"));
    opName = new wxChoice( itemPanel1, ID_CHOICE, wxDefaultPosition, wxDefaultSize, opNameStrings, 0 );
    opName->SetStringSelection(_("Or"));
    itemBoxSizer3->Add(opName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mBoxQTypeStrings;
    mBoxQTypeStrings.Add(_("&Spatial"));
    mBoxQTypeStrings.Add(_("&Random"));
    mBoxQTypeStrings.Add(_("&All"));
    mBoxQType = new wxRadioBox( itemPanel1, ID_BOX_QTYPE, _("Types in \"AND\""), wxDefaultPosition, wxDefaultSize, mBoxQTypeStrings, 1, wxRA_SPECIFY_ROWS );
    mBoxQType->SetSelection(0);
    itemBoxSizer3->Add(mBoxQType, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    btnUp = new wxButton( itemPanel1, ID_BTN_UP, _("Up"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer3->Add(btnUp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    mBtnDown = new wxButton( itemPanel1, ID_BTN_DOWN, _("Down"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer3->Add(mBtnDown, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    mBtnDelete = new wxButton( itemPanel1, ID_BTN_DELETE, _("Delete"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer3->Add(mBtnDelete, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    mBossVSizer->Add(itemBoxSizer12, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemPanel1, wxID_STATIC, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    editDescription = new wxTextCtrl( itemPanel1, ID_TEXT_SET_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(editDescription, 1, wxGROW|wxALL, 5);

    mSizerStation = new wxBoxSizer(wxHORIZONTAL);
    mBossVSizer->Add(mSizerStation, 0, wxGROW|wxALL, 5);

    mBoxNameLike = new wxCheckBox( itemPanel1, ID_CHECK_LIKE, _("Station name like (%,_)"), wxDefaultPosition, wxDefaultSize, 0 );
    mBoxNameLike->SetValue(false);
    mSizerStation->Add(mBoxNameLike, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtLike = new wxTextCtrl( itemPanel1, ID_NAME_LIKE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    mSizerStation->Add(txtLike, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    sizerLatBand = new wxBoxSizer(wxHORIZONTAL);
    mBossVSizer->Add(sizerLatBand, 0, wxGROW|wxALL, 5);

    mBoxLatitude = new wxCheckBox( itemPanel1, ID_BOX_LATITUDE, _("Latitude band from"), wxDefaultPosition, wxDefaultSize, 0 );
    mBoxLatitude->SetValue(false);
    sizerLatBand->Add(mBoxLatitude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtLatSouth = new wxTextCtrl( itemPanel1, ID_SOUTH_LAT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    sizerLatBand->Add(txtLatSouth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText21 = new wxStaticText( itemPanel1, wxID_STATIC, _("to"), wxDefaultPosition, wxDefaultSize, 0 );
    sizerLatBand->Add(itemStaticText21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtLatNorth = new wxTextCtrl( itemPanel1, ID_NORTH_LAT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    sizerLatBand->Add(txtLatNorth, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mSizerRandom = new wxBoxSizer(wxHORIZONTAL);
    mBossVSizer->Add(mSizerRandom, 0, wxGROW|wxALL, 5);

    mBoxRandom = new wxCheckBox( itemPanel1, ID_BOX_RANDOM, _("Random subset of "), wxDefaultPosition, wxDefaultSize, 0 );
    mBoxRandom->SetValue(false);
    mSizerRandom->Add(mBoxRandom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    txtRowCount = new wxTextCtrl( itemPanel1, ID_RANDOM_COUNT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    mSizerRandom->Add(txtRowCount, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    staticRows = new wxStaticText( itemPanel1, wxID_STATIC, _("from total rows (# or #%)"), wxDefaultPosition, wxDefaultSize, 0 );
    mSizerRandom->Add(staticRows, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mSizerCountry = new wxBoxSizer(wxHORIZONTAL);
    mBossVSizer->Add(mSizerCountry, 0, wxGROW|wxALL, 5);

    mBoxCountry = new wxCheckBox( itemPanel1, ID_BOX_COUNTRY, _("Country name is "), wxDefaultPosition, wxDefaultSize, 0 );
    mBoxCountry->SetValue(false);
    mSizerCountry->Add(mBoxCountry, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mCountryName = new wxTextCtrl( itemPanel1, ID_COUNTRY_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    mSizerCountry->Add(mCountryName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton30 = new wxButton( itemPanel1, ID_COUNTRY_BTN, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    mSizerCountry->Add(itemButton30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine31 = new wxStaticLine( itemPanel1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemStaticLine31->SetForegroundColour(wxColour(150, 123, 54));
    itemStaticLine31->SetBackgroundColour(wxColour(152, 209, 42));
    mBossVSizer->Add(itemStaticLine31, 0, wxGROW|wxLEFT|wxRIGHT, 2);

    // Connect events and objects
    itemPanel1->Connect(ID_STATIONQUERY, wxEVT_DESTROY, wxWindowDestroyEventHandler(StationQuery::OnDestroy), NULL, this);
////@end StationQuery content construction
}


/*
 * Should we show tooltips?
 */

bool StationQuery::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap StationQuery::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin StationQuery bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end StationQuery bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon StationQuery::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin StationQuery icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end StationQuery icon retrieval
}


 const std::string JDoc::DESCRIBE("describe");
 const std::string JDoc::EFFECT("effect");
 const std::string JDoc::ID("id");
 const std::string JDoc::NAME_LIKE("name-like");
 const std::string JDoc::LAT_BAND("lat-band");
 const std::string JDoc::RANDOM("random");
 const std::string JDoc::COUNTRY("country");
 const std::string JDoc::QSEQ("qseq");
const std::string JDoc::SQL_OP("sql-op");
const std::string JDoc::QDISPLAY("query-type");

bool StationQuery::GetAsJSON(Json::Value& root)
{
    std::string value;

    double d1, d2;

    long nval;

    if (textValue(editDescription,value))
    {
        root[JDoc::DESCRIBE] = value;
    }

    value = getChoice(opName);
    root[JDoc::EFFECT] = value;

    int sel = mBoxQType->GetSelection();
    root[JDoc::QDISPLAY] = (sel == 0) ? "spatial" : "random";


    if (mBoxNameLike->IsChecked())
    {
        if (textValue(txtLike,value))
        {
            root[JDoc::NAME_LIKE] = value;
        }
    }
    if (mBoxLatitude->IsChecked())
    {
        bool hasSouth = doubleValue(txtLatSouth,d1);
        bool hasNorth = doubleValue(txtLatNorth,d2);

        if (hasSouth || hasNorth)
        {
            if (!hasSouth)
                d1 = -90.0;
            if (!hasNorth)
                d2 = 90.0;
            Json::Value dpair(Json::arrayValue);
            dpair[0] = d1;
            dpair[1] = d2;

            root[JDoc::LAT_BAND] = dpair;
        }
    }
    if (mBoxRandom->IsChecked() && longValue(txtRowCount, nval))
    {
        root[JDoc::RANDOM] = (uint) nval;
    }
    if (mBoxCountry->IsChecked() && textValue(mCountryName,value))
    {
        root[JDoc::COUNTRY] = value;
    }


    return true;
}

// get it from parent
bool  StationQuery::GetSetName(std::string& value)
{
    StationSets* sset = dynamic_cast<StationSets*> (this->myFrame);
    wxString temp = sset->editSetName->GetValue();
    temp.Trim();
    value = temp.ToStdString();

    return (value.size() > 0);
}


void StationQuery::LoadFromJSON(const Json::Value& root)
{
    std::stringstream mSQL;

    mSQL << " ";

    int ct = 0;

    if (root.isObject())
    {
        if (root.isMember(JDoc::NAME_LIKE))
        {
            mBoxNameLike->SetValue(true);
            std::string value = root[JDoc::NAME_LIKE].asString();
            txtLike->SetValue(value);

            mSQL << "(Name like '" << value << "')";
            ct++;
        }

        if (root.isMember(JDoc::EFFECT))
        {
            std::string value = root[JDoc::EFFECT].asString();
            setChoice(opName, value);
        }
        int sel = 0;
        if (root.isMember("query-type"))
        {
            std::string value = root["query-type"].asString();
            if (value == "random")
                sel = 1;
        }
        mBoxQType->SetSelection(sel);
        SetQueryType(sel);

        if (root.isMember(JDoc::DESCRIBE))
        {
            editDescription->SetValue(root[JDoc::DESCRIBE].asString());
        }

        if (root.isMember(JDoc::LAT_BAND))
        {
            mBoxLatitude->SetValue(true);
            const Json::Value& jpair = root[JDoc::LAT_BAND];
            if (jpair.isArray())
            {
                double d1 = jpair[0].asDouble();
                double d2 = jpair[1].asDouble();

                wxString v1, v2;

                v1 << d1;
                v2 << d2;
                txtLatSouth->SetValue(v1);
                txtLatNorth->SetValue(v2);
                if (ct > 0)
                    mSQL << " AND ";
                ct++;
                mSQL << "(Latitude >= " << d1 << " AND Latitude <= " << d2 << ")";
            }
        }
        if (root.isMember(JDoc::RANDOM))
        {
            mBoxRandom->SetValue(true);

            std::string randomCt = root[JDoc::RANDOM].asString();
            txtRowCount->SetValue(randomCt);
            if (ct > 0)
                    mSQL << " AND ";
            ct++;
            mSQL << "(" << "stationid in (SELECT stationid from gissloc ORDER BY RANDOM() LIMIT "
                    << randomCt << "))";
        }

        if (root.isMember(JDoc::COUNTRY))
        {
            mBoxCountry->SetValue(true);

            std::string value = root[JDoc::COUNTRY].asString();

            mCountryName->SetValue(value);

            if (ct > 0)
                    mSQL << " AND ";
            ct++;
            mSQL << "(stationid in (SELECT G.stationid from gissloc G, country C"
                    " where Contains(C.Geometry,G.Geometry)"
                    "and C.LONG_NAME = '" << value << "'))";
        }
    }
    this->mSQL = mSQL.str();

}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_UP
 */

void StationQuery::OnBtnUpClick( wxCommandEvent& event )
{
    event.Skip(false);
    setsFrame->MoveUp(this);

}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_DOWN
 */

void StationQuery::OnBtnDownClick( wxCommandEvent& event )
{
    event.Skip(false);
    setsFrame->MoveDown(this);

}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_DELETE
 */

void StationQuery::OnBtnDeleteClick( wxCommandEvent& event )
{
    event.Skip(false);
    setsFrame->Removing(this);
}




/*
 * wxEVT_DESTROY event handler for ID_STATIONQUERY
 */

void StationQuery::OnDestroy( wxWindowDestroyEvent& event )
{
    event.Skip(false);
    wxLogMessage("Station Query Destroy %p",(void*)event.GetEventObject());

}

Database* StationQuery::getDB()
{
    StationSets* ss = dynamic_cast<StationSets*> (this->myFrame);
    if (ss)
    {
        return &ss->db_;
    }
    else
        return nullptr;
}
/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_COUNTRY_BTN
 */

void StationQuery::OnCountryBtnClick( wxCommandEvent& event )
{
    event.Skip(true);

    Database* dbp = getDB();

    if (!dbp)
        return;

    Database& sdb = *dbp;

    Statement query(sdb, "select distinct LONG_NAME from country order by LONG_NAME");

    wxArrayString  countryList;
    std::string name;

    while(query.next())
    {
         query.get(0,name);
         countryList.Add(name);
    }
    CountryListDlg dlg(this);

    dlg.mCountry->InsertItems(countryList,0);

    if (dlg.ShowModal() == wxID_OK)
    {
        int select = dlg.mCountry->GetSelection();
        if (select != wxNOT_FOUND)
            this->mCountryName->SetValue(dlg.mCountry->GetString(select));
    }
}


/*
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_BOX_QTYPE
 */

void StationQuery::OnBoxQtypeSelected( wxCommandEvent& event )
{

    event.Skip(true);

    int sel = mBoxQType->GetSelection();

    SetQueryType(sel);
}

void StationQuery::SetQueryType(int sel)
{

    bool isRandom = (sel==1) || (sel==3);
    bool isLat = (sel==0) || (sel==3);
    bool isCountry = (sel==0) || (sel==3);
    bool isStation = (sel==0) || (sel==3);

    mBossVSizer->Show(mSizerRandom, isRandom, true);//random
    mBossVSizer->Show(sizerLatBand, isLat, true);//latitude
    mBossVSizer->Show(mSizerCountry, isCountry, true);//random
    mBossVSizer->Show(mSizerStation, isStation, true);//random

    mBossVSizer->Layout();
    this->Update();
    setsFrame->packQueryList();



}




