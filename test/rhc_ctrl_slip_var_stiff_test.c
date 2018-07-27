#include "rhc_ctrl_slip_var_stiff.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  ctrl_slip_var_stiff_create( &ctrl, &cmd, &model );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_slip_var_stiff_create)
{
  ASSERT_PTREQ( ctrl_slip_var_stiff_update, ctrl._update );
  ASSERT_PTREQ( ctrl_slip_var_stiff_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_slip_var_stiff_header, ctrl._header );
  ASSERT_PTREQ( ctrl_slip_var_stiff_writer, ctrl._writer );
  ASSERT_EQ( 0, ((ctrl_slip_var_stiff_prp*)ctrl.prp)->k );
}

TEST(test_ctrl_slip_var_stiff_destroy)
{
  ctrl_t c;
  ctrl_slip_var_stiff_create( &c, &cmd, &model );
  ASSERT_PTRNE( NULL, ctrl_cmd( &c ) );
  ASSERT_PTRNE( NULL, c.prp );
  ctrl_destroy( &c );
  ASSERT_PTREQ( NULL, ctrl_cmd( &c ) );
  ASSERT_PTREQ( NULL, c.prp );
}

TEST_SUITE(test_ctrl_slip_var_stiff)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_slip_var_stiff_create);
  RUN_TEST(test_ctrl_slip_var_stiff_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_slip_var_stiff);
  TEST_REPORT();
  TEST_EXIT();
}
