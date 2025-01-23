#include "rhc_logger.h"
#include "rhc_simulator.h"
#include "rhc_test.h"

logger_t logger;
double t;
vec_t state;
cmd_t cmd;
model_t model;
ctrl_t ctrl;
simulator_t simulator;

char TEST_LOG_FILENAME[] = "/tmp/rhc_test.log";

void setup()
{
  logger_init( &logger );
  t = 0;
  state = vec_create( 3 );
  vec_clear( state );
  cmd_init( &cmd );
  model_init( &model, 1.0 );
  ctrl_init( &ctrl, &cmd, &model );
  simulator_init( &simulator, &cmd, &ctrl, &model );
}

void teardown()
{
  logger_destroy( &logger );
  simulator_destroy( &simulator );
  ctrl_destroy( &ctrl );
  model_destroy( &model );
  cmd_destroy( &cmd );
  vec_destroy( state );
}

TEST(test_logger_init)
{
  logger_t l;
  logger_init( &l );
  ASSERT_EQ( '\0', logger_filename( &l )[0] );
  ASSERT_PTREQ( NULL, l.fp );
  ASSERT_PTREQ( NULL, l.header );
  ASSERT_PTREQ( NULL, l.writer );
  ASSERT_FALSE( logger_is_header_written( &l ) );
  ASSERT_STREQ( "\n", logger_eol( &l ) );
  logger_destroy( &l );
}

TEST(test_logger_destroy)
{
  logger_t l;
  logger_init( &l );
  logger_destroy( &l );
  ASSERT_PTREQ( NULL, l.fp );
}

TEST(test_logger_set_eol)
{
  ASSERT_STREQ( "\n", logger_eol( &logger ) );
  logger_set_eol( &logger, "\r\n" );
  ASSERT_STREQ( "\r\n", logger_eol( &logger ) );
}

TEST(test_logger_open)
{
  logger_open( &logger, TEST_LOG_FILENAME );
  ASSERT_STREQ( TEST_LOG_FILENAME, logger_filename(&logger) );
  ASSERT_PTRNE( NULL, logger.fp );
  ASSERT_PTREQ( NULL, logger.header );
  ASSERT_PTREQ( NULL, logger.writer );
  ASSERT_FALSE( logger_is_header_written( &logger ) );
  logger_close( &logger );
}

TEST(test_logger_close)
{
  logger_open( &logger, TEST_LOG_FILENAME );
  logger_close( &logger );
  ASSERT_PTREQ( NULL, logger.fp );
}

void header(FILE *fp, simulator_t *s, void *util) {
  fprintf( fp, "t,x,y,z,fe,za,zh,zb,m,az\n");
}

void output(FILE *fp, simulator_t *s, void *util) {
  vec_t state = simulator_state(s);
  cmd_t *cmd = simulator_cmd(s);
  model_t *model = simulator_model(s);
  fprintf( fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
           simulator_time(s),
           vec_elem(state,0), vec_elem(state,1), vec_elem(state,2),
           simulator_fe(s), cmd->za, cmd->zh, cmd->zb,
           model_mass(model), model_acc(model) );
}

TEST(test_logger_register)
{
  logger_register( &logger, header, output );
  ASSERT_PTREQ( header, logger.header );
  ASSERT_PTREQ( output, logger.writer );
}

TEST(test_logger_delegate)
{
  logger_t src, dst;
  FILE *tmp_fp;
  bool is_header_written = false;

  logger_open( &src, TEST_LOG_FILENAME );
  tmp_fp = src.fp;
  logger_register( &src, header, output );
  is_header_written = logger_is_header_written( &src );
  logger_delegate( &src, &dst );
  ASSERT_STREQ( "", logger_filename(&src) );
  ASSERT_PTREQ( NULL, src.fp );
  ASSERT_PTREQ( NULL, src.header );
  ASSERT_PTREQ( NULL, src.writer );
  ASSERT_STREQ( TEST_LOG_FILENAME, logger_filename(&dst) );
  ASSERT_PTREQ( tmp_fp, dst.fp );
  ASSERT_PTREQ( header, dst.header );
  ASSERT_PTREQ( output, dst.writer );
  ASSERT_EQ( is_header_written, logger_is_header_written(&dst) );
  logger_close( &src );
  logger_close( &dst );
}

