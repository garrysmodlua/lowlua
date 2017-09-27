#ifndef __LUAUSERDATA_H__
#define __LUAUSERDATA_H__

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

namespace GarrysMod {
	namespace Lua {
		struct UserData {
			void* data;
			unsigned char type;
		};
	}
}

#endif /* !__LUAUSERDATA_H__ */
