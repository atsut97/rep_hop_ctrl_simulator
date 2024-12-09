#include "rhc_cmd.h"
#include "rhc_test.h"
#include "rhc_test_util.h"

cmd_t cmd;

void setup()
{
  cmd_init( &cmd );
}

void teardown()
{
  cmd_destroy( &cmd );
}

TEST(test_cmd_init)
{
  SHAM( &cmd, cmd_t );
  cmd_init( &cmd );
  ASSERT_EQ( 0, cmd.za );
  ASSERT_EQ( 0, cmd.zh );
  ASSERT_EQ( 0, cmd.zm );
  ASSERT_EQ( 0, cmd.zb );
  ASSERT_EQ( 0, cmd.zmax );
  ASSERT_EQ( 0, cmd.zmin );
}

TEST(test_cmd_init_regulator)
{
  SHAM( &cmd, cmd_t );
  cmd_init( &cmd );
  ASSERT_EQ( 0, cmd.regulator.q1 );
  ASSERT_EQ( 0, cmd.regulator.q2 );
}

TEST(test_cmd_init_rep_hop_stand)
{
  SHAM( &cmd, cmd_t );
  cmd_init( &cmd );
  ASSERT_EQ( 0, cmd.rep_hop_stand.rho );
  ASSERT_EQ( 0, cmd.rep_hop_stand.k );
}

TEST(test_cmd_init_raibert)
{
  SHAM( &cmd, cmd_t );
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
  SHAM( &cmd, cmd_t );
  cmd_init( &cmd );
  ASSERT_EQ( 0, cmd.arl.k );
  ASSERT_EQ( 0, cmd.arl.beta );
}

TEST(test_cmd_init_mtoka)
{
  SHAM( &cmd, cmd_t );
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
  SHAM( &cmd, cmd_t );
  cmd_default_init( &cmd );
  ASSERT_EQ( 0.28, cmd.za );
  ASSERT_EQ( 0.26, cmd.zh );
  ASSERT_EQ( 0.25, cmd.zm );
  ASSERT_EQ( 0.24, cmd.zb );
  ASSERT_EQ( 0.26, cmd.zmax );
  ASSERT_EQ( 0.24, cmd.zmin );
}


TEST(test_cmd_destroy)
{
  SHAM( &cmd, cmd_t );
  cmd_destroy( &cmd );
  ASSERT_EQ( 0, cmd.za );
  ASSERT_EQ( 0, cmd.zh );
  ASSERT_EQ( 0, cmd.zm );
  ASSERT_EQ( 0, cmd.zb );
  ASSERT_EQ( 0, cmd.zmax );
  ASSERT_EQ( 0, cmd.zmin );
}

TEST(test_cmd_copy)
{
  cmd_t cmd1, cmd2;
  SHAM( &cmd1, cmd_t);
  SHAM( &cmd2, cmd_t);

  ASSERT_PTREQ( &cmd2, cmd_copy( &cmd1, &cmd2 ) );
  ASSERT_EQ( cmd1.za, cmd2.za );
  ASSERT_EQ( cmd1.zh, cmd2.zh );
  ASSERT_EQ( cmd1.zm, cmd2.zm );
  ASSERT_EQ( cmd1.zb, cmd2.zb );
  ASSERT_EQ( cmd1.zmax, cmd2.zmax );
  ASSERT_EQ( cmd1.zmin, cmd2.zmin );
}

TEST_SUITE(test_cmd)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_cmd_init);
  RUN_TEST(test_cmd_init_regulator);
  RUN_TEST(test_cmd_init_rep_hop_stand);
  RUN_TEST(test_cmd_init_raibert);
  RUN_TEST(test_cmd_init_arl);
  RUN_TEST(test_cmd_init_mtoka);
  RUN_TEST(test_cmd_default_init);
  RUN_TEST(test_cmd_destroy);
  RUN_TEST(test_cmd_copy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_cmd);
  TEST_REPORT();
  TEST_EXIT();
}
