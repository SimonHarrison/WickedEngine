#include "wiLua.h"
#include "wiBackLog.h"

wiLua *wiLua::globalLua = nullptr;

wiLua::wiLua()
{
	m_luaState = NULL;
	m_luaState = luaL_newstate();
	luaL_openlibs(m_luaState);
	Register("debugout", DebugOut);
}

wiLua::~wiLua()
{
	lua_close(m_luaState);
}

wiLua* wiLua::GetGlobal()
{
	if (globalLua == nullptr)
	{
		globalLua = new wiLua();
	}
	return globalLua;
}

bool wiLua::Success()
{
	return m_status == 0;
}
bool wiLua::Failed()
{
	return m_status != 0;
}
string wiLua::GetErrorMsg()
{
	if (Failed()) {
		string retVal =  lua_tostring(m_luaState, -1);
		return retVal;
	}
	return string("");
}
string wiLua::PopErrorMsg()
{
	string retVal = lua_tostring(m_luaState, -1);
	lua_pop(m_luaState, 1); // remove error message
	return retVal;
}
void wiLua::PostErrorMsg(bool todebug, bool tobacklog)
{
	if (Failed())
	{
		const char* str = lua_tostring(m_luaState, -1);
		if (str == nullptr)
			return;
		stringstream ss("");
		ss << "[Lua Error] " << str;
		if (tobacklog)
		{
			wiBackLog::post(ss.str().c_str());
		}
		if (todebug)
		{
			ss << endl;
			OutputDebugStringA(ss.str().c_str());
		}
		lua_pop(m_luaState, 1); // remove error message
	}
}
bool wiLua::RunFile(const string& filename)
{
	m_status = luaL_loadfile(m_luaState, filename.c_str());
	
	if (Success()) {
		return RunScript();
	}

	PostErrorMsg();
	return false;
}
bool wiLua::RunText(const string& script)
{
	m_status = luaL_loadstring(m_luaState, script.c_str());
	if (Success())
	{
		return RunScript();
	}

	PostErrorMsg();
	return false;
}
bool wiLua::RunScript()
{
	m_status = lua_pcall(m_luaState, 0, LUA_MULTRET, 0);
	if (Failed())
	{
		PostErrorMsg();
		return false;
	}
	return true;
}
bool wiLua::Register(const string& name, lua_CFunction function)
{
	lua_register(m_luaState, name.c_str(), function);

	PostErrorMsg();

	return Success();
}

int wiLua::DebugOut(lua_State* L)
{
	int argc = lua_gettop(L); 

	stringstream ss("");

	for (int i = 1; i <= argc; i++)
	{
		const char* str = lua_tostring(L, i);
		if (str != nullptr)
		{
			ss << str;
		}
	}
	ss << endl;

	OutputDebugStringA(ss.str().c_str());

	//number of results
	return 0;
}
