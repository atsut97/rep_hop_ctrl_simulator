#include "rhc_ctrl.h"
#include "rhc_test.h"

static cmd_t cmd;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  ctrl_init( &ctrl, &cmd );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_init)
{
  ASSERT_PTREQ( &cmd, ctrl_cmd( &ctrl ) );
}

TEST(test_ctrl_destroy)
{
  ctrl_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
}

TEST_SUITE(test_ctrl)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_init);
  RUN_TEST(test_ctrl_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl);
  TEST_REPORT();
  TEST_EXIT();
}
