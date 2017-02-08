#include "rhc_model.h"
#include "rhc_test.h"

model_t model;
cmd_t cmd;

void setup()
{
  model_init( &model, 1.0, &cmd );
}

void teardown()
{
  model_destroy( &model );
}

TEST(test_model_init)
{
  ASSERT_EQ( 1.0, model_mass( &model ) );
  ASSERT_PTREQ( &cmd, model_cmd( &model ) );
}

TEST(test_model_destroy)
{
  model_destroy( &model );
  ASSERT_EQ( 0.0, model_mass( &model ) );
  ASSERT_PTREQ( NULL, model_cmd( &model ) );
}

TEST(test_model_set_mass)
{
  model_set_mass( &model, 2.0 );
  ASSERT_EQ( 2.0, model_mass(&model) );
  model_set_mass( &model, 4.0 );
  ASSERT_EQ( 4.0, model_mass(&model) );
}

TEST(test_model_set_cmd)
{
  cmd_t cmd1, cmd2;

  model_set_cmd( &model, &cmd1 );
  ASSERT_PTREQ( &cmd1, model_cmd(&model) );
  model_set_cmd( &model, &cmd2 );
  ASSERT_PTREQ( &cmd2, model_cmd(&model) );
}

TEST_SUITE(test_model)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_model_init);
  RUN_TEST(test_model_destroy);
  RUN_TEST(test_model_set_mass);
  RUN_TEST(test_model_set_cmd);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_model);
  TEST_REPORT();
  TEST_EXIT();
}
