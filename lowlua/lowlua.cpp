#include "luainterface.h"
#include "luabase.h"
#include "luatypes.h"
#include "dispatch.h"
#include "const.h"
#include "macros.h"
#include "lowlua.h"

static const std::vector<luaL_Reg> lowlua_lib = {
#pragma region Basic stack manipulation
	{ "lua_gettop", proto_lua_gettop },
	{ "lua_settop", proto_lua_settop },
	{ "lua_pushvalue", proto_lua_pushvalue },
	{ "lua_remove", proto_lua_remove },
	{ "lua_insert", proto_lua_insert },
	{ "lua_replace", proto_lua_replace },
	{ "lua_checkstack", proto_lua_checkstack },
#pragma endregion

#pragma region Access functions (stack -> C)
	{ "lua_isnone", proto_lua_isnone },
	{ "lua_isnil", proto_lua_isnil },
	{ "lua_isnoneornil", proto_lua_isnoneornil },
	{ "lua_isboolean", proto_lua_isboolean },
	{ "lua_isnumber", proto_lua_isnumber },
	{ "lua_isstring", proto_lua_isstring },
	{ "lua_istable", proto_lua_istable },
	{ "lua_iscfunction", proto_lua_iscfunction },
	{ "lua_isuserdata", proto_lua_isuserdata },
	{ "lua_isthread", proto_lua_isthread },
	{ "lua_toboolean", proto_lua_toboolean },
	{ "lua_tointeger", proto_lua_tointeger },
	{ "lua_tonumber", proto_lua_tonumber },
	{ "lua_tostring", proto_lua_tostring },
	{ "lua_tolstring", proto_lua_tolstring },
	{ "lua_tocfunction", proto_lua_tocfunction },
	/*
	{ "lua_touserdata", proto_lua_touserdata },
	{ "lua_tothread", proto_lua_tothread },
	{ "lua_topointer", proto_lua_topointer },
	*/
	{ "lua_objlen", proto_lua_objlen },
	{ "lua_type", proto_lua_type },
	{ "lua_typename", proto_lua_typename },

	{ "luaL_typename", proto_luaL_typename },
#pragma endregion

#pragma region Push functions (C -> stack)
	{ "lua_pushnil", proto_lua_pushnil },
	{ "lua_pushboolean", proto_lua_pushboolean },
	{ "lua_pushinteger", proto_lua_pushinteger },
	{ "lua_pushnumber", proto_lua_pushnumber },
	{ "lua_pushstring", proto_lua_pushstring },
	{ "lua_pushlstring", proto_lua_pushlstring },
	{ "lua_pushcclosure", proto_lua_pushcclosure },
#pragma endregion

#pragma region Get functions (Lua -> stack)
	{ "lua_gettable", proto_lua_gettable },
	{ "lua_getfield", proto_lua_getfield },
	{ "lua_rawget", proto_lua_rawget },
	{ "lua_rawgeti", proto_lua_rawgeti },
	{ "lua_createtable", proto_lua_createtable },
	//{ "lua_newuserdata", proto_lua_newuserdata },
	{ "lua_getmetatable", proto_lua_getmetatable },
	{ "lua_getfenv", proto_lua_getfenv },
#pragma endregion

#pragma region Set functions (stack -> Lua)
	{ "lua_settable", proto_lua_settable },
	{ "lua_setfield", proto_lua_setfield },
	{ "lua_rawset", proto_lua_rawset },
	{ "lua_rawseti", proto_lua_rawseti },
	{ "lua_setmetatable", proto_lua_setmetatable },
	{ "lua_setfenv", proto_lua_setfenv },
#pragma endregion

#pragma region Call functions
	{ "lua_call", proto_lua_call },
	{ "lua_pcall", proto_lua_pcall },
#pragma endregion

	//{ nullptr, nullptr } // NOTE: Do not add the trailing null! A workaround has been introduced since "luaL_register" function does not work as expected - redesigned (adding a trailing null would crash)... Thanks to Garry for screwing it.
};

