#ifndef __RHC_TEST_H__
#define __RHC_TEST_H__

/* Note: This code is based on                         */
/*       http://www.jera.com/techinfo/jtns/jtn002.html */

/* TODO: */
/*   - Add assertion function to compare two floats */
/*   - Add setup/teardown functions */
/*   - Add a timer to measure test execution time */
/*   - Remove limit of failure message number */

#include <stdio.h>

#define RHC_TEST_FAILED_MESSAGE_NUM 64
#define RHC_TEST_FAILED_MESSAGE_LEN 1024

int rhc_test_run = 0;
int rhc_test_fail = 0;
int rhc_test_status = 0;
char rhc_test_last_message[RHC_TEST_FAILED_MESSAGE_LEN];
char rhc_test_messages[RHC_TEST_FAILED_MESSAGE_NUM][RHC_TEST_FAILED_MESSAGE_LEN];

#define RHC_TEST_SEPARATOR1 "======================================================================\n"
#define RHC_TEST_SEPARATOR2 "----------------------------------------------------------------------\n"

#define RHC_TEST(test_name) static void test_name()
#define RHC_TEST_SUITE(suite_name) static void suite_name()

#define RHC_STACK_FAILURE_MESSAGE(message) do{\
  if( rhc_test_fail < RHC_TEST_FAILED_MESSAGE_NUM - 1 )\
    snprintf( rhc_test_messages[rhc_test_fail], RHC_TEST_FAILED_MESSAGE_LEN, "FAIL: %s (%s:%d)\n%s%s\n", __FUNCTION__, __FILE__, __LINE__, RHC_TEST_SEPARATOR2, message );\
  else\
    snprintf( rhc_test_messages[RHC_TEST_FAILED_MESSAGE_NUM-1], RHC_TEST_FAILED_MESSAGE_LEN, "Too many failures! (subsequent messages are skipped)\n" );\
} while( 0 )

#define RHC_ASSERT(test, message) do{\
  if( !(test) ){\
    rhc_test_status = 1;\
    RHC_STACK_FAILURE_MESSAGE(message);\
    rhc_test_fail++;\
  }\
} while( 0 )

#define RHC_ASSERT_EQ(expected, actual) do{\
  if( expected != actual ){\
    rhc_test_status = 1;\
    snprintf( rhc_test_last_message, RHC_TEST_FAILED_MESSAGE_LEN, "Value of: %s\nExpected: %d\n But was: %d", #actual, expected, actual);\
    RHC_STACK_FAILURE_MESSAGE( rhc_test_last_message );\
    rhc_test_fail++;\
   }\
} while( 0 )

#define RHC_RUN_TEST(test_name) do{\
  rhc_test_run++;\
  rhc_test_status = 0;\
  test_name();\
  if( rhc_test_status ) printf( "F" );\
  else printf( "." );\
} while( 0 )

#define RHC_RUN_SUITE(suite_name) do{\
  suite_name();\
} while( 0 )

#define RHC_TEST_REPORT() do{\
  int __rhc_test_iter;\
  for( __rhc_test_iter=0; __rhc_test_iter<rhc_test_fail && __rhc_test_iter<RHC_TEST_FAILED_MESSAGE_NUM; __rhc_test_iter++){\
    printf( "\n" RHC_TEST_SEPARATOR1 );\
    printf( "%s", rhc_test_messages[__rhc_test_iter] );\
  }\
  printf( "\n" RHC_TEST_SEPARATOR2 );\
  printf( "Ran %d tests\n\n", rhc_test_run );\
  if( rhc_test_fail == 0 ){\
    printf( "OK\n" );\
  } else {\
    printf( "FAILED (failures=%d)\n", rhc_test_fail );\
  }\
} while( 0 )

#define RHC_TEST_EXIT() return rhc_test_fail != 0

#endif /* __RHC_TEST_H__ */
