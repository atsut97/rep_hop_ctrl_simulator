#include "rhc_cmd.h"
#include "rhc_test.h"

cmd_t cmd;

void setup()
{
  srand( 0 );
  cmd_init( &cmd );
}

void teardown()
{
  cmd_destroy( &cmd );
}

void set_rand(cmd_t *cmd)
{
  cmd->zd = rand();
  cmd->z0 = rand();
  cmd->zb = rand();
}

TEST(test_cmd_init)
{
  set_rand( &cmd );
  cmd_init( &cmd );
  ASSERT_EQ( 0, cmd.zd );
  ASSERT_EQ( 0, cmd.z0 );
  ASSERT_EQ( 0, cmd.zb );
}

TEST(test_cmd_destroy)
{
  set_rand( &cmd );
  cmd_destroy( &cmd );
  ASSERT_EQ( 0, cmd.zd );
  ASSERT_EQ( 0, cmd.z0 );
  ASSERT_EQ( 0, cmd.zb );
}

TEST_SUITE(test_cmd)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_cmd_init);
  RUN_TEST(test_cmd_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_cmd);
  TEST_REPORT();
  TEST_EXIT();
}