static const std::vector<luaX_Const<int>> lowlua_ints = {
	{ "LUA_ERRFILE", LUA_ERRFILE },
	{ "LUA_NOREF", LUA_NOREF },
	{ "LUA_REFNIL", LUA_REFNIL },
	{ "LUA_VERSION_NUM", LUA_VERSION_NUM },
	{ "LUA_MULTRET", LUA_MULTRET },
	{ "LUA_REGISTRYINDEX", LUA_REGISTRYINDEX },
	{ "LUA_ENVIRONINDEX", LUA_ENVIRONINDEX },
	{ "LUA_GLOBALSINDEX", LUA_GLOBALSINDEX },
	{ "LUA_YIELD", LUA_YIELD },
	{ "LUA_ERRRUN", LUA_ERRRUN },
	{ "LUA_ERRSYNTAX", LUA_ERRSYNTAX },
	{ "LUA_ERRMEM", LUA_ERRMEM },
	{ "LUA_ERRERR", LUA_ERRERR },
	{ "LUA_TNONE", LUA_TNONE },
	{ "LUA_TNIL", LUA_TNIL },
	{ "LUA_TBOOLEAN", LUA_TBOOLEAN },
	{ "LUA_TLIGHTUSERDATA", LUA_TLIGHTUSERDATA },
	{ "LUA_TNUMBER", LUA_TNUMBER },
	{ "LUA_TSTRING", LUA_TSTRING },
	{ "LUA_TTABLE", LUA_TTABLE },
	{ "LUA_TFUNCTION", LUA_TFUNCTION },
	{ "LUA_TUSERDATA", LUA_TUSERDATA },
	{ "LUA_TTHREAD", LUA_TTHREAD },
	{ "LUA_MINSTACK", LUA_MINSTACK },
	{ "LUA_GCSTOP", LUA_GCSTOP },
	{ "LUA_GCRESTART", LUA_GCRESTART },
	{ "LUA_GCCOLLECT", LUA_GCCOLLECT },
	{ "LUA_GCCOUNT", LUA_GCCOUNT },
	{ "LUA_GCCOUNTB", LUA_GCCOUNTB },
	{ "LUA_GCSTEP", LUA_GCSTEP },
	{ "LUA_GCSETPAUSE", LUA_GCSETPAUSE },
	{ "LUA_GCSETSTEPMUL", LUA_GCSETSTEPMUL },
	{ "LUA_HOOKCALL", LUA_HOOKCALL },
	{ "LUA_HOOKRET", LUA_HOOKRET },
	{ "LUA_HOOKLINE", LUA_HOOKLINE },
	{ "LUA_HOOKCOUNT", LUA_HOOKCOUNT },
	{ "LUA_HOOKTAILRET", LUA_HOOKTAILRET },
	{ "LUA_MASKCALL", LUA_MASKCALL },
	{ "LUA_MASKRET", LUA_MASKRET },
	{ "LUA_MASKLINE", LUA_MASKLINE },
	{ "LUA_MASKCOUNT", LUA_MASKCOUNT },
	{ "WINVER", WINVER },
	{ "LUAI_MAXSTACK", LUAI_MAXSTACK },
	{ "LUAI_MAXCSTACK", LUAI_MAXCSTACK },
	{ "LUAI_GCPAUSE", LUAI_GCPAUSE },
	{ "LUAI_GCMUL", LUAI_GCMUL },
	{ "LUA_MAXCAPTURES", LUA_MAXCAPTURES },
	{ "LUA_IDSIZE", LUA_IDSIZE },
	{ "LUAL_BUFFERSIZE", LUAL_BUFFERSIZE },
	{ "LUAI_MAXNUMBER2STR", LUAI_MAXNUMBER2STR },
	{ "LUAJIT_VERSION_NUM", LUAJIT_VERSION_NUM },
	{ "LUAJIT_MODE_MASK", LUAJIT_MODE_MASK },
	{ "LUAJIT_MODE_OFF", LUAJIT_MODE_OFF },
	{ "LUAJIT_MODE_ON", LUAJIT_MODE_ON },
	{ "LUAJIT_MODE_FLUSH", LUAJIT_MODE_FLUSH }
};

