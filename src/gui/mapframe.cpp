/////////////////////////////////////////////////////////////////////////////
// Name:        mapframe.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Thu 11 Feb 2016 20:13:08 AEDT
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
#include "../include/mapwindow.h"
////@end includes

#include "mapframe.h"

////@begin XPM images
////@end XPM images


/*
 * MapFrame type definition
 */

using namespace agw;

IMPLEMENT_CLASS( MapFrame, wxFrame )


/*
 * MapFrame event table definition
 */

BEGIN_EVENT_TABLE( MapFrame, wxFrame )

////@begin MapFrame event table entries
////@end MapFrame event table entries

END_EVENT_TABLE()


/*
 * MapFrame constructors
 */

MapFrame::MapFrame()
{
    Init();
}

MapFrame::MapFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * MapFrame creator
 */

bool MapFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MapFrame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end MapFrame creation
    return true;
}


/*
 * MapFrame destructor
 */

MapFrame::~MapFrame()
{
////@begin MapFrame destruction
////@end MapFrame destruction
}


/*
 * Member initialisation
 */

void MapFrame::Init()
{
////@begin MapFrame member initialisation
    mMap = NULL;
    mStatus = NULL;
////@end MapFrame member initialisation
}


/*
 * Control creation for MapFrame
 */

void MapFrame::CreateControls()
{
////@begin MapFrame content construction
    MapFrame* itemFrame1 = this;

    mMap = new MapWindow( itemFrame1, MapWindow::ID_SCROLLEDWINDOW, wxDefaultPosition, wxSize(600, 300), wxSUNKEN_BORDER|wxFULL_REPAINT_ON_RESIZE|wxHSCROLL|wxVSCROLL );

    mStatus = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    mStatus->SetFieldsCount(3);
    int mStatusWidths[3];
    mStatusWidths[0] = 100;
    mStatusWidths[1] = 100;
    mStatusWidths[2] = -1;
    mStatus->SetStatusWidths(3, mStatusWidths);
    itemFrame1->SetStatusBar(mStatus);

////@end MapFrame content construction


    mMap->status_ = mStatus;
}


/*
 * Should we show tooltips?
 */

bool MapFrame::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap MapFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MapFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end MapFrame bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon MapFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MapFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MapFrame icon retrieval
}
