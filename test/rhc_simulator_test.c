#include "rhc_simulator.h"
#include "rhc_test.h"

cmd_t cmd;
ctrl_t ctrl;
model_t model;
simulator_t sim;

void setup()
{
  cmd_default_init( &cmd );
  ctrl_init( &ctrl, &cmd );
  model_init( &model, 1.0 );
  simulator_init( &sim, &cmd, &ctrl, &model );
}

void teardown()
{
  simulator_destroy( &sim );
  model_destroy( &model );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_simulator_init)
{
  ASSERT_PTREQ( &cmd, simulator_cmd( &sim ) );
  ASSERT_PTREQ( &ctrl, simulator_ctrl( &sim ) );
  ASSERT_PTREQ( &model, simulator_model( &sim ) );
}

TEST(test_simulator_destroy)
{
  simulator_destroy( &sim );
  ASSERT_PTREQ( NULL, simulator_cmd( &sim ) );
  ASSERT_PTREQ( NULL, simulator_ctrl( &sim ) );
  ASSERT_PTREQ( NULL, simulator_model( &sim ) );
}

TEST_SUITE(test_simulator)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_simulator_init);
  RUN_TEST(test_simulator_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_simulator);
  TEST_REPORT();
  TEST_EXIT();
}
