#ifndef __CONST_H__
#define __CONST_H__

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

template<typename T>
struct luaX_Const {
	const char* name;
	T value;
};

#endif /* !__CONST_H__ */
