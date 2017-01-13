#include "plotlua.h"

#define PLOT_LIB "plot"

#include "plotframe.h"
#include "plotlayer.h"
#include "appdata.h"
#include "helper.h"
#include "plotxy.h"
#include <fstream>
#include <wx/font.h>
#include <wx/settings.h>

using namespace agw;

PlotLua::PlotLua() : m_window(nullptr)
{
}

PlotLua::~PlotLua()
{
}

PlotLua_sptr PlotLua::create_sptr()
{
    PlotLua_sptr sptr = std::make_shared<PlotLua>();
    sptr->self_ = sptr;
    return sptr;
}
void PlotLua::writeStream(std::ostream& s)
{
    Json::Value root(Json::objectValue);

    saveJson(root);

    s << root;
}

wxPoint PlotLua::getPlotPlace(PlotPlace pp)
{
    switch(pp)
    {
    case LEFT_BORDER:
        return wxPoint(world_.left_, world_.top_ + world_.yspan_/2);
    case TOP_BORDER:
        return wxPoint(world_.left_ + world_.xspan_/2, world_.top_);
    case RIGHT_BORDER:
        return wxPoint(world_.left_ + world_.xspan_, world_.top_ + world_.yspan_/2 );
    case BOTTOM_BORDER:
        return wxPoint(world_.left_ + world_.xspan_/2, world_.top_ + world_.yspan_);
    case TLEFT_CORNER:
        return wxPoint(world_.left_, world_.top_);
    case TRIGHT_CORNER:
        return wxPoint(world_.left_ + world_.xspan_, world_.top_);
    case BLEFT_CORNER:
        return wxPoint(world_.left_, world_.top_ + world_.yspan_);
    case BRIGHT_CORNER:
        return wxPoint(world_.left_ + world_.xspan_, world_.top_ + world_.yspan_);
    case MIDDLE:
        return wxPoint(world_.left_ + world_.xspan_/2, world_.top_ + world_.yspan_/2 );
    default:
        return wxPoint(world_.left_, world_.top_);
    }
}

void PlotLua::saveJson(Json::Value& root)
{
    root["plot"] = plotTitle_;
    root["version"] = 1;

    Json::Value world(Json::objectValue);

    this->world_.SaveJSON(world);

    root["world"] = world;
    Json::Value layers(Json::arrayValue);


    auto it = layers_.begin();
    auto fin = layers_.end();

	for(int i = 0; it != fin; it++, i++)
	{

		PlotLayer* pp = it->get();

        Json::Value json(Json::objectValue);
        json["jype"] = pp->get_jype();

        pp->SaveJSON(json);
        layers[i] = json;
    }
    root["layers"] = layers;

    it = text_.labels_.begin();
    fin = text_.labels_.end();

    Json::Value labels(Json::arrayValue);
    for(int i = 0; it != fin; i++, it++)
    {
        PlotLayer* pp = it->get();
        Json::Value json(Json::objectValue);
        json["jype"] = pp->get_jype();

        pp->SaveJSON(json);
        labels[i] = json;
    }
    root["labels"] = labels;

    if (text_.legend_){
        Json::Value legend(Json::objectValue);
        text_.legend_->SaveJSON(legend);
        root["legend"] = legend;
    }
}
void PlotLua::readJson(const Json::Value& jplot)
{
    if (jplot.isMember("version"))
    {
        version_ = jplot["version"].asString();
    }
    if (jplot.isMember("plot"))
    {
        plotTitle_ = jplot["plot"].asString();
    }
    else {
        wxLogMessage("No plot name found");
        return;
    }
    if (jplot.isMember("labels"))
    {
        const Json::Value& labels = jplot["labels"];
        text_.ReadJSON(labels);
    }
    if (jplot.isMember("legend"))
    {
        const Json::Value& legend = jplot["legend"];

        wxPoint offset(0,0);
        int  place = PlotPlace::TLEFT_CORNER;

        readPoint(legend, "offset", offset);
        readInt(legend, "attach", place);

        auto box = std::make_shared<LegendLayer>(PlotPlace(place), offset);
        text_.setLegend(box);

    }
    if (jplot.isMember("layers"))
    {
        const Json::Value& layers = jplot["layers"];

        if (jplot.isMember("world"))
        {
            world_.ReadJSON(jplot["world"]);
        }
        for( uint i = 0; i < layers.size(); i++)
        {
            const Json::Value&  jlay = layers[i];
            if (jlay.isMember("jype"))
            {
                std::string layertype = jlay["jype"].asString();
                if (layertype == "data_layer")
                {
                    agw::PlotLayer_sptr dlay = std::make_shared<DataLayer>();

                    dlay->ReadJSON(jlay);
                    addLayer(dlay);
                }
                else if (layertype == "line_fit")
                {
                    PlotLayer_sptr lfit = std::make_shared<LineFit>();
                    lfit->ReadJSON(jlay);
                    addLayer(lfit);
                }
            }
        }
    }
}

