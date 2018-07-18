#include "rhc_logger.h"
#include "rhc_test.h"

logger_t logger;
double t;
vec_t state;
cmd_t cmd;
model_t model;

void setup()
{
  logger_init( &logger );
  t = 0;
  state = vec_create( 3 );
  vec_clear( state );
  cmd_init( &cmd );
  model_init( &model, 1.0 );
}

void teardown()
{
  logger_destroy( &logger );
}

TEST(test_logger_init)
{
  logger_t l;
  logger_init( &l );
  ASSERT_PTREQ( NULL, l.fp );
  ASSERT_PTREQ( NULL, l.writer );
  logger_destroy( &l );
}

TEST(test_logger_destroy)
{
  logger_t l;
  logger_init( &l );
  logger_destroy( &l );
  ASSERT_PTREQ( NULL, l.fp );
}

TEST(test_logger_open)
{
  logger_open( &logger, "/tmp/test.log" );
  ASSERT_STREQ( "/tmp/test.log", logger_filename(&logger) );
  ASSERT_PTRNE( NULL, logger.fp );
  logger_close( &logger );
}

TEST(test_logger_close)
{
  logger_open( &logger, "/tmp/test.log" );
  logger_close( &logger );
  ASSERT_PTREQ( NULL, logger.fp );
}

void output(FILE *fp, double t, vec_t state, double fe, cmd_t *cmd, model_t *model, void *util) {
  fprintf( fp, "%f %f %f %f %f %f %f %f %f %f\n",
           t, vec_elem(state,0), vec_elem(state,1), vec_elem(state,2),
           fe, cmd->zd, cmd->z0, cmd->zb,
           model_mass(model), model_acc(model) );
}

TEST(test_logger_register)
{
  logger_register( &logger, output );
  ASSERT_PTREQ( output, logger.writer );
}

TEST(test_logger_write)
{
  logger_register( &logger, output );
  /* logger_write( &logger, 0.001, state, -0.1, &cmd, &model, NULL ); */
}

TEST(test_logger_write_not_regiseter_writer)
{
  ECHO_OFF();
  logger_write( &logger, 0.0, state, 0.0, &cmd, &model, NULL);
  ECHO_ON();
}

TEST(test_logger_is_open)
{
  ASSERT_FALSE( logger_is_open(&logger) );
  logger_open( &logger, "/tmp/test.log" );
  ASSERT_TRUE( logger_is_open(&logger) );
  logger_close( &logger );
  ASSERT_FALSE( logger_is_open(&logger) );
}

TEST_SUITE(test_logger)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST( test_logger_init );
  RUN_TEST( test_logger_destroy );
  RUN_TEST( test_logger_open );
  RUN_TEST( test_logger_close );
  RUN_TEST( test_logger_register );
  RUN_TEST( test_logger_write );
  RUN_TEST( test_logger_write_not_regiseter_writer );
  RUN_TEST( test_logger_is_open );
}

int main(int argc, char *argv[])
{
  RUN_SUITE( test_logger );
  TEST_REPORT();
  TEST_EXIT();
}
