#include "rhc_ctrl.h"
#include "rhc_ctrl_arl.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup() {
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  p = vec_create( 2 );
}

void teardown() {
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_arl_create)
{
  ctrl_arl_create( &ctrl, &cmd, &model, none );
  ASSERT_PTREQ( ctrl_cmd( &ctrl ), &cmd );
  ASSERT_PTREQ( ctrl_model( &ctrl ), &model );
  ASSERT_PTREQ( ctrl_arl_update, ctrl._update );
  ASSERT_PTREQ( ctrl_arl_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_arl_header, ctrl._header );
  ASSERT_PTREQ( ctrl_arl_writer, ctrl._writer );
  ASSERT_EQ( 0, ctrl_arl_k(&ctrl) );
  ASSERT_EQ( 0, ctrl_arl_beta(&ctrl) );
  ASSERT_PTRNE( NULL, ctrl.prp );
  ASSERT_EQ( none, ((ctrl_arl_prp*)ctrl.prp)->type );
  ASSERT_EQ( 0, ((ctrl_arl_prp*)ctrl.prp)->sqr_R0 );
  ASSERT_EQ( 0, ((ctrl_arl_prp*)ctrl.prp)->sqr_R );
  ASSERT_EQ( 0, ((ctrl_arl_prp*)ctrl.prp)->delta );
}

TEST(test_ctrl_arl_destroy)
{
  ctrl_arl_create( &ctrl, &cmd, &model, none );
  ctrl_arl_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl_model( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
  ASSERT_EQ( 0, ctrl_n( &ctrl ) );
  ASSERT_EQ( 0, ctrl_phi( &ctrl ) );
}

TEST(test_ctrl_arl_set_k)
{
  double k = 10.0;
  ctrl_arl_create( &ctrl, &cmd, &model, none );
  ctrl_arl_set_k( &ctrl, k );
  ASSERT_EQ( k, ctrl_arl_k(&ctrl) );
}

TEST(test_ctrl_arl_set_beta)
{
  double beta = 500.0;
  ctrl_arl_create( &ctrl, &cmd, &model, none );
  ctrl_arl_set_beta( &ctrl, beta );
  ASSERT_EQ( beta, ctrl_arl_beta(&ctrl) );
}

TEST(test_ctrl_arl_set_params)
{
  struct case_t {
    double k, beta;
  } cases[] = {
    { 82.8, 73.8 },
    { 31.0, 15.1 },
    { 30.2, 89.9 },
    { 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->k>0; c++ ){
    ctrl_arl_create( &ctrl, &cmd, &model, none );
    ctrl_arl_set_params( &ctrl, c->k, c->beta );
    ASSERT_EQ( c->k, ctrl_arl_k(&ctrl) );
    ASSERT_EQ( c->beta, ctrl_arl_beta(&ctrl) );
    ctrl_arl_destroy( &ctrl );
  }
}

TEST_SUITE(test_ctrl_arl)
{
  CONFIGURE_SUITE(setup, teardown);
  RUN_TEST(test_ctrl_arl_create);
  RUN_TEST(test_ctrl_arl_destroy);
  RUN_TEST(test_ctrl_arl_set_k);
  RUN_TEST(test_ctrl_arl_set_beta);
  RUN_TEST(test_ctrl_arl_set_params);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_arl);
  TEST_REPORT();
  TEST_EXIT();
}
