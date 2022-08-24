#include "rhc_matsuoka_oscillator.h"
#include "rhc_misc.h"
#include "rhc_ode.h"
#include "rhc_vec.h"

mtoka_osci_neuron_t *mtoka_osci_neuron_init(mtoka_osci_neuron_t *self, int n)
{
  self->tau = 1.0;
  self->T = 1.0;
  self->a = vec_create(n);
  vec_clear( self->a );
  self->b = 0.0;
  self->th = 0.0;
  mtoka_osci_neuron_reset( self );
  return self;
}

void mtoka_osci_neuron_destroy(mtoka_osci_neuron_t *self)
{
  mtoka_osci_neuron_reset( self );
  vec_destroy( mtoka_osci_neuron_mutual_inhibit_weights(self) );
  mtoka_osci_neuron_mutual_inhibit_weights(self) = NULL;
}

void mtoka_osci_neuron_reset(mtoka_osci_neuron_t *self)
{
  self->c = 0.0;
  self->s = 0.0;
  self->x = 0.0;
  self->y = 0.0;
  self->v = 0.0;
}

double mtoka_osci_neuron_dxdt(mtoka_osci_neuron_t *self, vec_t y)
{
  return ( -self->x + self->c - vec_dot( self->a, y ) - self->b * self->v + self->s ) / self->tau;
}

double mtoka_osci_neuron_dvdt(mtoka_osci_neuron_t *self)
{
  return ( -self->v + self->y ) / self->T;
}

mtoka_osci_t *mtoka_osci_init(mtoka_osci_t *self, int n_neuron)
{
  mtoka_osci_n_neuron(self) = n_neuron;
  if( !( self->neurons = nalloc( mtoka_osci_neuron_t, n_neuron ) ) ||
      !( mtoka_osci_membrane_potential(self) = vec_create(n_neuron) ) ||
      !( mtoka_osci_firing_rate(self) = vec_create(n_neuron) ) ||
      !( mtoka_osci_adapt_property(self) = vec_create(n_neuron) ) ){
    ALLOC_ERR();
    return NULL;
  }
  ode_assign( &self->ode, rk4 );
  if( !ode_init( &self->ode, 2 * n_neuron, mtoka_osci_dp ) ){
    ALLOC_ERR();
    return NULL;
  }
  mtoka_osci_reset( self );
  return self;
}

void mtoka_osci_destroy(mtoka_osci_t *self)
{
  vec_destroy( mtoka_osci_membrane_potential(self) );
  mtoka_osci_membrane_potential(self) = NULL;
  vec_destroy( mtoka_osci_firing_rate(self) );
  mtoka_osci_firing_rate(self) = NULL;
  vec_destroy( mtoka_osci_adapt_property(self) );
  mtoka_osci_adapt_property(self) = NULL;
  if( self->ode._ws )
    ode_destroy( &self->ode );
  sfree( self->neurons );
}

vec_t mtoka_osci_dp(double t, vec_t x, void *util, vec_t v)
{
  return x;
}

bool mtoka_osci_reset(mtoka_osci_t *self)
{
  register int i;

  mtoka_osci_time(self) = 0.0;
  mtoka_osci_step(self) = 0;
  vec_clear( mtoka_osci_membrane_potential(self) );
  vec_clear( mtoka_osci_firing_rate(self) );
  vec_clear( mtoka_osci_adapt_property(self) );
  for( i=0; i<mtoka_osci_n_neuron(self); i++ )
    mtoka_osci_neuron_reset( mtoka_osci_neuron(self, i) );
  return true;
}

void mtoka_osci_update_time(mtoka_osci_t *self, double dt)
{
  mtoka_osci_inc_step( self );
  mtoka_osci_time(self) = mtoka_osci_step(self) * dt;
}
