/////////////////////////////////////////////////////////////////////////////
// Name:        mapwindow.cpp
// Purpose:
// Author:      Michael Rynn
// Modified by:
// Created:     Thu 11 Feb 2016 20:16:12 AEDT
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

#include "mapwindow.h"
#include "mapframe.h"
#include "stationtable.h"
#include "database.hpp"
#include "plotsymbol.h"

#include <wx/statusbr.h>
////@begin XPM images
////@end XPM images

using namespace agw;
/*
 * MapWindow type definition
 */

IMPLEMENT_DYNAMIC_CLASS( MapWindow, wxScrolledCanvas )


/*
 * MapWindow event table definition
 */

BEGIN_EVENT_TABLE( MapWindow, wxScrolledCanvas )

////@begin MapWindow event table entries
    EVT_PAINT( MapWindow::OnPaint )
    EVT_LEFT_DOWN( MapWindow::OnLeftDown )
////@end MapWindow event table entries

END_EVENT_TABLE()


/*
 * MapWindow constructors
 */

MapWindow::MapWindow()
{
    Init();
}

MapWindow::MapWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}


/*
 * MapWindow creator
 */

bool MapWindow::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
////@begin MapWindow creation
    wxScrolledCanvas::Create(parent, id, pos, size, style);
    CreateControls();
////@end MapWindow creation
    return true;
}


/*
 * MapWindow destructor
 */

MapWindow::~MapWindow()
{
////@begin MapWindow destruction
////@end MapWindow destruction
}


/*
 * Member initialisation
 */

void MapWindow::Init()
{
////@begin MapWindow member initialisation
////@end MapWindow member initialisation

    status_ = nullptr;
}


/*
 * Control creation for MapWindow
 */

void MapWindow::CreateControls()
{
////@begin MapWindow content construction
    MapWindow* itemScrolledCanvas1 = this;

    this->SetScrollbars(1, 1, 0, 0);
////@end MapWindow content construction
}


/*
 * Should we show tooltips?
 */

bool MapWindow::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap MapWindow::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MapWindow bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end MapWindow bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon MapWindow::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MapWindow icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MapWindow icon retrieval
}

void MapWindow::drawStationLayer(wxDC& dc)
{
    wxColor stationColor("red");

    wxBrush bStation(stationColor);

    auto saveBrush = dc.GetBrush();

    dc.SetBrush(bStation);

    const LocList& data = stations_->data();

    auto it = data.begin();
    auto fin = data.end();

    auto sdraw = agw::SymbolDraw::MakeSymbolDraw(PlotShape::CIRCLE, 3, dc);
    for(; it != fin; it++)
    {
        const GissLocation& loc = *(it->get());
        wxCoord xpt = (loc.long_ + xoffset_) * xscale_;
        wxCoord ypt = (loc.lat_ + yoffset_) * yscale_;
        sdraw->draw(xpt,ypt);
    }
    dc.SetBrush(saveBrush);
}

void MapWindow::drawCountryLayer(wxDC& dc)
{
    CountryLayer* p = map_c.get();

    auto it = p->clist.begin();
    auto fin = p->clist.end();

    wxColor borderColor("green");

    wxPen borderPen(borderColor);

    auto savePen = dc.GetPen();
    dc.SetPen(borderPen);

    for( ; it != fin; it++)
    {
        CountryRecord& cntry = *(it->get());
        agw::GMultiPoly& mp =  cntry.area_;

        auto pit = mp.data.begin();
        auto pfin = mp.data.end();

        for(; pit != pfin; pit++)
        {
           agw::GPoly& poly= *(pit->get());

            auto rit = poly.data.begin();
            auto rfin = poly.data.end();

            for(; rit != rfin; rit++)
            {
                agw::GRing& g = *(rit->get());

                auto lit = g.data.begin();
                auto lfin = g.data.end();

                wxPoint p1, p2;
                int ct = 0;
                for(; lit != lfin; lit++, ct++)
                {

                    p2.x = int((lit->X_ + xoffset_) * xscale_ );
                    p2.y = int((lit->Y_ + yoffset_) * yscale_ );
                    if (ct > 0)
                    {
                        dc.DrawLine(p1,p2);
                    }
                    p1 = p2;
                }

            }
        }
    }

    dc.SetPen(savePen);
}

void MapWindow::OnPaint( wxPaintEvent& event )
{

    wxPaintDC dc(this);

    wxColor backColor("white");
    wxBrush bback(backColor);

    this->GetVirtualSize(&mwidth_, &mheight_);

    auto saveBrush = dc.GetBrush();
    dc.SetBackground(bback);
    dc.DrawRectangle(0,0,mwidth_,mheight_);

    xscale_ = mwidth_/360.0;
    yscale_ = -(mheight_/180.0);

    xoffset_ = 180.0;
    yoffset_ = -90.0;

    drawCountryLayer(dc);
    drawStationLayer(dc);
    // draw the station locations

    dc.SetBrush(saveBrush);

}

const CountryRecord* MapWindow::findCountry(const wxPoint& p)
{

    CountryLayer* layer = map_c.get();

    auto it = layer->clist.begin();
    auto fin = layer->clist.end();

    agw::GPoint P(  p.x / xscale_ - xoffset_ ,  p.y / yscale_ - yoffset_);

    for( ; it != fin; it++)
    {
        CountryRecord* cntry = it->get();

        if (cntry->area_.containsPoint(P))
            return cntry;
    }
    return nullptr;

}
const GissLocation* MapWindow::findStation(const wxPoint& p)
{
    // Convert to Lat, Long

    //(point = (g + offset) * scale so  g = point/scale - offset)
    const LocList& data = stations_->data();
    auto it = data.begin();
    auto fin = data.end();

    double minDistance;
    const GissLocation* minloc;

    for(int ct = 0; it != fin; it++, ct++)
    {
        const GissLocation* loc = it->get();
        wxCoord xpt = (loc->long_ + xoffset_) * xscale_;
        wxCoord ypt = (loc->lat_ + yoffset_) * yscale_;
        double xdiff = (xpt - p.x);
        double ydiff = (ypt - p.y);

        double distance = sqrt(xdiff*xdiff+ydiff*ydiff);

        if ((ct==0) || (distance < minDistance))
        {
            minDistance = distance;
            minloc = loc;
        }
    }
    if (minDistance <= sqrt(3*3 + 3*3))
    {
        return minloc;
    }
    return nullptr;
    // interested in pixel closeness hear
    // go through table
}

/*
 * wxEVT_LEFT_DOWN event handler for ID_SCROLLEDWINDOW
 */

void MapWindow::OnLeftDown( wxMouseEvent& event )
{
    event.Skip(false);

    wxPoint p = event.GetPosition();

    const GissLocation* loc = findStation(p);

    if (loc)
    {
        if (status_)
        {
            wxString f0, f1, f2;

            f0 << loc->long_;
            f1 << loc->lat_;
            f2 << loc->name_;

            status_->SetStatusText(f0,0);
            status_->SetStatusText(f1,1);
            status_->SetStatusText(f2,2);
        }
    }
    else {
        const CountryRecord* crec = findCountry(p);
        if (crec)
        {
            status_->SetStatusText(crec->name_,2);
        }
    }
}

