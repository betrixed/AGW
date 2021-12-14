// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "fromlua.h"
extern "C" {
#include "lualib.h"
}
#include "wx/log.h"
#include "lua_series.h"
#include "plotlua.h"
#include "appdata.h"

extern "C" {

//int luaopen_cjson(lua_State *l);
int luaopen_lsqlite3(lua_State * L);

};

using namespace agw;

/**
* Assume that lua C code is compiled with the project, ( from 5.3) and luaxlib.h is altered.
* near line 200 --

* "Abstraction Layer" for basic report of messages and errors

extern int cpp_fwrite(const void* data, size_t unitSize, size_t nitems, FILE* stream);
extern void cpp_fflush(FILE* stream);

// print a string
#if !defined(lua_writestring)
#define lua_writestring(s,l)   cpp_fwrite((s), sizeof(char), (l), stdout)
#endif

// print a newline and flush the output
#if !defined(lua_writeline)
#define lua_writeline()        (lua_writestring("\n", 1), cpp_fflush(stdout))
#endif

The cpp_fwrite, and cpp_fflush hijack the fwrite calls to stdout only, for lua_writeline, and lua_writestring.
They are redirected to std::cout, which is redirected in the application.

Otherwise the stdout is a "sink" which is hard to capture.

**/

extern "C" {
    int cpp_fwrite(const void* data, size_t unitSize, size_t nitems, FILE* stream);
    void cpp_fflush(FILE* stream);
};


int cpp_fwrite(const void* data, size_t unitSize, size_t nitems, FILE* stream)
{
    if ((stream == stdout)&& (unitSize==1))
    {
        std::cout.write((const char*) data, nitems);
        return nitems;
    }
    else {
        return fwrite(data,unitSize,nitems,stream);
    }

}
void cpp_fflush(FILE* stream)
{
    if (stream == stdout)
    {
        std::cout.flush();
    }
    else
    {
        fflush(stream);
    }
}

LuaEngine::LuaEngine()
: state_(0)
{
}

LuaEngine::~LuaEngine()
{
	Stop();
}


void
LuaEngine::StackDump()
{

  int i;
  int top = lua_gettop(state_);
  for (i = 1; i <= top; i++) {  /* repeat for each level */
    int t = lua_type(state_, i);
    switch (t) {

      case LUA_TSTRING:  /* strings */
		  wxLogInfo("%3d: \"%s\"", i, lua_tostring(state_, i));
        break;

      case LUA_TBOOLEAN:  /* booleans */
        wxLogInfo("%3d: \"%s\"", i, lua_toboolean(state_, i) ? "true" : "false");
        break;

      case LUA_TNUMBER:  /* numbers */
        wxLogInfo("%3d: %g", i, lua_tonumber(state_, i));
        break;

      default:  /* other values */
        wxLogInfo("%3d: \"%s\"",i, lua_typename(state_, t));
        break;

    }
  }
  wxLogInfo("\n");  /* end the listing */

}


int
LuaEngine::flua_logMessage(lua_State *lua)
{
   assert (lua_isstring (lua,1));

   const char *msg = lua_tostring (lua, 1);

   wxLogMessage("%s",msg);
   return 0;

}

void LuaEngine::LogLuaError(int iErr)
{
	int ix =  lua_gettop (state_);
	if (ix > 0)
	{
		const char* msg = luaL_checkstring(state_,ix);
		switch(iErr)
		{
		case LUA_ERRFILE:
			std::cerr << "Lua load error" << msg;
			break;
		default:
			std::cerr << "Lua run error" << msg;
		}
	}

}


bool
LuaEngine::RunFile(const std::string& path)
{
	wxLog::SetLogLevel(wxLOG_Debug);
	wxLog::SetVerbose(true);
    int iErr = 0;

    if ((iErr = luaL_loadfile (state_, path.c_str())) != 0)
    {
		LogLuaError(iErr);
		return false;
    }
	else {
		iErr = lua_pcall(state_, 0, LUA_MULTRET, 0);
		if (iErr != 0)
		{
			LogLuaError(iErr);
			return false;
		}
	}
	return true;
}


int
LuaEngine::Run(const char* script, int len, const char* label)
{
	wxLog::SetLogLevel(wxLOG_Debug);
	wxLog::SetVerbose(true);
   int iErr = 0;

   if (!label)
   {
      label = "chunk";
   }

   if ((iErr = luaL_loadbuffer (state_, script, len, label)) == 0)
   {
      // Call main...
       iErr = lua_pcall (state_, 0, LUA_MULTRET, 0);
		if (iErr != 0)
			LogLuaError(iErr);
   }
   else {
		if (iErr != 0)
			LogLuaError(iErr);
   }
   wxLog::SetVerbose(false);
   return iErr;
}

void
LuaEngine::flua_panic(lua_State *FPUNUSED(lua))
{
}

bool
LuaEngine::Init()
{
	Stop();
    state_ = luaL_newstate ();

	if (state_)
	{
	// Load util libs into lua
		luaL_openlibs(state_);

        luaL_requiref(state_, "sqlite3", luaopen_lsqlite3, 1);
        lua_pop(state_, 1);

		SeriesRay::setup_require(state_);
		SeriesDate::setup_require(state_);
		PlotLua::setup_require(state_);
        AppData::setup_require(state_);
        NormalStats::setup_require(state_);
		//const char* CJSON_LIB = "cjson";
	    //luaL_requiref(state_, CJSON_LIB, luaopen_cjson, 1);
        //lua_pop(state_, 1);  /* remove lib */


		// setup global printing (trace)
		lua_pushcclosure (state_, LuaEngine::flua_logMessage, 0);
		lua_setglobal (state_, "trace");

		lua_atpanic (state_, (lua_CFunction) LuaEngine::flua_panic);

		return true;
	}
	else
		return false;

}

void
LuaEngine::Stop()
{
	if (state_)
	{
		lua_close (state_);
		state_ = 0;
	}
}
