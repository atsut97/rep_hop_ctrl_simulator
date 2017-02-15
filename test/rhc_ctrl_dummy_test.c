#include "rhc_ctrl_dummy.h"
#include "rhc_test.h"

static cmd_t cmd;
static ctrl_t ctrl;
static vec_t p;

TEST(test_ctrl_dummy_create)
{
  ctrl_dummy_create( &ctrl, &cmd, 10 );
  ASSERT_EQ( 10, ((ctrl_dummy_prp*)ctrl.prp)->k );
  ctrl_dummy_destroy( &ctrl );
}

TEST(test_ctrl_dummy_update)
{
  ctrl_dummy_create( &ctrl, &cmd, 10 );
  ctrl_update( &ctrl, 0, p );
  ASSERT_EQ( 10, ctrl_fz( &ctrl ) );
  ctrl_dummy_destroy( &ctrl );
}

TEST(test_ctrl_dummy_destroy)
{
  ctrl_dummy_create( &ctrl, &cmd, 10 );
  ASSERT_EQ( 10, ((ctrl_dummy_prp*)ctrl.prp)->k );
  ctrl_dummy_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
}

TEST_SUITE(test_ctrl_dummy)
{
  RUN_TEST(test_ctrl_dummy_create);
  RUN_TEST(test_ctrl_dummy_update);
  RUN_TEST(test_ctrl_dummy_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_dummy);
  TEST_REPORT();
  TEST_EXIT();
}
