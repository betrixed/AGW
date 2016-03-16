#include "fromlua.h"

#include "lua_series.h"
//#include <lauxlib.h>

#include <iostream>
#include <fstream>


#include "series_helper.h"

using namespace agw;

static const struct luaL_Reg seriesLib_funct [] = {
  {"double", SeriesRay::newDouble},
  {"float", SeriesRay::newFloat},
  {"range", SeriesRay::newRange},
  {"year_month", SeriesRay::newYearMonth},
  {"read",  SeriesRay::fromFile},
  {nullptr, nullptr}
};


static const struct luaL_Reg seriesLib_method [] = {
  {"__gc", SeriesRay::gcSeries},
  {"__tostring", SeriesRay::ray2string},
  {"set", SeriesRay::setSeries},
  {"get", SeriesRay::getSeries},
  {"size", SeriesRay::getSeriesSize},
  {"setLabel", SeriesRay::setLabel},
  {"getLabel", SeriesRay::getLabel},
  {"save", SeriesRay::toFile},
  {"append", SeriesRay::appendSeries},
  {"toJson", SeriesRay::toJsonText},
  {nullptr, nullptr}
};

static const struct luaL_Reg dateLib_funct [] = {
  {"new", SeriesDate::newDate},
  {nullptr, nullptr}
};

static const struct luaL_Reg dateLib_method [] = {
  {"__tostring", SeriesDate::toString},
  {"year", SeriesDate::getYear},
  {"month", SeriesDate::getMonth},
  {"day", SeriesDate::getDay},
  {"julian", SeriesDate::getJulian},
  {"mod_julian", SeriesDate::getModJulian},
  {"day_ofyear", SeriesDate::getDayYear},
  {nullptr, nullptr}
};

#include "series.hpp"
#include <wx/log.h>

#define SERIES_LIB "ray"
#define DATE_LIB "date"



//#define SERIES_HANDLE "Series"
//#define SERIES_UDATA "Series.ray"
void agw::Series_typerror (lua_State *L, int narg, const char *tname) {
  const char *msg = lua_pushfstring(L, "%s expected, got %s",
                                    tname, luaL_typename(L, narg));
  luaL_argerror(L, narg, msg);
}

SeriesPtr* SeriesRay::checkSeries (lua_State *L, int index ) {
  luaL_checktype(L, index, LUA_TUSERDATA);
  void *ud = luaL_checkudata(L, index, SERIES_LIB);
  luaL_argcheck(L, ud != nullptr, 1, "`ray' userdata expected");
  return (SeriesPtr *)ud;
}


SeriesPtr* SeriesRay::toSeries(lua_State* L, int index)
{
    SeriesPtr* p = (SeriesPtr*) lua_touserdata(L,index);
    if (p == nullptr)
        Series_typerror(L,index,SERIES_LIB);
    return p;
}

// return empty shared pointer
SeriesPtr* SeriesRay::pushSeries(lua_State* L)
{
    size_t nbytes = sizeof(SeriesPtr);
    void *a = lua_newuserdata(L, nbytes);

    SeriesPtr* mem = new (a) SeriesPtr();  // new blank shared pointer placement syntax=
    luaL_setmetatable(L, SERIES_LIB);

    return mem;
}
    // expect methods table already on stack
void SeriesRay::createmeta (lua_State *L) {
}

static void stackDump (lua_State *L) {
      int i;
      int top = lua_gettop(L);
      std::cout << "STACK " << std::endl;
      for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {

          case LUA_TSTRING:  /* strings */
            std::cout << i << ": " <<  lua_tostring(L, i);
            break;

          case LUA_TBOOLEAN:  /* booleans */
            std::cout << i << ": " <<  (lua_toboolean(L, i) ? "true" : "false");
            break;

          case LUA_TNUMBER:  /* numbers */
            std::cout << i << ": " <<  lua_tonumber(L, i);
            break;

          default:  /* other values */
            std::cout << i << ": " <<  lua_typename(L, t);
            break;

        }
        std::cout << std::endl;
      }
      std::cout << "END" << std::endl;
    }

