#ifndef __LUABASE_H__
#define __LUABASE_H__

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#ifdef _WIN32
#define LIBRARY_FILE_EXT ".dll"
#elif __linux__
#define LIBRARY_FILE_EXT ".so"
#elif __APPLE__
#define LIBRARY_FILE_EXT ".dylib"
#else
#error Unsupported platform!
#endif

#ifdef LOWLUA_EXPORTS
#ifdef __cplusplus
#define LOWLUA_API extern "C" __declspec(dllexport)
#else
#define LOWLUA_API __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define LOWLUA_API extern "C" __declspec(dllimport)
#else
#define LOWLUA_API __declspec(dllimport)
#endif
#endif

#include <vector>

// Lua(JIT) headers:
#ifdef __cplusplus
#include <lua.hpp>
#else
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <luajit.h>
#endif

#define LUA_CCONV __cdecl

#define GMOD_MODULE_OPEN()  LOWLUA_API int LUA_CCONV gmod13_open(lua_State* L)
#define GMOD_MODULE_CLOSE() LOWLUA_API int LUA_CCONV gmod13_close(lua_State* L)
#define LUA_FUNCTION(_function_)        LOWLUA_API int LUA_CCONV _function_(lua_State* L)
#define LUA_STATIC_FUNCTION(_function_) static int LUA_CCONV _function_(lua_State* L)

#define LUA L->luabase

#endif /* !__LUABASE_H__ */
