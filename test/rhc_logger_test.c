#include "rhc_logger.h"
#include "rhc_test.h"

logger_t logger;

void setup()
{
  /* logger_init( &logger, NULL ); */
}

void teardown()
{
  /* logger_destroy( &logger ); */
}

TEST(test_logger_init)
{
  logger_t l;
  logger_init( &l, "/tmp/test.log" );
  ASSERT_STREQ( "/tmp/test.log", logger_filename(&l) );
  ASSERT_PTRNE( NULL, l.fp );
  logger_destroy( &l );
}

TEST(test_logger_init_stdout)
{
  logger_t l;
  logger_init( &l, NULL );
  logger_destroy( &l );
}

TEST(test_logger_destroy)
{
  logger_t l;
  logger_init( &l, "/tmp/test.log" );
  logger_destroy( &l );
  ASSERT_PTREQ( NULL, l.fp );
}

TEST_SUITE(test_logger)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST( test_logger_init );
  RUN_TEST( test_logger_init_stdout );
  RUN_TEST( test_logger_destroy );
}

int main(int argc, char *argv[])
{
  RUN_SUITE( test_logger );
  TEST_REPORT();
  TEST_EXIT();
}
