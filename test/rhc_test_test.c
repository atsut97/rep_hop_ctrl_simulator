#include "rhc_test.h"

int foo = 7;
int bar = 4;

RHC_TEST(test_assert)
{
  RHC_ASSERT( foo == 7, "foo should be 7" );
}

RHC_TEST(test_assert_fail)
{
  RHC_ASSERT( bar == 5, "bar should be 5" );
}

RHC_TEST(test_assert_eq)
{
  RHC_ASSERT_EQ( 7, foo );
}

RHC_TEST(test_assert_eq_fail)
{
  RHC_ASSERT_EQ( 5, bar );
}

RHC_TEST_SUITE(all_tests)
{
  RHC_RUN_TEST( test_assert );
  RHC_RUN_TEST( test_assert_fail );
  RHC_RUN_TEST( test_assert_eq );
  RHC_RUN_TEST( test_assert_eq_fail );
}

int main(int argc, char *argv[])
{
  RHC_RUN_SUITE( all_tests );
  RHC_TEST_REPORT();
  RHC_TEST_EXIT();
}
