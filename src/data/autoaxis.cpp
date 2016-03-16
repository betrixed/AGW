#include <cmath>

#include "plotxy.h"

#include "helper.h"

using namespace agw;

void DataLayer::renderDC(wxDC& dc, PixelWorld& px)
{
    Series& xdata = *(xdata_);
    Series& ydata = *(ydata_);
    auto savepen = dc.GetPen();


    wxPen colpen(symbolBorder_);

    dc.SetPen(colpen);


    auto savefill = dc.GetBrush();
    wxBrush fill(symbolFill_);
    dc.SetBrush(fill);
    const double& xscale = px.xScale_.scale_;
    const double& yscale = px.yScale_.scale_;
    const double& xoffset = px.xScale_.offset_;
    const double& yoffset = px.yScale_.offset_;

    const int radius = (this->symbolSize_)*2 + 1;

    // draw lines first
    wxPenStyle linepen = indexToPenStyle(lineStyle_);

    if ((lineWidth_ > 0) && (linepen != wxPENSTYLE_TRANSPARENT))
    {
        wxPen linePen(lineColor_, lineWidth_, linepen);

        auto savePen = dc.GetPen();
        dc.SetPen(linePen);

        int xprev = 0;
        int yprev = 0;
        for(size_t i = 0; i < xdata.size(); i++)
        {
            auto x = xdata[i];
            auto y = ydata[i];
            if (std::isnan(x) || std::isnan(y))
                continue;
            auto xpt = (int)( (x-xoffset)* xscale ) + px.left_;
            auto ypt = (int)( (y-yoffset)* yscale ) + px.top_;

            if (i > 0)
            {
                dc.DrawLine(xprev,yprev,xpt,ypt);
            }
            xprev = xpt;
            yprev = ypt;
        }

        dc.SetPen(savePen);
    }

    auto sdraw = SymbolDraw::MakeSymbolDraw((PlotShape)symbolShape_, radius, dc);
    wxPen sympen(this->symbolBorder_, 1, wxPENSTYLE_SOLID);

    dc.SetPen(sympen);
    auto errors = errorbar_;
    for(size_t i = 0; i < xdata.size(); i++)
    {
        auto x = xdata[i];
        auto y = ydata[i];
        if (std::isnan(x) || std::isnan(y))
            continue;
        auto xpt = (int)( (x-xoffset)* xscale ) + px.left_;
        auto ypt = (int)( (y-yoffset)* yscale ) + px.top_;

        sdraw->draw(xpt,ypt);

        if (errors != nullptr)
        {
            auto errorSize = (*errors)[i];
            auto ept = (int)( errorSize*yscale);
            dc.DrawLine(xpt,ypt-ept,xpt,ypt+ept);
            dc.DrawLine(xpt-radius,ypt-ept,xpt+radius, ypt-ept);
            dc.DrawLine(xpt-radius,ypt+ept,xpt+radius, ypt+ept);
        }
    }

    dc.SetBrush(savefill);
    dc.SetPen(savepen);
}
void LineFit::renderDC(wxDC& dc, PixelWorld& px)
{
	Series& xdata = *(xdata_);
	Series& ydata = *(ydata_);
	auto savepen = dc.GetPen();
	wxPen colpen(lineColor_);
	colpen.SetWidth(lineWidth_);

	dc.SetPen(colpen);

	double xscale, yscale, xoffset, yoffset;

	px.getXScale(xscale, xoffset);
	px.getYScale(yscale, yoffset);

	for (size_t i = 1; i < xdata.size(); i++)
	{
		auto x0 = xdata[i - 1];
		auto y0 = ydata[i - 1];
		auto x = xdata[i];
		auto y = ydata[i];

		auto xpt0  = (int)((x0 - xoffset)* xscale) + px.left_;
		auto ypt0 = (int)((y0 - yoffset)* yscale) + px.top_;
		auto xpt = (int)((x - xoffset)* xscale) + px.left_;
		auto ypt = (int)((y - yoffset)* yscale) + px.top_;

		dc.DrawLine(xpt0, ypt0, xpt, ypt);


	}
	// put some text label showing the rate of warming halfway along line


	dc.SetPen(savepen);
}
