#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstopcd
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "plotxy.h"
#include "helper.h"

#define	kTestTickSize 9
double sRealTick[kTestTickSize] = {0.1,0.2,0.5,1,2,5,10,20,50};
int sRealLabels[kTestTickSize] = {5,5,4,5,5,4,5,5,4};

using namespace agw;

LinearScale::LinearScale(ScaleCoord co, ScaleFlow flow) : scale_(1.0), offset_(0.0),
                axisTicks_(1.0), refValue_(0.0), coord_(co), direct_(flow)

{
    dataMin_ = 0.0;
    dataMax_ = 1.0;

    units_ = DEFAULT;

    autoLimits_ = true;
    autoTicks_ = true;
    showMajorGrid_ = true;
    showMinorTicks_ = false;
    doFixedSize_ = true;
    refLine_ = true;
    axisDiv_ = 1;
    fixedSize_ = 400;
    insideTick_ = false;
    insideLabel_ = false;
    if (coord_ == ScaleCoord::YCOORD)
        tickRotate_ = TextRotate::ACW_90;
    else
        tickRotate_ = TextRotate::R_0;


}

void LinearScale::drawBottomXAxis(wxDC& dc, PixelWorld& px)
{
    double dtick;
    double firstTick;

    if (units_ == DATE_MONTH_NUM)
    {
        firstTick = 1.0;
        axisDiv_= 0;
    }
    else {
        firstTick = dataMin_;
    }
    modf(firstTick/axisTicks_,&dtick);
    if (dtick == -0.0)
        dtick = 0.0;


    double tick = dtick * axisTicks_;
    wxString label;
    wxCoord txw, txh; // retrieve Text extents

    wxPen gridPen(px.gridColor_);

    auto axispos = px.yspan_ + px.top_;
    auto endpos = px.left_ + px.xspan_;

    int bigTickTop, bigTickBottom;
    int tickTop, tickBottom;

    int otherSide = px.top_;
    if (insideTick_)
    {
        bigTickTop = axispos - 10;
        bigTickBottom = axispos;
        tickTop = axispos - 5;
        tickBottom = axispos;
    }
    else {
         bigTickTop = axispos;
         bigTickBottom = axispos+10;
         tickTop = axispos;
         tickBottom = axispos+5;
    }
    int angle = degreesRotation(tickRotate_);

    double prevTick = (dtick-1.0)* axisTicks_;
    int prevPt = (int)( (prevTick-offset_)* scale_ ) + px.left_;
    int tempx;

    while (prevTick <= dataMax_)
    {
        int xpt = (int)( (tick- offset_)*  scale_ ) + px.left_;
        if (xpt >= px.left_ && xpt <= endpos)
        {
            dc.DrawLine(xpt, bigTickTop, xpt, bigTickBottom);

            if (showMajorGrid_ && (xpt > px.left_) && (xpt < endpos))
            {
                auto savePen = dc.GetPen();
                dc.SetPen(gridPen);
                dc.DrawLine(xpt, bigTickTop, xpt, otherSide);
                dc.SetPen(savePen);
            }

            // into textaxispos
            if (units_ == DATE_MONTH_NUM)
            {
                int checkMonth = (int) tick;
                if (checkMonth >= 1 && checkMonth <= 12)
                {
                    auto monthno = (wxDateTime::Month) (checkMonth-1);

                    label.Printf(wxT("%s"),wxDateTime::GetMonthName(monthno,wxDateTime::NameFlags::Name_Abbr));
                }
                else
                    label.Clear();
            }
            else
                label.Printf(wxT("%g"),tick);

            label.Trim(false);label.Trim(true);

            if (label.size() > 0)
            {
                dc.GetTextExtent(label, &txw, &txh);
                if (tickRotate_ == TextRotate::R_0)
                {
                    int ylabel = insideLabel_ ? bigTickTop - txh - 3 : bigTickBottom + 3;
                    dc.DrawText(label, xpt - txw/2, ylabel);
                }
                else if (tickRotate_ == TextRotate::CW_90)
                {
                     int ylabel = insideLabel_ ? bigTickTop - txw - 3 : bigTickBottom + 3;
                     dc.DrawRotatedText(label, xpt + txh/2, ylabel, angle);
                }
                else if (tickRotate_ == TextRotate::ACW_90)
                {
                     int ylabel = insideLabel_ ? bigTickTop + 3 : bigTickBottom - txw - 3;
                    dc.DrawRotatedText(label, xpt + txh/2, ylabel, angle);
                }
            }
        }
        if (axisDiv_ > 0)
        {
            for(uint ix = 1; ix < axisDiv_; ix++)
            {
                tempx = prevPt - ((prevPt-xpt)*(int)ix) / (int)axisDiv_;
                if ((tempx <= endpos) && (tempx > px.left_))
                {
                    dc.DrawLine(tempx,tickTop, tempx, tickBottom );
                    if (showMinorTicks_)
                    {
                        auto savePen = dc.GetPen();
                        dc.SetPen(gridPen);
                        dc.DrawLine(tempx,tickTop, tempx, otherSide );
                        dc.SetPen(savePen);
                    }
                }
            }
            prevPt = xpt;
        }
        dtick += 1.0;
        prevTick = tick;
        tick = dtick * axisTicks_;
    }
}

