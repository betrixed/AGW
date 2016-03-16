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
#include "json/json.h"
#include "plotlua.h"

using namespace agw;

int agw::degreesRotation(TextRotate r)
{
    return (r == TextRotate::R_0) ? 0
                : (r == TextRotate::CW_90) ? -90
                : (r == TextRotate::ACW_90) ? 90
                : 180;

}

TextLayer::TextLayer(PlotPlace pp, const wxPoint& offset): BoxLayer(), kind_(LabelKind::NORMAL), align_(LEFT), rotate_(R_0)
    {
        place_ = pp;
        offset_ = offset;
        jype_ = "textbox";
    }

void TextLayer::setLines(const std::vector<std::string>& lines)
{
    lines_ = lines;
}

void TextLayer::value(const std::string& s)
{
    lines_.clear();
    //TODO: parse line breaks
    std::string temp = s;

    size_t pos = temp.find('\n');
    while(pos != std::string::npos)
    {
        lines_.push_back(temp.substr(0,pos));
        pos += 1;
        if (pos < temp.size())
        {
            temp = temp.substr(pos);
            pos = temp.find('\n');
        }
        else
            break;
    }
    if (temp.size() > 0)
        lines_.push_back(temp);
}

const std::vector<std::string>& TextLayer::getLines()
{
    return lines_;
}

void TextLayer::SaveJSON(Json::Value& json)
{
    BoxLayer::SaveJSON(json);

    if (lines_.size() > 0)
    {
        Json::Value text(Json::arrayValue);
        for(Json::ArrayIndex ix = 0; ix < lines_.size(); ix++)
            text[ix] = lines_[ix];
        json["text"] = text;
    }
    json["align"] = align_;
    json["rotate"] = rotate_;
    if (fontFace_.size() > 0)
    {
        Json::Value font(Json::objectValue);
        font["name"] = fontFace_;
        font["size"] = fontPoints_;
        json["font"] = font;
    }

}

void TextLayer::ReadJSON(const Json::Value& json)
{
    BoxLayer::ReadJSON(json);
    int itemp;

    if (json.isMember("text"))
    {
        const Json::Value& text = json["text"];
        uint ct = text.size();
        if (ct > 0)
        {
            lines_.resize(ct);
            for(uint ix = 0; ix < ct; ix++)
                lines_[ix] = text[ix].asString();
        }
    }
    if (json.isMember("font"))
    {
        const Json::Value& font = json["font"];
        fontFace_ = font["name"].asString();
        fontPoints_ = font["size"].asInt();
    }
    if (readInt(json,"align", itemp))
        align_ = TextAlign(itemp);
    if (readInt(json,"rotate", itemp))
        rotate_ = TextRotate(itemp);

}
/*
static void dumpBox(const wxRect& box)
{
    wxLogMessage("Box is p(%d,%d), d(%d,%d)", box.x, box.y, box.width, box.height);
}
*/
void TextLayer::renderDC(wxDC& dc, PixelWorld& px)
{
	ignore(px);

    int maxWide = 0;
    int maxHigh = 0;
    wxRect box(wxPoint(0,0),wxSize(0,0));
    wxFont  savedFont;

    bool needsFont = (fontFace_.size() > 0);

    if (needsFont)
    {
        savedFont = dc.GetFont();
        wxFont theFont(wxFontInfo(fontPoints_).FaceName(fontFace_));
        dc.SetFont(theFont);
    }


    for (uint ix = 0; ix < lines_.size(); ix++)
    {
        wxCoord w, h, descent, leading;
        wxString theText = lines_[ix];
        dc.GetTextExtent(theText, &w, &h, &descent, &leading, nullptr);
        wxCoord sumh = h + descent + leading;
        maxHigh += sumh;
        if (maxWide < w)
            maxWide = w;
    }


    if ((rotate_ == TextRotate::R_0) || (rotate_ == TextRotate::R_180))
    {


        int totalHeight = maxHigh + 2 * border;
        int totalWidth = maxWide + 2 * border;

        box.SetSize(wxSize(totalWidth, totalHeight));

        // now get the position of the place, and offset from there
        wxSize dim = box.GetSize();
        wxPoint place = thePlot_->getPlotPlace(place_) + this->getPlaceAdjust(dim);
        wxPoint boffset = place + offset_; // gets real top left

        box.Offset(boffset);
        this->bounds_ = box;
        //dc.DrawRectangle(box);

    }
    else if ((rotate_ == TextRotate::ACW_90)||(rotate_ == TextRotate::CW_90))
    {
        // switch height and width here!
        int totalHeight = maxWide + 2 * border;
        int totalWidth = maxHigh + 2 * border;

        box.SetSize(wxSize(totalWidth, totalHeight));
        if (rotate_ == ACW_90)
        {
            //box.Offset(0, totalHeight);
        }
        else {
            //box.Offset( totalWidth,0);
        }
        // now get the position of the place, and offset from there
        wxSize dim = box.GetSize();
        wxPoint place = thePlot_->getPlotPlace(place_) + this->getPlaceAdjust(dim);
        wxPoint boffset = place + offset_; // gets real top left

        box.Offset(boffset);
        this->bounds_ = box;
        //dc.DrawRectangle(box);
    }
    //dumpBox(box);
    drawInBox(dc, box);

    if (needsFont)
        dc.SetFont(savedFont);
}

