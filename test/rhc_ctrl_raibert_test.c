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
  ASSERT_PTRNE( NULL, ctrl.prp );
  ASSERT_EQ( none, ((ctrl_raibert_prp*)ctrl.prp)->type );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->delta );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->tau );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->gamma );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->yeta1 );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->zr );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->mu );
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