void PlotLua::readFile(const std::string& path)
{
    std::ifstream fin(path);

    if (fin.is_open())
    {
        this->fileName_ = path;
        this->readStream(fin);
    }
    else
        wxLogMessage("Cannot open %s", path.c_str());
    fin.close();
}

void PlotLua::readStream(std::istream& s)
{

    std::unique_ptr<Json::Reader> rdr(new Json::Reader());
    Json::Value   jplot;
    try {
        if (!rdr->parse(s,jplot))
        {
            wxLogMessage("Json parse error");
            return;
        }
        readJson(jplot);
    }
    catch(std::exception& ex)
    {
        wxLogMessage("Exception %s" , ex.what());
    }
}


int PlotLua::setTitle(lua_State* L)
{
    PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
    if (mem)
    {
        std::string title = luaL_checkstring(L,2);
        (*mem)->title(title);
    }
    return 1;
}


int PlotLua::setXLabel(lua_State* L)
{
    PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
    if (mem)
    {
        std::string title = luaL_checkstring(L,2);
        (*mem)->xlabel(title);
    }
    return 1;
}


int PlotLua::setYLabel(lua_State* L)
{
    PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
    if (mem)
    {
        std::string title = luaL_checkstring(L,2);
        (*mem)->ylabel(title);
    }
    return 1;
}

void PlotLua::title(const std::string& s)
{
    text_.title(s);
}
void PlotLua::xlabel(const std::string& s)
{
    text_.xlabel(s);
}
void PlotLua::ylabel(const std::string& s)
{
    text_.ylabel(s);
}

BoxLayer* PlotLua::findLayer(const wxPoint& pos)
{

    PlotLayer* play = text_.legend_.get();
    BoxLayer* box = dynamic_cast<BoxLayer*>(play);


    if (box != nullptr && box->contains(pos))
        return box;

    auto it = text_.labels_.begin();
    auto fin = text_.labels_.end();

    for(; it != fin; it++)
    {
        play = it->get();
        box = dynamic_cast<BoxLayer*>(play);

        if (box != nullptr)
        {
            if (box->contains(pos))
                return box;
        }
    }
    return nullptr;
}

bool PlotLua::isDrawReady()
{
    auto a = layers_.begin();
    auto b = layers_.end();
    bool ready = false;
    while( b != a)
    {
        auto layer = dynamic_cast<DataLayer*>(a->get());
        if (layer && layer->isReady())
            ready = true;
        a++;
    }
    if (ready)
        world_.calcScales(layers_);
    return ready;
}

void PlotLua::removeLayer(PlotLayer_sptr& pp)
{
    auto it = std::find(layers_.begin(), layers_.end(), pp);
    if (it != layers_.end())
        layers_.erase(it);
}

void PlotLua::addLayer(PlotLayer_sptr& pp)
{
    layers_.push_back(pp);

    DataLayer* layer = dynamic_cast<DataLayer*>(pp.get());

	if (layer && layer->isReady())
	{
        auto sp = this->self_.lock();

        layer->setPlot( sp);
		//layer->xdata_->calcLimits();
		//layer->ydata_->calcLimits();
		//if (layer->errorbar_!= nullptr)
			//layer->errorbar_->calcLimits();
		layer->calcStats();
        //world_.xScale_.units(layer->xdata_->units());
        //world_.yScale_.units(layer->ydata_->units());

		if ((world_.xspan_ > 0) && (world_.yspan_ > 0))
            world_.calcScales(layers_);
	}
}
void PlotLua::addLayer(SeriesPtr& xdata, SeriesPtr& ydata)
{

    PlotLayer_sptr pp = std::make_shared<DataLayer>();
    DataLayer* dlay = static_cast<DataLayer*>(pp.get());

    dlay->xdata_ = xdata;
    dlay->ydata_ = ydata;
    dlay->label_ = xdata->getLabel() + " vs " + ydata->getLabel();
    addLayer(pp);

}

