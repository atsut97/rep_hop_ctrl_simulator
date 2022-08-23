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
  ASSERT_EQ( 1.0, mtoka_osci_neuron_rise_time_const(&neuron) );
  ASSERT_EQ( 1.0, mtoka_osci_neuron_adapt_time_const(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_mutual_inhibit_weight(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_steady_firing_rate(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_tonic_input(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_sensory_feedback(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_membrane_potential(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_firing_rate(&neuron) );
  ASSERT_EQ( 0.0, mtoka_osci_neuron_adapt_property(&neuron) );
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

TEST(test_mtoka_osci_neuron_set_mutual_inhibit_weight)
{
  double cases[] = {9.0, 0.3, 4.5, 0};
  double *c;
  for( c=cases; *c>0; c++ ){
    mtoka_osci_neuron_set_mutual_inhibit_weight( &neuron, *c );
    ASSERT_EQ( *c, mtoka_osci_neuron_mutual_inhibit_weight(&neuron) );
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

TEST(test_mtoka_osci_neuron_set_params)
{
  struct case_t {
    double tau, T, a, b;
  } cases[] = {
    { 4.5, 7.9, 6.4, 3.4 },
    { 5.4, 4.1, 6.6, 2.4 },
    { 6.5, 9.3, 0.3, 6.1 },
    { 0.0, 0.0, 0.0, 0.0 },
  };
  struct case_t *c;

  for( c=cases; c->tau>0; c++ ){
    mtoka_osci_neuron_set_params( &neuron, c->tau, c->T, c->a, c->b );
    ASSERT_EQ( c->tau, mtoka_osci_neuron_rise_time_const(&neuron) );
    ASSERT_EQ( c->T, mtoka_osci_neuron_adapt_time_const(&neuron) );
    ASSERT_EQ( c->a, mtoka_osci_neuron_mutual_inhibit_weight(&neuron) );
    ASSERT_EQ( c->b, mtoka_osci_neuron_steady_firing_rate(&neuron) );
  }
}

TEST(test_mtoka_osci_neuron)
{
  CONFIGURE_SUITE(setup, teardown);
  RUN_TEST(test_mtoka_osci_neuron_init);
  RUN_TEST(test_mtoka_osci_neuron_set_rise_time_const);
  RUN_TEST(test_mtoka_osci_neuron_set_adapt_time_const);
  RUN_TEST(test_mtoka_osci_neuron_set_mutual_inhibit_weight);
  RUN_TEST(test_mtoka_osci_neuron_set_steady_firing_rate);
  RUN_TEST(test_mtoka_osci_neuron_set_tonic_input);
  RUN_TEST(test_mtoka_osci_neuron_set_sensory_feedback);
  RUN_TEST(test_mtoka_osci_neuron_set_params);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_mtoka_osci_neuron);
  TEST_REPORT();
  TEST_EXIT();
}
