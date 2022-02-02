#include "rhc_cmd.h"
#include "rhc_ctrl_raibert.h"
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

TEST(test_ctrl_raibert_create)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ASSERT_PTREQ( ctrl_cmd( &ctrl ), &cmd );
  ASSERT_PTREQ( ctrl_model( &ctrl ), &model );
  ASSERT_PTREQ( ctrl_raibert_update, ctrl._update );
  ASSERT_PTREQ( ctrl_raibert_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_raibert_header, ctrl._header );
  ASSERT_PTREQ( ctrl_raibert_writer, ctrl._writer );
  ASSERT_EQ( 0, ctrl_raibert_delta(&ctrl) );
  ASSERT_EQ( 0, ctrl_raibert_tau(&ctrl) );
  ASSERT_EQ( 0, ctrl_raibert_gamma(&ctrl) );
  ASSERT_EQ( 0, ctrl_raibert_yeta1(&ctrl) );
  ASSERT_EQ( 0, ctrl_raibert_zr(&ctrl) );
  ASSERT_EQ( 0, ctrl_raibert_mu(&ctrl) );
  ASSERT_PTRNE( NULL, ctrl.prp );
  ASSERT_EQ( none, ((ctrl_raibert_prp*)ctrl.prp)->type );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->tb );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->zb );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->tet );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->zet );
}

TEST(test_ctrl_raibert_destroy)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl_model( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
  ASSERT_EQ( 0, ctrl_n( &ctrl ) );
  ASSERT_EQ( 0, ctrl_phi( &ctrl ) );
}

TEST(test_ctrl_raibert_set_delta)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_delta( &ctrl, 0.01 );
  ASSERT_DOUBLE_EQ( 0.01, ctrl_raibert_delta(&ctrl) );
}

TEST(test_ctrl_raibert_set_tau)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_tau( &ctrl, 0.02 );
  ASSERT_DOUBLE_EQ( 0.02, ctrl_raibert_tau(&ctrl) );
}

TEST(test_ctrl_raibert_set_gamma)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_gamma( &ctrl, 0.03 );
  ASSERT_DOUBLE_EQ( 0.03, ctrl_raibert_gamma(&ctrl) );
}

TEST(test_ctrl_raibert_set_yeta1)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_yeta1( &ctrl, 0.04 );
  ASSERT_DOUBLE_EQ( 0.04, ctrl_raibert_yeta1(&ctrl) );
}

TEST(test_ctrl_raibert_set_zr)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_zr( &ctrl, 0.05 );
  ASSERT_DOUBLE_EQ( 0.05, ctrl_raibert_zr(&ctrl) );
}

TEST(test_ctrl_raibert_set_mu)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_mu( &ctrl, 0.06 );
  ASSERT_DOUBLE_EQ( 0.06, ctrl_raibert_mu(&ctrl) );
}

