#include "rhc_matsuoka_oscillator.h"
#include "rhc_test.h"
#include "rhc_vec.h"

static mtoka_osci_neuron_t neuron;

void setup_neuron()
{
  mtoka_osci_neuron_init( &neuron, 2 );
}

void teardown_neuron()
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
    vec_set_elem( mtoka_osci_neuron_mutual_inhibit_weights(&neuron), 1, c->a );
    mtoka_osci_neuron_set_steady_firing_rate( &neuron, c->b );
    dxdt = ( -c->x + c->c - c->a * c->y - c->b * c->v + c->s ) / c->tau;
    vec_set_elem_list( y, 0.0, c->y );
    ASSERT_DOUBLE_EQ( dxdt, mtoka_osci_neuron_dxdt( &neuron, c->x, c->v, y, c->c, c->s ) );
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
    dvdt = ( -c->v + c->y ) / c->T;
    ASSERT_DOUBLE_EQ( dvdt, mtoka_osci_neuron_dvdt( &neuron, c->v, c->y ) );
  }
}

TEST(test_mtoka_osci_neuron)
{
  CONFIGURE_SUITE(setup_neuron, teardown_neuron);
  RUN_TEST(test_mtoka_osci_neuron_init);
  RUN_TEST(test_mtoka_osci_neuron_destroy);
  RUN_TEST(test_mtoka_osci_neuron_set_rise_time_const);
  RUN_TEST(test_mtoka_osci_neuron_set_adapt_time_const);
  RUN_TEST(test_mtoka_osci_neuron_set_steady_firing_rate);
  RUN_TEST(test_mtoka_osci_neuron_set_firing_threshold);
  RUN_TEST(test_mtoka_osci_neuron_dxdt);
  RUN_TEST(test_mtoka_osci_neuron_dvdt);
}


static mtoka_osci_t osci;

void setup()
{
  mtoka_osci_init( &osci, 2 );
}

void teardown()
{
  mtoka_osci_destroy( &osci );
}

typedef struct{
  vec_t x, k[4];
} _ode_rk4;

TEST(test_mtoka_osci_init)
{
  mtoka_osci_neuron_t *np;
  register int i;

  ASSERT_EQ( 2, mtoka_osci_n_neuron(&osci) );
  ASSERT_PTRNE( NULL, osci.neurons );
  ASSERT_EQ( 0.0, mtoka_osci_time(&osci) );
  ASSERT_EQ( 2, vec_size(mtoka_osci_membrane_potential(&osci)) );
  ASSERT_EQ( 2, vec_size(mtoka_osci_firing_rate(&osci)) );
  ASSERT_EQ( 2, vec_size(mtoka_osci_adapt_property(&osci)) );
  ASSERT_EQ( 2, vec_size(mtoka_osci_tonic_input(&osci)) );
  ASSERT_EQ( 2, vec_size(mtoka_osci_sensory_feedback(&osci)) );
  ASSERT_EQ( 4, vec_size(osci.xv) );
  ASSERT_PTREQ( mtoka_osci_dp, osci.ode.f );
  ASSERT_PTRNE( NULL, osci.ode._ws );
  ASSERT_EQ( 4, vec_size(((_ode_rk4 *)osci.ode._ws)->x) );

  for( i=0; i<2; i++ ){
    np = mtoka_osci_neuron( &osci, i );
    ASSERT_EQ( 1.0, mtoka_osci_neuron_rise_time_const(np) );
    ASSERT_EQ( 1.0, mtoka_osci_neuron_adapt_time_const(np) );
    ASSERT_EQ( 2, vec_size(mtoka_osci_neuron_mutual_inhibit_weights(np)) );
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_neuron_mutual_inhibit_weights(np), 0) );
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_neuron_mutual_inhibit_weights(np), 1) );
    ASSERT_EQ( 0.0, mtoka_osci_neuron_steady_firing_rate(np) );
    ASSERT_EQ( 0.0, mtoka_osci_neuron_firing_threshold(np) );
  }
}

