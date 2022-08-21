#include "rhc_matsuoka_oscillator.h"
#include "rhc_test.h"

static mtoka_osci_neuron_t neuron;

void setup()
{
  mtoka_osci_neuron_init( &neuron );
}

void teardown()
{
  mtoka_osci_neuron_destroy( &neuron );
}

TEST(test_mtoka_osci_neuron_init)
{
  mtoka_osci_neuron_init( &neuron );
  ASSERT_EQ( 1.0, neuron.tau );
  ASSERT_EQ( 1.0, neuron.T );
  ASSERT_EQ( 0.0, neuron.a );
  ASSERT_EQ( 0.0, neuron.b );
  ASSERT_EQ( 0.0, neuron.c );
  ASSERT_EQ( 0.0, neuron.s );
  ASSERT_EQ( 0.0, neuron.x );
  ASSERT_EQ( 0.0, neuron.y );
  ASSERT_EQ( 0.0, neuron.v );
}

TEST(test_mtoka_osci_neuron)
{
  CONFIGURE_SUITE(setup, teardown);
  RUN_TEST(test_mtoka_osci_neuron_init);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_mtoka_osci_neuron);
  TEST_REPORT();
  TEST_EXIT();
}
