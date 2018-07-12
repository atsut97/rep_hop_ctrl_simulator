#include "rhc_logger.h"
#include "rhc_test.h"

logger_t logger;

void setup()
{
  logger_init( &logger );
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
}

TEST(test_logger_close)
{
  logger_open( &logger, "/tmp/test.log" );
  logger_close( &logger );
  ASSERT_PTREQ( NULL, logger.fp );
}

TEST_SUITE(test_logger)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST( test_logger_init );
  RUN_TEST( test_logger_destroy );
  RUN_TEST( test_logger_open );
  RUN_TEST( test_logger_close );
}

int main(int argc, char *argv[])
{
  RUN_SUITE( test_logger );
  TEST_REPORT();
  TEST_EXIT();
}
