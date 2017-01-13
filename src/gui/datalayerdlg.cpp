/////////////////////////////////////////////////////////////////////////////
// Name:        datalayerdlg.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Wed 27 Jan 2016 11:43:55 AEDT
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

//#include "linestylecombo.h"

////@begin includes
#include "wx/imaglist.h"
#include "../include/linestylecombo.h"
////@end includes
#include "plotxy.h"
#include "datalayerdlg.h"
#include "helper.h"
#include "plotlayer.h"
#include "plotlua.h"

////@begin XPM images
////@end XPM images

using namespace agw;
/*
 * DataLayerDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( DataLayerDlg, wxDialog )


/*
 * DataLayerDlg event table definition
 */

BEGIN_EVENT_TABLE( DataLayerDlg, wxDialog )

////@begin DataLayerDlg event table entries
    EVT_LISTBOX( ID_LISTBOX, DataLayerDlg::OnListboxSelected )
    EVT_BUTTON( ID_REMOVE_LAYER, DataLayerDlg::OnRemoveLayerClick )
    EVT_LISTBOX( ID_LISTBOX1, DataLayerDlg::OnListbox1Selected )
    EVT_SPINCTRL( ID_SPINCTRL, DataLayerDlg::OnSpinctrlUpdated )
    EVT_COLOURPICKER_CHANGED( ID_COLOURCTRL, DataLayerDlg::OnColourctrlColourPickerChanged )
    EVT_COLOURPICKER_CHANGED( ID_COLOURPICKERCTRL, DataLayerDlg::OnColourpickerctrlColourPickerChanged )
    EVT_SPINCTRL( ID_SPINCTRL1, DataLayerDlg::OnSpinctrl1Updated )
    EVT_COLOURPICKER_CHANGED( ID_COLOURPICKERCTRL1, DataLayerDlg::OnColourpickerctrl1ColourPickerChanged )
////@end DataLayerDlg event table entries

END_EVENT_TABLE()



/*
 * DataLayerDlg constructors
 */

DataLayerDlg::DataLayerDlg()
{
    Init();
}

DataLayerDlg::DataLayerDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * DataLayerDlg creator
 */

bool DataLayerDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin DataLayerDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end DataLayerDlg creation
    return true;
}


/*
 * DataLayerDlg destructor
 */

DataLayerDlg::~DataLayerDlg()
{
////@begin DataLayerDlg destruction
////@end DataLayerDlg destruction
}


/*
 * Member initialisation
 */

void DataLayerDlg::Init()
{
////@begin DataLayerDlg member initialisation
    mLayer = NULL;
    btnRemoveLayer = NULL;
    mSymbol = NULL;
    mSymbolSize = NULL;
    mSymbolBorder = NULL;
    mSymbolFill = NULL;
    mLineStyle = NULL;
    mLineThick = NULL;
    mLineColor = NULL;
    mLabel = NULL;
    mDisplay = NULL;
////@end DataLayerDlg member initialisation
    /*m_arrItems.Add( wxT("Solid") );
    m_arrItems.Add( wxT("Transparent") );
    m_arrItems.Add( wxT("Dot") );
    m_arrItems.Add( wxT("Long Dash") );
    m_arrItems.Add( wxT("Short Dash") );
    m_arrItems.Add( wxT("Dot Dash") );*/

    layerIX_ = -1;
}


/*
 * Control creation for DataLayerDlg
 */

