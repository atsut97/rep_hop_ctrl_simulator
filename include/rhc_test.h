#ifndef __RHC_TEST_H__
#define __RHC_TEST_H__

/* Note: This code is based on                         */
/*       http://www.jera.com/techinfo/jtns/jtn002.html */

/* TODO: */
/*   - Make timer function be compatible with otherwise Linux */
/*   - Remove limit of failure message number */

#include <unistd.h>
#include <time.h>	/* clock_gettime() */
#include <stdio.h>
#include <math.h>
#include <string.h>

#define TEST_BUFSIZ      1024
#define TEST_N_MSG_STACK 64
#define TEST_TOL         ( 1.0e-12 )

#define TEST_SEPARATOR1 "======================================================================\n"
#define TEST_SEPARATOR2 "----------------------------------------------------------------------\n"

/* Counters */
static int __test_run = 0;
static int __test_fail = 0;
static int __test_status = 0;

/* Timer */
static double __test_timer_time = 0;

/* Message */
static char __test_last_message[TEST_BUFSIZ];
static char __test_messages[TEST_N_MSG_STACK][TEST_BUFSIZ];
static char __test_running_test_name[TEST_BUFSIZ];

/* Setup and teardown function */
static void (*__test_setup)() = NULL;
static void (*__test_teardown)() = NULL;

#define TEST(test_name) static void test_name()
#define TEST_SUITE(suite_name) static void suite_name()

#define STACK_FAILURE_MESSAGE(message) do{\
  if( __test_fail < TEST_N_MSG_STACK - 1 )\
    snprintf( __test_messages[__test_fail], TEST_BUFSIZ, "FAIL: %s (%s:%d)\n%s%s\n", __test_running_test_name, __FILE__, __LINE__, TEST_SEPARATOR2, message );\
  else\
    snprintf( __test_messages[TEST_N_MSG_STACK-1], TEST_BUFSIZ, "Too many failures! (subsequent messages are skipped)\n" );\
} while( 0 )

#define ASSERT(test, message) do{\
  if( !(test) ){\
    __test_status = 1;\
    STACK_FAILURE_MESSAGE(message);\
    __test_fail++;\
    return;\
  }\
} while( 0 )

#define FAIL(message) do{\
  snprintf( __test_last_message, TEST_BUFSIZ, "%s", message);\
  ASSERT( 0, __test_last_message );\
} while( 0 )

#define ASSERT_TRUE(cond) do{\
  snprintf( __test_last_message, TEST_BUFSIZ, "Value of %s\nExpected: true\n But was: false", #cond );\
  ASSERT( (cond), __test_last_message );\
} while( 0 )

#define ASSERT_FALSE(cond) do{\
  snprintf( __test_last_message, TEST_BUFSIZ, "Value of %s\nExpected: false\n But was: true", #cond );\
  ASSERT( !(cond), __test_last_message );\
} while( 0 )

#define ASSERT_EQ(expected, actual) do{\
  if( (expected) != (actual) ){\
    __test_status = 1;\
    snprintf( __test_last_message, TEST_BUFSIZ, "Value of: %s\nExpected: %g\n But was: %g", #actual, (double)expected, (double)actual );\
    STACK_FAILURE_MESSAGE( __test_last_message );\
    __test_fail++;\
    return;\
   }\
} while( 0 )

#define ASSERT_NE(expected, actual) do{\
  if( (expected) == (actual) ){\
    __test_status = 1;\
    snprintf( __test_last_message, TEST_BUFSIZ, "Expected: (%s) != (%s)\n But was: %g vs %g", #expected, #actual, (double)expected, (double)actual );\
    STACK_FAILURE_MESSAGE( __test_last_message );\
    __test_fail++;\
    return;\
  }\
} while( 0 )

#define ASSERT_DOUBLE_EQ(expected, actual) do{\
  double __expected_double = (expected);\
  double __actual_double   = (actual);\
  if( fabs( (__expected_double) - (__actual_double) ) > TEST_TOL ){\
    __test_status = 1;\
    snprintf( __test_last_message, TEST_BUFSIZ, "Value of: %s\nExpected: %g\n But was: %g", #actual, __expected_double, __actual_double);\
    STACK_FAILURE_MESSAGE( __test_last_message );\
    __test_fail++;\
    return;\
  } \
} while( 0 )

#define ASSERT_GT(val1, val2) do{ \
  double __tmp_val1 = (val1);\
  double __tmp_val2 = (val2);\
  if( !( __tmp_val1 > __tmp_val2 ) ){\
    __test_status = 1;\
    snprintf( __test_last_message, TEST_BUFSIZ, "Expected: (%s) > (%s)\n But was: %g vs %g", #val1, #val2, __tmp_val1, __tmp_val2 );\
    STACK_FAILURE_MESSAGE( __test_last_message );\
    __test_fail++;\
    return;\
  }\
} while( 0 )

