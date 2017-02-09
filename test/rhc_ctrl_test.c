#include "rhc_ctrl.h"
#include "rhc_test.h"

TEST(test_ctrl_init)
{
  cmd_t cmd;
  ctrl_t ctrl;

  ctrl_init( &ctrl, &cmd );
  ASSERT_PTREQ( &cmd, ctrl_cmd( &ctrl ) );
  ctrl_destroy( &ctrl );
}

TEST(test_ctrl_destroy)
{
  cmd_t cmd;
  ctrl_t ctrl;

  ctrl_init( &ctrl, &cmd );
  ctrl_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
}

TEST_SUITE(test_ctrl)
{
  RUN_TEST(test_ctrl_init);
  RUN_TEST(test_ctrl_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl);
  TEST_REPORT();
  TEST_EXIT();
}