void LinearScale::drawBottomDateAxis(wxDC& dc, PixelWorld& px)
{
    wxString label;
    wxCoord txw, txh; // retrieve Text extents
    double dtick;
    wxDateTime startDate(dataMin_ + 2400000.5);
    auto year = startDate.GetYear();
    modf(year/axisTicks_,&dtick);
    auto tick = dtick * axisTicks_; // in years
    auto yeardate = wxDateTime(1,wxDateTime::Month::Jan, tick);
    auto datetick = yeardate.GetModifiedJulianDayNumber();
    auto axispos = px.yspan_ + px.top_;

    while (datetick <= dataMax_)
    {
        auto xpt = (int)( (datetick-offset_)* scale_ ) + px.left_;
        if (xpt >= px.left_)
        {
            dc.DrawLine(xpt, axispos, xpt, axispos+10);
            // into text
            label.Printf(wxT("%d"),yeardate.GetYear());
            dc.GetTextExtent(label, &txw, &txh);
            dc.DrawText(label, xpt - txw/2, axispos + txh + 10);
        }
        dtick += 1.0;
        tick = dtick * axisTicks_;
        yeardate = wxDateTime(1, wxDateTime::Month::Jan, tick);
        datetick = yeardate.GetModifiedJulianDayNumber();
    }
}

void LinearScale::drawLeftYAxis(wxDC& dc, PixelWorld& px)
{
    double firstTick = dataMin_;
    double dtick;
    modf(firstTick/axisTicks_,&dtick);
    if (dtick == -0.0)
        dtick = 0.0;
    wxString label; // generate numeric strings

	wxCoord txw, txh; // retrieve Text extents
	wxCoord axispos = px.left_;
	wxCoord axisLimit = px.top_ + px.yspan_;
    int tempy;

    //wxCoord maxGridTickWidth = 0;
	// left axis
    double prevTick = (dtick-1.0)* axisTicks_;
    auto prevYPt = (int)( (prevTick-offset_)* scale_ ) + px.top_;
    double tick = dtick * axisTicks_;

    wxPen gridPen(px.gridColor_);

    // draw reference line ??
            // draw yaxisDiv_ small ticks between prevYPt and ypt
    int bigTickLeft, bigTickRight;
    int tickLeft, tickRight;

    if (insideTick_)
    {
        bigTickLeft = axispos;
        bigTickRight = axispos + 10;
        tickLeft = axispos;
        tickRight = axispos + 5;
    }
    else {
        bigTickLeft = axispos - 10;
        bigTickRight = axispos;
        tickLeft = axispos - 5;
        tickRight = axispos;
    }
    int otherSide = axispos+px.xspan_;
    int angle = degreesRotation(tickRotate_);
    int minLeftBound = axispos;
    int textx = 0;

    if (bigTickLeft < minLeftBound)
        minLeftBound = bigTickLeft;

    while (prevTick < dataMax_)
    {
        auto ypt = (int)( (tick - offset_)* scale_ ) + px.top_;

		if ((ypt <= axisLimit) && (ypt >= px.top_))
		{
			dc.DrawLine(bigTickLeft, ypt, bigTickRight, ypt);

            if (showMajorGrid_ && (ypt < axisLimit) && (ypt > px.top_))
            {
                auto savePen = dc.GetPen();
                dc.SetPen(gridPen);

                // draw grid line
                dc.DrawLine(bigTickRight, ypt, otherSide, ypt);
                dc.SetPen(savePen);
            }
			// into text
			label.Printf(wxT("%g"),tick);
			dc.GetTextExtent(label, &txw, &txh);
            /* if (txw > maxGridTickWidth)
                maxGridTickWidth = txw; */

            if (tickRotate_ == TextRotate::R_0)
            {
                textx = insideTick_ ? bigTickRight + 3 : bigTickLeft - txw - 10;
                dc.DrawText(label, textx, ypt - txh/2);

			}
			else if (tickRotate_ == TextRotate::CW_90)
			{
                textx = insideTick_ ? bigTickRight + txh + 2 : bigTickLeft - txh - 2;
                dc.DrawRotatedText(label, textx, ypt - txw/2, angle);
			}
			else if (tickRotate_ == TextRotate::ACW_90)
			{
                textx = insideTick_ ? bigTickRight + 2 : bigTickLeft - txh - 2;
                dc.DrawRotatedText(label, textx, ypt + txw/2, angle);
			}
            if (textx < minLeftBound)
                    minLeftBound = textx;
		}
        if (axisDiv_ > 0)
        {
            for(uint ix = 1; ix < axisDiv_; ix++)
            {
                tempy = prevYPt - ((prevYPt-ypt)*(int)ix)/ (int)axisDiv_;
                if ((tempy <= axisLimit) && (tempy > px.top_))
                {
                    dc.DrawLine(tickLeft,tempy, tickRight, tempy );
                    if (showMinorTicks_)
                    {

                        auto savePen = dc.GetPen();
                        dc.SetPen(gridPen);
                        dc.DrawLine(tickRight,tempy, otherSide, tempy );
                        dc.SetPen(savePen);

                    }
                }
            }
        }
        prevTick = tick;
        prevYPt = ypt;

		dtick += 1.0;
		tick = dtick * axisTicks_;
    }
    px.leftAxisBound_ = axispos - minLeftBound; // save it?
    if (this->refLine_)
    {
        auto ypt = (int)( (this->refValue_ - offset_)* scale_ ) + px.top_;
        if (ypt <= axisLimit)
        {
            wxPen refPen(px.refColor_);
            auto savePen = dc.GetPen();
            dc.SetPen(refPen);

            dc.DrawLine(axispos, ypt, otherSide, ypt);
            dc.SetPen(savePen);
        }
    }
}