void SeriesRay::setup_require(lua_State *L)
{
    luaL_requiref(L, SERIES_LIB, SeriesRay::init_lib, 1);
    lua_pop(L, 1);  /* remove lib */
}

// the methods table is returned anonymously.
int SeriesRay::init_lib(lua_State* L)
{
    // top of stack contains argument SERIES_LIB
    luaL_newlibtable(L, seriesLib_funct);  //stacktop+1 == function table

    luaL_setfuncs(L, seriesLib_funct, 0);  // setup the functions


    luaL_newmetatable(L, SERIES_LIB);     // stacktop+2 == meta-methods table, attached to Series Objects

    // leaves function table on stack
    luaL_setfuncs(L, seriesLib_method, 0);  /* add methods to new metatable, no closure */


    lua_pushstring(L, "__index"); // stacktop+3 (-1)        = "__index"

    lua_pushvalue(L, -2);         // stacktop+4 = metatable, 3 __index  (4 - 2 + 1

    lua_settable(L,  -3);  // set table at 2 metatable.__index = metatable (at 3) Pop 3 & 4

    lua_pop(L, 1);  /* pop stacktop+2, leaving function table on stacktop+1 */


/*
    lua_pushstring(L, "__index");   // 3 == string __index
    lua_pushstring(L, "get");       // 4 == string get
    lua_gettable(L, 1);  // get value from table at 1, using key "get", series.get at  top of stack == 4
    lua_settable(L, 2); // set the metatable at 2, to the key __index at 3, to the value series.get at 4, pops 3 and 4

    lua_pushstring(L,  "__newindex");   // 3 == string __newindex
    lua_pushstring(L, "set");       // 4 == string set
    lua_gettable(L, 1);  // get value from table at 1, using key "set", series.set at  top of stack == 4
    lua_settable(L, 2); // set the metatable at 2, to the key __newindex at 3, to the value series.set at 4, pops 3 and 4
*/
  //gets value t[k] where t is at given index  and k is at top of stack "get",
  // k is replaced by value.


    //wxLogMessage("init_lib");
    return 1;
}

// [count]
int SeriesRay::newDouble(lua_State* L)
{
    int stacktop = lua_gettop(L); // number of arguments
    int nsize;
    if (stacktop > 0)
    {
        nsize = luaL_checkinteger(L, 1); // request size first item on stack
    }
    else
        nsize = 0;

    SeriesPtr* mem = pushSeries(L); // return result
    (*mem) = std::make_shared<DoubleSeries>();

    if (nsize > 0)
    {
        (*mem)->setSize(nsize);
    }

    return 1;
}
// [count]
int SeriesRay::newFloat(lua_State* L)
{
    int stacktop = lua_gettop(L); // number of arguments
    int nsize;
    if (stacktop > 0)
    {
        nsize = luaL_checkinteger(L, 1); // request size first item on stack
    }
    else
        nsize = 0;

    SeriesPtr* mem = pushSeries(L); // return result
    (*mem) = std::make_shared<FloatSeries>();

    if (nsize > 0)
    {
        (*mem)->setSize(nsize);
    }

    return 1;
}
int SeriesRay::newRange(lua_State* L)
{
    int stacktop = lua_gettop(L); // number of arguments
    int nsize = 2;
    double v2 = 1.0;
    double v1 = 0.0;
    if (stacktop > 0)
    {
        nsize = luaL_checkinteger(L, 1); // request size first item on stack
    }
    if (stacktop > 1)
    {
        v1 = luaL_checknumber(L,2);
    }
    if (stacktop > 2)
    {
        v2 = luaL_checknumber(L,3);
    }

    SeriesPtr* mem = pushSeries(L); // return result
    (*mem) = std::make_shared<LineSeries>(nsize, v1, v2);

    return 1;
}

