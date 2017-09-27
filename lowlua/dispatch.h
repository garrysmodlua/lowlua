#ifndef __DISPATCH_H__
#define __DISPATCH_H__

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

//#include <type_traits>
#include "luabase.h"
#include "const.h"

enum class DISPATCH : unsigned char {
	ENTRY_POINT_EVENT,
	EXIT_POINT_EVENT
};

template<DISPATCH Event>
static int LUA_CCONV luaX_dispatch(lua_State* L, const int idx, const std::vector<luaL_Reg> l) {
	lua_pushvalue(L, idx);
	for (const auto& fn : l) {
		// Register/Push a new C function if called from entry-point; otherwise, push a nil value (removes a function) if called from exit-point.
		if constexpr (Event == DISPATCH::ENTRY_POINT_EVENT) lua_pushcclosure(L, fn.func, 0);
		else if constexpr (Event == DISPATCH::EXIT_POINT_EVENT) lua_pushnil(L);
		lua_setfield(L, -2, fn.name);
	}
	lua_pop(L, 1);
	return 0;
}
/*
// TODO: Specialize/Fix this to replace two of the template functions, below.
template<DISPATCH Event, class T>
static int LUA_CCONV luaX_dispatch_const(lua_State* L, const int idx, const std::vector<luaX_Const<T>> l) {
	lua_pushvalue(L, idx);
	for (const auto& cn : l) {
		if constexpr (Event == DISPATCH::ENTRY_POINT_EVENT) {
			if (std::is_same<T, const char*>::value) { // Not this way.
				lua_pushstring(L, cn.value);
			}
			else if (std::is_same<T, int>::value) {
				lua_pushnumber(L, static_cast<lua_Number>(cn.value));
			}
		}
		else if constexpr (Event == DISPATCH::EXIT_POINT_EVENT) {
			lua_pushnil(L);
		}
		lua_setfield(L, -2, cn.name);
	}
	lua_pop(L, 1);
	return 0;
}
*/
template<DISPATCH Event>
static int LUA_CCONV luaX_dispatch_const_int(lua_State* L, const int idx, const std::vector<luaX_Const<int>> l) {
	lua_pushvalue(L, idx);
	for (const auto& cn : l) {
		if constexpr (Event == DISPATCH::ENTRY_POINT_EVENT)	lua_pushnumber(L, static_cast<lua_Number>(cn.value));
		else if constexpr (Event == DISPATCH::EXIT_POINT_EVENT) lua_pushnil(L);
		lua_setfield(L, -2, cn.name);
	}
	lua_pop(L, 1);
	return 0;
}

template<DISPATCH Event>
static int LUA_CCONV luaX_dispatch_const_string(lua_State* L, const int idx, const std::vector<luaX_Const<const char*>> l) {
	lua_pushvalue(L, idx);
	for (const auto& cn : l) {
		if constexpr (Event == DISPATCH::ENTRY_POINT_EVENT)	lua_pushstring(L, cn.value);
		else if constexpr (Event == DISPATCH::EXIT_POINT_EVENT) lua_pushnil(L);
		lua_setfield(L, -2, cn.name);
	}
	lua_pop(L, 1);
	return 0;
}

#endif /* !__DISPATCH_H__ */
