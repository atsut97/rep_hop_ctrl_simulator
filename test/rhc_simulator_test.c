#include "rhc_ctrl_dummy.h"
#include "rhc_logger.h"
#include "rhc_simulator.h"
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
  ASSERT_EQ( 0, simulator_step( &sim ) );
  ASSERT_EQ( 2, vec_size( simulator_state( &sim ) ) );
  ASSERT_EQ( 0, simulator_fe( &sim ) );
  ASSERT_PTREQ( simulator_dp, sim.ode.f );
  ASSERT_PTRNE( NULL, sim.ode._ws );
  ASSERT_EQ( 0, simulator_n_trial( &sim ) );
  ASSERT_PTREQ( simulator_reset_ctrl, sim.reset_fp );
  ASSERT_PTREQ( simulator_update, sim.update_fp );
  ASSERT_PTREQ( simulator_dump, sim.dump_fp );
  ASSERT_STREQ("", simulator_tag( &sim ) );
}

TEST(test_simulator_destroy)
{
  simulator_destroy( &sim );
  ASSERT_PTREQ( NULL, simulator_cmd( &sim ) );
  ASSERT_PTREQ( NULL, simulator_ctrl( &sim ) );
  ASSERT_PTREQ( NULL, simulator_model( &sim ) );
  ASSERT_EQ( 0, simulator_time( &sim ) );
  ASSERT_EQ( 0, simulator_step( &sim ) );
  ASSERT_PTREQ( NULL, simulator_state( &sim ) );
  ASSERT_EQ( 0, simulator_fe( &sim ) );
  ASSERT_EQ( 0, simulator_n_trial( &sim ) );
  ASSERT_STREQ("", simulator_tag( &sim ) );
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

TEST(test_simulator_inc_step)
{
  ASSERT_EQ( 0, simulator_step( &sim ) );
  simulator_inc_step( &sim );
  ASSERT_EQ( 1, simulator_step( &sim ) );
  simulator_inc_step( &sim );
  ASSERT_EQ( 2, simulator_step( &sim ) );
  simulator_inc_step( &sim );
  ASSERT_EQ( 3, simulator_step( &sim ) );
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

struct reset_fp_prp{
  bool ret_val;
};

bool reset_test(simulator_t *simulator, void *util)
{
  struct reset_fp_prp *prp = util;
  return prp->ret_val;
}

TEST(test_simulator_set_reset_fp)
{
  simulator_set_reset_fp( &sim, reset_test );
  ASSERT_PTREQ( reset_test, sim.reset_fp );
}

bool update_test(simulator_t *simulator, double dt, double t, void *util)
{
  return true;
}

TEST(test_simulator_set_update_fp)
{
  simulator_set_update_fp( &sim, update_test );
  ASSERT_PTREQ( update_test, sim.update_fp );
}

void dump_test(simulator_t *simulator, logger_t *logger, void *util)
{}

TEST(test_simulator_set_dump_fp)
{
  simulator_set_dump_fp( &sim, dump_test );
  ASSERT_PTREQ( dump_test, sim.dump_fp );
}

TEST(test_simulator_set_tag)
{
  char tag[] = "test tag";

  simulator_set_tag( &sim, tag );
  ASSERT_STREQ( "test tag", simulator_tag( &sim ) );
}

TEST(test_simulator_update_default_tag)
{
  simulator_update_default_tag( &sim );
  ASSERT_STREQ( "sc00000", simulator_tag(&sim) );
  simulator_inc_trial( &sim );
  simulator_update_default_tag( &sim );
  ASSERT_STREQ( "sc00001", simulator_tag(&sim) );
}

TEST(test_simulator_has_default_tag)
{
  simulator_set_tag( &sim, "" );
  ASSERT_TRUE( simulator_has_default_tag( &sim ) );
  sim.n_trial = 1;
  simulator_set_tag( &sim, "00000" );
  ASSERT_TRUE( simulator_has_default_tag( &sim ) );
  simulator_set_tag( &sim, "sc00000" );
  ASSERT_TRUE( simulator_has_default_tag( &sim ) );
  simulator_set_tag( &sim, "hoge00000" );
  ASSERT_TRUE( simulator_has_default_tag( &sim ) );
  simulator_set_tag( &sim, "test" );
  ASSERT_FALSE( simulator_has_default_tag( &sim ) );
  simulator_set_tag( &sim, "test033" );
  ASSERT_FALSE( simulator_has_default_tag( &sim ) );
}

bool reset_test_failure(simulator_t *simulator, void *util)
{
  return false;
}

TEST(test_simulator_reset_ctrl_return_fail)
{
  simulator_set_reset_fp( &sim, reset_test_failure );
  ASSERT_FALSE( simulator_reset( &sim, NULL ) );
}

TEST(test_simulator_reset)
{
  simulator_update_time( &sim, 0.01 );
  ASSERT_NE( 0.0, simulator_time( &sim ) );
  ASSERT_NE( 0, simulator_step( &sim ) );
  ASSERT_TRUE( simulator_reset( &sim, NULL ) );
  ASSERT_EQ( 0.0, simulator_time( &sim ) );
  ASSERT_EQ( 0, simulator_step( &sim ) );
}

TEST(test_simulator_reset_check_fail)
{
  struct reset_fp_prp prp = { false };

  RESET_ERR_MSG();
  ECHO_OFF();
  simulator_set_reset_fp( &sim, reset_test );
  simulator_reset( &sim, &prp );
  ASSERT_STREQ( ERR_RESET_FAIL, __err_last_msg );
  ECHO_ON();
}

TEST(test_simulator_update)
{
  double dt;

  vec_set_elem_list( z, 2, 0.28, 0.0 );
  dt = 0.01;
  simulator_update_time( &sim, dt );
  ASSERT_EQ( 0.01, simulator_time( &sim ) );
  ASSERT_EQ( 1, simulator_step( &sim ) );
}

TEST(test_simulator_run)
{
  double T, dt;

  vec_set_elem_list( z, 2, 0.28, 0.0 );
  T = 10;
  dt = 0.01;
  ASSERT_EQ( 0, simulator_time( &sim ) );
  ASSERT_EQ( 0, simulator_step( &sim ) );
  simulator_run( &sim, z, T, dt, NULL, NULL );
  ASSERT_DOUBLE_EQ( T, simulator_time( &sim ) );
  ASSERT_EQ( (int)(T/dt), simulator_step( &sim ) );
  ASSERT_EQ( 1, simulator_n_trial( &sim ) );
  ASSERT_STREQ( "sc00000", simulator_tag( &sim ) );
}

TEST(test_simulator_run_multiple_times)
{
  double T, dt;

  vec_set_elem_list( z, 2, 0.28, 0.0 );
  T = 10;
  dt = 0.01;
  ASSERT_EQ( 0, simulator_time( &sim ) );
  ASSERT_EQ( 0, simulator_step( &sim ) );
  simulator_run( &sim, z, T, dt, NULL, NULL );
  ASSERT_DOUBLE_EQ( T, simulator_time( &sim ) );
  ASSERT_EQ( (int)(T/dt), simulator_step( &sim ) );
  ASSERT_EQ( 1, simulator_n_trial( &sim ) );
  ASSERT_STREQ( "sc00000", simulator_tag( &sim ) );
  T = 0.8;
  simulator_run( &sim, z, T, dt, NULL, NULL );
  ASSERT_DOUBLE_EQ( T, simulator_time( &sim ) );
  ASSERT_EQ( (int)(T/dt), simulator_step( &sim ) );
  ASSERT_EQ( 2, simulator_n_trial( &sim ) );
  ASSERT_STREQ( "sc00001", simulator_tag( &sim ) );
}

TEST(test_simulator_run_specify_tag)
{
  double T, dt;
  char tag[] = "test";

  vec_set_elem_list( z, 2, 0.28, 0.0 );
  T = 10;
  dt = 0.01;
  simulator_set_tag( &sim, tag );
  simulator_run( &sim, z, T, dt, NULL, NULL );
  ASSERT_STREQ( tag, simulator_tag( &sim ) );
}

TEST_SUITE(test_simulator)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_simulator_init);
  RUN_TEST(test_simulator_destroy);
  RUN_TEST(test_simulator_set_reset_fp);
  RUN_TEST(test_simulator_set_update_fp);
  RUN_TEST(test_simulator_set_dump_fp);
  RUN_TEST(test_simulator_set_state);
  RUN_TEST(test_simulator_inc_step);
  RUN_TEST(test_simulator_set_fe);
  RUN_TEST(test_simulator_set_tag);
  RUN_TEST(test_simulator_update_default_tag);
  RUN_TEST(test_simulator_has_default_tag);
  RUN_TEST(test_simulator_reset_ctrl_return_fail);
  RUN_TEST(test_simulator_reset);
  RUN_TEST(test_simulator_reset_check_fail);
  RUN_TEST(test_simulator_update);
  RUN_TEST(test_simulator_run);
  RUN_TEST(test_simulator_run_multiple_times);
  RUN_TEST(test_simulator_run_specify_tag);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_simulator);
  TEST_REPORT();
  TEST_EXIT();
}
