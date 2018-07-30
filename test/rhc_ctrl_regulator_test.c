#include "rhc_ctrl_regulator.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 1 );
  ctrl_regulator_create( &ctrl, &cmd, &model );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_regulator_cmd_init)
{
  ctrl_regulator_cmd_init( &ctrl, &cmd );
  ASSERT_EQ( 1.0, ctrl_regulator_q1( &ctrl ) );
  ASSERT_EQ( 1.0, ctrl_regulator_q2( &ctrl ) );
}

TEST(test_ctrl_regulator_create)
{
  ASSERT_PTREQ( ctrl_regulator_update, ctrl._update );
  ASSERT_PTREQ( ctrl_regulator_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_regulator_header, ctrl._header );
  ASSERT_PTREQ( ctrl_regulator_writer, ctrl._writer );
  ASSERT_EQ( 0, ((ctrl_regulator_prp*)ctrl.prp)->xi );
}

TEST_SUITE(test_ctrl_regulator)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_regulator_cmd_init);
  RUN_TEST(test_ctrl_regulator_create);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_regulator);
  TEST_REPORT();
  TEST_EXIT();
}
