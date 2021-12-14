#ifndef H_FromLuaH
#define H_FromLuaH

//#include <lua.hpp>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
}
#include <string>


#define FPUNUSED(x)

class LuaEngine {
public:
	LuaEngine();
	virtual ~LuaEngine();

	bool Init();
	void Stop();
	int  Run(const char* script, int len, const char* label = 0);
	void StackDump();

	bool  RunFile(const std::string& path);
    bool  IsReady() const { return state_ != nullptr;}
	static int flua_logMessage(lua_State *lua);
    static void flua_panic(lua_State *lua);

	lua_State* State() const { return state_; }
protected:

	void LogLuaError(int iErr);

	lua_State *state_;
};


class LuaStack {
public:
	LuaStack(lua_State* s)
		: state_(s), pushct_(0)
	{}

	~LuaStack()
	{
		if (pushct_ > 0)
			lua_pop(state_,pushct_);
	}



	void PushGlobal(const std::string& name){
		lua_getglobal(state_, name.c_str());
		pushct_++;
	}

	void Pop(int ix = 1)
	{
		if (ix > pushct_)
			ix = pushct_;
		lua_pop(state_,ix);
		pushct_ -= ix;

	}

	void PopAll()
	{
		if (pushct_ > 0)
		{
			lua_pop(state_,pushct_);
			pushct_ = 0;
		}
	}

	std::string ToString(int ix = -1){return lua_tostring(state_, ix);}

	bool     ToBoolean(int ix = -1){return lua_toboolean(state_, ix) ? true : false;}
	double	 ToNumber(int ix = -1){return lua_tonumber(state_, ix);}

	bool IsBoolean(int ix = -1) { return lua_isboolean(state_, ix) ? true : false; }
	bool IsNil(int ix = -1){ return lua_isnil(state_, ix)? true : false; }
	bool IsTable(int ix = -1) { return lua_istable(state_, ix)? true : false; }
	bool IsString(int ix = -1) { return lua_isstring(state_, ix)? true : false; }
	bool IsNumber(int ix = -1) { return lua_isnumber(state_, ix)? true : false; }
protected:
	lua_State* state_;
	int		   pushct_;
};

class TableIterator : public LuaStack {
public:
	// index table is the stack index of the table at the time
	// of calling the constructor
	// so if index is +ve,
	TableIterator(lua_State* s, int ixtable = -1)
		: LuaStack(s), ixtable_(ixtable)
	{ }
	bool Start()
	{
		lua_pushnil(state_);  /* key at -1, table at index ixtable_*/
		pushct_++; // should be 1
		GoNext();
		return hasNext_;
	}

	bool IsOK() const { return hasNext_; }
	void GoNext()
	{
		if (pushct_ > 1)
		{
			lua_pop(state_,pushct_ - 1);
			pushct_ = 1;
		}
		int spos = (ixtable_ < 0) ? ixtable_ - 1:  ixtable_;
		hasNext_ =  (lua_next(state_, spos) == 0) ? false : true;
		if (hasNext_)
			pushct_++; // value has been pushed on stack
		else
			PopAll();
	}

	bool FindKey(const std::string& key)
	{
		//assuming top of stack is table
		Pop(pushct_);
		lua_pushstring(state_, key.c_str());
		int spos = (ixtable_ < 0) ? ixtable_ - 1:  ixtable_;
		lua_gettable(state_, spos);
		pushct_++; // pops key, returns value if any
		return !IsNil();
	}
	bool FindIndex(double ix)
	{
		//assuming top of stack at pushct_ == 0 is table,
		Pop(pushct_);
		lua_pushnumber(state_, ix);
		int spos = (ixtable_ < 0) ? ixtable_ - 1:  ixtable_;
		lua_gettable(state_, spos);
		pushct_++; // pops key, returns value if any
		return !IsNil();
	}
private:
	bool	  hasNext_;
	int		  ixtable_;
};


#endif //H_FromLuaH