TEST(test_logger_delegate_2)
{
  logger_t src, dst;
  FILE *tmp_fp;
  bool is_header_written = false;

  logger_open( &src, TEST_LOG_FILENAME );
  tmp_fp = src.fp;
  logger_register( &src, header, output );
  ECHO_OFF();
  logger_write_data( &logger, &simulator, NULL );
  ECHO_ON();
  is_header_written = logger_is_header_written( &src );
  logger_delegate( &src, &dst );
  ASSERT_STREQ( "", logger_filename(&src) );
  ASSERT_PTREQ( NULL, src.fp );
  ASSERT_PTREQ( NULL, src.header );
  ASSERT_PTREQ( NULL, src.writer );
  ASSERT_STREQ( TEST_LOG_FILENAME, logger_filename(&dst) );
  ASSERT_PTREQ( tmp_fp, dst.fp );
  ASSERT_PTREQ( header, dst.header );
  ASSERT_PTREQ( output, dst.writer );
  ASSERT_EQ( is_header_written, logger_is_header_written(&dst) );
  logger_close( &src );
  logger_close( &dst );
}

TEST(test_logger_write_header)
{
  logger_open( &logger, TEST_LOG_FILENAME );
  logger_register( &logger, header, output );
  ASSERT_FALSE( logger_is_header_written(&logger) );
  logger_write_header( &logger, &simulator, NULL );
  ASSERT_TRUE( logger_is_header_written(&logger) );
  logger_write_header( &logger, &simulator, NULL );
  ASSERT_TRUE( logger_is_header_written(&logger) );
  logger_close( &logger );
}

TEST(test_logger_write)
{
  logger_register( &logger, header, output );
  /* logger_write( &logger, 0.001, state, -0.1, &cmd, &model, NULL ); */
  /* logger_write( &logger, 0.001, state, -0.1, &cmd, &model, NULL ); */
}

TEST(test_logger_write_data)
{
  logger_register( &logger, header, output );
  /* logger_write_data( &logger, 0.001, state, -0.1, &cmd, &model, NULL ); */
  /* logger_write_data( &logger, 0.001, state, -0.1, &cmd, &model, NULL ); */
}

TEST(test_logger_write_not_regiseter_header)
{
  logger_register( &logger, NULL, output );
  /* logger_write_data( &logger, 0.0, state, 0.0, &cmd, &model, NULL); */
}

TEST(test_logger_write_not_regiseter_writer)
{
  ECHO_OFF();
  logger_write_data( &logger, &simulator, NULL);
  ECHO_ON();
}

TEST(test_logger_is_open)
{
  ASSERT_FALSE( logger_is_open(&logger) );
  logger_open( &logger, TEST_LOG_FILENAME );
  ASSERT_TRUE( logger_is_open(&logger) );
  logger_close( &logger );
  ASSERT_FALSE( logger_is_open(&logger) );
}

TEST(test_logger_create)
{
  logger_t *l = logger_create( TEST_LOG_FILENAME, header, output );
  ASSERT_STREQ( TEST_LOG_FILENAME, logger_filename(l) );
  ASSERT_PTRNE( NULL, l->fp );
  ASSERT_PTREQ( output, l->writer );
  logger_destroy( l );
  free( l );
}

TEST_SUITE(test_logger)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST( test_logger_init );
  RUN_TEST( test_logger_destroy );
  RUN_TEST(test_logger_set_eol);
  RUN_TEST( test_logger_open );
  RUN_TEST( test_logger_close );
  RUN_TEST( test_logger_register );
  RUN_TEST( test_logger_delegate );
  RUN_TEST( test_logger_delegate_2 );
  RUN_TEST( test_logger_write );
  RUN_TEST( test_logger_write_header );
  RUN_TEST( test_logger_write_data );
  RUN_TEST( test_logger_write_not_regiseter_header );
  RUN_TEST( test_logger_write_not_regiseter_writer );
  RUN_TEST( test_logger_is_open );
  RUN_TEST( test_logger_create );
}

int main(int argc, char *argv[])
{
  RUN_SUITE( test_logger );
  TEST_REPORT();
  TEST_EXIT();
}
