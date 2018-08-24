#include "rhc_vec_ring.h"
#include "rhc_test.h"

#define SIZE 3
vec_ring_t ring;

void setup()
{
  vec_ring_init( &ring, SIZE );
}

void teardown()
{
  vec_ring_destroy( &ring );
}

TEST(test_vec_ring_init)
{
  ASSERT_EQ( 0, vec_ring_size(&ring) );
}

TEST_SUITE(test_vec_ring)
{
  CONFIGURE_SUITE(setup, teardown);
  RUN_TEST(test_vec_ring_init);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_vec_ring);
  TEST_REPORT();
  TEST_EXIT();
}