void DataLayerDlg::CreateControls()
{
////@begin DataLayerDlg content construction
    DataLayerDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Layer"));
    wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
    itemBoxSizer3->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL, 5);

    wxArrayString mLayerStrings;
    mLayer = new wxListBox( itemStaticBoxSizer4->GetStaticBox(), ID_LISTBOX, wxDefaultPosition, wxSize(150, -1), mLayerStrings, wxLB_SINGLE );
    itemStaticBoxSizer4->Add(mLayer, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer4->Add(itemBoxSizer6, 0, wxGROW|wxALL, 5);

    btnRemoveLayer = new wxButton( itemStaticBoxSizer4->GetStaticBox(), ID_REMOVE_LAYER, _("Remove"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer6->Add(btnRemoveLayer, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxNotebook* itemNotebook8 = new wxNotebook( itemDialog1, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    wxPanel* itemPanel9 = new wxPanel( itemNotebook8, ID_SYM_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel9->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel9->SetSizer(itemBoxSizer10);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer10->Add(itemBoxSizer11, 0, wxGROW|wxALL, 5);
    wxArrayString mSymbolStrings;
    mSymbolStrings.Add(_("None"));
    mSymbolStrings.Add(_("Plus"));
    mSymbolStrings.Add(_("Cross"));
    mSymbolStrings.Add(_("Circle"));
    mSymbolStrings.Add(_("Triangle"));
    mSymbolStrings.Add(_("Square"));
    mSymbol = new wxListBox( itemPanel9, ID_LISTBOX1, wxDefaultPosition, wxSize(100, -1), mSymbolStrings, wxLB_SINGLE );
    itemBoxSizer11->Add(mSymbol, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemPanel9, wxID_STATIC, _("Shape"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer11->Add(itemStaticText13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer10->Add(itemBoxSizer14, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer14->Add(itemBoxSizer15, 0, wxALIGN_RIGHT|wxALL, 15);
    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel9, wxID_STATIC, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(itemStaticText16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mSymbolSize = new wxSpinCtrl( itemPanel9, ID_SPINCTRL, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 6, 1 );
    itemBoxSizer15->Add(mSymbolSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer14->Add(itemBoxSizer18, 0, wxALIGN_RIGHT|wxALL, 10);
    wxStaticText* itemStaticText19 = new wxStaticText( itemPanel9, wxID_STATIC, _("Border"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemStaticText19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mSymbolBorder = new wxColourPickerCtrl( itemPanel9, ID_COLOURCTRL, wxColour(0, 0, 0), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemBoxSizer18->Add(mSymbolBorder, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer14->Add(itemBoxSizer21, 0, wxALIGN_RIGHT|wxALL, 10);
    wxStaticText* itemStaticText22 = new wxStaticText( itemPanel9, wxID_STATIC, _("Fill"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer21->Add(itemStaticText22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mSymbolFill = new wxColourPickerCtrl( itemPanel9, ID_COLOURPICKERCTRL, wxColour(30, 144, 255), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemBoxSizer21->Add(mSymbolFill, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook8->AddPage(itemPanel9, _("Symbol"));

    wxPanel* itemPanel24 = new wxPanel( itemNotebook8, ID_LINE_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel24->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxVERTICAL);
    itemPanel24->SetSizer(itemBoxSizer25);

    wxFlexGridSizer* itemFlexGridSizer26 = new wxFlexGridSizer(4, 2, 15, 0);
    itemBoxSizer25->Add(itemFlexGridSizer26, 1, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText27 = new wxStaticText( itemPanel24, wxID_STATIC, _("Style"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText27, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString mLineStyleStrings;
    mLineStyleStrings.Add(_("Solid"));
    mLineStyleStrings.Add(_("Transparent"));
    mLineStyleStrings.Add(_("Dot"));
    mLineStyleStrings.Add(_("Long Dash"));
    mLineStyleStrings.Add(_("Short Dash"));
    mLineStyleStrings.Add(_("Dot Dash"));
    mLineStyle = new LineStyleCombo( itemPanel24, LineStyleCombo::ID_OWNERDRAWNCOMBOCTRL, _("Solid"), wxDefaultPosition, wxDefaultSize, mLineStyleStrings, 0 );
    mLineStyle->SetStringSelection(_("Solid"));
    itemFlexGridSizer26->Add(mLineStyle, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel24, wxID_STATIC, _("Thickness"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText29, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mLineThick = new wxSpinCtrl( itemPanel24, ID_SPINCTRL1, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 4, 1 );
    itemFlexGridSizer26->Add(mLineThick, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText31 = new wxStaticText( itemPanel24, wxID_STATIC, _("Colour"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer26->Add(itemStaticText31, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mLineColor = new wxColourPickerCtrl( itemPanel24, ID_COLOURPICKERCTRL1, wxColour(128, 0, 128), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
    itemFlexGridSizer26->Add(mLineColor, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer26->AddGrowableCol(1);

    itemNotebook8->AddPage(itemPanel24, _("Line"));

    itemBoxSizer3->Add(itemNotebook8, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer33, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText34 = new wxStaticText( itemDialog1, wxID_STATIC, _("Label"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer33->Add(itemStaticText34, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mLabel = new wxTextCtrl( itemDialog1, ID_LABEL, wxEmptyString, wxDefaultPosition, wxSize(150, -1), 0 );
    itemBoxSizer33->Add(mLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mDisplay = new wxPanel( itemDialog1, ID_DISPLAY, wxDefaultPosition, wxSize(-1, 60), wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    mDisplay->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    itemBoxSizer33->Add(mDisplay, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer37 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer37, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton38 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer37->AddButton(itemButton38);

    wxButton* itemButton39 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer37->AddButton(itemButton39);

    itemStdDialogButtonSizer37->Realize();

    // Connect events and objects
    mDisplay->Connect(ID_DISPLAY, wxEVT_PAINT, wxPaintEventHandler(DataLayerDlg::OnPaintResult), NULL, this);
////@end DataLayerDlg content construction


}


/*
 * Should we show tooltips?
 */

bool DataLayerDlg::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap DataLayerDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin DataLayerDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end DataLayerDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon DataLayerDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin DataLayerDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end DataLayerDlg icon retrieval
}


/*
 * wxEVT_PAINT event handler for ID_PANEL
 */

void DataLayerDlg::OnPaintResult( wxPaintEvent& event )
{

    wxPaintDC dc(wxDynamicCast(event.GetEventObject(), wxWindow));

    wxColor back_color(0xff,0xff,0xff);

    wxBrush brush_1(back_color);

    auto savePen = dc.GetPen();
    auto saveBrush = dc.GetBrush();

    int xspan, yspan;

    dc.GetSize(&xspan, &yspan);

    dc.DrawRectangle(0,0,xspan,yspan);

    int linestyle = mLineStyle->GetSelection();
    wxColor linecolor = mLineColor->GetColour();
    int linewidth = mLineThick->GetValue();

    wxPen linepen(linecolor, linewidth, indexToPenStyle(linestyle));

    dc.SetPen(linepen);

    int ypt = yspan/2;

    dc.DrawLine(1,ypt, xspan, ypt);

    wxBrush fill(mSymbolFill->GetColour());
    dc.SetBrush(fill);

    wxPoint triangle[4];

    int symbolShape_ = mSymbol->GetSelection();
    const int halfCross = (mSymbolSize->GetValue())*2 + 1;
	const int symsize = halfCross*2;

    int tbase = (int)((double) symsize *  1.155 );
    if (symbolShape_ == PlotShape::TRIANGLE)
    {
        triangle[0] = wxPoint(-halfCross,halfCross);
        triangle[1] = wxPoint(tbase/2-halfCross, -halfCross);
        triangle[2] = wxPoint(tbase-halfCross, halfCross);
    }


    wxPen border(mSymbolBorder->GetColour());

    dc.SetPen(border);

    int xpt = xspan/2;

       switch(symbolShape_)
        {
        case PlotShape::PLUS:
            dc.DrawLine(xpt - halfCross + 1, ypt, xpt + halfCross, ypt);
            dc.DrawLine(xpt, ypt- halfCross + 1, xpt, ypt + halfCross);
            break;
        case PlotShape::CROSS:
            dc.DrawLine(xpt - halfCross, ypt - halfCross, xpt+ halfCross+1, ypt + halfCross+1);
            dc.DrawLine(xpt - halfCross, ypt + halfCross, xpt+ halfCross+1, ypt - halfCross-1);
            break;
        case PlotShape::CIRCLE:
            dc.DrawCircle(xpt, ypt, halfCross);
            break;
        case PlotShape::SQUARE:
            dc.DrawRectangle(xpt - halfCross, ypt - halfCross, symsize, symsize);
            break;
        case PlotShape::TRIANGLE:
            dc.DrawPolygon(3, triangle, xpt, ypt);
            break;
        }


    dc.SetPen(savePen);
    dc.SetBrush(saveBrush);
}

void DataLayerDlg::fromLayers(std::vector<agw::PlotLayer_sptr>& layers)
{
    layers_.clear();

    for(auto it = layers.begin(); it != layers.end(); it++)
    {
        agw::PlotLayer_sptr pl = (*it);
        agw::DataLayer* d = dynamic_cast<DataLayer*>(pl.get());
        if (d)
        {
            layers_.push_back(pl);
        }
    }

    mLayer->Clear();
    store = Json::Value(Json::arrayValue); // replace with empty array
    for(auto it = layers_.begin(); it != layers_.end(); it++)
    {
        Json::Value dlayer(Json::objectValue);
        PlotLayer_sptr pp = (*it);
        agw::DataLayer* d = dynamic_cast<agw::DataLayer*>(pp.get());
        if (d)
        {
            d->SaveJSON(dlayer);
            store.append(dlayer);
            mLayer->AppendAndEnsureVisible(d->label_);
        }
    }
}

void DataLayerDlg::toJSON()
{
    // return variables from controls back to json for
    // currently selected layer before another select or save
    Json::Value& json = store[layerIX_];

    json["plotcolor"] = colorAsString(mSymbolBorder->GetColour());
    json["plotfill"] = colorAsString(mSymbolFill->GetColour());
    json["linecolor"] = colorAsString(mLineColor->GetColour());
    json["plotsize"] = mSymbolSize->GetValue();
    json["linesize"] = mLineThick->GetValue();
    json["label"] = mLabel->GetValue().ToStdString();
    json["plotshape"] = shapeToString(mSymbol->GetSelection());
    json["linestyle"] = indexToPenName(mLineStyle->GetSelection());

    layerIX_ = -1;
}

/*
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX
 */

void DataLayerDlg::OnListboxSelected( wxCommandEvent& event )
{
    event.Skip(false);

    int sel = mLayer->GetSelection();
    if (layerIX_ >= 0)
    {
        toJSON();
    }
    if (sel >= 0)
    {
        layerIX_ = sel;
        wxColor ctemp(0,0,0);
        int itemp;
        std::string stemp;

        const Json::Value& json = store[sel];

        if (readColor(json,"plotcolor",ctemp))
            mSymbolBorder->SetColour(ctemp);

        if (readColor(json,"plotfill",ctemp))
            mSymbolFill->SetColour(ctemp);

        if (readColor(json,"linecolor",ctemp))
            mLineColor->SetColour(ctemp);

        if (readInt(json,"plotsize",itemp))
            mSymbolSize->SetValue(itemp);

        if (readInt(json,"linesize",itemp))
            mLineThick->SetValue(itemp);

        if (readString(json,"label",stemp))
            mLabel->SetValue(stemp);

        if (readString(json, "plotshape", stemp))
        {
            int symbolSel = strToPlotShape(stemp);
            mSymbol->SetSelection(symbolSel);
        }
        if (readString(json,"linestyle",stemp))
        {
            mLineStyle->SetSelection(penNameToIndex(stemp));
        }
    }
    mDisplay->Refresh();

}
void DataLayerDlg::toLayers()
{
    if (layerIX_ >= 0)
        toJSON();

    for(uint ix = 0; ix < store.size(); ix++)
    {
        PlotLayer_sptr pp = layers_[ix];
        DataLayer* d = dynamic_cast<DataLayer*>(pp.get());
        if (!d)
            continue;
        wxColor ctemp(0,0,0);
        int itemp;
        std::string stemp;

        const Json::Value& json = store[ix];

        if (readColor(json,"plotcolor",ctemp))
            d->symbolBorder_ = ctemp;

        if (readColor(json,"plotfill",ctemp))
            d->symbolFill_ = ctemp;

        if (readColor(json,"linecolor",ctemp))
            d->lineColor_ = ctemp;

        if (readInt(json,"plotsize",itemp))
            d->symbolSize_ = itemp;

        if (readInt(json,"linesize",itemp))
            d->lineWidth_ = itemp;

        if (readString(json,"label",stemp))
            d->label_ = stemp;

        if (readString(json, "plotshape", stemp))
        {
            int symbolSel = strToPlotShape(stemp);
            d->symbolShape_ = symbolSel;
        }
        if (readString(json,"linestyle",stemp))
        {
            d->lineStyle_ = penNameToIndex(stemp);
        }
    }

}


/*
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL
 */

void DataLayerDlg::OnSpinctrlUpdated( wxSpinEvent& event )
{
    event.Skip(false);
    mDisplay->Refresh();
}


/*
 * wxEVT_COLOURPICKER_CHANGED event handler for ID_COLOURCTRL
 */

void DataLayerDlg::OnColourctrlColourPickerChanged( wxColourPickerEvent& event )
{
    event.Skip(false);
    mDisplay->Refresh();
}


/*
 * wxEVT_COLOURPICKER_CHANGED event handler for ID_COLOURPICKERCTRL
 */

void DataLayerDlg::OnColourpickerctrlColourPickerChanged( wxColourPickerEvent& event )
{
    event.Skip(false);
    mDisplay->Refresh();
}


/*
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL1
 */

void DataLayerDlg::OnSpinctrl1Updated( wxSpinEvent& event )
{
    event.Skip(false);
    mDisplay->Refresh();
}


/*
 * wxEVT_COLOURPICKER_CHANGED event handler for ID_COLOURPICKERCTRL1
 */

void DataLayerDlg::OnColourpickerctrl1ColourPickerChanged( wxColourPickerEvent& event )
{

    event.Skip(false);
    mDisplay->Refresh();

}


/*
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX1
 */

void DataLayerDlg::OnListbox1Selected( wxCommandEvent& event )
{
    event.Skip(false);
    mDisplay->Refresh();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_REMOVE_LAYER
 */

void DataLayerDlg::OnRemoveLayerClick( wxCommandEvent& event )
{
    // Before editing this code, remove the block markers.
    event.Skip();
    int select = mLayer->GetSelection();
    if (select >= 0)
    {
        agw::PlotLayer_sptr pp = layers_[select];
        auto plot = pp->getPlot();
        if (plot)
        {
            layerIX_ = -1; // user selection hack reset
            layers_.clear();
            plot->removeLayer(pp);
            fromLayers(plotLua_->layers_);
            return;
        }
    }


}

void DataLayerDlg::setPlot(agw::PlotLua_sptr& pl)
{
    plotLua_ = pl;
    fromLayers(plotLua_->layers_);
}
