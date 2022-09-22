#include "rhc_cmd.h"
#include "rhc_ctrl.h"
#include "rhc_ctrl_mtoka.h"
#include "rhc_model.h"
#include "rhc_mtoka_osci.h"
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
  register int i;
  mtoka_osci_neuron_t *np;

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
  ASSERT_PTRNE( NULL, ctrl.prp );
  ASSERT_EQ( 2, mtoka_osci_n_neuron(ctrl_mtoka_osci(&ctrl)) );
  for( i=0; i<2; i++ ){
    np = mtoka_osci_neuron( ctrl_mtoka_osci(&ctrl), i );
    ASSERT_EQ( 1.0, mtoka_osci_neuron_rise_time_const(np) );
    ASSERT_EQ( 1.0, mtoka_osci_neuron_adapt_time_const(np) );
    ASSERT_EQ( 2, vec_size(mtoka_osci_neuron_mutual_inhibit_weights(np)) );
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_neuron_mutual_inhibit_weights(np), 0) );
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_neuron_mutual_inhibit_weights(np), 1) );
    ASSERT_EQ( 0.0, mtoka_osci_neuron_steady_firing_rate(np) );
    ASSERT_EQ( 0.0, mtoka_osci_neuron_firing_threshold(np) );
  }
}

TEST(test_ctrl_mtoka_destroy)
{
  ctrl_mtoka_create( &ctrl, &cmd, &model );
  ctrl_mtoka_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl_model( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
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
