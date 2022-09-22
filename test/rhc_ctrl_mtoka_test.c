#include "rhc_cmd.h"
#include "rhc_ctrl.h"
#include "rhc_ctrl_mtoka.h"
#include "rhc_model.h"
#include "rhc_test.h"
#include "rhc_vec.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_mtoka_create)
{
  ctrl_mtoka_create( &ctrl, &cmd, &model );
  ASSERT_PTREQ( ctrl_cmd( &ctrl ), &cmd );
  ASSERT_PTREQ( ctrl_model( &ctrl ), &model );
  ASSERT_PTREQ( ctrl_mtoka_update, ctrl._update );
  ASSERT_PTREQ( ctrl_mtoka_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_mtoka_header, ctrl._header );
  ASSERT_PTREQ( ctrl_mtoka_writer, ctrl._writer );
  ASSERT_EQ( 0, ctrl_mtoka_rise_time_const(&ctrl) );
  ASSERT_EQ( 0, ctrl_mtoka_adapt_time_const(&ctrl) );
  ASSERT_EQ( 0, ctrl_mtoka_mutual_inhibit_weights(&ctrl) );
  ASSERT_EQ( 0, ctrl_mtoka_steady_firing_rate(&ctrl) );
  ASSERT_EQ( 0, ctrl_mtoka_steady_firing_rate(&ctrl) );
  ASSERT_EQ( 0, ctrl_mtoka_firing_threshold(&ctrl) );
  ASSERT_EQ( 0, ctrl_mtoka_feedback_gain(&ctrl) );
  ASSERT_EQ( 0, ctrl_mtoka_sensory_gain(&ctrl) );
  ASSERT_EQ( 0, ctrl_mtoka_saturation_gain(&ctrl) );
  /* ASSERT_PTRNE( NULL, ctrl.prp ); */
  /* ASSERT_EQ( none, ((ctrl_mtoka_prp*)ctrl.prp)->type ); */
  /* ASSERT_EQ( 0, ((ctrl_mtoka_prp*)ctrl.prp)->end_of_thrust.t ); */
  /* ASSERT_EQ( 0, ((ctrl_mtoka_prp*)ctrl.prp)->end_of_thrust.z ); */
  /* ASSERT_EQ( 0, ((ctrl_mtoka_prp*)ctrl.prp)->end_of_thrust.v ); */
  /* ASSERT_FALSE( ((ctrl_mtoka_prp*)ctrl.prp)->is_in_thrust ); */

}

TEST(test_ctrl_mtoka_destroy)
{
  ctrl_mtoka_create( &ctrl, &cmd, &model );
  ctrl_mtoka_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl_model( &ctrl ) );
  /* ASSERT_PTREQ( NULL, ctrl.prp ); */
  ASSERT_EQ( 0, ctrl_n( &ctrl ) );
  ASSERT_EQ( 0, ctrl_phi( &ctrl ) );
}

TEST_SUITE(test_ctrl_mtoka)
{
  CONFIGURE_SUITE(setup, teardown);
  RUN_TEST(test_ctrl_mtoka_create);
  RUN_TEST(test_ctrl_mtoka_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_mtoka);
  TEST_REPORT();
  TEST_EXIT();
}