TEST(test_mtoka_osci_init_specify_n_neuron)
{
  int n = 3;
  mtoka_osci_t osci_n;
  mtoka_osci_neuron_t *np;
  register int i;

  mtoka_osci_init( &osci_n, n );
  ASSERT_EQ( n, mtoka_osci_n_neuron(&osci_n) );
  ASSERT_PTRNE( NULL, osci.neurons );
  ASSERT_EQ( 0.0, mtoka_osci_time(&osci_n) );
  ASSERT_EQ( 0, mtoka_osci_step(&osci_n) );
  ASSERT_EQ( n, vec_size(mtoka_osci_membrane_potential(&osci_n)) );
  ASSERT_EQ( n, vec_size(mtoka_osci_firing_rate(&osci_n)) );
  ASSERT_EQ( n, vec_size(mtoka_osci_adapt_property(&osci_n)) );
  ASSERT_EQ( n, vec_size(mtoka_osci_tonic_input(&osci_n)) );
  ASSERT_EQ( n, vec_size(mtoka_osci_sensory_feedback(&osci_n)) );
  ASSERT_EQ( 2*n, vec_size(osci_n.xv) );
  ASSERT_PTREQ( mtoka_osci_dp, osci_n.ode.f );
  ASSERT_PTRNE( NULL, osci_n.ode._ws );
  ASSERT_EQ( 2*n, vec_size(((_ode_rk4 *)osci_n.ode._ws)->x) );

  for( i=0; i<2; i++ ){
    np = mtoka_osci_neuron( &osci_n, i );
    ASSERT_EQ( 1.0, mtoka_osci_neuron_rise_time_const(np) );
    ASSERT_EQ( 1.0, mtoka_osci_neuron_adapt_time_const(np) );
    ASSERT_EQ( n, vec_size(mtoka_osci_neuron_mutual_inhibit_weights(np)) );
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_neuron_mutual_inhibit_weights(np), 0) );
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_neuron_mutual_inhibit_weights(np), 1) );
    ASSERT_EQ( 0.0, mtoka_osci_neuron_steady_firing_rate(np) );
    ASSERT_EQ( 0.0, mtoka_osci_neuron_firing_threshold(np) );
  }

  mtoka_osci_destroy( &osci_n );
}

TEST(test_mtoka_osci_destroy)
{
  mtoka_osci_destroy( &osci );
  ASSERT_EQ( 0.0, mtoka_osci_time(&osci) );
  ASSERT_PTREQ( NULL, osci.neurons );
  ASSERT_PTREQ( NULL, mtoka_osci_membrane_potential(&osci) );
  ASSERT_PTREQ( NULL, mtoka_osci_firing_rate(&osci) );
  ASSERT_PTREQ( NULL, mtoka_osci_adapt_property(&osci) );
  ASSERT_PTREQ( NULL, mtoka_osci_tonic_input(&osci) );
  ASSERT_PTREQ( NULL, mtoka_osci_sensory_feedback(&osci) );
  ASSERT_PTREQ( NULL, osci.xv );
  ASSERT_PTREQ( NULL, osci.ode._ws );
}

TEST(test_mtoka_osci_set_tonic_input)
{
  struct case_t{
    double c1, c2;
  } cases[] = {
    { 0.3, 9.4 },
    { 2.8, 4.5 },
    { 0.3, 6.8 },
    { 0, 0 },
  };
  struct case_t *c;
  vec_t v = vec_create( 2 );

  for( c=cases; c->c1>0; c++ ){
    vec_set_elem_list( v, c->c1, c->c2 );
    mtoka_osci_set_tonic_input( &osci, v );
    ASSERT_EQ( c->c1, vec_elem(mtoka_osci_tonic_input(&osci), 0) );
    ASSERT_EQ( c->c2, vec_elem(mtoka_osci_tonic_input(&osci), 1) );
  }
  vec_destroy( v );
}

TEST(test_mtoka_osci_set_sensory_feedback)
{
  struct case_t{
    double c1, c2;
  } cases[] = {
    { 6.1, 9.7 },
    { 7.0, 2.2 },
    { 9.3, 3.0 },
    { 0, 0 },
  };
  struct case_t *c;
  vec_t v = vec_create( 2 );

  for( c=cases; c->c1>0; c++ ){
    vec_set_elem_list( v, c->c1, c->c2 );
    mtoka_osci_set_sensory_feedback( &osci, v );
    ASSERT_EQ( c->c1, vec_elem(mtoka_osci_sensory_feedback(&osci), 0) );
    ASSERT_EQ( c->c2, vec_elem(mtoka_osci_sensory_feedback(&osci), 1) );
  }
  vec_destroy( v );
}

TEST(test_mtoka_osci_inc_step)
{
  ASSERT_EQ( 0, mtoka_osci_step(&osci) );
  mtoka_osci_inc_step( &osci );
  ASSERT_EQ( 1, mtoka_osci_step(&osci) );
  mtoka_osci_inc_step( &osci );
  ASSERT_EQ( 2, mtoka_osci_step(&osci) );
  mtoka_osci_inc_step( &osci );
  ASSERT_EQ( 3, mtoka_osci_step(&osci) );
}

