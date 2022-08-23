#include "rhc_matsuoka_oscillator.h"
#include "rhc_test.h"
#include "rhc_vec.h"

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
  ASSERT_EQ( 0.0, vec_elem(mtoka_osci_neuron_mutual_inhibit_weights(&neuron), 0) );
  ASSERT_EQ( 0.0, vec_elem(mtoka_osci_neuron_mutual_inhibit_weights(&neuron), 1) );
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

TEST(test_mtoka_osci_neuron_dxdt)
{
  struct case_t {
    double tau, a, b, c, s, x, y, v;
  } cases[] = {
    { 5.2, 6.9, 2.6, 5.0, 0.2, 4.8, 4.7, 1.5 },
    { 8.2, 6.0, 8.2, 9.5, 3.2, 4.3, 8.1, 9.7 },
    { 5.0, 4.5, 4.0, 8.7, 4.0, 5.3, 6.6, 9.3 },
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
  };
  struct case_t *c;
  vec_t y;
  double dxdt;

  y = vec_create( 2 );
  for( c=cases; c->tau>0; c++ ){
    mtoka_osci_neuron_set_rise_time_const( &neuron, c->tau );
    /* mtoka_osci_neuron_set_adapt_time_const( &neuron, c->T ); */
    vec_set_elem( mtoka_osci_neuron_mutual_inhibit_weights(&neuron), 1, c->a );
    mtoka_osci_neuron_set_steady_firing_rate( &neuron, c->b );
    mtoka_osci_neuron_set_tonic_input( &neuron, c->c );
    mtoka_osci_neuron_set_sensory_feedback( &neuron, c->s );
    mtoka_osci_neuron_membrane_potential(&neuron) = c->x;
    mtoka_osci_neuron_firing_rate(&neuron) = c->y;
    mtoka_osci_neuron_adapt_property(&neuron) = c->v;
    dxdt = ( -c->x + c->c - c->a * c->y - c->b * c->v + c->s ) / c->tau;
    vec_set_elem_list( y, 0.0, c->y );
    ASSERT_DOUBLE_EQ( dxdt, mtoka_osci_neuron_dxdt( &neuron, y ) );
  }
  vec_destroy( y );
}

TEST(test_mtoka_osci_neuron_dvdt)
{
  struct case_t {
    double T, y, v;
  } cases[] = {
    { 4.3, 7.1, 4.0 },
    { 9.6, 7.5, 7.5 },
    { 0.0, 0.0, 0.0 },
  };
  struct case_t *c;
  double dvdt;

  for( c=cases; c->T>0; c++ ){
    mtoka_osci_neuron_set_adapt_time_const( &neuron, c->T );
    mtoka_osci_neuron_firing_rate(&neuron) = c->y;
    mtoka_osci_neuron_adapt_property(&neuron) = c->v;
    dvdt = ( -c->v + c->y ) / c->T;
    ASSERT_DOUBLE_EQ( dvdt, mtoka_osci_neuron_dvdt( &neuron ) );
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
  RUN_TEST(test_mtoka_osci_neuron_dxdt);
  RUN_TEST(test_mtoka_osci_neuron_dvdt);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_mtoka_osci_neuron);
  TEST_REPORT();
  TEST_EXIT();
}