#define ASSERT_GE(val1, val2) do{ \
  double __tmp_val1 = (val1);\
  double __tmp_val2 = (val2);\
  if( !( __tmp_val1 >= __tmp_val2 ) ){\
    __test_status = 1;\
    snprintf( __test_last_message, TEST_BUFSIZ, "Expected: (%s) >= (%s)\n But was: %g vs %g", #val1, #val2, __tmp_val1, __tmp_val2 );\
    STACK_FAILURE_MESSAGE( __test_last_message );\
    __test_fail++;\
    return;\
  }\
} while( 0 )

#define ASSERT_LT(val1, val2) do{ \
  double __tmp_val1 = (val1);\
  double __tmp_val2 = (val2);\
  if( !( __tmp_val1 < __tmp_val2 ) ){\
    __test_status = 1;\
    snprintf( __test_last_message, TEST_BUFSIZ, "Expected: (%s) < (%s)\n But was: %g vs %g", #val1, #val2, __tmp_val1, __tmp_val2 );\
    STACK_FAILURE_MESSAGE( __test_last_message );\
    __test_fail++;\
    return;\
  }\
} while( 0 )

#define ASSERT_LE(val1, val2) do{ \
  double __tmp_val1 = (val1);\
  double __tmp_val2 = (val2);\
  if( !( __tmp_val1 <= __tmp_val2 ) ){\
    __test_status = 1;\
    snprintf( __test_last_message, TEST_BUFSIZ, "Expected: (%s) <= (%s)\n But was: %g vs %g", #val1, #val2, __tmp_val1, __tmp_val2 );\
    STACK_FAILURE_MESSAGE( __test_last_message );\
    __test_fail++;\
    return;\
  }\
} while( 0 )

#define ASSERT_STREQ(expected, actual) do{\
  if( strcmp( (expected), (actual) ) ){\
    __test_status = 1;\
    snprintf( __test_last_message, TEST_BUFSIZ, "Value of: %s\nExpected: %s\n But was: %s", #actual, expected, actual );\
    STACK_FAILURE_MESSAGE( __test_last_message );\
    __test_fail++;\
    return;\
  }\
} while( 0 )

#define ASSERT_PTREQ(expected, actual) do{\
  if( (expected) != (actual) ){\
    __test_status = 1;\
    snprintf( __test_last_message, TEST_BUFSIZ, "Value of: %s\nExpected: %p\n But was: %p", #actual, expected, actual );\
    STACK_FAILURE_MESSAGE( __test_last_message );\
    __test_fail++;\
    return;\
   }\
} while( 0 )

#define ASSERT_PTRNE(expected, actual) do{\
  if( (expected) == (actual) ){\
    __test_status = 1;\
    snprintf( __test_last_message, TEST_BUFSIZ, "Expected: (%s) != (%s)\n But was: %p vs %p", #expected, #actual, expected, actual );\
    STACK_FAILURE_MESSAGE( __test_last_message );\
    __test_fail++;\
    return;\
  }\
} while( 0 )

#define RUN_TEST(test_name) do{\
  if( __test_timer_time == 0 ) __test_timer_time = test_timer();\
  snprintf( __test_running_test_name, TEST_BUFSIZ, "%s", #test_name );\
  if( __test_setup ) (*__test_setup)();\
  __test_status = 0;\
  test_name();\
  __test_run++;\
  if( __test_status ) printf( "F" );\
  else printf( "." );\
  if( __test_teardown ) (*__test_teardown)();\
} while( 0 )

#define RUN_SUITE(suite_name) do{\
  suite_name();\
  __test_setup = NULL;\
  __test_teardown = NULL;\
} while( 0 )

#define CONFIGURE_SUITE(setup_fp, teardown_fp) do{\
  __test_setup = (setup_fp);\
  __test_teardown = (teardown_fp);\
} while( 0 )

#define TEST_REPORT() do{\
  double __test_timer_end;\
  int __test_iter;\
  __test_timer_end = test_timer();\
  for( __test_iter=0; __test_iter<__test_fail && __test_iter<TEST_N_MSG_STACK; __test_iter++){\
    printf( "\n" TEST_SEPARATOR1 );\
    printf( "%s", __test_messages[__test_iter] );\
  }\
  printf( "\n" TEST_SEPARATOR2 );\
  printf( "Ran %d tests in %.8fs\n\n", __test_run, __test_timer_end - __test_timer_time );\
  if( __test_fail == 0 ){\
    printf( "OK\n" );\
  } else {\
    printf( "FAILED (failures=%d)\n", __test_fail );\
  }\
} while( 0 )

#define TEST_EXIT() return __test_fail != 0

static double test_timer()
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