TEST(test_mtoka_osci_reset)
{
  double dt = 0.01;
  vec_t p = vec_create_list( 2, 4.3, 9.1 );
  register int i;

  mtoka_osci_update_time( &osci, dt );
  vec_copy( p, mtoka_osci_membrane_potential(&osci) );
  vec_copy( p, mtoka_osci_firing_rate(&osci) );
  vec_copy( p, mtoka_osci_adapt_property(&osci) );
  vec_copy( p, mtoka_osci_tonic_input(&osci) );
  vec_copy( p, mtoka_osci_sensory_feedback(&osci) );
  ASSERT_NE( 0.0, mtoka_osci_time( &osci ) );
  ASSERT_NE( 0, mtoka_osci_step( &osci ) );
  for( i=0; i<2; i++ ){
    ASSERT_NE( 0.0, vec_elem(mtoka_osci_membrane_potential(&osci), i) );
    ASSERT_NE( 0.0, vec_elem(mtoka_osci_firing_rate(&osci), i) );
    ASSERT_NE( 0.0, vec_elem(mtoka_osci_adapt_property(&osci), i) );
    ASSERT_NE( 0.0, vec_elem(mtoka_osci_tonic_input(&osci), i) );
    ASSERT_NE( 0.0, vec_elem(mtoka_osci_sensory_feedback(&osci), i) );
  }
  ASSERT_TRUE( mtoka_osci_reset( &osci ) );
  ASSERT_EQ( 0.0, mtoka_osci_time( &osci ) );
  ASSERT_EQ( 0, mtoka_osci_step( &osci ) );
  for( i=0; i<2; i++ ){
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_membrane_potential(&osci), i) );
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_firing_rate(&osci), i) );
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_adapt_property(&osci), i) );
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_tonic_input(&osci), i) );
    ASSERT_EQ( 0.0, vec_elem(mtoka_osci_sensory_feedback(&osci), i) );
  }
  vec_destroy( p );
}

TEST(test_mtoka_osci_update_state)
{
  vec_elem(osci.xv, 0) = 5.7;
  vec_elem(osci.xv, 1) = -4.9;
  vec_elem(osci.xv, 2) = 0.6;
  vec_elem(osci.xv, 3) = 7.2;
  mtoka_osci_update_state( &osci );
  ASSERT_EQ( 5.7, vec_elem(mtoka_osci_membrane_potential(&osci), 0) );
  ASSERT_EQ( -4.9, vec_elem(mtoka_osci_membrane_potential(&osci), 1) );
  ASSERT_EQ( 0.6, vec_elem(mtoka_osci_adapt_property(&osci), 0) );
  ASSERT_EQ( 7.2, vec_elem(mtoka_osci_adapt_property(&osci), 1) );
  ASSERT_EQ( 5.7, vec_elem(mtoka_osci_firing_rate(&osci), 0) );
  ASSERT_EQ( 0.0, vec_elem(mtoka_osci_firing_rate(&osci), 1) );
}

TEST(test_mtoka_osci_update_time)
{
  double dt = 0.01;
  ASSERT_DOUBLE_EQ( 0.0, mtoka_osci_time(&osci) );
  mtoka_osci_update_time( &osci, dt );
  ASSERT_DOUBLE_EQ( 0.01, mtoka_osci_time(&osci) );
  mtoka_osci_update_time( &osci, dt );
  ASSERT_DOUBLE_EQ( 0.02, mtoka_osci_time(&osci) );
  mtoka_osci_update_time( &osci, dt );
  ASSERT_DOUBLE_EQ( 0.03, mtoka_osci_time(&osci) );
}

TEST(test_mtoka_osci)
{
  CONFIGURE_SUITE(setup, teardown);
  RUN_TEST(test_mtoka_osci_init);
  RUN_TEST(test_mtoka_osci_init_specify_n_neuron);
  RUN_TEST(test_mtoka_osci_destroy);
  RUN_TEST(test_mtoka_osci_set_tonic_input);
  RUN_TEST(test_mtoka_osci_set_sensory_feedback);
  RUN_TEST(test_mtoka_osci_inc_step);
  RUN_TEST(test_mtoka_osci_reset);
  RUN_TEST(test_mtoka_osci_update_state);
  RUN_TEST(test_mtoka_osci_update_time);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_mtoka_osci_neuron);
  RUN_SUITE(test_mtoka_osci);
  TEST_REPORT();
  TEST_EXIT();
}
