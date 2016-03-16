#include "plotxy.h"
#include "helper.h"
#include <string>
#include "lua_series.h"

using namespace agw;

static PlotShape rand_shape()
{
    return PlotShape( (rand() % int(PlotShape::SQUARE)) + 1);
}
static int rand_shade()
{
    return ((rand() % 250) + 2);
}

static wxPenStyle rand_line()
{
    int ix = (rand() % 3);

    switch(ix)
    {
    case 1:
        return wxPENSTYLE_DOT;
    case 2:
        return wxPENSTYLE_DOT_DASH;
    case 0:
    default:
        return wxPENSTYLE_SOLID;

    }
}

void DataLayer::randomStyle()
{
    wxColor match(rand_shade(), rand_shade(), rand_shade());

    symbolBorder_.Set(0,0,0);

    symbolFill_ = match;
    lineColor_ = match;

    symbolShape_ = rand_shape();
    symbolSize_ = 2;

    lineStyle_ = penStyleToIndex(rand_line());
    lineWidth_ = 1;
}
DataLayer::DataLayer() : PlotLayer(), xdata_(nullptr), ydata_(nullptr), errorbar_(nullptr)
{
    jype_ = "data_layer";

    randomStyle();

}

DataLayer::~DataLayer()
{
    /* if (xdata_ != nullptr)
        delete xdata_;
    if (ydata_ != nullptr)
        delete ydata_;
    if (errorbar_ != nullptr)
        delete errorbar_;
        */
}

// argument 1 - DataLayer ( PlotPtr )
// argument 2 -  Lua Table
// Convert table to json string, parse json, and then pass to PlotPtr ?
// how about iterate table, and go direct to a Json::Value tree. No recursion yet.
int DataLayer::setDisplay(lua_State* L)
{
    PlotPtr* mem = DataLayer::checkLayer(L,1);
    luaL_checktype(L, 2, LUA_TTABLE);
    lua_pushnil(L);
    Json::Value root(Json::objectValue);

    while (lua_next(L, -2))
    {
        /* uses 'key' (at index -2) and 'value' (at index -1) */
        const char* key = luaL_checkstring(L,-2);
        int ltype = lua_type(L,-1);
        // TODO this could be done at a lower level with varient types, Index2Addr, on lua object
        switch(ltype)
        {
        case LUA_TSTRING:
            root[key] = lua_tostring(L,-1);
            break;
        case LUA_TNUMBER:
            if (lua_isinteger(L,-1))
                root[key] = lua_tointeger(L,-1);
            else
                root[key] = lua_tonumber(L,-1);
            break;
        default:
            break;

        }
        lua_pop(L,1);
    }
    lua_pop(L,1); //pop key

    (*mem)->ReadJSON(root);
    return 0;
}


void DataLayer::SaveJSON(Json::Value& json)
{
    // save to allow for child classes, x
    //     Series* xdata_; Series* ydata_;Series* errorbar_;wxColour plotColor_;
    json["jype"] = this->get_jype();
    if (xdata_)
    {
        Json::Value xseries(Json::objectValue);

        xdata_->SaveJSON(xseries);
        json["xdata"] = xseries;
    }
    if (ydata_)
    {
        Json::Value yseries(Json::objectValue);

        ydata_->SaveJSON(yseries);
        json["ydata"] = yseries;
    }
    if (errorbar_)
    {
        Json::Value errorbar(Json::objectValue);

        errorbar_->SaveJSON(errorbar);
        json["errorbar"] = errorbar;
    }

    if (label_.size() == 0)
    {
        label_ = "untitled";
    }
    json["label"] = label_;
    json["plotcolor"] = colorAsString(symbolBorder_);
    json["plotfill"] = colorAsString(symbolFill_);
    json["plotsize"] = symbolSize_;
    json["plotshape"] = shapeToString(symbolShape_);

    json["linecolor"] = colorAsString(lineColor_);
    json["linesize"] = lineWidth_;
    json["linestyle"] = indexToPenName(lineStyle_);

}

void DataLayer::ReadJSON(const Json::Value& json)
{
    if (json.isMember("xdata"))
    {
        const Json::Value& xd = json["xdata"];
        xdata_ = readJSONSeries(xd);

    }
    if (json.isMember("ydata"))
    {
        const Json::Value& xd = json["ydata"];
        ydata_ = readJSONSeries(xd);
    }
    if (json.isMember("error-bar"))
    {
        const Json::Value& xd = json["error-bar"];
        errorbar_ = readJSONSeries(xd);
    }

    readColor(json,"plotcolor",symbolBorder_);
    readColor(json,"plotfill",symbolFill_);
    readColor(json,"linecolor",lineColor_);

    readInt(json,"plotsize",symbolSize_);
    readInt(json,"linesize",lineWidth_);
    readString(json,"label",label_);
    std::string tempstr;
    if (readString(json, "plotshape", tempstr))
    {
        symbolShape_ = strToPlotShape(tempstr);
    }
    if (readString(json,"linestyle",tempstr))
    {
        lineStyle_ = penNameToIndex(tempstr);
    }
}