wxBitmap* TextLayer::makeImage()
{
    wxBitmap* bits = new wxBitmap(bounds_.width, bounds_.height);

    wxMemoryDC dc;

    dc.SelectObject(*bits);

    dc.Clear();
    wxRect box(0,0,bounds_.width, bounds_.height);

    wxFont  savedFont;

    bool needsFont = (fontFace_.size() > 0);

    if (needsFont)
    {
        savedFont = dc.GetFont();
        wxFont theFont(wxFontInfo(fontPoints_).FaceName(fontFace_));
        dc.SetFont(theFont);
    }
    else
        dc.SetFont(thePlot_->getFont());
    //dumpBox(box);
    drawInBox(dc, box);

    if (needsFont)
    {
        dc.SetFont(savedFont);
    }
    return bits;
}

void TextLayer::drawInBox(wxDC& dc,const wxRect& box)
{

    if ((rotate_ == TextRotate::R_0) || (rotate_ == TextRotate::R_180))
    {
        wxCoord xpt = box.x;
        wxCoord ypt = box.y;
        for(uint ix = 0; ix < lines_.size(); ix++)
        {
            wxCoord w, h, descent, leading;
            wxString theText = lines_[ix];
            dc.GetTextExtent(theText, &w, &h, &descent, &leading, nullptr);
            // perform a centering?

            int hinset = (box.width - w)/2;
            dc.DrawText(theText, xpt + hinset, ypt);
            ypt += h + descent + leading;
        }
    }
    else if ((rotate_ == TextRotate::ACW_90)||(rotate_ == TextRotate::CW_90))
    {
        wxCoord xpt;
        wxCoord ypt;

        if (rotate_ == TextRotate::CW_90)
        {   //draw from top-right, and step left
            xpt = box.x + box.width - border;
            ypt = box.y + border;
        }
        else {
            // draw from bottom-left and step right
            xpt = box.x + border;
            ypt = box.y + box.height - border;
        }
        int angle = degreesRotation(rotate_);
        for(uint ix = 0; ix < lines_.size(); ix++)
        {
            wxCoord w, h, descent, leading;
            wxString theText = lines_[ix];
            dc.GetTextExtent(theText, &w, &h, &descent, &leading, nullptr);

            int yinset = (box.height - w)/2;

            if (rotate_ == TextRotate::ACW_90)
                yinset = -yinset;

            dc.DrawRotatedText(theText, xpt , ypt + yinset, angle);

            int step = h + descent + leading;
            if (rotate_ == TextRotate::CW_90)
                xpt -= step;
            else
                xpt += step;
        }
    }

}

