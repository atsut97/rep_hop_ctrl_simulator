#include "rhc_matsuoka_oscillator.h"

mtoka_osci_neuron_t *mtoka_osci_neuron_init(mtoka_osci_neuron_t *self)
{
  self->tau = 1.0;
  self->T = 1.0;
  self->a = 0.0;
  self->b = 0.0;
  self->c = 0.0;
  self->s = 0.0;
  self->x = 0.0;
  self->y = 0.0;
  self->v = 0.0;
  return self;
}

void mtoka_osci_neuron_destroy(mtoka_osci_neuron_t *self) {
  mtoka_osci_neuron_init( self );
}
