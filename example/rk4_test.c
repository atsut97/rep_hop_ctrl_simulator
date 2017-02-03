#include "rhc_ode.h"

/* unit circle function */
vec_t dp(double t, vec_t p, void *dummy, vec_t v)
{
  vec_set_elem( v, 0, -vec_elem(p,1) );
  vec_set_elem( v, 1,  vec_elem(p,0) );
  return v;
}

const double T  = 10;
const double DT = 0.01;

int main(int argc, char *argv[])
{
  ode_t ode;
  vec_t x;
  double t;

  ode_assign( &ode, rk4 );
  ode_init( &ode, 2, dp );
  x = vec_create_list( 2, 1.0, 0.0 );
  vec_write( x );
  for( t=0; t<T; t+=DT ){
    ode_update( &ode, t, x, DT, NULL );
    vec_write( x );
  }
  vec_destroy( x );
  ode_destroy( &ode );
  return 0;
}
