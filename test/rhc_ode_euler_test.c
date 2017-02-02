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
  ode_destroy( &ode );
}


TEST(test_ode_euler_init)
{
  ASSERT_PTREQ( &dp, ode.f );
  ASSERT_EQ( 2, vec_size( (vec_t)ode._ws ) );
}

/* TEST(test_ode_euler_update) */
/* { */
/*   vec_write( x ); */
/*   for( t=0; t<T; t+=DT ){ */
/*     ode_update( &ode, t, x, DT, NULL ); */
/*     vec_write( x ); */
/*   } */
/* } */

TEST_SUITE(test_ode_euler)
{
  CONFIGURE_SUITE( &setup, &teardown );
  RUN_TEST(test_ode_euler_init);
  /* RUN_TEST(test_ode_euler_update); */
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ode_euler);
  TEST_REPORT();
  TEST_EXIT();
}
