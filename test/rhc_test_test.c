#include "rhc_test.h"

int foo = 7;
int bar = 4;
double baz = 0.1;
char hoge[] = "hoge";

TEST(test_assert)
{
  ASSERT( foo == 7, "foo should be 7" );
}

TEST(test_assert_fail)
{
  ASSERT( bar == 5, "bar should be 5" );
}

TEST(test_true)
{
  ASSERT_TRUE( foo == 7 );
}

TEST(test_true_fail)
{
  ASSERT_TRUE( bar == 5 );
}

TEST(test_false)
{
  ASSERT_FALSE( bar == 5 );
  ASSERT_FALSE( NULL );
}

TEST(test_false_fail)
{
  ASSERT_FALSE( foo == 7 );
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

TEST(test_assert_streq)
{
  ASSERT_STREQ( "hoge", hoge );
}

TEST(test_assert_streq_fail)
{
  ASSERT_STREQ( "fuga", hoge );
}

TEST(test_assert_ptreq)
{
  int *foop = &foo;

  ASSERT_PTREQ( &foo, foop );
  foop = NULL;
  ASSERT_PTREQ( NULL, foop );
  ASSERT_PTREQ( NULL, NULL );
}

TEST(test_assert_ptreq_fail)
{
  int *foop = &foo;

  ASSERT_PTREQ( &bar, foop );
}

TEST(test_assert_ptreq_fail2)
{
  int *foop = &foo;

  ASSERT_PTREQ( NULL, foop );
}

TEST_SUITE(all_tests)
{
  RUN_TEST( test_assert );
  RUN_TEST( test_true );
  RUN_TEST( test_false );
  RUN_TEST( test_assert_eq );
  RUN_TEST( test_assert_double_eq );
  RUN_TEST( test_assert_gt );
  RUN_TEST( test_assert_ge );
  RUN_TEST( test_assert_lt );
  RUN_TEST( test_assert_le );
  RUN_TEST( test_assert_streq );
  RUN_TEST( test_assert_ptreq );

  RUN_TEST( test_assert_fail );
  RUN_TEST( test_true_fail );
  RUN_TEST( test_false_fail );
  RUN_TEST( test_assert_eq_fail );
  RUN_TEST( test_assert_double_eq_fail );
  RUN_TEST( test_assert_gt_fail );
  RUN_TEST( test_assert_ge_fail );
  RUN_TEST( test_assert_lt_fail );
  RUN_TEST( test_assert_le_fail );
  RUN_TEST( test_assert_streq_fail );
  RUN_TEST( test_assert_ptreq_fail );
  RUN_TEST( test_assert_ptreq_fail2 );
}

int main(int argc, char *argv[])
{
  RUN_SUITE( all_tests );
  TEST_REPORT();
  TEST_EXIT();
}
