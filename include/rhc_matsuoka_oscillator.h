#ifndef __RHC_MATSUOKA_OSCILLATOR_H__
#define __RHC_MATSUOKA_OSCILLATOR_H__

#include "rhc_vec.h"

typedef struct{
  /* parameters */
  double tau;  /* rise time constant */
  double T;    /* adaptation time constant */
  vec_t a;     /* mutual inhibitory connection weights */
  double b;    /* steady-state firing rate */
  double th;   /* firing threshold */
  /* tonic input and sensory feedback */
  double c;    /* excitatory tonic input */
  double s;    /* sensory feedback */
  /* variables */
  double x;    /* membrane potential */
  double y;    /* firing rate */
  double v;    /* self-inhibitory input (adaptation or fatigue property) */
} mtoka_osci_neuron_t;

mtoka_osci_neuron_t *mtoka_osci_neuron_init(mtoka_osci_neuron_t *self, int n);
void mtoka_osci_neuron_destroy(mtoka_osci_neuron_t *self);

#define mtoka_osci_neuron_rise_time_const(self)        (self)->tau
#define mtoka_osci_neuron_adapt_time_const(self)       (self)->T
#define mtoka_osci_neuron_mutual_inhibit_weights(self) (self)->a
#define mtoka_osci_neuron_steady_firing_rate(self)     (self)->b
#define mtoka_osci_neuron_firing_threshold(self)       (self)->th
#define mtoka_osci_neuron_tonic_input(self)            (self)->c
#define mtoka_osci_neuron_sensory_feedback(self)       (self)->s
#define mtoka_osci_neuron_membrane_potential(self)     (self)->x
#define mtoka_osci_neuron_firing_rate(self)            (self)->y
#define mtoka_osci_neuron_adapt_property(self)         (self)->v

#define mtoka_osci_neuron_set_rise_time_const(self,tau)     \
  ( mtoka_osci_neuron_rise_time_const(self) = (tau) )
#define mtoka_osci_neuron_set_adapt_time_const(self,T)      \
  ( mtoka_osci_neuron_adapt_time_const(self) = (T) )
#define mtoka_osci_neuron_set_steady_firing_rate(self,b)    \
  ( mtoka_osci_neuron_steady_firing_rate(self) = (b) )
#define mtoka_osci_neuron_set_firing_threshold(self,th)     \
  ( mtoka_osci_neuron_firing_threshold(self) = (th) )
#define mtoka_osci_neuron_set_tonic_input(self,c)           \
  ( mtoka_osci_neuron_tonic_input(self) = (c) )
#define mtoka_osci_neuron_set_sensory_feedback(self,s)      \
  ( mtoka_osci_neuron_sensory_feedback(self) = (s) )


#endif /* __RHC_MATSUOKA_OSCILLATOR_H__ */