void PlotLua::saveCSV(const std::string& path)
{
    std::ofstream outfile(path);
    std::stringstream outs;

    std::vector<SeriesPtr> columns;

    auto fwd = layers_.begin();
    auto fend = layers_.end();
    for( ; fwd != fend; fwd++)
    {
        auto layer = dynamic_cast<DataLayer*>(fwd->get());
        if (layer)
        {
            if (layer->xdata_ != nullptr)
            {
                columns.push_back(layer->xdata_);
            }
            if (layer->ydata_ != nullptr)
            {
                columns.push_back(layer->ydata_);
            }
            if (layer->errorbar_ != nullptr)
            {
                columns.push_back(layer->errorbar_);
            }
        }
    }

    auto si = columns.begin();
    auto send = columns.end();
    bool first = true;
    for( ; si != send; si++)
    {
        if (first)
        {
            first = false;
        }
        else
            outs << '\t';
        outs << (*si)->getLabel();
    }
    outs << std::endl;
    outfile << outs.str();
    outs.str(std::string());
    // its a row based iteration
    bool notEmpty = true;
    size_t  rowix = 0;
    while(notEmpty)
    {
        bool hadData = false;
        bool firstCol = true;
        for(si = columns.begin(); si != send; si++)
        {
            Series& s = *(*si);
            if (firstCol)
                firstCol = false;
            else
                outs << "\t";
            if (rowix < s.size())
            {
                outs << s[rowix];
                hadData = true;
            }
        }
        notEmpty = hadData;
        rowix++;

        outs << std::endl;

        if (notEmpty)
        {
            outfile << outs.str();
            outs.str(std::string());
        }
    }
}

bool agw::L_asBoolean(lua_State* L, int ix, bool useDefault)
{
    int top = lua_gettop(L);
    if (top >= ix)
        return lua_toboolean(L,ix);
    else
        return useDefault;
}

int PlotLua::show(lua_State* L)
{
    PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
    if (mem)
    {
        bool visible = agw::L_asBoolean(L, 2, true);
        (*mem)->showPlot(*mem, visible);
    }
    return 0;
}

wxFont PlotLua::getFont()
{
    if (!font_.Ok())
    {
        font_ = wxSystemSettings::GetFont(wxSYS_DEVICE_DEFAULT_FONT);
    }
    return font_;
}
void PlotLua::showPlot(PlotLua_sptr& pl, bool visible)
{
    if (m_window != nullptr)
    {
        m_window->Close();
        m_window = nullptr;
    }
    if (m_window == nullptr)
    {
        AppData& app = AppData::instance();
        m_window = new PlotFrame((wxWindow*) app.mainFrame());

        PlotXY*  thePlot = new PlotXY(m_window);
        thePlot->plotLua_ = pl;
        m_window->setPlot(thePlot);
        if (this->fileName_.size() > 0)
        {
            wxFileName fn;

            fn.Assign(this->fileName_);

            m_window->SetTitle(fn.GetName());
        }
        m_window->Show();

    }
    else {
        // update? synchronize?
        // remove old data ?
        // get user interactive alterations?
        // already set with shared pointer?
        // add new layers?
        // create new version?
    }

}

static const struct luaL_Reg plotLua_funct [] = {
      {"new", PlotLua::newPlot},
      {"load", PlotLua::readPlot},
      {"linefit", PlotLua::fitLine},
      {"plotxy", PlotLua::plotXY},
      {nullptr, nullptr}
};

static const struct luaL_Reg datalayer_method [] = {
    { "xdata", DataLayer::getXData},
    { "ydata", DataLayer::getYData},
    { "legend_text", DataLayer::setLegend},
    { "display",  DataLayer::setDisplay },
    {nullptr, nullptr}
};

