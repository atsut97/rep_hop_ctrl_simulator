#include "rhc_model.h"
#include "rhc_test.h"

TEST(test_model_init)
{
  model_t m;
  cmd_t cmd;

  model_init( &m, 1.0, &cmd );
  ASSERT_EQ( 1.0, model_mass( &m ) );
  ASSERT_PTREQ( &cmd, model_cmd( &m ) );
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
