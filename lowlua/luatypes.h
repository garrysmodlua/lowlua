#ifndef __LUATYPES_H__
#define __LUATYPES_H__

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

namespace GarrysMod {
	namespace Lua {
		namespace Type {
			enum {
				NONE = -1,
				INVALID = NONE,
				NIL,
				BOOLEAN,
				BOOL = BOOLEAN,
				LIGHTUSERDATA,
				NUMBER,
				STRING,
				TABLE,
				FUNCTION,
				USERDATA,
				THREAD,
				LAST = THREAD,

				// UserData
				ENTITY,
				PLAYER = ENTITY,
				VEHICLE = ENTITY,
				VECTOR,
				ANGLE,
				PHYSOBJ,
				SAVE,
				RESTORE,
				DAMAGEINFO,
				EFFECTDATA,
				MOVEDATA,
				RECIPIENTFILTER,
				USERCMD,
				SCRIPTEDVEHICLE,

				// Client Only
				MATERIAL,
				PANEL,
				PARTICLE,
				PARTICLEEMITTER,
				TEXTURE,
				USERMSG,

				CONVAR,
				IMESH,
				MATRIX,
				SOUND,
				PIXELVISHANDLE,
				DLIGHT,
				VIDEO,
				FILE,

				LOCOMOTION,
				PATHFOLLOWER,
				NAVMESH,
				AUDIOCHANNEL,

				COUNT
			};

			static const char* Name[] = {
				"nil",
				"bool",
				"lightuserdata",
				"number",
				"string",
				"table",
				"function",
				"userdata",
				"thread",
				"entity",
				"vector",
				"angle",
				"physobj",
				"save",
				"restore",
				"damageinfo",
				"effectdata",
				"movedata",
				"recipientfilter",
				"usercmd",
				"vehicle",
				"material",
				"panel",
				"particle",
				"particleemitter",
				"texture",
				"usermsg",
				"convar",
				"mesh",
				"matrix",
				"sound",
				"pixelvishandle",
				"dlight",
				"video",
				"file",
				"locomotion",
				"pathfollower",
				"navmesh",
				"audiochannel",

				nullptr
			};
		}
	}
}

#endif /* !__LUATYPES_H__ */
