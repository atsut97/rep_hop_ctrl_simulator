#include "rhc_simulator.h"
#include "rhc_ctrl_dummy.h"
#include "rhc_test.h"

cmd_t cmd;
ctrl_t ctrl;
model_t model;
simulator_t sim;
vec_t z;

void setup()
{
  cmd_default_init( &cmd );
  ctrl_dummy_create( &ctrl, &cmd, &model, 10 );
  model_init( &model, 1.0 );
  simulator_init( &sim, &cmd, &ctrl, &model );
  z = vec_create( 2 );
}

void teardown()
{
  vec_destroy( z );
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
  ASSERT_EQ( 0, simulator_fe( &sim ) );
  ASSERT_PTREQ( simulator_dp, sim.ode.f );
  ASSERT_PTRNE( NULL, sim.ode._ws );
}

TEST(test_simulator_destroy)
{
  simulator_destroy( &sim );
  ASSERT_PTREQ( NULL, simulator_cmd( &sim ) );
  ASSERT_PTREQ( NULL, simulator_ctrl( &sim ) );
  ASSERT_PTREQ( NULL, simulator_model( &sim ) );
  ASSERT_EQ( 0, simulator_time( &sim ) );
  ASSERT_PTREQ( NULL, simulator_state( &sim ) );
  ASSERT_EQ( 0, simulator_fe( &sim ) );
}

TEST(test_simulator_set_state)
{
  struct case_t {
    double x1, x2;
    bool abort;
  } cases[] = {
    { 0, 0, false },
    { 10, -20, false },
    { -5, 13, false },
    { -3, -22, false },
    { 0, 0, true },
  };
  struct case_t *c;

  for( c=cases; !c->abort; c++ ){
    vec_set_elem_list( z, 2, c->x1, c->x2 );
    simulator_set_state( &sim, z );
    ASSERT_EQ( c->x1, vec_elem( simulator_state(&sim), 0 ) );
    ASSERT_EQ( c->x2, vec_elem( simulator_state(&sim), 1 ) );
  }
}

TEST(test_simulator_inc_time)
{
  double dt;

  dt = 0.01;
  ASSERT_EQ( 0, simulator_time( &sim ) );
  simulator_inc_time( &sim, dt );
  ASSERT_EQ( 0.01, simulator_time( &sim ) );
  simulator_inc_time( &sim, dt );
  ASSERT_EQ( 0.02, simulator_time( &sim ) );
  simulator_inc_time( &sim, dt );
  ASSERT_EQ( 0.03, simulator_time( &sim ) );

  dt = 0.02;
  simulator_inc_time( &sim, dt );
  ASSERT_EQ( 0.05, simulator_time( &sim ) );
  simulator_inc_time( &sim, dt );
  ASSERT_EQ( 0.07, simulator_time( &sim ) );
}

TEST(test_simulator_set_fe)
{
  double cases[] = { 0.1, 0.3, 0.5, 0.0 };
  double *c;

  for( c=cases; *c>0; c++ ){
    simulator_set_fe( &sim, *c );
    ASSERT_EQ( *c, simulator_fe( &sim ) );
  }
}

TEST(test_simulator_update)
{
  double dt;

  vec_set_elem_list( z, 2, 0.28, 0.0 );
  dt = 0.01;
  simulator_update( &sim, 0.0, dt );
  ASSERT_EQ( 0.01, simulator_time( &sim ) );
}

TEST(test_simulator_run)
{
  double T, dt;

  vec_set_elem_list( z, 2, 0.28, 0.0 );
  T = 10;
  dt = 0.01;
  ASSERT_EQ( 0, simulator_time( &sim ) );
  simulator_run( &sim, z, T, dt, NULL );
  ASSERT_DOUBLE_EQ( T+dt, simulator_time( &sim ) );
}

TEST_SUITE(test_simulator)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_simulator_init);
  RUN_TEST(test_simulator_destroy);
  RUN_TEST(test_simulator_set_state);
  RUN_TEST(test_simulator_inc_time);
  RUN_TEST(test_simulator_set_fe);
  RUN_TEST(test_simulator_update);
  RUN_TEST(test_simulator_run);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_simulator);
  TEST_REPORT();
  TEST_EXIT();
}
