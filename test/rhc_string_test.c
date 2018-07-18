#include "rhc_string.h"
#include "rhc_test.h"

void setup()
{
}

void teardown()
{
}

TEST(test_string_copy)
{
  const char s[] = "test string";
  char d[BUFSIZ];
  char *ret;

  ret = string_copy(s, d);
  ASSERT_STREQ("test string", d);
  ASSERT_PTREQ(d, ret);
}

TEST_SUITE(test_string)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_string_copy);
}

int main(int arg, char *argv[])
{
  RUN_SUITE(test_string);
  TEST_REPORT();
  TEST_EXIT();
}
