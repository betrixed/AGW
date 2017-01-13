#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "plotxy.h"
#include "helper.h"
#include <vector>
#include <memory>
#include "plotlua.h"

using namespace agw;

void LegendLayer::SaveJSON(Json::Value& json)
{
    BoxLayer::SaveJSON(json);
}
void LegendLayer::ReadJSON(const Json::Value& json)
{
    BoxLayer::ReadJSON(json);
}

LegendLayer::LegendLayer (PlotPlace pp, const wxPoint& offset) : BoxLayer()
{
    place_ = pp;
    offset_ = offset;
    jype_ = "legend";
}

LegendLayer::~LegendLayer()
{}

// make image for DragImage
wxBitmap* LegendLayer::makeImage()
{
    wxBitmap* bits = new wxBitmap(bounds_.width, bounds_.height);

    wxMemoryDC temp_dc;

    temp_dc.SelectObject(*bits);

    temp_dc.Clear();
    wxRect box(0,0,bounds_.width, bounds_.height);

    temp_dc.SetFont(thePlot_->getFont());
    drawInBox(temp_dc, box);

    return bits;
}

void LegendLayer::drawInBox(wxDC& dc,const wxRect& box)
{
    std::vector<PlotLayer_sptr>& collect = thePlot_->layers_;

    auto it = collect.begin();
    auto fin = collect.end();

    auto savePen = dc.GetPen();
    auto saveBrush = dc.GetBrush();

    dc.DrawRectangle(box);
    int ypt = box.y + border;

    int w,h,descent,leading;
    int vspace = 0;

    for( ; it != fin; it++)
    {
        PlotLayer* play = (*it).get();
        DataLayer* dlay = dynamic_cast<DataLayer*>(play);
        if (dlay == nullptr)
            continue;
        DataLayer& dl = *dlay;

        int xpt = box.x + border;

        wxString Label = dl.legendText_;

        dc.GetTextExtent(Label, &w, &h, &descent, &leading, nullptr);
        int height = h + descent + leading;
        if (vspace==0)
            vspace = h/3;
        else
            ypt += vspace;
        wxPenStyle pstyle = indexToPenStyle(dl.lineStyle_);
        int half = height/2;

        if (dl.lineWidth_ > 0 && pstyle != wxPENSTYLE_TRANSPARENT)
        {


            wxPen linepen(dl.lineColor_, dl.lineWidth_, pstyle);
            dc.SetPen(linepen);
            dc.DrawLine(xpt, ypt+half, xpt+symwidth, ypt+half);
            //dc.SetPen(savePen);

        }


        wxPen   spen(dl.symbolBorder_, 1, wxPENSTYLE_SOLID);
        wxBrush sbrush(dl.symbolFill_, wxBRUSHSTYLE_SOLID);

        dc.SetPen(spen);
        dc.SetBrush(sbrush);
        const int radius = (dl.symbolSize_)*2 + 1;
        auto sdraw = SymbolDraw::MakeSymbolDraw( (PlotShape) dl.symbolShape_, radius, dc);

        sdraw->draw(xpt + symwidth/2, ypt + half);
        dc.SetPen(savePen);
        dc.DrawText(Label, xpt + symwidth + border, ypt);

        ypt += (height);

    }

    dc.SetPen(savePen);
    dc.SetBrush(saveBrush);

}

void LegendLayer::renderDC(wxDC& dc, PixelWorld& px)
{
    //* calculated on the fly, to display all datalayers

    int totalWidth = 0;
    int totalHeight = 0;
    wxCoord w, h, descent, leading;
    int vspace = 0;

    {
        auto it = thePlot_->layers_.begin();
        auto fin = thePlot_->layers_.end();

        while( it != fin)
        {
            PlotLayer* pl = it->get();
            DataLayer* data = dynamic_cast<DataLayer*>(pl);

            if (data != nullptr)
            {
                if (data->legendText_.size() == 0)
                    data->legendText_ = data->label_;

                wxString Label = data->legendText_;

                dc.GetTextExtent(Label, &w, &h, &descent, &leading, nullptr);
                int height = h + descent + leading;

                if (vspace == 0)
                    vspace = h/3;
                else
                    totalHeight += vspace;
                if (totalWidth < w)
                    totalWidth = w;
                totalHeight += height;
            }
            it++;
        }
    }

    wxRect box(0, 0, border*3 + symwidth + totalWidth, border*2 + totalHeight);

    // now get the position of the place, and offset from there
    wxSize dim = box.GetSize();
    wxPoint place = thePlot_->getPlotPlace(place_) + this->getPlaceAdjust(dim);
    wxPoint boffset = place + offset_; // gets real top left

    box.Offset(boffset);

    dc.DrawRectangle(box);

    this->bounds_ = box;

    drawInBox(dc, box);



}
