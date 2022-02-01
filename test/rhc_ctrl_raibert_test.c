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
  ctrl_raibert_create_full_nonlinear( &ctrl, &cmd, &model );
  ASSERT_EQ( full_nonlinear, ctrl_raibert_type( &ctrl ) );
}

TEST(test_ctrl_raibert_create_simplified_nonlinear)
{
  ctrl_raibert_create_simplified_nonlinear( &ctrl, &cmd, &model );
  ASSERT_EQ( simplified_nonlinear, ctrl_raibert_type( &ctrl ) );
}

TEST(test_ctrl_raibert_create_full_linear)
{
  ctrl_raibert_create_full_linear( &ctrl, &cmd, &model );
  ASSERT_EQ( full_linear, ctrl_raibert_type( &ctrl ) );
}

TEST(test_ctrl_raibert_create_simplified_linear)
{
  ctrl_raibert_create_simplified_linear( &ctrl, &cmd, &model );
  ASSERT_EQ( simplified_linear, ctrl_raibert_type( &ctrl ) );
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
