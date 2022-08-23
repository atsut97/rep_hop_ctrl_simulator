#include "rhc_matsuoka_oscillator.h"
#include "rhc_test.h"

static mtoka_osci_neuron_t neuron;

void setup()
{
  mtoka_osci_neuron_init( &neuron, 2 );
}

void teardown()
{
  mtoka_osci_neuron_destroy( &neuron );
}

TEST(test_mtoka_osci_neuron_init)
{
  mtoka_osci_neuron_init( &neuron, 2 );
  ASSERT_EQ( 1.0, mtoka_osci_neuron_rise_time_const(&neuron) );
  ASSERT_EQ( 1.0, mtoka_osci_neuron_adapt_time_const(&neuron) );
  ASSERT_EQ( 2, vec_size(mtoka_osci_neuron_mutual_inhibit_weights(&neuron)) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_steady_firing_rate(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_firing_threshold(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_tonic_input(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_sensory_feedback(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_membrane_potential(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_firing_rate(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_adapt_property(&neuron) );
}

TEST(test_mtoka_osci_neuron_destroy)
{
  mtoka_osci_neuron_destroy( &neuron );
  ASSERT_PTREQ( NULL, mtoka_osci_neuron_mutual_inhibit_weights(&neuron) );
}

TEST(test_mtoka_osci_neuron_set_rise_time_const)
{
  double cases[] = {0.5, 6.5, 4.8, 0};
  double *c;
  for( c=cases; *c>0; c++ ){
    mtoka_osci_neuron_set_rise_time_const( &neuron, *c );
    ASSERT_EQ( *c, mtoka_osci_neuron_rise_time_const(&neuron) );
  }
}

TEST(test_mtoka_osci_neuron_set_adapt_time_const)
{
  double cases[] = {8.1, 4.4, 4.1, 0};
  double *c;
  for( c=cases; *c>0; c++ ){
    mtoka_osci_neuron_set_adapt_time_const( &neuron, *c );
    ASSERT_EQ( *c, mtoka_osci_neuron_adapt_time_const(&neuron) );
  }
}

TEST(test_mtoka_osci_neuron_set_steady_firing_rate)
{
  double cases[] = {0.7, 3.5, 6.6, 0};
  double *c;
  for( c=cases; *c>0; c++ ){
    mtoka_osci_neuron_set_steady_firing_rate( &neuron, *c );
    ASSERT_EQ( *c, mtoka_osci_neuron_steady_firing_rate(&neuron) );
  }
}

TEST(test_mtoka_osci_neuron_set_firing_threshold)
{
  double cases[] = {4.1, 5.8, 6.7, 0};
  double *c;
  for( c=cases; *c>0; c++ ){
    mtoka_osci_neuron_set_firing_threshold( &neuron, *c );
    ASSERT_EQ( *c, mtoka_osci_neuron_firing_threshold(&neuron) );
  }
}

TEST(test_mtoka_osci_neuron_set_tonic_input)
{
  double cases[] = {7.5, 6.0, 4.9, 0};
  double *c;
  for( c=cases; *c>0; c++ ){
    mtoka_osci_neuron_set_tonic_input( &neuron, *c );
    ASSERT_EQ( *c, mtoka_osci_neuron_tonic_input(&neuron) );
  }
}

TEST(test_mtoka_osci_neuron_set_sensory_feedback)
{
  double cases[] = {4.1, 0.8, 8.1, 0};
  double *c;
  for( c=cases; *c>0; c++ ){
    mtoka_osci_neuron_set_sensory_feedback( &neuron, *c );
    ASSERT_EQ( *c, mtoka_osci_neuron_sensory_feedback(&neuron) );
  }
}

TEST(test_mtoka_osci_neuron)
{
  CONFIGURE_SUITE(setup, teardown);
  RUN_TEST(test_mtoka_osci_neuron_init);
  RUN_TEST(test_mtoka_osci_neuron_destroy);
  RUN_TEST(test_mtoka_osci_neuron_set_rise_time_const);
  RUN_TEST(test_mtoka_osci_neuron_set_adapt_time_const);
  RUN_TEST(test_mtoka_osci_neuron_set_steady_firing_rate);
  RUN_TEST(test_mtoka_osci_neuron_set_firing_threshold);
  RUN_TEST(test_mtoka_osci_neuron_set_tonic_input);
  RUN_TEST(test_mtoka_osci_neuron_set_sensory_feedback);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_mtoka_osci_neuron);
  TEST_REPORT();
  TEST_EXIT();
}
