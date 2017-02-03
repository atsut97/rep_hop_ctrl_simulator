#include "rhc_ode.h"
#include "rhc_test.h"

/* unit circle function */
vec_t dp(double t, vec_t p, void *dummy, vec_t v)
{
  vec_set_elem( v, 0, -vec_elem(p,1) );
  vec_set_elem( v, 1,  vec_elem(p,0) );
  return v;
}

ode_t ode;
vec_t x;
double t;
const double T  = 10;
const double DT = 0.01;

void setup()
{
  ode_assign( &ode, euler );
  ode_init( &ode, 2, &dp );
  x = vec_create_list( 2, 1.0, 0.0 );
}

void teardown()
{
  vec_destroy( x );
  if( ode._ws )
    ode_destroy( &ode );
}


TEST(test_ode_euler_init)
{
  ASSERT_PTREQ( &dp, ode.f );
  ASSERT_EQ( 2, vec_size( (vec_t)ode._ws ) );
}

TEST(test_ode_euler_destroy)
{
  ode_destroy( &ode );
  ASSERT_PTREQ( NULL, ode.f );
  ASSERT_PTREQ( NULL, ode._ws );
}

TEST(test_ode_euler_update)
{
  ode_update( &ode, t, x, DT, NULL );
  ASSERT_DOUBLE_EQ( 1.0,  vec_elem(x,0) );
  ASSERT_DOUBLE_EQ( 0.01, vec_elem(x,1) );
  ode_update( &ode, t, x, DT, NULL );
  ASSERT_DOUBLE_EQ( 0.9999, vec_elem(x,0) );
  ASSERT_DOUBLE_EQ( 0.02,   vec_elem(x,1) );
}

TEST_SUITE(test_ode_euler)
{
  CONFIGURE_SUITE( &setup, &teardown );
  RUN_TEST(test_ode_euler_init);
  RUN_TEST(test_ode_euler_destroy);
  RUN_TEST(test_ode_euler_update);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ode_euler);
  TEST_REPORT();
  TEST_EXIT();
}
