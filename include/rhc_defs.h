#ifndef __RHC_DEFS_H__
#define __RHC_DEFS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifndef BUFSIZ
#define BUFSIZ 512
#endif  /* BUFSIZ */

#ifndef __USE_MISC
typedef unsigned int       uint;
typedef unsigned long int  ulong;
typedef unsigned short int ushort;
#endif
typedef signed char        byte;
typedef unsigned char      ubyte;
typedef signed short int   word;
typedef unsigned short int uword;

#if !(defined (__GNUG__) && defined (size_t))
typedef ulong size_t;
#endif

typedef ubyte bool;
enum{
  false = 0, true = 1
};

#endif /* __RHC_DEFS_H__ */
