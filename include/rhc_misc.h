#ifndef __RHC_MISC_H__
#define __RHC_MISC_H__

#include <rhc_defs.h>

/* for dynamic memory allocation */
#define nalloc(t,n) ( (n) == 0 ? NULL : (t *)calloc( (n), sizeof(t) ) )
#define sfree(m)    do{ if((m)){ free(m); (m)=NULL; } } while(0)

/* for error messages */
#define eprintf(fmt,...) fprintf( stderr, fmt, ##__VA_ARGS__ )

static bool __err_echo = true;
static char __err_last_msg[BUFSIZ];
#define ECHO_ON()  ( __err_echo = true )
#define ECHO_OFF() ( __err_echo = false )

#define RUNTIME_ERR(msg,...) do{\
  snprintf( __err_last_msg, BUFSIZ, "Error: " msg "(%s)\n", ##__VA_ARGS__, __FUNCTION__ );\
  __err_echo ? eprintf( "%s", __err_last_msg ) : 0;\
} while( 0 )
#define RUNTIME_WARN(msg,...) do{\
  snprintf( __err_last_msg, BUFSIZ, "Warning: " msg "(%s)\n", ##__VA_ARGS__, __FUNCTION__ );\
  __err_echo ? eprintf( "%s", __err_last_msg ) : 0;\
} while( 0 )

#define ALLOC_ERR() RUNTIME_ERR( "cannot allocate memory" )
#define OPEN_ERR(m) RUNTIME_ERR( "cannot open file: %s", (m) )

#endif /* __RHC_MISC_H__ */
