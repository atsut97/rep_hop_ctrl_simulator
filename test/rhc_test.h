#ifndef __RHC_TEST_H__
#define __RHC_TEST_H__

/* Note: This code is based on                         */
/*       http://www.jera.com/techinfo/jtns/jtn002.html */

#include <stdio.h>

int rhc_test_run = 0;
int rhc_test_fail = 0;

#define RHC_TEST(test_name) static char *test_name()
#define RHC_TEST_SUITE(suite_name) static char *suite_name()

#define RHC_ASSERT(test, message) do{\
    if( !(test) ){\
      rhc_test_fail++;\
      printf( "F" );\
      return "ERROR: " message;\
    } else\
      printf( "." );\
} while( 0 )

#define RHC_RUN_TEST(test) do{\
    char *message = test();\
    rhc_test_run++;\
    if( message ) return message;\
} while( 0 )

#define RHC_TEST_REPORT() printf( "\n%d tests, %d failures\n", rhc_test_run, rhc_test_fail )

#endif /* __RHC_TEST_H__ */
