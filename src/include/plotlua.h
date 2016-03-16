#ifndef _H_PLOTLUA
#define _H_PLOTLUA

#ifndef _H_AGW_FIRST
#include "agw.h"
#endif

#include "fromlua.h"
#include "series.hpp"
#include "lua_series.h"
#include <boost/date_time/gregorian/gregorian_types.hpp>

#include "linearscale.h"
#include "plotlayer.h"
// make a lua plot from Lua SeriesPtr user data
#include "plotxy.h"
#include "plottable.h"
#include <wx/dc.h>

class PlotFrame;

namespace agw {
	class PlotLua;


	typedef std::shared_ptr<DataTable> DataTablePtr;
	typedef std::shared_ptr<PlotLua>    PlotLuaPtr;

	// this class generates the plot image

	bool L_asBoolean(lua_State* L, int ix, bool useDefault);


	class PlotLua {
	public:
		PlotLua();
		virtual ~PlotLua();

		void addLayer(SeriesPtr& xdata, SeriesPtr& ydata);
		void addLayer(PlotPtr& layer);

		void showPlot(PlotLuaPtr& pl, bool visible = true);
		void title(const std::string& s);
		void xlabel(const std::string& s);
		void ylabel(const std::string& s);

		void readJson(const Json::Value& json);
		void writeJson(const Json::Value& json);

		void readStream(std::istream& s);
		void writeStream(std::ostream& s);
		void saveToFile(const std::string& path);

		void readFile(const std::string& path);

		void addLegend();
		void removeLegend();

		wxPoint getPlotPlace(PlotPlace pp);
		BoxLayer* findLayer(const wxPoint& pos);

		wxFont getFont();  // get default font

		void saveJson(Json::Value& root);
		void saveCSV(const std::string& path);
		bool isDrawReady();

		std::string plotTitle_;
		std::string version_;
		wxFont      font_;  //

		PixelWorld  world_;
		LabelWorld  text_;

		std::vector<PlotPtr> layers_;

		DataTablePtr  m_table;

		PlotFrame*    m_window;

		// lua interface
		static void setup_require(lua_State *L);
		static int init_lib(lua_State* L);
		static int newPlot(lua_State* L);
		static int readPlot(lua_State* L);
		static int fitLine(lua_State* L);
		static int plotXY(lua_State* L);
		static int addXY(lua_State* L);
		static int toString(lua_State* L);
		static int show(lua_State* L);
		static int setTitle(lua_State* L);
		static int setXLabel(lua_State* L);
		static int setYLabel(lua_State* L);
		static int setDisplay(lua_State* L);

		static int toFile(lua_State* L);
		static int toCSV(lua_State* L);
		static int getLayer(lua_State* L);
		static int showLegend(lua_State* L);
		static int layerCount(lua_State* L);
		static PlotLuaPtr* checkPlot(lua_State *L, int index);
		static PlotLuaPtr* pushPlot(lua_State* L);
		static int addLayer(lua_State* L);
	};

}; // namespace agw



#endif // _H_PLOTLUA
