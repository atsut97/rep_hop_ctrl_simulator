#include "rhc_ode.h"

ode_t *ode_init_euler(ode_t *self, int dim, vec_t (* f)(double,vec_t,void*,vec_t))
{
  self->f = f;
  if( ( self->_ws = vec_create( dim ) ) == NULL ){
    ALLOC_ERR();
    return NULL;
  }
  return self;
}

vec_t ode_update_euler(ode_t *self, double t, vec_t x, double dt, void *util)
{
  return x;
}

void ode_destroy_euler(ode_t *self)
{
  vec_destroy( (vec_t)self->_ws );
  self->f = NULL;
  self->_ws = NULL;
}
