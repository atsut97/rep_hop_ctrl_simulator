#ifndef __RHC_TEST_H__
#define __RHC_TEST_H__

/* Note: This code is based on                         */
/*       http://www.jera.com/techinfo/jtns/jtn002.html */

/* TODO: */
/*   - Add setup/teardown functions */
/*   - Make timer function be compatible with otherwise Linux */
/*   - Remove limit of failure message number */

#include <unistd.h>
#include <time.h>	/* clock_gettime() */
#include <stdio.h>
#include <math.h>

#define RHC_TEST_FAILED_MESSAGE_NUM 64
#define RHC_TEST_FAILED_MESSAGE_LEN 1024
#define RHC_TEST_TOL ( 1.0e-12 )

#define RHC_TEST_SEPARATOR1 "======================================================================\n"
#define RHC_TEST_SEPARATOR2 "----------------------------------------------------------------------\n"

/* Counters */
static int rhc_test_run = 0;
static int rhc_test_fail = 0;
static int rhc_test_status = 0;

/* Timer */
static double rhc_timer = 0;

/* Message */
static char rhc_test_last_message[RHC_TEST_FAILED_MESSAGE_LEN];
static char rhc_test_messages[RHC_TEST_FAILED_MESSAGE_NUM][RHC_TEST_FAILED_MESSAGE_LEN];

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
    return;\
  }\
} while( 0 )

#define RHC_ASSERT_EQ(expected, actual) do{\
  long int __rhc_expected_int = (expected);\
  long int __rhc_actual_int   = (actual);\
  if( (__rhc_expected_int) != (__rhc_actual_int) ){\
    rhc_test_status = 1;\
    snprintf( rhc_test_last_message, RHC_TEST_FAILED_MESSAGE_LEN, "Value of: %s\nExpected: %ld\n But was: %ld", #actual, __rhc_expected_int, __rhc_actual_int );\
    RHC_STACK_FAILURE_MESSAGE( rhc_test_last_message );\
    rhc_test_fail++;\
    return;\
   }\
} while( 0 )

#define RHC_ASSERT_DOUBLE_EQ(expected, actual) do{\
  double __rhc_expected_double = (expected);\
  double __rhc_actual_double   = (actual);\
  if( fabs( (__rhc_expected_double) - (__rhc_actual_double) ) > RHC_TEST_TOL ){\
    rhc_test_status = 1;\
    snprintf( rhc_test_last_message, RHC_TEST_FAILED_MESSAGE_LEN, "Value of: %s\nExpected: %g\n But was: %g", #actual, __rhc_expected_double, __rhc_actual_double);\
    RHC_STACK_FAILURE_MESSAGE( rhc_test_last_message );\
    rhc_test_fail++;\
    return;\
  } \
} while( 0 )

#define RHC_ASSERT_GT(val1, val2) do{ \
  double __rhc_val1 = (val1);\
  double __rhc_val2 = (val2);\
  if( !( __rhc_val1 > __rhc_val2 ) ){\
  rhc_test_status = 1;\
  snprintf( rhc_test_last_message, RHC_TEST_FAILED_MESSAGE_LEN, "Expected: (%s) > (%s)\n But was: %g vs %g", #val1, #val2, __rhc_val1, __rhc_val2 );\
  RHC_STACK_FAILURE_MESSAGE( rhc_test_last_message );\
  rhc_test_fail++;\
  return;\
  }\
} while( 0 )

#define RHC_ASSERT_GE(val1, val2) do{ \
  double __rhc_val1 = (val1);\
  double __rhc_val2 = (val2);\
  if( !( __rhc_val1 >= __rhc_val2 ) ){\
  rhc_test_status = 1;\
  snprintf( rhc_test_last_message, RHC_TEST_FAILED_MESSAGE_LEN, "Expected: (%s) >= (%s)\n But was: %g vs %g", #val1, #val2, __rhc_val1, __rhc_val2 );\
  RHC_STACK_FAILURE_MESSAGE( rhc_test_last_message );\
  rhc_test_fail++;\
  return;\
  }\
} while( 0 )

#define RHC_ASSERT_LT(val1, val2) do{ \
  double __rhc_val1 = (val1);\
  double __rhc_val2 = (val2);\
  if( !( __rhc_val1 < __rhc_val2 ) ){\
  rhc_test_status = 1;\
  snprintf( rhc_test_last_message, RHC_TEST_FAILED_MESSAGE_LEN, "Expected: (%s) < (%s)\n But was: %g vs %g", #val1, #val2, __rhc_val1, __rhc_val2 );\
  RHC_STACK_FAILURE_MESSAGE( rhc_test_last_message );\
  rhc_test_fail++;\
  return;\
  }\
} while( 0 )

#define RHC_ASSERT_LE(val1, val2) do{ \
  double __rhc_val1 = (val1);\
  double __rhc_val2 = (val2);\
  if( !( __rhc_val1 <= __rhc_val2 ) ){\
  rhc_test_status = 1;\
  snprintf( rhc_test_last_message, RHC_TEST_FAILED_MESSAGE_LEN, "Expected: (%s) <= (%s)\n But was: %g vs %g", #val1, #val2, __rhc_val1, __rhc_val2 );\
  RHC_STACK_FAILURE_MESSAGE( rhc_test_last_message );\
  rhc_test_fail++;\
  return;\
  }\
} while( 0 )

#define RHC_RUN_TEST(test_name) do{\
  if( rhc_timer == 0 ) rhc_timer = rhcTimer();\
  rhc_test_status = 0;\
  test_name();\
  rhc_test_run++;\
  if( rhc_test_status ) printf( "F" );\
  else printf( "." );\
} while( 0 )

#define RHC_RUN_SUITE(suite_name) do{\
  suite_name();\
} while( 0 )

#define RHC_TEST_REPORT() do{\
  double __rhc_timer_end;\
  int __rhc_test_iter;\
  __rhc_timer_end = rhcTimer();\
  for( __rhc_test_iter=0; __rhc_test_iter<rhc_test_fail && __rhc_test_iter<RHC_TEST_FAILED_MESSAGE_NUM; __rhc_test_iter++){\
    printf( "\n" RHC_TEST_SEPARATOR1 );\
    printf( "%s", rhc_test_messages[__rhc_test_iter] );\
  }\
  printf( "\n" RHC_TEST_SEPARATOR2 );\
  printf( "Ran %d tests in %.8fs\n\n", rhc_test_run, __rhc_timer_end - rhc_timer );\
  if( rhc_test_fail == 0 ){\
    printf( "OK\n" );\
  } else {\
    printf( "FAILED (failures=%d)\n", rhc_test_fail );\
  }\
} while( 0 )

#define RHC_TEST_EXIT() return rhc_test_fail != 0

static double rhcTimer()
/* NOTE: Only works on Linux */
{
  struct timespec ts;
  const clockid_t id = CLOCK_MONOTONIC_RAW;

  if( clock_gettime( id, &ts ) != -1 )
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
  else
    return -1.0;
}

#endif /* __RHC_TEST_H__ */