TEST(test_ctrl_raibert_set_params)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_params( &ctrl, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6 );
  ASSERT_DOUBLE_EQ( 0.1, ctrl_raibert_delta(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.2, ctrl_raibert_tau(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.3, ctrl_raibert_gamma(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.4, ctrl_raibert_yeta1(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.5, ctrl_raibert_zr(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.6, ctrl_raibert_mu(&ctrl) );
}

TEST(test_ctrl_raibert_set_params_full_nonlinear)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_params_full_nonlinear( &ctrl, 0.1, 0.2, 0.3, 0.4, 0.5 );
  ASSERT_DOUBLE_EQ( 0.1, ctrl_raibert_delta(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.2, ctrl_raibert_tau(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.3, ctrl_raibert_gamma(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.4, ctrl_raibert_yeta1(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.0, ctrl_raibert_zr(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.5, ctrl_raibert_mu(&ctrl) );
}

TEST(test_ctrl_raibert_set_params_simplified_nonlinear)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_params_simplified_nonlinear( &ctrl, 0.1, 0.2 );
  ASSERT_DOUBLE_EQ( 0.0, ctrl_raibert_delta(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.1, ctrl_raibert_tau(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.0, ctrl_raibert_gamma(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.2, ctrl_raibert_yeta1(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.0, ctrl_raibert_zr(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.0, ctrl_raibert_mu(&ctrl) );
}

TEST(test_ctrl_raibert_set_params_full_linear)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_params_full_linear( &ctrl, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6 );
  ASSERT_DOUBLE_EQ( 0.1, ctrl_raibert_delta(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.2, ctrl_raibert_tau(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.3, ctrl_raibert_gamma(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.4, ctrl_raibert_yeta1(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.5, ctrl_raibert_zr(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.6, ctrl_raibert_mu(&ctrl) );
}

TEST(test_ctrl_raibert_set_params_simplified_linear)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_params_simplified_linear( &ctrl, 0.1, 0.2, 0.3, 0.4, 0.5 );
  ASSERT_DOUBLE_EQ( 0.1, ctrl_raibert_delta(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.2, ctrl_raibert_tau(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.3, ctrl_raibert_gamma(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.4, ctrl_raibert_yeta1(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.5, ctrl_raibert_zr(&ctrl) );
  ASSERT_DOUBLE_EQ( 0.0, ctrl_raibert_mu(&ctrl) );
}

TEST(test_ctrl_raibert_create_full_nonlinear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 73.6, 90.5, 61.4, 8.7, 35.2, 3.4 },
    { 14.6, 43.4, 47.5, 62.2, 40.1, 47.8 },
    { 20.6, 95.4, 64.2, 61.4, 88.4, 69.3 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->tau>0; c++ ){
    ctrl_raibert_create_full_nonlinear( &ctrl, &cmd, &model, c->delta, c->tau, c->gamma, c->yeta1, c->mu );
    ASSERT_EQ( full_nonlinear, ctrl_raibert_type( &ctrl ) );
    ASSERT_EQ( c->delta, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->gamma, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( c->mu, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_create_simplified_nonlinear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 54.5, 15.7, 52.3, 52.6, 66.9, 62.3 },
    { 66.9, 42.4, 59.2, 24.1, 45.4, 62.8 },
    { 40.3, 70.5, 10.0, 63.7, 89.9, 86.4 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->tau>0; c++ ){
    ctrl_raibert_create_simplified_nonlinear( &ctrl, &cmd, &model, c->tau, c->yeta1 );
    ASSERT_EQ( simplified_nonlinear, ctrl_raibert_type( &ctrl ) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_create_full_linear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 4.2, 26.7, 65.1, 16.7, 12.7, 20.9 },
    { 47.5, 80.1, 36.0, 71.3, 27.9, 36.3 },
    { 28.3, 84.1, 77.1, 41.3, 57.6, 90.0 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->tau>0; c++ ){
    ctrl_raibert_create_full_linear( &ctrl, &cmd, &model, c->delta, c->tau, c->gamma, c->yeta1, c->zr, c->mu );
    ASSERT_EQ( full_linear, ctrl_raibert_type( &ctrl ) );
    ASSERT_EQ( c->delta, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->gamma, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( c->zr, ctrl_raibert_zr(&ctrl) );
    ASSERT_EQ( c->mu, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_create_simplified_linear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 29.6, 68.0, 42.1, 44.6, 48.7, 80.6 },
    { 64.3, 51.1, 66.6, 22.2, 80.0, 86.4 },
    { 85.8, 48.9, 5.4, 24.1, 48.1, 34.3 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->tau>0; c++ ){
    ctrl_raibert_create_simplified_linear( &ctrl, &cmd, &model, c->delta, c->tau, c->gamma, c->yeta1, c->zr );
    ASSERT_EQ( simplified_linear, ctrl_raibert_type( &ctrl ) );
    ASSERT_EQ( c->delta, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->gamma, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( c->zr, ctrl_raibert_zr(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST_SUITE(test_ctrl_raibert)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_raibert_create);
  RUN_TEST(test_ctrl_raibert_destroy);
  RUN_TEST(test_ctrl_raibert_set_delta);
  RUN_TEST(test_ctrl_raibert_set_tau);
  RUN_TEST(test_ctrl_raibert_set_gamma);
  RUN_TEST(test_ctrl_raibert_set_yeta1);
  RUN_TEST(test_ctrl_raibert_set_zr);
  RUN_TEST(test_ctrl_raibert_set_mu);
  RUN_TEST(test_ctrl_raibert_set_params);
  RUN_TEST(test_ctrl_raibert_set_params_full_nonlinear);
  RUN_TEST(test_ctrl_raibert_set_params_simplified_nonlinear);
  RUN_TEST(test_ctrl_raibert_set_params_full_linear);
  RUN_TEST(test_ctrl_raibert_set_params_simplified_linear);
  RUN_TEST(test_ctrl_raibert_create_full_nonlinear);
  RUN_TEST(test_ctrl_raibert_create_simplified_nonlinear);
  RUN_TEST(test_ctrl_raibert_create_full_linear);
  RUN_TEST(test_ctrl_raibert_create_simplified_linear);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_raibert);
  TEST_REPORT();
  TEST_EXIT();
}