// [year1, month1, year2, month2]
int SeriesRay::newYearMonth(lua_State* L)
{
    static const char* argmsg = "year1,month1,year2,month2 required";
    int stacktop = lua_gettop(L); // number of arguments

    luaL_argcheck(L, (stacktop >= 4), stacktop,argmsg);


    int year1 = luaL_checkinteger(L,1);
    int month1 = luaL_checkinteger(L,2);
    int year2 = luaL_checkinteger(L,3);
    int month2 = luaL_checkinteger(L,4);

    SeriesPtr* mem = pushSeries(L); // return result
    (*mem) = std::make_shared<DateYearMonth>(year1,month1,year2,month2);
    return 1;
}

int SeriesRay::ray2string (lua_State* L) {
      SeriesPtr *mem = checkSeries(L,1);
      lua_pushfstring(L, "ray(%d)", (*mem)->size());
      return 1;
}
int SeriesRay::appendSeries(lua_State* L)
{
    SeriesPtr *mem = checkSeries(L,1);
    double value = luaL_checknumber(L, 2);

    (*mem)->append(value);

    lua_settop(L,1);
    return 1;
}
int SeriesRay::setSeries(lua_State* L)
{
    SeriesPtr *mem = checkSeries(L,1);
    int index = luaL_checkinteger(L, 2);

    size_t ct = (*mem)->size();

    luaL_argcheck(L, index < (int)ct, 2,
                       "index out of range");

    double value = luaL_checknumber(L, 3);
    (*mem)->set(index,value);

    lua_settop(L,1);
    return 1;
}

// get and set cannot be factored further, because Series uses polymorphic methods.
int SeriesRay::getSeries (lua_State* L) {
      SeriesPtr *mem = checkSeries(L,1);
      int index = luaL_checkinteger(L, 2);

      size_t ct = (*mem)->size();
      luaL_argcheck(L, index < (int)ct, 2,
                       "index out of range");

      lua_pushnumber(L, (*(mem->get()))[index]);
      return 1;
}

int SeriesRay::getLabel(lua_State* L){
      SeriesPtr *mem = checkSeries(L,1);
      lua_pushstring(L, (*mem)->getLabel().c_str());
      return 1;
}

int SeriesRay::setLabel(lua_State* L){
      SeriesPtr *mem = checkSeries(L,1);
      const char* value = luaL_checkstring(L,2);

      if (value != nullptr)
        (*mem)->setLabel(value);
      lua_settop(L,1);
      return 1;
}

int SeriesRay::fromFile(lua_State* L)
{
// get file path
      const char* path = luaL_checkstring(L,1);

      try {
          SeriesPtr sp = seriesFromFile(path);
          SeriesPtr* mem = pushSeries(L);
          *mem = sp;
          return 1;
      }
      catch(MsgException& ex)
      {
          luaL_error(L, "%s", ex.msg());
      }

      return 0;
}

int SeriesRay::toFile(lua_State* L)
{
// get file path
      SeriesPtr *mem = checkSeries(L,1);
      const char* path = luaL_checkstring(L,2);
      if (path != nullptr)
      {
          (*mem)->saveToFile(path);
      }
      lua_settop(L,1);
      return 1;

}

int SeriesRay::toJsonText(lua_State* L)
{
// get file path
      SeriesPtr *mem = checkSeries(L,1);

      std::string json = (*mem)->toJsonText();

      lua_pushlstring(L, json.c_str(), json.size());
      return 1;

}

int SeriesRay::gcSeries(lua_State* L)
{
    SeriesPtr *mem = checkSeries(L,1);
    wxLogMessage("~SeriesPtr()");
    mem->~SeriesPtr();
    return 0;

}
int SeriesRay::getSeriesSize(lua_State* L)
{

      SeriesPtr *mem = checkSeries(L,1);
      size_t ct = (*mem)->size();
      lua_pushinteger(L, ct);
      return 1;
}