void LinearScale::renderDC(wxDC& dc, PixelWorld& px)
{
    // y axis
    if (coord_ == YCOORD)
    {
        drawLeftYAxis(dc,px);
    }
    else if (coord_ == XCOORD)
    {
        if ((units_==DATE_JULIAN_MOD) || (units_ == DATE_YEAR_MONTH) )
        {
            drawBottomDateAxis(dc,px);
        }
        else {
            drawBottomXAxis(dc,px);
        }
    }
}

void LinearScale::AutoAxis(long pspan, double a1, double a2, double &tics, uint &labels)
{
	double		pTen,iTen;
	double		range,nRange;
	int			i;
	int			pAdjust;
	int			pIndex;
	double		dmajor,dminor;

	range = fabs(a1 - a2);
	pTen = log10(range);
	pTen = modf(pTen,&iTen);
	if( pTen < 0.0)	{
		pAdjust = 2-(int)iTen;
	}else {
		pAdjust = 1-(int)iTen;
	}
	nRange = range * pow((double)10.0,(double)pAdjust);
	// normalise to between 10  and 100

	dminor = (10*nRange)/pspan;
	// aim for at least 10 pixels per minor tic
	dmajor = (range/2);
	// try to fit 2 major ticks in the range
	pIndex = kTestTickSize-1;
	for( i = 0; i < kTestTickSize; i++)
	{
		if( dminor <= sRealTick[i] )
		{
			pIndex = i;
			break;
		}
	}
	labels = sRealLabels[pIndex];
	tics = sRealTick[pIndex] * pow(10.0,-pAdjust) * labels;
	if( (tics > dmajor))
	{
		if( pIndex > 0)
		{
			pIndex -= 1;
			labels = sRealLabels[pIndex];
			tics = sRealTick[pIndex] * pow(10.0,-pAdjust) * labels;
			if( tics > dmajor )
			{
				tics /= labels;
				labels = 1;
			}
		}
	}
}


