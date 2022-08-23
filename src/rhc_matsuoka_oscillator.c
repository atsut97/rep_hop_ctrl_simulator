#include "rhc_matsuoka_oscillator.h"
#include "rhc_vec.h"

mtoka_osci_neuron_t *mtoka_osci_neuron_init(mtoka_osci_neuron_t *self, int n)
{
  self->tau = 1.0;
  self->T = 1.0;
  self->a = vec_create(n);
  self->b = 0.0;
  self->c = 0.0;
  self->th = 0.0;
  self->s = 0.0;
  self->x = 0.0;
  self->y = 0.0;
  self->v = 0.0;
  return self;
}

void mtoka_osci_neuron_destroy(mtoka_osci_neuron_t *self)
{
  vec_destroy( mtoka_osci_neuron_mutual_inhibit_weights(self) );
  mtoka_osci_neuron_mutual_inhibit_weights(self) = NULL;
}