static const std::vector<luaX_Const<const char*>> lowlua_strings = {
	{ "LUA_VERSION", LUA_VERSION },
	{ "LUA_RELEASE", LUA_RELEASE },
	{ "LUA_COPYRIGHT", LUA_COPYRIGHT },
	{ "LUA_AUTHORS", LUA_AUTHORS },
	{ "LUA_SIGNATURE", LUA_SIGNATURE },
	{ "LUA_LDIR", LUA_LDIR },
	{ "LUA_CDIR", LUA_CDIR },
	{ "LUA_PATH_DEFAULT", LUA_PATH_DEFAULT },
	{ "LUA_CPATH_DEFAULT", LUA_CPATH_DEFAULT },
	{ "LUA_PATH", LUA_PATH },
	{ "LUA_CPATH", LUA_CPATH },
	{ "LUA_INIT", LUA_INIT },
	{ "LUA_DIRSEP", LUA_DIRSEP },
	{ "LUA_PATHSEP", LUA_PATHSEP },
	{ "LUA_PATH_MARK", LUA_PATH_MARK },
	{ "LUA_EXECDIR", LUA_EXECDIR },
	{ "LUA_IGMARK", LUA_IGMARK },
	{ "LUA_PATH_CONFIG", LUA_PATH_CONFIG },
	{ "LUA_QS", LUA_QS },
	{ "LUA_NUMBER_SCAN", LUA_NUMBER_SCAN },
	{ "LUA_NUMBER_FMT", LUA_NUMBER_FMT },
	{ "LUA_INTFRMLEN", LUA_INTFRMLEN },
	{ "LUAJIT_VERSION", LUAJIT_VERSION },
	{ "LUAJIT_COPYRIGHT", LUAJIT_COPYRIGHT },
	{ "LUAJIT_URL", LUAJIT_URL },
	{ "LUA_FILEHANDLE", LUA_FILEHANDLE },
	{ "LUA_COLIBNAME", LUA_COLIBNAME },
	{ "LUA_MATHLIBNAME", LUA_MATHLIBNAME },
	{ "LUA_STRLIBNAME", LUA_STRLIBNAME },
	{ "LUA_TABLIBNAME", LUA_TABLIBNAME },
	{ "LUA_IOLIBNAME", LUA_IOLIBNAME },
	{ "LUA_OSLIBNAME", LUA_OSLIBNAME },
	{ "LUA_LOADLIBNAME", LUA_LOADLIBNAME },
	{ "LUA_DBLIBNAME", LUA_DBLIBNAME },
	{ "LUA_BITLIBNAME", LUA_BITLIBNAME },
	{ "LUA_JITLIBNAME", LUA_JITLIBNAME },
	{ "LUA_FFILIBNAME", LUA_FFILIBNAME }
};

// Called by Garry's Mod, when a module is opened (using the "require" function).
GMOD_MODULE_OPEN() {
	// Register <lowlua_lib> functions into the <_G> table.
	luaX_dispatch<DISPATCH::ENTRY_POINT_EVENT>(L, LUA_GLOBALSINDEX, lowlua_lib);
	//luaX_dispatch_const<DISPATCH::ENTRY_POINT_EVENT, int>(L, LUA_GLOBALSINDEX, lowlua_ints);
	luaX_dispatch_const_int<DISPATCH::ENTRY_POINT_EVENT>(L, LUA_GLOBALSINDEX, lowlua_ints);
	//luaX_dispatch_const<DISPATCH::ENTRY_POINT_EVENT, const char*>(L, LUA_GLOBALSINDEX, lowlua_strings);
	luaX_dispatch_const_string<DISPATCH::ENTRY_POINT_EVENT>(L, LUA_GLOBALSINDEX, lowlua_strings);
	return 0;
}

// Called by Garry's Mod, when a module is closed (i.e. on disconnect/shutdown, or by using the "unrequire" module).
GMOD_MODULE_CLOSE() {
	// Unregister <lowlua_lib> functions from the <_G> table.
	luaX_dispatch<DISPATCH::EXIT_POINT_EVENT>(L, LUA_GLOBALSINDEX, lowlua_lib);
	//luaX_dispatch_const<DISPATCH::EXIT_POINT_EVENT, int>(L, LUA_GLOBALSINDEX, lowlua_ints);
	luaX_dispatch_const_int<DISPATCH::EXIT_POINT_EVENT>(L, LUA_GLOBALSINDEX, lowlua_ints);
	//luaX_dispatch_const<DISPATCH::EXIT_POINT_EVENT, const char*>(L, LUA_GLOBALSINDEX, lowlua_strings);
	luaX_dispatch_const_string<DISPATCH::EXIT_POINT_EVENT>(L, LUA_GLOBALSINDEX, lowlua_strings);
	return 0;
}

#pragma region Basic stack manipulation

