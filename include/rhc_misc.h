#ifndef __RHC_MISC_H__
#define __RHC_MISC_H__

#include <rhc_defs.h>

/* for dynamic memory allocation */
#define nalloc(t,n) ( (n) == 0 ? NULL : (t *)calloc( (n), sizeof(t) ) )
#define sfree(m)    do{ if((m)){ free(m); (m)=NULL; } } while(0)

/* for error messages */
#define eprintf(fmt,...) fprintf( stderr, fmt, ##__VA_ARGS__ )


#endif /* __RHC_MISC_H__ */