static const struct luaL_Reg linefit_method [] = {
    { "xdata", DataLayer::getXData},
    { "ydata", DataLayer::getYData},
    { "legend_text", DataLayer::setLegend},
    { "display",  DataLayer::setDisplay },
    { "rval",  LineFit::getRVal},
    { "slope", LineFit::getSlope},
    { "offset",LineFit::getYOffset},
    {nullptr, nullptr}
};

static const struct luaL_Reg plotLua_method [] = {
      {"__tostring", PlotLua::toString},
      {"addxy",  PlotLua::addXY},
      {"addlayer", PlotLua::addLayer},
      {"title", PlotLua::setTitle},
      {"xlabel", PlotLua::setXLabel},
      {"ylabel", PlotLua::setYLabel},
      {"show", PlotLua::show},
      {"save", PlotLua::toFile},
      {"exportCSV", PlotLua::toCSV},

       {"layer", PlotLua::getLayer},
       {"lsize", PlotLua::layerCount},
      {"legend", PlotLua::showLegend},
      {nullptr, nullptr}
};

void PlotLua::setup_require(lua_State *L)
{
    luaL_requiref(L, PLOT_LIB, PlotLua::init_lib, 1);
    lua_pop(L, 1);  /* remove lib */
}


static void create_pmeta(lua_State* L, const char* name, const luaL_Reg* lib)
{
    luaL_newmetatable(L, name);     // stacktop+2 == meta-methods table, attached to Series Objects

    // leaves function table on stack
    luaL_setfuncs(L, lib, 0);  /* add methods to new metatable, no closure */


    lua_pushstring(L, "__index"); // stacktop+3 (-1)        = "__index"

    lua_pushvalue(L, -2);         // stacktop+4 = metatable, 3 __index  (4 - 2 + 1

    lua_settable(L,  -3);  // set table at 2 metatable.__index = metatable (at 3) Pop 3 & 4

    lua_pop(L, 1);  // pop stacktop+2

}
int PlotLua::init_lib(lua_State* L)
{

    luaL_newlibtable(L, plotLua_funct);  //stacktop+1 == function table
    luaL_setfuncs(L, plotLua_funct, 0);  // setup the functions

    create_pmeta(L, PLOT_LIB, plotLua_method);
    create_pmeta(L, DataLayer::DL_LUA, datalayer_method);
    create_pmeta(L, LineFit::LF_LUA, linefit_method);
    return 1;
}

PlotLua_sptr* PlotLua::checkPlot (lua_State *L, int index ) {
  luaL_checktype(L, index, LUA_TUSERDATA);
  void *ud = luaL_checkudata(L, index, PLOT_LIB);
  luaL_argcheck(L, ud != nullptr, 1, "`plot' expected");
  return (PlotLua_sptr *)ud;
}

PlotLua_sptr*
PlotLua::pushPlot(lua_State* L)
{
    size_t nbytes = sizeof(PlotLua_sptr);
    void *a = lua_newuserdata(L, nbytes);

    PlotLua_sptr* mem = new (a) PlotLua_sptr();  // new blank shared pointer placement syntax=
    luaL_setmetatable(L, PLOT_LIB);
    return mem;
}

// argument is a table, with plot information, or X, Y, two SERIES
int PlotLua::addXY(lua_State* L)
{
    int top = lua_gettop(L);
    if (top >= 3)
    {
        PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
        SeriesPtr* xdata = SeriesRay::checkSeries(L,2);
        SeriesPtr* ydata = SeriesRay::checkSeries(L,3);
        if (mem != nullptr && xdata != nullptr && ydata != nullptr)
        {
            // new PlotLayer
            (*mem)->addLayer(*xdata, *ydata);
        }
    }
    lua_settop(L,1);
    return 1;
}

// function, create PlotLayer_sptr from series ( a plotxy ), not added to a plot



