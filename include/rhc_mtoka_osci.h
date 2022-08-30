#ifndef __RHC_MATSUOKA_OSCILLATOR_H__
#define __RHC_MATSUOKA_OSCILLATOR_H__

#include "rhc_ode.h"
#include "rhc_vec.h"

typedef struct{
  /* parameters */
  double tau;  /* rise time constant */
  double T;    /* adaptation time constant */
  vec_t a;     /* mutual inhibitory connection weights */
  double b;    /* steady-state firing rate */
  double th;   /* firing threshold */
} mtoka_osci_neuron_t;

mtoka_osci_neuron_t *mtoka_osci_neuron_init(mtoka_osci_neuron_t *self, int n);
void mtoka_osci_neuron_destroy(mtoka_osci_neuron_t *self);

#define mtoka_osci_neuron_rise_time_const(self)        (self)->tau
#define mtoka_osci_neuron_adapt_time_const(self)       (self)->T
#define mtoka_osci_neuron_mutual_inhibit_weights(self) (self)->a
#define mtoka_osci_neuron_steady_firing_rate(self)     (self)->b
#define mtoka_osci_neuron_firing_threshold(self)       (self)->th

#define mtoka_osci_neuron_set_rise_time_const(self,tau)      \
  ( mtoka_osci_neuron_rise_time_const(self) = (tau) )
#define mtoka_osci_neuron_set_adapt_time_const(self,T)       \
  ( mtoka_osci_neuron_adapt_time_const(self) = (T) )
#define mtoka_osci_neuron_set_mutual_inhibit_weights(self,a) \
  vec_copy( a, mtoka_osci_neuron_mutual_inhibit_weights(self) )
#define mtoka_osci_neuron_set_steady_firing_rate(self,b)     \
  ( mtoka_osci_neuron_steady_firing_rate(self) = (b) )
#define mtoka_osci_neuron_set_firing_threshold(self,th)      \
  ( mtoka_osci_neuron_firing_threshold(self) = (th) )

double mtoka_osci_neuron_dxdt(mtoka_osci_neuron_t *self, double x, double v, vec_t y, double c, double s);
double mtoka_osci_neuron_dvdt(mtoka_osci_neuron_t *self, double v, double y);

typedef struct{
  int n_neuron;
  mtoka_osci_neuron_t *neurons;
  double t;
  int step;
  ode_t ode;
  vec_t x;
  vec_t y;
  vec_t v;
  vec_t c;
  vec_t s;
  vec_t xv;
} mtoka_osci_t;

mtoka_osci_t *mtoka_osci_init(mtoka_osci_t *self, int n_neuron);
void mtoka_osci_destroy(mtoka_osci_t *self);

#define mtoka_osci_n_neuron(self)           (self)->n_neuron
#define mtoka_osci_neuron(self,i)           ( &(self)->neurons[i] )
#define mtoka_osci_time(self)               (self)->t
#define mtoka_osci_step(self)               (self)->step
#define mtoka_osci_membrane_potential(self) (self)->x
#define mtoka_osci_firing_rate(self)        (self)->y
#define mtoka_osci_adapt_property(self)     (self)->v
#define mtoka_osci_tonic_input(self)        (self)->c
#define mtoka_osci_sensory_feedback(self)   (self)->s

#define mtoka_osci_set_tonic_input(self,c)      vec_copy( c, mtoka_osci_tonic_input(self) )
#define mtoka_osci_set_sensory_feedback(self,s) vec_copy( s, mtoka_osci_sensory_feedback(self) )

#define mtoka_osci_inc_step(self)           ( mtoka_osci_step(self)++ )

vec_t mtoka_osci_dp(double t, vec_t x, void *util, vec_t v);
bool mtoka_osci_reset(mtoka_osci_t *self);
void mtoka_osci_update_state(mtoka_osci_t *self);
void mtoka_osci_update_time(mtoka_osci_t *self, double dt);
bool mtoka_osci_update(mtoka_osci_t *self, vec_t c, vec_t s, double dt);

#endif /* __RHC_MATSUOKA_OSCILLATOR_H__ */
