#include "rhc_test.h"

int foo = 7;
int bar = 4;
double baz = 0.1;

TEST(test_assert)
{
  ASSERT( foo == 7, "foo should be 7" );
}

TEST(test_assert_fail)
{
  ASSERT( bar == 5, "bar should be 5" );
}

TEST(test_assert_eq)
{
  ASSERT_EQ( 7, foo );
}

TEST(test_assert_eq_fail)
{
  ASSERT_EQ( 5, bar );
}

TEST(test_assert_double_eq)
{
  ASSERT_DOUBLE_EQ( 0.1, baz );
}

TEST(test_assert_double_eq_fail)
{
  ASSERT_DOUBLE_EQ( 0.2, baz );
}

TEST(test_assert_gt)
{
  ASSERT_GT( 13, foo );
}

TEST(test_assert_gt_fail)
{
  ASSERT_GT( 5, foo );
}

TEST(test_assert_ge)
{
  ASSERT_GE( 13, foo );
  ASSERT_GE( 7,  foo );
}

TEST(test_assert_ge_fail)
{
  ASSERT_GE( 5, foo );
}

TEST(test_assert_lt)
{
  ASSERT_LT( 5, foo );
}

TEST(test_assert_lt_fail)
{
  ASSERT_LT( 13, foo );
}

TEST(test_assert_le)
{
  ASSERT_LE( 5, foo );
  ASSERT_LE( 7,  foo );
}

TEST(test_assert_le_fail)
{
  ASSERT_LE( 13, foo );
}

TEST_SUITE(all_tests)
{
  RUN_TEST( test_assert );
  RUN_TEST( test_assert_eq );
  RUN_TEST( test_assert_double_eq );
  RUN_TEST( test_assert_gt );
  RUN_TEST( test_assert_ge );
  RUN_TEST( test_assert_lt );
  RUN_TEST( test_assert_le );

  RUN_TEST( test_assert_fail );
  RUN_TEST( test_assert_eq_fail );
  RUN_TEST( test_assert_double_eq_fail );
  RUN_TEST( test_assert_gt_fail );
  RUN_TEST( test_assert_ge_fail );
  RUN_TEST( test_assert_lt_fail );
  RUN_TEST( test_assert_le_fail );}

int main(int argc, char *argv[])
{
  RUN_SUITE( all_tests );
  TEST_REPORT();
  TEST_EXIT();
}
