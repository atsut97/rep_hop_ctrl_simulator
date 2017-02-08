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

TEST(test_model_destroy)
{
  model_t m;
  cmd_t cmd;

  model_init( &m, 1.0, &cmd );
  model_destroy( &m );
  ASSERT_EQ( 0.0, model_mass( &m ) );
  ASSERT_PTREQ( NULL, model_cmd( &m ) );
}

TEST_SUITE(test_model)
{
  RUN_TEST(test_model_init);
  RUN_TEST(test_model_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_model);
  TEST_REPORT();
  TEST_EXIT();
}
