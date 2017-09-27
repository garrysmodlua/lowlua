#ifndef __LOWLUA_H__
#define __LOWLUA_H__

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#pragma region Basic stack manipulation

LUA_STATIC_FUNCTION(proto_lua_gettop);
LUA_STATIC_FUNCTION(proto_lua_settop);
LUA_STATIC_FUNCTION(proto_lua_pushvalue);
LUA_STATIC_FUNCTION(proto_lua_remove);
LUA_STATIC_FUNCTION(proto_lua_insert);
LUA_STATIC_FUNCTION(proto_lua_replace);
LUA_STATIC_FUNCTION(proto_lua_checkstack);

#pragma endregion

#pragma region Access functions (stack -> C)

LUA_STATIC_FUNCTION(proto_lua_isnone);
LUA_STATIC_FUNCTION(proto_lua_isnil);
LUA_STATIC_FUNCTION(proto_lua_isnoneornil);
LUA_STATIC_FUNCTION(proto_lua_isboolean);
LUA_STATIC_FUNCTION(proto_lua_isnumber);
LUA_STATIC_FUNCTION(proto_lua_isstring);
LUA_STATIC_FUNCTION(proto_lua_istable);
LUA_STATIC_FUNCTION(proto_lua_iscfunction);
LUA_STATIC_FUNCTION(proto_lua_isuserdata);
LUA_STATIC_FUNCTION(proto_lua_isthread);
LUA_STATIC_FUNCTION(proto_lua_toboolean);
LUA_STATIC_FUNCTION(proto_lua_tointeger);
LUA_STATIC_FUNCTION(proto_lua_tonumber);
LUA_STATIC_FUNCTION(proto_lua_tostring);
LUA_STATIC_FUNCTION(proto_lua_tolstring);
LUA_STATIC_FUNCTION(proto_lua_tocfunction);
/*
LUA_STATIC_FUNCTION(proto_lua_touserdata);
LUA_STATIC_FUNCTION(proto_lua_tothread);
LUA_STATIC_FUNCTION(proto_lua_topointer);
*/
LUA_STATIC_FUNCTION(proto_lua_objlen);
LUA_STATIC_FUNCTION(proto_lua_type);
LUA_STATIC_FUNCTION(proto_lua_typename);

LUA_STATIC_FUNCTION(proto_luaL_typename);

#pragma endregion

#pragma region Push functions (C -> stack)

LUA_STATIC_FUNCTION(proto_lua_pushnil);
LUA_STATIC_FUNCTION(proto_lua_pushboolean);
LUA_STATIC_FUNCTION(proto_lua_pushinteger);
LUA_STATIC_FUNCTION(proto_lua_pushnumber);
LUA_STATIC_FUNCTION(proto_lua_pushstring);
LUA_STATIC_FUNCTION(proto_lua_pushlstring);
LUA_STATIC_FUNCTION(proto_lua_pushcclosure);

#pragma endregion

#pragma region Get functions (Lua -> stack)

LUA_STATIC_FUNCTION(proto_lua_gettable);
LUA_STATIC_FUNCTION(proto_lua_getfield);
LUA_STATIC_FUNCTION(proto_lua_rawget);
LUA_STATIC_FUNCTION(proto_lua_rawgeti);
LUA_STATIC_FUNCTION(proto_lua_createtable);
//LUA_STATIC_FUNCTION(proto_lua_newuserdata);
LUA_STATIC_FUNCTION(proto_lua_getmetatable);
LUA_STATIC_FUNCTION(proto_lua_getfenv);

#pragma endregion

#pragma region Set functions (stack -> Lua)

LUA_STATIC_FUNCTION(proto_lua_settable);
LUA_STATIC_FUNCTION(proto_lua_setfield);
LUA_STATIC_FUNCTION(proto_lua_rawset);
LUA_STATIC_FUNCTION(proto_lua_rawseti);
LUA_STATIC_FUNCTION(proto_lua_setmetatable);
LUA_STATIC_FUNCTION(proto_lua_setfenv);

#pragma endregion

#pragma region Call functions

LUA_STATIC_FUNCTION(proto_lua_call);
LUA_STATIC_FUNCTION(proto_lua_pcall);

#pragma endregion

#endif /* !__LOWLUA_H__ */
