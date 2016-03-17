#ifndef _H_LUA_SERIES_
#define _H_LUA_SERIES_

#include "fromlua.h"
#include "series.hpp"
#include <boost/date_time/gregorian/gregorian_types.hpp>

namespace agw {
	class SeriesDate {
	public:

		// 3 arguements -  expect year, month, day
		// 1 argument - expect ISO string
		static void setup_require(lua_State *L);
		static int init_lib(lua_State* L);

		static int toString(lua_State* L);

		static int newDate(lua_State* L);
		static boost::gregorian::date* checkDate(lua_State* L, int index);
		static void* pushDate(lua_State* L);

		static int getDay(lua_State* L);
		static int getMonth(lua_State* L);
		static int getYear(lua_State* L);
		static int getJulian(lua_State* L);
		static int getModJulian(lua_State* L);
		static int getDayYear(lua_State* L);
	};


	class SeriesRay {
	public:
		// type= [size_t number]
		// type=table to json string?
		// library setup for lua
		static void setup_require(lua_State *L);
		static int init_lib(lua_State* L);
		// object creation
		static int newDouble(lua_State* L);
		static int newFloat(lua_State* L);
		static int fromFile(lua_State* L);
		static int newRange(lua_State* L);
		static int newYearMonth(lua_State* L);


		static int gcSeries(lua_State* L);

		static int setSeries(lua_State* L);
		static int getSeries(lua_State* L);
		static int appendSeries(lua_State* L);
		static int getSeriesSize(lua_State* L);

		// type=string
		static int setLabel(lua_State* L);
		static int getLabel(lua_State* L);

		// type = string (enum)
		static int setUnits(lua_State* L);
		static int getUnits(lua_State* L);

		// save to file.ser
		static int toFile(lua_State* L);
		// save to Json Text String
		static int toJsonText(lua_State* L);

		static int ray2string(lua_State* L);
		static void createmeta(lua_State *L);




		static SeriesPtr* checkSeries(lua_State *L, int index);
		static SeriesPtr* toSeries(lua_State* L, int index);
		static SeriesPtr* pushSeries(lua_State* L);

	};

	void Series_typerror(lua_State *L, int narg, const char *tname);

}; // namespace agw

#endif // _H_LUA_SERIES_
