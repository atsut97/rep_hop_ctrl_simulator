#include "rhc_model.h"
#include "rhc_test.h"

TEST(test_model_init)
{
  model_t m;

  model_init( &m, 1.0 );
  ASSERT_EQ( 1.0, model_mass( &m ) );
}

TEST_SUITE(test_model)
{
  RUN_TEST(test_model_init);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_model);
  TEST_REPORT();
  TEST_EXIT();
}