int PlotLua::plotXY(lua_State* L)
{
    int top = lua_gettop(L);
    PlotLayer_sptr pp = std::make_shared<DataLayer>();
    PlotLayer_sptr* mem = DataLayer::pushLayer(L);
    (*mem) = pp;

    DataLayer* dlay = static_cast<DataLayer*>(pp.get());
    if (top >= 1)
    {
        SeriesPtr* xdata = SeriesRay::checkSeries(L,1);
        if (xdata)
            dlay->xdata_ = (*xdata);
    }
    if (top >= 2)
    {

        SeriesPtr* ydata = SeriesRay::checkSeries(L,2);
        if (ydata)
            dlay->ydata_ = (*ydata);
   }
   return 1;
}


// argument is a table, with plot information, or X, Y, two SERIES
int PlotLua::addLayer(lua_State* L)
{
    int top = lua_gettop(L);
    if (top >= 2)
    {
        PlotLua_sptr* mem = PlotLua::checkPlot(L,1);

        PlotLayer_sptr* layer = DataLayer::checkLayer(L,2);
            // new PlotLayer
        (*mem)->addLayer(*layer);
    }
    else {
        // error halt
        const char *msg = lua_pushstring(L, "need plot and layer arguments");
        luaL_argerror(L, 2, msg);
    }
    lua_settop(L,1);
    return 1;
}

int PlotLua::newPlot(lua_State* L)
{
    PlotLua_sptr* mem = pushPlot(L);
    (*mem) = PlotLua::create_sptr();;
    return 1;
}

int PlotLua::readPlot(lua_State* L)
{
    std::string path = luaL_checkstring(L,1);

    PlotLua_sptr* mem = pushPlot(L);
    (*mem) = PlotLua::create_sptr();
    (*mem)->readFile(path);

    return 1;
}


void PlotLua::addLegend()
{
    text_.addLegend();
}

void PlotLua::removeLegend()
{
    text_.removeLegend();
}

int PlotLua::showLegend(lua_State* L)
{
    PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
    bool visible = L_asBoolean(L,2,true);
    if (visible)
        (*mem)->addLegend();
    else
        (*mem)->removeLegend();
    lua_settop(L,1);
    return 1;

}

void PlotLua::saveToFile(const std::string& path)
{
    std::ofstream fout(path);
    this->writeStream(fout);
}

int PlotLua::toFile(lua_State* L)
{
    PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
    std::string path = luaL_checkstring(L,2);
    (*mem)->saveToFile(path);
    lua_settop(L,1);
    return 1;
}
int PlotLua::toCSV(lua_State* L)
{
    PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
    std::string path = luaL_checkstring(L,2);
    (*mem)->saveCSV(path);
    lua_settop(L,1);
    return 1;
}
int PlotLua::layerCount(lua_State* L)
{
    PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
    lua_pushinteger(L, (*mem)->layers_.size());
    return 1;

}

// given two series, return a linefit layer with R value
// The series for the regression calculation are not stored and could be discarded
// This is a function, not a method. It does not add too any particular plot display,
// but returns a PlotLayer which can be added to any display.
int PlotLua::fitLine(lua_State* L)
{
    SeriesPtr* xdata = SeriesRay::checkSeries(L,1);
    SeriesPtr* ydata = SeriesRay::checkSeries(L,2);
    PlotLayer_sptr* pp = LineFit::pushLineFit(L);
    (*pp) = std::make_shared<LineFit>();
    LineFit* fit = static_cast<LineFit*>(pp->get());
    fit->regression( *(xdata->get()), *(ydata->get()));
    return 1;
}


int PlotLua::getLayer(lua_State* L)
{
    PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
    int index = luaL_checkinteger(L,2);
    if (index >= (int) (*mem)->layers_.size())
    {
        luaL_error(L, "layer index error: %d", index);
    }
    PlotLayer_sptr p = (*mem)->layers_[index];

    // what have we got?
    PlotLayer_sptr* result;
    if (typeid(*p) == typeid(DataLayer))
    {
        result = DataLayer::pushLayer(L);
        (*result) = p;
    }
    else if (typeid(*p) == typeid(LineFit))
    {
        result = LineFit::pushLineFit(L);
        (*result) = p;
    }
    return 1;

}
int PlotLua::toString (lua_State* L) {
    PlotLua_sptr* mem = PlotLua::checkPlot(L,1);
    lua_pushfstring(L, "plot(%s)", (*mem)->plotTitle_.c_str());
    return 1;
}
