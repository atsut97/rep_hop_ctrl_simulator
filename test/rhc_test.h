#ifndef __RHC_TEST_H__
#define __RHC_TEST_H__

/* Note: This code is based on                         */
/*       http://www.jera.com/techinfo/jtns/jtn002.html */

#include <stdio.h>

#define RHC_TEST_MESSAGE_MAX_LEN 1024

int rhc_test_run = 0;
int rhc_test_fail = 0;
int rhc_test_status = 0;
char rhc_test_last_message[RHC_TEST_MESSAGE_MAX_LEN];

#define RHC_TEST(test_name) static void test_name()
#define RHC_TEST_SUITE(suite_name) static void suite_name()

#define RHC_ASSERT(test, message) do{\
  if( !(test) ){\
    rhc_test_fail++;\
    printf( "F" );\
    rhc_test_status = 1;\
    sprintf( rhc_test_last_message, "%s", message );\
  } else\
    printf( "." );\
} while( 0 )

#define RHC_RUN_TEST(test_name) do{\
  rhc_test_run++;\
  rhc_test_status = 0;\
  test_name();\
  if( rhc_test_status ){\
    printf( "\nFAIL: %s (%s:%d)\n%s\n", #test_name, __FILE__, __LINE__, rhc_test_last_message );\
  }\
} while( 0 )

#define RHC_RUN_SUITE(suite_name) do{\
  suite_name();\
} while( 0 )

#define RHC_TEST_REPORT() do{\
  printf( "\n----------------------------------------------------------------------");\
  printf( "\nRan %d tests\n", rhc_test_run );\
  if( rhc_test_fail == 0 ){\
    printf( "\nOK\n" );\
  } else {\
    printf( "\nFAILED (failures=%d)\n", rhc_test_fail );\
  }\
} while( 0 )

#define RHC_TEST_EXIT() return rhc_test_fail != 0

#endif /* __RHC_TEST_H__ */