void LinearScale::ReadJSON(const Json::Value& json)
{
    std::string tempStr;
    int itemp;
    bool tbool;

    if (readInt(json,"direction",itemp))
        direct_ = ScaleFlow(itemp);

    if (readString(json, "coord", tempStr))
    {
        if (tempStr == "X")
        {
            coord_ = ScaleCoord::XCOORD;
        }
        else if (tempStr == "Y")
        {
            coord_ = ScaleCoord::YCOORD;
        }
   }
    if (json.isMember("unit"))
    {
        auto temp = json["unit"].asString();
        this->units_ = toSeriesUnit(temp);
    }
    if (readString(json,"label",tempStr))
        label_ = tempStr;
    readBool(json,"showGrid", showMajorGrid_);
    readBool(json,"smallTicks", showMinorTicks_);
    readDouble(json, "dataMax", dataMax_);
    readDouble(json, "dataMin", dataMin_);
     readBool(json,"autoLimits", autoLimits_);
    readBool(json,"autoTicks", autoTicks_);
    readBool(json,"fixSize", doFixedSize_);
    readUInt(json, "pixels", fixedSize_);
    readDouble(json, "majorTick", axisTicks_);
    readUInt(json, "smallTick", axisDiv_);
    readDouble(json, "refLine", refValue_);
    readBool(json,"showRef", refLine_);

    if (readInt(json,"tickRotate", itemp))
        tickRotate_ = TextRotate(itemp);
   if (readBool(json,"tickInside", tbool))
        insideTick_ = TextRotate(tbool);
   if (readBool(json,"labelInside", tbool))
        insideLabel_ = TextRotate(tbool);

}


void LinearScale::SaveJSON(Json::Value& json)
{
    json["direction"] = (int) direct_;
    json["coord"] = (coord_ == ScaleCoord::XCOORD) ? "X" : "Y";
    json["unit"] = unitAsString(units_);
    json["showGrid"] = showMajorGrid_;
    json["smallTicks"] = showMinorTicks_;
    json["dataMax"] = dataMax_;
    json["dataMin"] = dataMin_;
    json["autoTicks"] = autoTicks_;
    json["autoLimits"] = autoLimits_;

    json["fixSize"] = doFixedSize_;
    json["pixels"] = fixedSize_;
    json["majorTick"] = axisTicks_;
    json["smallTick"] = axisDiv_;
    json["refLine"] = refValue_;
    json["showRef"] = refLine_;
    json["label"] = label_;
    json["tickRotate"] = tickRotate_;
    json["tickInside"] = insideTick_;
    json["labelInside"] = insideLabel_;

}

void PixelWorld::ReadJSON(const Json::Value& json)
{
    readColor(json,"back-color",backColor_);
    readColor(json,"axis-color",axisColor_);
    readColor(json,"ref-color",refColor_);
    readColor(json,"grid-color",gridColor_);
    readBool(json,"left-frame", leftFrame_);
    readBool(json,"top-frame", topFrame_);
    readBool(json,"right-frame", rightFrame_);
    readBool(json,"bottom-frame", bottomFrame_);

    if (json.isMember("margins"))
    {
        const Json::Value& margins = json["margins"];
        left_ = margins[0].asInt();
        top_ = margins[1].asInt();
        right_ = margins[2].asInt();
        bottom_ = margins[3].asInt();
    }
    if (json.isMember("fixed-size"))
    {
         const Json::Value& fixed = json["fixed-size"];
         xfixed_ = fixed[0].asInt();
         yfixed_ = fixed[1].asInt();
    }
    if (json.isMember("var-size"))
    {
         const Json::Value& varsize = json["var-size"];
         xspan_ = varsize[0].asInt();
         yspan_ = varsize[1].asInt();
    }
    readBool(json,"is-fixed-size", fixed_);

    if (json.isMember("xaxis"))
    {
        const Json::Value& axis = json["xaxis"];
        xScale_.ReadJSON(axis);
    }
    if (json.isMember("yaxis"))
    {
        const Json::Value& axis = json["yaxis"];
        yScale_.ReadJSON(axis);
    }


}


void PixelWorld::SaveJSON(Json::Value& json)
{

    Json::Value margins(Json::arrayValue);

    margins[0] = left_;
    margins[1] = top_;
    margins[2] = right_;
    margins[3] = bottom_;

    json["margins"] = margins;

    Json::Value fixed(Json::arrayValue);

    fixed[0] = xfixed_; // dimensions of plot area if fixed
    fixed[1] = yfixed_;

    json["is-fixed-size"] = fixed_;
    json["fixed-size"] = fixed;

    Json::Value varsize(Json::arrayValue);
    varsize[0] = xspan_; // dimensions of plot area if resizeable
    varsize[1] = yspan_;

    json["var-size"] = varsize;

    // flags for axis border line
    json["left-frame"] = leftFrame_;
    json["top-frame"] = topFrame_;
    json["right-frame"] = rightFrame_;
    json["bottom-frame"] = bottomFrame_;

    Json::Value xsys (Json::objectValue);
    Json::Value ysys (Json::objectValue);


    xScale_.SaveJSON(xsys);
    yScale_.SaveJSON(ysys);

    json["xaxis"] = xsys;
    json["yaxis" ] = ysys;


    json["back-color"] = colorAsString(backColor_);
    json["axis-color"] = colorAsString(axisColor_);
    json["ref-color"] = colorAsString(refColor_);
    json["grid-color"] = colorAsString(gridColor_);

}

