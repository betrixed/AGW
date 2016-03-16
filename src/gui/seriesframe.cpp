/////////////////////////////////////////////////////////////////////////////
// Name:        seriesframe.cpp
// Purpose:     
// Author:      Michael Rynn
// Modified by: 
// Created:     Sun 31 Jan 2016 15:32:33 AEDT
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
#include "seriesgrid.h"
////@end includes

#include "seriesframe.h"

////@begin XPM images
////@end XPM images


/*
 * SeriesFrame type definition
 */

IMPLEMENT_CLASS( SeriesFrame, wxFrame )


/*
 * SeriesFrame event table definition
 */

BEGIN_EVENT_TABLE( SeriesFrame, wxFrame )

////@begin SeriesFrame event table entries
////@end SeriesFrame event table entries

END_EVENT_TABLE()


/*
 * SeriesFrame constructors
 */

SeriesFrame::SeriesFrame()
{
    Init();
}

SeriesFrame::SeriesFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * SeriesFrame creator
 */

bool SeriesFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SeriesFrame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end SeriesFrame creation
    return true;
}


/*
 * SeriesFrame destructor
 */

SeriesFrame::~SeriesFrame()
{
////@begin SeriesFrame destruction
////@end SeriesFrame destruction
}


/*
 * Member initialisation
 */

void SeriesFrame::Init()
{
////@begin SeriesFrame member initialisation
    mGrid = NULL;
////@end SeriesFrame member initialisation
}


/*
 * Control creation for SeriesFrame
 */

void SeriesFrame::CreateControls()
{    
////@begin SeriesFrame content construction
    SeriesFrame* itemFrame1 = this;

    mGrid = new SeriesGrid( itemFrame1, SeriesGrid::ID_SERIES_GRID, wxDefaultPosition, wxSize(200, 150), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    mGrid->SetDefaultColSize(50);
    mGrid->SetDefaultRowSize(25);
    mGrid->SetColLabelSize(25);
    mGrid->SetRowLabelSize(50);
    mGrid->CreateGrid(5, 5, wxGrid::wxGridSelectCells);

////@end SeriesFrame content construction
}


/*
 * Should we show tooltips?
 */

bool SeriesFrame::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap SeriesFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SeriesFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SeriesFrame bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon SeriesFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SeriesFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SeriesFrame icon retrieval
}
