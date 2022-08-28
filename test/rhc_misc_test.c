#include "rhc_test.h"
#include "rhc_misc.h"

TEST(test_misc_min)
{
  ASSERT_EQ( 1.0, min( 1.0, 3.0 ) );
  ASSERT_EQ( 2.3, min( 4.1, 2.3 ) );
  ASSERT_EQ( 0, min( 0, 1 ) );
  ASSERT_EQ( -1, min( 1, -1 ) );
  ASSERT_EQ( -3, min( -3, 1.0 ) );
  ASSERT_EQ( 1.5, min( 2, 1.5 ) );
}

TEST(test_misc_max)
{
  ASSERT_EQ( 3.0, max( 1.0, 3.0 ) );
  ASSERT_EQ( 4.1, max( 4.1, 2.3 ) );
  ASSERT_EQ( 1, max( 0, 1 ) );
  ASSERT_EQ( 1, max( 1, -1 ) );
  ASSERT_EQ( 1.0, max( -3, 1.0 ) );
  ASSERT_EQ( 2, max( 2, 1.5 ) );
}

TEST_SUITE(test_misc)
{
  RUN_TEST(test_misc_min);
  RUN_TEST(test_misc_max);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_misc);
  TEST_REPORT();
  TEST_EXIT();
}