PixelWorld::PixelWorld() : left_(100), right_(50), top_(80), bottom_(80)
               , xspan_(500), yspan_(350), xScale_(ScaleCoord::XCOORD, ScaleFlow::POSITIVE), yScale_(ScaleCoord::YCOORD, ScaleFlow::NEGATIVE)
{

    backColor_.Set("white");
    axisColor_.Set("black");
    refColor_.Set("black");
    gridColor_.Set(0xFF,0xD7,0xE1);
    leftFrame_ = true;
    topFrame_ = true;
    rightFrame_ = true;
    bottomFrame_ = true;
    fixed_ = false;
    xfixed_ = 500;
    yfixed_ = 350;

}


void PixelWorld::autoXDataLimits(std::vector<PlotPtr>& layers, double& xMin, double &xMax)
{
    bool xvalid = false;
    auto it = layers.begin();
    auto fin = layers.end();

    for( ; it != fin; it++)
	{
		DataLayer* layer = dynamic_cast<DataLayer*>(it->get());
		if (layer == nullptr)
            continue;
        if (!xvalid)
        {
			if (!layer->xstats_.valid_)
			{
				layer->calcStats();
			}
            if (layer->xstats_.valid_)
            {
                xvalid = (layer->xstats_.n_ > 0);
                if (xvalid)
                {
                    xMax = layer->xstats_.maxval_;
                    xMin = layer->xstats_.minval_;
                }
            }
        }
        else {
             if (layer->xstats_.valid_ && (layer->xstats_.n_ > 0))
             {
                if(xMax < layer->xstats_.maxval_)
                    xMax = layer->xstats_.maxval_;

                if (xMin > layer->xstats_.minval_)
                    xMin = layer->xstats_.minval_;
             }
        }
    }
}

void PixelWorld::autoYDataLimits(std::vector<PlotPtr>& layers, double& yMin, double &yMax)
{
    bool yvalid = false;

    auto it = layers.begin();
    auto fin = layers.end();

    for( ; it != fin; it++)
	{
        DataLayer* layer = dynamic_cast<DataLayer*>(it->get());
        if (layer == nullptr)
            continue;
        if (!yvalid)
        {
            if (layer->ystats_.valid_)
            {
                yvalid = (layer->ystats_.n_ > 0);
                if (yvalid)
                {
                    yMax = layer->ystats_.maxval_;
                    yMin = layer->ystats_.minval_;
                }
            }
        }
        else {
             if (layer->ystats_.valid_ && (layer->ystats_.n_ > 0))
             {
                if(yMax < layer->ystats_.maxval_)
                    yMax = layer->ystats_.maxval_;

                if (yMin > layer->ystats_.minval_)
                    yMin = layer->ystats_.minval_;
             }
        }
        if (layer->errorbar_ != nullptr)
        {
            double maxError = layer->errorbar_->dataMax();
            yMin -= maxError;
            yMax += maxError;
        }
    }

}


void PixelWorld::calcScales(std::vector<PlotPtr>& layers)
{


    if (this->xScale_.autoLimits_)
    {
        double xMax = 0.0;
        double xMin = 0.0;
        autoXDataLimits(layers, xMin, xMax);
        xScale_.setDataLimits(xMin, xMax);
        double widen = 0.02;

        if (xScale_.insideLabel_)
            widen += 0.01;
        if (xScale_.insideTick_)
            widen += 0.01;
        xScale_.widenLimits(widen);
        xScale_.calcScale(this->xspan_);
    }
    else {
        xScale_.calcScale(this->xspan_);
    }

    if (this->yScale_.autoLimits_)
    {
        double yMax = 0.0;
        double yMin = 0.0;
        autoYDataLimits(layers, yMin, yMax);
        yScale_.setDataLimits(yMin, yMax);
        double widen = 0.02;

        if (yScale_.insideLabel_)
            widen += 0.01;
        if (yScale_.insideTick_)
            widen += 0.01;

        yScale_.widenLimits(widen);
        yScale_.calcScale(this->yspan_);
    }
    else {
        yScale_.calcScale(this->yspan_);
    }
}



