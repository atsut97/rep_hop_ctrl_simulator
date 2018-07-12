#include "rhc_logger.h"
#include "rhc_test.h"

logger_t logger;

void setup()
{
}

void teardown()
{
}

TEST(test_logger_init)
{
  logger_t l;
  logger_init(&l, "test.log");
  ASSERT_STREQ( "test.log", logger_filename(&l) );
}

TEST_SUITE(test_logger)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_logger_init);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_logger);
  TEST_REPORT();
  TEST_EXIT();
}