boost::gregorian::date* SeriesDate::checkDate (lua_State *L, int index ) {
  luaL_checktype(L, index, LUA_TUSERDATA);
  void *ud = luaL_checkudata(L, index, DATE_LIB);
  luaL_argcheck(L, ud != nullptr, 1, DATE_LIB " expected");
  return (boost::gregorian::date*) ud;
}

void*
SeriesDate::pushDate(lua_State* L)
{
    size_t nbytes = sizeof(boost::gregorian::date);
    void *a = lua_newuserdata(L, nbytes);


    luaL_setmetatable(L, DATE_LIB);

    return a;

 }

int SeriesDate::newDate(lua_State* L)
{
    int stacktop = lua_gettop(L);
    if (stacktop >= 3)
    {
        boost::gregorian::date* mem = (boost::gregorian::date*)pushDate(L);
        int year = luaL_checkinteger(L,1);
        int month = luaL_checkinteger(L,2);
        int day = luaL_checkinteger(L,3);

        new (mem) boost::gregorian::date(year,month,day);
    }
    else {
        lua_pushnil(L);
    }
    return 1;
}

 int SeriesDate::getDay(lua_State* L)
 {
    boost::gregorian::date* mem = SeriesDate::checkDate(L,1);
    lua_pushinteger(L, mem->day());
    return 1;
 }

 int SeriesDate::getMonth(lua_State* L)
 {
     boost::gregorian::date* mem = SeriesDate::checkDate(L,1);
    lua_pushinteger(L, mem->month());
    return 1;
 }

 int SeriesDate::getYear(lua_State* L)
 {
    boost::gregorian::date* mem = SeriesDate::checkDate(L,1);
    lua_pushinteger(L, mem->year());
    return 1;
}

 int SeriesDate::getJulian(lua_State* L)
 {
    boost::gregorian::date* mem = SeriesDate::checkDate(L,1);
    lua_pushinteger(L, mem->julian_day());
    return 1;
}

 int SeriesDate::getDayYear(lua_State* L)
 {
    boost::gregorian::date* mem = SeriesDate::checkDate(L,1);
    lua_pushinteger(L, mem->day_of_year());
    return 1;
}

 int SeriesDate::getModJulian(lua_State* L)
 {
    boost::gregorian::date* mem = SeriesDate::checkDate(L,1);
    lua_pushinteger(L, mem->modjulian_day());
    return 1;
}
int SeriesDate::toString (lua_State* L) {
      boost::gregorian::date* mem = SeriesDate::checkDate(L,1);
      std::string text = boost::gregorian::to_iso_string(*mem);
      lua_pushfstring(L, "date(%s)", text.c_str());
      return 1;
}
// the methods table is returned anonymously.
int SeriesDate::init_lib(lua_State* L)
{
    // top of stack contains argument SERIES_LIB
    luaL_newlibtable(L, dateLib_funct);  //stacktop+1 == function table

    luaL_setfuncs(L, dateLib_funct, 0);  // setup the functions


    luaL_newmetatable(L, DATE_LIB);     // stacktop+2 == meta-methods table, attached to Series Objects

    // leaves function table on stack
    luaL_setfuncs(L, dateLib_method, 0);  /* add methods to new metatable, no closure */


    lua_pushstring(L, "__index"); // stacktop+3 (-1)        = "__index"

    lua_pushvalue(L, -2);         // stacktop+4 = metatable, 3 __index  (4 - 2 + 1

    lua_settable(L,  -3);  // set table at 2 metatable.__index = metatable (at 3) Pop 3 & 4

    lua_pop(L, 1);  /* pop stacktop+2, leaving function table on stacktop+1 */


    return 1;
}
void SeriesDate::setup_require(lua_State *L)
{
    luaL_requiref(L, DATE_LIB, SeriesDate::init_lib, 1);
    lua_pop(L, 1);  /* remove lib */
}
