/////////////////////////////////////////////////////////////////////////////
// Name:        plotoptions.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     02/11/2013 11:52:48
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

#include "plotoptions.h"
#include "plotlua.h"

////@begin XPM images
////@end XPM images


/*
 * PlotOptions type definition
 */

IMPLEMENT_DYNAMIC_CLASS( PlotOptions, wxDialog )


/*
 * PlotOptions event table definition
 */

BEGIN_EVENT_TABLE( PlotOptions, wxDialog )

////@begin PlotOptions event table entries
    EVT_BUTTON( wxID_OK, PlotOptions::OnOkClick )
    EVT_BUTTON( wxID_APPLY, PlotOptions::OnApplyClick )
////@end PlotOptions event table entries

END_EVENT_TABLE()


/*
 * PlotOptions constructors
 */

PlotOptions::PlotOptions()
{
    Init();
}

PlotOptions::PlotOptions( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * PlotOptions creator
 */

bool PlotOptions::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin PlotOptions creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end PlotOptions creation
    return true;
}


/*
 * PlotOptions destructor
 */

PlotOptions::~PlotOptions()
{
////@begin PlotOptions destruction
////@end PlotOptions destruction
}


/*
 * Member initialisation
 */

void PlotOptions::Init()
{
////@begin PlotOptions member initialisation
    editLeftAxisArea = NULL;
    editPixelsHeight_ = NULL;
    editPixelsWidth_ = NULL;
    checkYAxisMajorGrid = NULL;
    checkYAxisMinorTicks = NULL;
    btnOK_ = NULL;
////@end PlotOptions member initialisation
}


/*
 * Control creation for PlotOptions
 */

void PlotOptions::CreateControls()
{
////@begin PlotOptions content construction
    PlotOptions* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxGridBagSizer* itemGridBagSizer3 = new wxGridBagSizer(0, 0);
    itemGridBagSizer3->SetEmptyCellSize(wxSize(10, 20));
    itemBoxSizer2->Add(itemGridBagSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Left axis width"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(itemStaticText4, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    editLeftAxisArea = new wxTextCtrl( itemDialog1, ID_TEXTCTRL7, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(editLeftAxisArea, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Right axis width"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(itemStaticText6, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl7 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL8, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer3->Add(itemTextCtrl7, wxGBPosition(0, 3), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridBagSizer* itemGridBagSizer8 = new wxGridBagSizer(3, 0);
    itemGridBagSizer8->SetEmptyCellSize(wxSize(10, 20));
    itemBoxSizer2->Add(itemGridBagSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("Pixels Height"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer8->Add(itemStaticText9, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    editPixelsHeight_ = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer8->Add(editPixelsHeight_, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Pixels Width"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer8->Add(itemStaticText11, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    editPixelsWidth_ = new wxTextCtrl( itemDialog1, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemGridBagSizer8->Add(editPixelsWidth_, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    checkYAxisMajorGrid = new wxCheckBox( itemDialog1, ID_CHECKBOX, _("Y axis major grid"), wxDefaultPosition, wxDefaultSize, 0 );
    checkYAxisMajorGrid->SetValue(false);
    itemGridBagSizer8->Add(checkYAxisMajorGrid, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    checkYAxisMinorTicks = new wxCheckBox( itemDialog1, ID_CHECKBOX1, _("Y axis minor ticks"), wxDefaultPosition, wxDefaultSize, 0 );
    checkYAxisMinorTicks->SetValue(false);
    itemGridBagSizer8->Add(checkYAxisMinorTicks, wxGBPosition(0, 3), wxGBSpan(1, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemGridBagSizer8->AddGrowableCol(1);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    btnOK_ = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(btnOK_, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer15->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton18 = new wxButton( itemDialog1, wxID_APPLY, _("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(itemButton18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end PlotOptions content construction
}


/*
 * Should we show tooltips?
 */

bool PlotOptions::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap PlotOptions::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin PlotOptions bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end PlotOptions bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon PlotOptions::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin PlotOptions icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end PlotOptions icon retrieval
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
 */

void PlotOptions::OnApplyClick( wxCommandEvent& event )
{
    event.Skip();
    wxString s;
    long xval,yval;
    s = editPixelsWidth_->GetValue();
    s.ToLong(&xval);
    s = editPixelsHeight_->GetValue();
    s.ToLong(&yval);
    s = editLeftAxisArea->GetValue();

    plot_->world_.left_ = wxAtoi(s);
    plot_->world_.yScale_.setGridDisplay( checkYAxisMajorGrid->GetValue(), checkYAxisMinorTicks->GetValue());

    plotWindow_->SetVirtualSize(xval,yval);
    plotWindow_->calcScales();
    plotWindow_->Refresh();
}

void PlotOptions::SetFromPlot()
{
    oldHPixels_ = plot_->world_.xdim();
    oldVPixels_ = plot_->world_.ydim();
    wxString s;
    s.Printf(wxT("%d"),oldHPixels_);
    editPixelsWidth_->SetValue(s);
    s.Printf(wxT("%d"),oldVPixels_);
     editPixelsHeight_->SetValue(s);
     s.Printf(wxT("%d"),plot_->world_.left_);
     editLeftAxisArea->SetValue(s);
     checkYAxisMajorGrid->SetValue( plot_->world_.yScale_.showGrid() );
     checkYAxisMinorTicks->SetValue( plot_->world_.yScale_.showTicks() );
}
/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void PlotOptions::OnOkClick( wxCommandEvent& event )
{
    event.Skip();
    OnApplyClick(event);
}

