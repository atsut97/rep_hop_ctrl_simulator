#ifndef __RHC_MISC_H__
#define __RHC_MISC_H__

#include <rhc_defs.h>

/* for dynamic memory allocation */
#define nalloc(t,n) ( (n) == 0 ? NULL : (t *)calloc( (n), sizeof(t) ) )
#define sfree(m)    do{ if((m)){ free(m); (m)=NULL; } } while(0)

/* for error messages */
#define eprintf(fmt,...) fprintf( stderr, fmt, ##__VA_ARGS__ )

extern bool __err_echo;
char __err_last_msg[BUFSIZ];
#define ECHO_ON()  ( __err_echo = true )
#define ECHO_OFF() ( __err_echo = false )

#define RUNTIME_ERR(msg,...) do{\
  snprintf( __err_last_msg, BUFSIZ, "%s", msg );\
  __err_echo ? eprintf( "Error: %s(%s)\n", ##__VA_ARGS__, __err_last_msg, __FUNCTION__ ) : 0;\
} while( 0 )
#define RUNTIME_WARN(msg,...) do{\
  snprintf( __err_last_msg, BUFSIZ, "%s", msg );\
  __err_echo ? eprintf( "Warning: %s(%s)\n", ##__VA_ARGS__, __err_last_msg, __FUNCTION__ ) : 0;\
} while( 0 )
#define RESET_ERR_MSG() ( __err_last_msg[0] = '\0' )

#define ALLOC_ERR_MSG "cannot allocate memory"
#define OPEN_ERR_MSG  "cannot open file"
#define ALLOC_ERR() RUNTIME_ERR( ALLOC_ERR_MSG )
#define OPEN_ERR(m) RUNTIME_ERR( OPEN_ERR_MSG ": %s", (m) )

#endif /* __RHC_MISC_H__ */