std::string  LineFit::resultAsText()
{
    wxString text;
    text.Printf("Slope = %5.2g, R-val = %5.2g", slope_, rval_);
    return text.ToStdString();
}

static void* UserDataMatch(lua_State *L, int index, const char* const tablename[], const char** match)
{
    void *ud = lua_touserdata(L, index);

    if (ud != nullptr && lua_getmetatable(L, index))
    {
        auto mtkey = tablename;
        while (*mtkey)
        {
            luaL_getmetatable(L, *mtkey);  /* get correct metatable */
            if (lua_rawequal(L, -1, -2))  /* the same? */
            {
                *match = *mtkey;
                lua_pop(L,1);
                break;
            }
            mtkey++;
            lua_pop(L,1);
        }
        lua_pop(L,1);
        if (*mtkey == nullptr)
        {
            ud = nullptr;
            *match = nullptr;
        }
    }
    return ud;
}

PlotPtr*
DataLayer::checkLayer (lua_State *L, int index)
{

    static const char* const tables[] = {DL_LUA, LineFit::LF_LUA,nullptr};

  //luaL_checktype(L, index, LUA_TUSERDATA);
  // problem - DataLayer and LineFit are different userdata types but both DataLayer -
    const char* match;
    void *ud = UserDataMatch(L,index, tables, &match);
    luaL_argcheck(L, ud != nullptr, 1, "'plotxy' expected");
    return (PlotPtr *)ud;
}

PlotPtr*
LineFit::checkLineFit (lua_State *L, int index)
{
  luaL_checktype(L, index, LUA_TUSERDATA);
  void *ud = luaL_checkudata(L, index, LF_LUA);
  luaL_argcheck(L, ud != nullptr, 1, "'linefit' expected");
  return (PlotPtr *)ud;
}

PlotPtr*
DataLayer::toLayer(lua_State* L, int index)
{
    PlotPtr* p = (PlotPtr*) lua_touserdata(L,index);
    if (p == nullptr)
        Series_typerror(L,index,DL_LUA);
    return p;
}

const char* DataLayer::DL_LUA = "plotxy";
const char* LineFit::LF_LUA = "linefit";

static LineFit* PlotPtrLineFit(lua_State* L)
{
    PlotPtr* pp = LineFit::checkLayer(L,1);
    PlotLayer* p = (*pp).get();
    if (typeid(*p) != typeid(LineFit))
    {
        const char *msg = lua_pushstring(L, "dynamic_cast to LineFit failed");
        luaL_argerror(L, 1, msg);
    }
    return static_cast<LineFit*>(p);
}

PlotPtr* LineFit::pushLineFit(lua_State* L)
{
    size_t nbytes = sizeof(PlotPtr);
    void *a = lua_newuserdata(L, nbytes);

    PlotPtr* mem = new (a) PlotPtr();  // new blank shared pointer placement syntax=
    luaL_setmetatable(L, LF_LUA);

    return mem;
}

int LineFit::getRVal(lua_State* L)
{
    LineFit* fit = PlotPtrLineFit(L);
    lua_pushnumber(L, fit->rval_);
    return 1;
}

int LineFit::getSlope(lua_State* L)
{
    LineFit* fit = PlotPtrLineFit(L);
    lua_pushnumber(L, fit->slope_);
    return 1;
}
int LineFit::getYOffset(lua_State* L)
{
    LineFit* fit = PlotPtrLineFit(L);
    lua_pushnumber(L, fit->yintercept_);
    return 1;
}

PlotPtr* DataLayer::pushLayer(lua_State* L)
{
    size_t nbytes = sizeof(PlotPtr);
    void *a = lua_newuserdata(L, nbytes);

    PlotPtr* mem = new (a) PlotPtr();  // new blank shared pointer placement syntax=
    luaL_setmetatable(L, DL_LUA);

    return mem;
}

static DataLayer* PlotPtrDataLayer(lua_State* L)
{
    PlotPtr* pp = DataLayer::checkLayer(L,1);
    DataLayer* dlay = dynamic_cast<DataLayer*>((*pp).get());
    if (dlay==nullptr)
    {
        const char *msg = lua_pushstring(L, "dynamic_cast to DataLayer failed");
        luaL_argerror(L, 1, msg);
    }
    return dlay;
}

int DataLayer::setLegend(lua_State* L)
{
    DataLayer* dlay = PlotPtrDataLayer(L);
    const char* text = luaL_checkstring(L,2);
    dlay->legendText_ = text;
    return 0;
}

int DataLayer::getXData(lua_State* L)
{
    DataLayer* dlay = PlotPtrDataLayer(L);
    SeriesPtr* result = SeriesRay::pushSeries(L);
    (*result) = dlay->xdata_;
    return 1;
}

int DataLayer::getYData(lua_State* L)
{
    DataLayer* dlay = PlotPtrDataLayer(L);
    SeriesPtr* result = SeriesRay::pushSeries(L);
    (*result) = dlay->ydata_;
    return 1;
}
