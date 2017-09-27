#ifndef __LUAINTERFACE_H__
#define __LUAINTERFACE_H__

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

struct lua_State
{
	unsigned char _ignore_this_common_lua_header_[69];
	void* luabase;
};

#endif /* !__LUAINTERFACE_H__ */
