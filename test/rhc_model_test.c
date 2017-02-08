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

TEST_SUITE(test_model)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_model_init);
  RUN_TEST(test_model_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_model);
  TEST_REPORT();
  TEST_EXIT();
}
