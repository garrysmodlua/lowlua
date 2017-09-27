require("lowlua");

local lua_getregistry = lua_getregistry or function() return lua_pushvalue(LUA_REGISTRYINDEX); end;
local lua_getglobal = lua_getglobal or function(s) assert(isstring(s)); return lua_getfield(LUA_GLOBALSINDEX, s); end;
local lua_setglobal = lua_setglobal or function(s) assert(isstring(s)); return lua_setfield(LUA_GLOBALSINDEX, s); end;
local lua_pop = lua_pop or function(n) assert(isnumber(n)); return lua_settop(-(n) - 1); end;
local lua_newtable = lua_newtable or function() return lua_createtable(0, 0); end;
local lua_register = lua_register or function(f, s) assert(isfunction(f) and isstring(s)); lua_pushcclosure(f, 0); lua_setglobal(s); return nil; end;
local lua_strlen = lua_strlen or function(i) assert(isnumber(i)); return lua_objlen(i); end;

lua_settop(0); -- clear entire stack(?) ....hmmmmm....
lua_getglobal("print");
lua_pushstring("I am a string to be printed!");
lua_call(1, 0);

-- Last tested: 27 Sep 2017. Test failed.
