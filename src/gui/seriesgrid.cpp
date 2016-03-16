/////////////////////////////////////////////////////////////////////////////
// Name:        wxseriesgrid.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Sun 31 Jan 2016 15:35:37 AEDT
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

#include "seriesgrid.h"

////@begin XPM images
////@end XPM images


/*
 * SeriesGrid type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SeriesGrid, wxGrid )


/*
 * SeriesGrid event table definition
 */

BEGIN_EVENT_TABLE( SeriesGrid, wxGrid )

////@begin SeriesGrid event table entries
////@end SeriesGrid event table entries

END_EVENT_TABLE()


/*
 * SeriesGrid constructors
 */

SeriesGrid::SeriesGrid()
{
    Init();
}

SeriesGrid::SeriesGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}


/*
 * wxSeriesGrid creator
 */

bool SeriesGrid::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
////@begin SeriesGrid creation
    wxGrid::Create(parent, id, pos, size, style);
    CreateControls();
////@end SeriesGrid creation
    return true;
}


/*
 * SeriesGrid destructor
 */

SeriesGrid::~SeriesGrid()
{
////@begin SeriesGrid destruction
////@end SeriesGrid destruction
}


/*
 * Member initialisation
 */

void SeriesGrid::Init()
{
////@begin SeriesGrid member initialisation
////@end SeriesGrid member initialisation
}


/*
 * Control creation for wxSeriesGrid
 */

void SeriesGrid::CreateControls()
{
////@begin SeriesGrid content construction
////@end SeriesGrid content construction
}


/*
 * Should we show tooltips?
 */

bool SeriesGrid::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap SeriesGrid::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SeriesGrid bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SeriesGrid bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon SeriesGrid::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SeriesGrid icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SeriesGrid icon retrieval
}
