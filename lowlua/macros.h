#ifndef __MACROS_H__
#define __MACROS_H__

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef clamp
#define clamp(num,low,high) (min(max((num), (low)), (high)))
#endif

#endif /* !__MACROS_H__ */
