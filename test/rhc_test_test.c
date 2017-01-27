#include "rhc_test.h"

int foo = 7;
int bar = 4;
double baz = 0.1;

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

RHC_TEST(test_assert_double_eq)
{
  RHC_ASSERT_DOUBLE_EQ( 0.1, baz );
}

RHC_TEST(test_assert_double_eq_fail)
{
  RHC_ASSERT_DOUBLE_EQ( 0.2, baz );
}

RHC_TEST(test_assert_gt)
{
  RHC_ASSERT_GT( 13, foo );
}

RHC_TEST(test_assert_gt_fail)
{
  RHC_ASSERT_GT( 5, foo );
}

RHC_TEST(test_assert_ge)
{
  RHC_ASSERT_GE( 13, foo );
  RHC_ASSERT_GE( 7,  foo );
}

RHC_TEST(test_assert_ge_fail)
{
  RHC_ASSERT_GE( 5, foo );
}

RHC_TEST(test_assert_lt)
{
  RHC_ASSERT_LT( 5, foo );
}

RHC_TEST(test_assert_lt_fail)
{
  RHC_ASSERT_LT( 13, foo );
}

RHC_TEST(test_assert_le)
{
  RHC_ASSERT_LE( 5, foo );
  RHC_ASSERT_LE( 7,  foo );
}

RHC_TEST(test_assert_le_fail)
{
  RHC_ASSERT_LE( 13, foo );
}

RHC_TEST_SUITE(all_tests)
{
  RHC_RUN_TEST( test_assert );
  RHC_RUN_TEST( test_assert_eq );
  RHC_RUN_TEST( test_assert_double_eq );
  RHC_RUN_TEST( test_assert_gt );
  RHC_RUN_TEST( test_assert_ge );
  RHC_RUN_TEST( test_assert_lt );
  RHC_RUN_TEST( test_assert_le );

  RHC_RUN_TEST( test_assert_fail );
  RHC_RUN_TEST( test_assert_eq_fail );
  RHC_RUN_TEST( test_assert_double_eq_fail );
  RHC_RUN_TEST( test_assert_gt_fail );
  RHC_RUN_TEST( test_assert_ge_fail );
  RHC_RUN_TEST( test_assert_lt_fail );
  RHC_RUN_TEST( test_assert_le_fail );}

int main(int argc, char *argv[])
{
  RHC_RUN_SUITE( all_tests );
  RHC_TEST_REPORT();
  RHC_TEST_EXIT();
}
