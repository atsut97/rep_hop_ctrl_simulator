#include "rhc_test_util.h"
#include "rhc_test.h"

void setup() {

}

void teardown() {

}

TEST(test_test_util_set_spurious_values)
{
  double val, val_orig;
  val = val_orig = 0;
  set_spurious_values( &val, sizeof(double) );
  ASSERT_NE( val_orig, val );
  val_orig = val;
  set_spurious_values( &val, sizeof(double) );
  ASSERT_NE( val_orig, val );
}

TEST(test_test_util_spurious)
{
  typedef struct _C {
    double x;
    double y;
  } C;
  C c;
  c.x = c.y = 0.0;
  SHAM( &c, C );
  ASSERT_NE( 0.0, c.x );
  ASSERT_NE( 0.0, c.y );
}

TEST_SUITE(test_test_util)
{
  CONFIGURE_SUITE( &setup, &teardown );
  RUN_TEST(test_test_util_set_spurious_values);
  RUN_TEST(test_test_util_spurious);
}

int main(int argc, char *argv[])
{
  RUN_SUITE( test_test_util );
  TEST_REPORT();
  TEST_EXIT();
}
