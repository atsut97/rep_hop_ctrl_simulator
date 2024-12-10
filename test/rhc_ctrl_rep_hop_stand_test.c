#include "rhc_ctrl.h"
#include "rhc_ctrl_rep_hop_stand.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  ctrl_rep_hop_stand_create( &ctrl, &cmd, &model, none );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_rep_hop_stand_create)
{
  ASSERT_PTREQ( ctrl_cmd( &ctrl ), &cmd );
  ASSERT_PTREQ( ctrl_model( &ctrl ), &model );
  ASSERT_PTREQ( ctrl_rep_hop_stand_update, ctrl._update );
  ASSERT_PTREQ( ctrl_rep_hop_stand_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_rep_hop_stand_header, ctrl._header );
  ASSERT_PTREQ( ctrl_rep_hop_stand_writer, ctrl._writer );
  ASSERT_PTRNE( NULL, ctrl.prp );

  ASSERT_EQ( none, ctrl_rep_hop_stand_type(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_q1(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_q2(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_vh(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_vm(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_r(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_sqr_gamma(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_f_gamma(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_phase(&ctrl) );

  ASSERT_EQ( 0, ctrl_rep_hop_stand_rho(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_k(&ctrl) );

  ASSERT_EQ( 0.28, ctrl_rep_hop_stand_params_za(&ctrl) );
  ASSERT_EQ( 0.26, ctrl_rep_hop_stand_params_zh(&ctrl) );
  ASSERT_EQ( 0.25, ctrl_rep_hop_stand_params_zm(&ctrl) );
  ASSERT_EQ( 0.24, ctrl_rep_hop_stand_params_zb(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_params_rho(&ctrl) );
}

TEST(test_ctrl_rep_hop_stand_destroy)
{
  ctrl_rep_hop_stand_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl_model( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
  ASSERT_EQ( 0, ctrl_n( &ctrl ) );
  ASSERT_EQ( 0, ctrl_phi( &ctrl ) );
}

TEST(test_ctrl_rep_hop_stand_set_rho)
{
  struct case_t {
    double rho;
  } cases[] = {
    {1.0}, {0.5}, {0.0}, {-1.0}
  };
  struct case_t *c;

  for( c=cases; c->rho>0; c++ ){
    ctrl_rep_hop_stand_set_rho( &ctrl, c->rho );
    ASSERT_EQ( c->rho, ctrl_rep_hop_stand_rho(&ctrl) );
  }
}

TEST(test_ctrl_rep_hop_stand_set_k)
{
  struct case_t {
    double k;
  } cases[] = {
    {1.0}, {0.5}, {0.0}, {-1.0}
  };
  struct case_t *c;

  for( c=cases; c->k>0; c++ ){
    ctrl_rep_hop_stand_set_k( &ctrl, c->k );
    ASSERT_EQ( c->k, ctrl_rep_hop_stand_k(&ctrl) );
  }
}

TEST_SUITE(test_ctrl_rep_hop_stand)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_rep_hop_stand_create);
  RUN_TEST(test_ctrl_rep_hop_stand_destroy);
  RUN_TEST(test_ctrl_rep_hop_stand_set_rho);
  RUN_TEST(test_ctrl_rep_hop_stand_set_k);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_rep_hop_stand);
  TEST_REPORT();
  TEST_EXIT();
}
