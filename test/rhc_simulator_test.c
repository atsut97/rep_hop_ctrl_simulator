#include "rhc_simulator.h"
#include "rhc_test.h"

cmd_t cmd;
ctrl_t ctrl;
model_t model;
simulator_t sim;
vec_t z0;

void setup()
{
  cmd_default_init( &cmd );
  ctrl_init( &ctrl, &cmd );
  model_init( &model, 1.0 );
  simulator_init( &sim, &cmd, &ctrl, &model );
  z0 = vec_create( 2 );
}

void teardown()
{
  vec_destroy( z0 );
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
  ASSERT_EQ( 0, simulator_time( &sim ) );
  ASSERT_EQ( 2, vec_size( simulator_state( &sim ) ) );
}

TEST(test_simulator_destroy)
{
  simulator_destroy( &sim );
  ASSERT_PTREQ( NULL, simulator_cmd( &sim ) );
  ASSERT_PTREQ( NULL, simulator_ctrl( &sim ) );
  ASSERT_PTREQ( NULL, simulator_model( &sim ) );
  ASSERT_EQ( 0, simulator_time( &sim ) );
  ASSERT_PTREQ( NULL, simulator_state( &sim ) );
}

TEST(test_simulator_run)
{
  double T;

  vec_set_elem_list( z0, 2, 0.28, 0.0 );
  T = 10;
  ASSERT_EQ( 0, simulator_time( &sim ) );
  simulator_run( &sim, z0, T );
  ASSERT_EQ( T, simulator_time( &sim ) );
}

TEST_SUITE(test_simulator)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_simulator_init);
  RUN_TEST(test_simulator_destroy);
  RUN_TEST(test_simulator_run);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_simulator);
  TEST_REPORT();
  TEST_EXIT();
}