LUA_STATIC_FUNCTION(proto_lua_gettop) {
	const lua_Integer gettop = static_cast<lua_Integer>(lua_gettop(L));
	lua_pushinteger(L, gettop);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_settop) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_settop(L, idx);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_pushvalue) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_pushvalue(L, idx);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_remove) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_remove(L, idx);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_insert) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_insert(L, idx);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_replace) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_replace(L, idx);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_checkstack) {
	const int sz = static_cast<int>(luaL_checkinteger(L, 1));
	const lua_Integer checkstack = static_cast<lua_Integer>(lua_checkstack(L, sz));
	lua_pushinteger(L, checkstack);
	return 1;
}

#pragma endregion

#pragma region Access functions (stack -> C)

LUA_STATIC_FUNCTION(proto_lua_isnone) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int isnone = lua_type(L, idx) == GarrysMod::Lua::Type::NONE ? 1 : 0;
	lua_pushboolean(L, isnone);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_isnil) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int isnil = lua_type(L, idx) == GarrysMod::Lua::Type::NIL ? 1 : 0;
	lua_pushboolean(L, isnil);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_isnoneornil) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int isnoneornil = lua_type(L, idx) <= GarrysMod::Lua::Type::NIL ? 1 : 0;
	lua_pushboolean(L, isnoneornil);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_isboolean) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int isboolean = lua_type(L, idx) == GarrysMod::Lua::Type::BOOLEAN ? 1 : 0;
	lua_pushboolean(L, isboolean);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_isnumber) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int isnumber = lua_isnumber(L, idx);
	lua_pushboolean(L, isnumber);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_isstring) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int isstring = lua_isstring(L, idx);
	lua_pushboolean(L, isstring);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_istable) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int istable = lua_type(L, idx) == GarrysMod::Lua::Type::TABLE ? 1 : 0;
	lua_pushboolean(L, istable);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_iscfunction) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int iscfunction = lua_iscfunction(L, idx);
	lua_pushboolean(L, iscfunction);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_isuserdata) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int isuserdata = lua_isuserdata(L, idx);
	lua_pushboolean(L, isuserdata);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_isthread) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int istable = lua_type(L, idx) == GarrysMod::Lua::Type::THREAD ? 1 : 0;
	lua_pushboolean(L, istable);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_toboolean) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int b = lua_toboolean(L, idx);
	lua_pushboolean(L, b);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_tointeger) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const lua_Integer n = lua_tointeger(L, idx);
	lua_pushinteger(L, n);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_tonumber) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const lua_Number n = lua_tonumber(L, idx);
	lua_pushnumber(L, n);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_tostring) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const char* s = lua_tolstring(L, idx, nullptr);
	lua_pushstring(L, s);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_tolstring) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	size_t* len = reinterpret_cast<size_t*>(luaL_checkinteger(L, 2));
	const char* s = lua_tolstring(L, idx, len);
	lua_pushstring(L, s);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_tocfunction) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const lua_CFunction fn = lua_tocfunction(L, idx);
	lua_pushcclosure(L, fn, 0);
	return 1;
}
/*
LUA_STATIC_FUNCTION(proto_lua_touserdata) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_touserdata(L, idx);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_tothread) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_tothread(L, idx);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_topointer) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_topointer(L, idx);
	return 1;
}
*/
LUA_STATIC_FUNCTION(proto_lua_objlen) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const lua_Number objlen = static_cast<lua_Number>(lua_objlen(L, idx));
	lua_pushnumber(L, objlen);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_type) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const lua_Integer type_id = static_cast<lua_Integer>(lua_type(L, idx));
	lua_pushinteger(L, type_id);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_typename) {
	const int tp = clamp(static_cast<int>(luaL_checkinteger(L, 1)), GarrysMod::Lua::Type::NONE, GarrysMod::Lua::Type::LAST);
	const char* type_name = lua_typename(L, tp);
	//lua_pushlstring(L, type_name, sizeof type_name / sizeof(char) - 1);
	lua_pushstring(L, type_name);
	return 1;
}

LUA_STATIC_FUNCTION(proto_luaL_typename) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const char* type_name = lua_typename(L, lua_type(L, idx));
	//lua_pushlstring(L, type_name, sizeof type_name / sizeof(char) - 1);
	lua_pushstring(L, type_name);
	return 1;
}

#pragma endregion

#pragma region Push functions (C -> stack)

