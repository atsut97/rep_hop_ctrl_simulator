#include <fcntl.h>
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

int set_rand(cmd_t *cmd)
{
  int rnd = open("/dev/urandom", O_RDONLY);
  if( rnd < 0 ){
    RUNTIME_ERR("opening \"/dev/urandom\" has failed");
    return 1;
  } else {
    size_t len = 0;
    while( len < sizeof(cmd_t) ){
      ssize_t result = read( rnd, cmd + len, sizeof(cmd_t) - len );
      if( result < 0 ){
        RUNTIME_ERR("reading file handler \"rnd\" has failed");
        close( rnd );
        return 1;
      }
      len += result;
    }
    close( rnd );
  }
  return 0;
}

TEST(test_cmd_init)
{
  ASSERT(!set_rand( &cmd ), "Fail randomizing cmd_t instance" );
  cmd_init( &cmd );
  ASSERT_EQ( 0, cmd.zd );
  ASSERT_EQ( 0, cmd.z0 );
  ASSERT_EQ( 0, cmd.zb );
}

TEST(test_cmd_init_regulator)
{
  ASSERT(!set_rand( &cmd ), "Fail randomizing cmd_t instance" );
  cmd_init( &cmd );
  ASSERT_EQ( 0, cmd.regulator.q1 );
  ASSERT_EQ( 0, cmd.regulator.q2 );
}

TEST(test_cmd_init_raibert)
{
  ASSERT(!set_rand( &cmd ), "Fail randomizing cmd_t instance" );
  cmd_init( &cmd );
  ASSERT_EQ( 0, cmd.raibert.delta );
  ASSERT_EQ( 0, cmd.raibert.tau );
  ASSERT_EQ( 0, cmd.raibert.gamma );
  ASSERT_EQ( 0, cmd.raibert.yeta1 );
  ASSERT_EQ( 0, cmd.raibert.zr );
  ASSERT_EQ( 0, cmd.raibert.mu );
}

TEST(test_cmd_init_arl)
{
  ASSERT(!set_rand( &cmd ), "Fail randomizing cmd_t instance" );
  cmd_init( &cmd );
  ASSERT_EQ( 0, cmd.arl.k );
  ASSERT_EQ( 0, cmd.arl.beta );
}

TEST(test_cmd_init_mtoka)
{
  ASSERT(!set_rand( &cmd ), "Fail randomizing cmd_t instance" );
  cmd_init( &cmd );
  ASSERT_EQ( 0, cmd.mtoka.tau );
  ASSERT_EQ( 0, cmd.mtoka.T );
  ASSERT_EQ( 0, cmd.mtoka.a );
  ASSERT_EQ( 0, cmd.mtoka.b );
  ASSERT_EQ( 0, cmd.mtoka.c );
  ASSERT_EQ( 0, cmd.mtoka.th );
  ASSERT_EQ( 0, cmd.mtoka.mu );
  ASSERT_EQ( 0, cmd.mtoka.rho );
  ASSERT_EQ( 0, cmd.mtoka.lam );
}

TEST(test_cmd_default_init)
{
  ASSERT(!set_rand( &cmd ), "Fail randomizing cmd_t instance" );
  cmd_default_init( &cmd );
  ASSERT_EQ( 0.28, cmd.zd );
  ASSERT_EQ( 0.26, cmd.z0 );
  ASSERT_EQ( 0.24, cmd.zb );
}


TEST(test_cmd_destroy)
{
  ASSERT(!set_rand( &cmd ), "Fail randomizing cmd_t instance" );
  cmd_destroy( &cmd );
  ASSERT_EQ( 0, cmd.zd );
  ASSERT_EQ( 0, cmd.z0 );
  ASSERT_EQ( 0, cmd.zb );
}

TEST_SUITE(test_cmd)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_cmd_init);
  RUN_TEST(test_cmd_init_regulator);
  RUN_TEST(test_cmd_init_raibert);
  RUN_TEST(test_cmd_init_arl);
  RUN_TEST(test_cmd_init_mtoka);
  RUN_TEST(test_cmd_default_init);
  RUN_TEST(test_cmd_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_cmd);
  TEST_REPORT();
  TEST_EXIT();
}
