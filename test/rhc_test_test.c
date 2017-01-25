#include "rhc_test.h"

int foo = 7;
int bar = 4;

RHC_TEST(test_foo)
{
  RHC_ASSERT( foo == 7, "foo should be 7" );
}

RHC_TEST(test_bar)
{
  RHC_ASSERT( bar == 5, "bar should be 5" );
}

RHC_TEST_SUITE(all_tests)
{
  RHC_RUN_TEST( test_foo );
  RHC_RUN_TEST( test_foo );
  RHC_RUN_TEST( test_bar );
  RHC_RUN_TEST( test_foo );
  RHC_RUN_TEST( test_bar );
  RHC_RUN_TEST( test_foo );
}

int main(int argc, char *argv[])
{
  RHC_RUN_SUITE( all_tests );
  RHC_TEST_REPORT();
  RHC_TEST_EXIT();
}