LUA_STATIC_FUNCTION(proto_lua_pushnil) {
	lua_pushnil(L);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_pushboolean) {
	const int b = static_cast<int>(luaL_checkinteger(L, 1));
	lua_pushboolean(L, b);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_pushinteger) {
	const lua_Integer n = luaL_checkinteger(L, 1);
	lua_pushinteger(L, n);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_pushnumber) {
	const lua_Number n = luaL_checknumber(L, 1);
	lua_pushnumber(L, n);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_pushstring) {
	const char* s = luaL_checklstring(L, 1, nullptr);
	lua_pushstring(L, s);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_pushlstring) {
	const char* s = luaL_checklstring(L, 1, nullptr);
	const size_t l = static_cast<size_t>(luaL_checkinteger(L, 2));
	lua_pushlstring(L, s, l);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_pushcclosure) {
	//const lua_CFunction fn = luaL_checkcfunction(L, 1);
	luaL_checktype(L, 1, GarrysMod::Lua::Type::FUNCTION);
	const lua_CFunction fn = lua_tocfunction(L, 1);
	const int n = static_cast<int>(luaL_optinteger(L, 2, static_cast<lua_Integer>(0)));
	lua_pushcclosure(L, fn, n);
	return 0;
}

#pragma endregion

#pragma region Get functions (Lua -> stack)

LUA_STATIC_FUNCTION(proto_lua_gettable) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_gettable(L, idx);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_getfield) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const char* k = luaL_checklstring(L, 2, nullptr);
	lua_getfield(L, idx, k);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_rawget) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_rawget(L, idx);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_rawgeti) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int n = static_cast<int>(luaL_checkinteger(L, 2));
	lua_rawgeti(L, idx, n);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_createtable) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int nrec = static_cast<int>(luaL_checkinteger(L, 2));
	lua_createtable(L, idx, nrec);
	return 0;
}
/*
LUA_STATIC_FUNCTION(proto_lua_newuserdata) {
	const size_t sz = static_cast<size_t>(luaL_checkinteger(L, 1));
	void* ud = lua_newuserdata(L, sz);
	return 1;
}
*/
LUA_STATIC_FUNCTION(proto_lua_getmetatable) {
	const int objindex = static_cast<int>(luaL_checkinteger(L, 1));
	const lua_Integer getmetatable = static_cast<lua_Integer>(lua_getmetatable(L, objindex));
	lua_pushinteger(L, getmetatable);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_getfenv) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_getfenv(L, idx);
	return 0;
}

#pragma endregion

#pragma region Set functions (stack -> Lua)

LUA_STATIC_FUNCTION(proto_lua_settable) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_settable(L, idx);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_setfield) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const char* k = luaL_checklstring(L, 2, nullptr);
	lua_setfield(L, idx, k);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_rawset) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	lua_rawset(L, idx);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_rawseti) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const int n = static_cast<int>(luaL_checkinteger(L, 2));
	lua_rawseti(L, idx, n);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_setmetatable) {
	const int objindex = static_cast<int>(luaL_checkinteger(L, 1));
	const lua_Integer setmetatable = static_cast<lua_Integer>(lua_setmetatable(L, objindex));
	lua_pushinteger(L, setmetatable);
	return 1;
}

LUA_STATIC_FUNCTION(proto_lua_setfenv) {
	const int idx = static_cast<int>(luaL_checkinteger(L, 1));
	const lua_Integer setfenv = static_cast<lua_Integer>(lua_setfenv(L, idx));
	lua_pushinteger(L, setfenv);
	return 1;
}

#pragma endregion

#pragma region Call functions

LUA_STATIC_FUNCTION(proto_lua_call) {
	const int nargs = static_cast<int>(luaL_checkinteger(L, 1));
	const int nresults = static_cast<int>(luaL_checkinteger(L, 2));
	lua_call(L, nargs, nresults);
	return 0;
}

LUA_STATIC_FUNCTION(proto_lua_pcall) {
	const int nargs = static_cast<int>(luaL_checkinteger(L, 1));
	const int nresults = static_cast<int>(luaL_checkinteger(L, 2));
	const int errfunc = static_cast<int>(luaL_checkinteger(L, 3));
	const lua_Integer pcall = static_cast<lua_Integer>(lua_pcall(L, nargs, nresults, errfunc));
	lua_pushinteger(L, pcall);
	return 1;
}

#pragma endregion
