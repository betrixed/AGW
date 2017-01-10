#include "appdata.h"
#include "lua_series.h"

using namespace agw;

static const struct luaL_Reg appLua_funct [] = {
      {"dbpath", AppData::dbpath},
      {"global", AppData::global},
      {"fileChooser", AppData::fileChooser},
      {nullptr, nullptr}
};

int AppData::init_lib(lua_State* L)
{

    luaL_newlibtable(L, appLua_funct);  //stacktop+1 == function table
    luaL_setfuncs(L, appLua_funct, 0);  // setup the functions

    /*luaL_newmetatable(L, PLOT_LIB);     // stacktop+2 == meta-methods table, attached to Series Objects

    // leaves function table on stack
    luaL_setfuncs(L, plotLua_method, 0);  // add methods to new metatable, no closure


    lua_pushstring(L, "__index"); // stacktop+3 (-1)        = "__index"

    lua_pushvalue(L, -2);         // stacktop+4 = metatable, 3 __index  (4 - 2 + 1

    lua_settable(L,  -3);  // set table at 2 metatable.__index = metatable (at 3) Pop 3 & 4

    lua_pop(L, 1);  // pop stacktop+2, leaving function table on stacktop+1
    */

    return 1;
}


int AppData::global(lua_State* L)
{
    const char* gname = luaL_checkstring(L,1);
    SeriesPtr sp = gAppData->getGlobal(gname);
    if (sp == nullptr)
    {
        sp = std::make_shared<DoubleSeries>();
        sp->setLabel(gname);
    }
    SeriesPtr* mem = SeriesRay::pushSeries(L);
    *mem = sp;
    return 1;
}

int AppData::dbpath(lua_State* L)
{
    std::string path = gAppData->getDBPath();
    lua_pushlstring(L, path.c_str(), path.size() );
    return 1;
}

void AppData::setup_require(lua_State *L)
{
    luaL_requiref(L, APP_LUA, AppData::init_lib, 1);
    lua_pop(L, 1);  /* remove lib */
}

int AppData::fileChooser(lua_State* L)
{
    std::string msg;
    std::string extension;
    std::string result;

    int stacktop = lua_gettop(L); // number of arguments

    if (stacktop > 1)
    {
        const char* arg2 = luaL_checkstring(L,2);
        if (arg2)
        {
            extension.assign(arg2,strlen(arg2));
        }
    }

    if (stacktop > 0)
    {
        const char* arg1 = luaL_checkstring(L,1);
        if (arg1)
        {
            msg.assign(arg1,strlen(arg1));
        }

    }
    if (msg.size()==0)
        msg = "Running script";
    if (extension.size()==0)
        extension = "*";

    if (gAppData->luaFileChooser(msg,extension,result))
    {
        lua_pushlstring(L, result.c_str(), result.size() );
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}
