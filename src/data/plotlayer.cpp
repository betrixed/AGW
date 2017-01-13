#include "plotxy.h"
#include "helper.h"
#include "plotlua.h"
// file read and write code

using namespace agw;


void BoxLayer::SaveJSON(Json::Value& json)
{
    json["jype"] = this->get_jype();
    json["attach"] = int(place_);
    Json::Value offset(Json::arrayValue);
    offset[0] = offset_.x;
    offset[1] = offset_.y;
    json["offset"] = offset;
}

void BoxLayer::ReadJSON(const Json::Value& json)
{
    if (json.isMember("attach"))
        place_ = PlotPlace(json["attach"].asInt());
    if (json.isMember("offset"))
    {
        const Json::Value& offset = json["offset"];
        offset_.x = offset[0].asInt();
        offset_.y = offset[1].asInt();
    }
}

wxPoint BoxLayer::getPlaceAdjust(const wxSize& dim)
{
    if (place_ < TLEFT_CORNER)
    {
        switch(place_)
        {
            case LEFT_BORDER:
                return wxPoint(-thePlot_->world_.leftAxisBound_,-dim.y/2);
            case TOP_BORDER:
                return wxPoint(-dim.x/2,0);
            case BOTTOM_BORDER:
                return wxPoint(-dim.x/2,0);
            case RIGHT_BORDER:
                return wxPoint(0,-dim.y/2);
            default:
                return wxPoint(0,0);
        }
    }
    return wxPoint(0,0);
}

void LineFit::SaveJSON(Json::Value& json)
{
    DataLayer::SaveJSON(json);
    json["jype"] = this->get_jype();

    Json::Value fit(Json::objectValue);
    fit["r-value"] = rval_;
    fit["slope"] = slope_;
    fit["intercept"] = yintercept_;
    json["fit"] = fit;
    json["legend"] = legendText_;
}

void LineFit::ReadJSON(const Json::Value& json)
{
    DataLayer::ReadJSON(json);
    if (json.isMember("fit"))
    {
        const Json::Value& fit  = json["fit"];

        rval_ = fit["r-value"].asDouble();
        slope_ = fit["slope"].asDouble();
        yintercept_ = fit["intercept"].asDouble();
    }
    if (json.isMember("legend"))
    {
        legendText_ = json["legend"].asString();
    }

}

void LabelWorld::SaveJSON(Json::Value& json)
{
    auto it = labels_.begin();
    auto fin = labels_.end();

    Json::Value labels(Json::arrayValue);

    for( ; it != fin; it++)
    {
        BoxLayer* b = static_cast<BoxLayer*>(it->get());

        Json::Value theLabel(Json::objectValue);
        b->SaveJSON(theLabel);
        labels.append(theLabel);
    }
    json["labels"] = labels;
}

void LabelWorld::ReadJSON(const Json::Value& json)
{
    labels_.clear();

    uint ct = json.size();

    const wxPoint defaultOffset(0,0);

    for(uint i = 0; i < ct; i++)
    {
        const Json::Value& theLabel = json[i];
        std::string jype;
        if (readString(theLabel,"jype", jype))
        {
            if (jype == "textbox")
            {

                PlotLayer_sptr p = std::make_shared<TextLayer>(PlotPlace::TLEFT_CORNER,defaultOffset);
                p.get()->ReadJSON(theLabel);
                labels_.push_back(p);
            }
         }
    }
}



LegendLayer* LabelWorld::getLegend()
{
   PlotLayer* pl = legend_.get();
   if (pl == nullptr)
        return nullptr;
   return dynamic_cast<LegendLayer*>(pl);
}
void LabelWorld::removeLegend()
{
    legend_ = nullptr;
}

void LabelWorld::setLegend(PlotLayer_sptr pp)
{
    legend_ = pp;
}

void LabelWorld::addLegend()
{
    legend_ = std::make_shared<LegendLayer>(TLEFT_CORNER, wxPoint(10,10));
}

void LabelWorld::title(const std::string& s)
{
    PlotLayer_sptr p = getLabelKind(LabelKind::TITLE);
    bool isNew = (p == nullptr);
    if (isNew)
    {
        p = std::make_shared<TextLayer>(PlotPlace::TOP_BORDER,wxPoint(0,-30));
        labels_.push_back(p);
    }
    TextLayer* t = static_cast<TextLayer*>(p.get());
    if (isNew)
        t->kind(LabelKind::TITLE);
    t->value(s);

}
void LabelWorld::xlabel(const std::string& s)
{

    PlotLayer_sptr p = getLabelKind(LabelKind::XLABEL);
    bool isNew = (p == nullptr);
    if (isNew)
    {
        p = std::make_shared<TextLayer>(PlotPlace::BOTTOM_BORDER,wxPoint(0,40));
        labels_.push_back(p);
    }
    TextLayer* t = static_cast<TextLayer*>(p.get());
    if (isNew)
    {
        t->kind(LabelKind::XLABEL);
    }
    t->value(s);

}
void LabelWorld::ylabel(const std::string& s)
{
    PlotLayer_sptr p = getLabelKind(LabelKind::YLABEL);
    bool isNew = (p == nullptr);
    if (isNew)
    {
        p = std::make_shared<TextLayer>(PlotPlace::LEFT_BORDER,wxPoint(-50,0));
        labels_.push_back(p);
    }
    TextLayer* t = static_cast<TextLayer*>(p.get());
    if (isNew) {
        t->kind(LabelKind::YLABEL);
        t->rotate(TextRotate::ACW_90);
    }
    t->value(s);

}
PlotLayer_sptr
LabelWorld::getLabelKind(LabelKind k)
{
    auto it = labels_.begin();
    auto fin = labels_.end();
    for( ; it != fin; it++)
    {
        TextLayer* b = dynamic_cast<TextLayer*>(it->get());
        if (b && (b->kind() == k))
            return (*it);
    }
    return nullptr;

}

