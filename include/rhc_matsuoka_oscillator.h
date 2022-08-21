#ifndef __RHC_MATSUOKA_OSCILLATOR_H__
#define __RHC_MATSUOKA_OSCILLATOR_H__

typedef struct{
  /* parameters */
  double tau;  /* rise time constant */
  double T;    /* adaptation time constant */
  double a;    /* mutual inhibitory connection weight */
  double b;    /* steady-state firing rate */
  /* tonic input and sensory feedback */
  double c;    /* excitatory tonic input */
  double s;    /* sensory feedback */
  /* variables */
  double x;    /* membrane potential */
  double y;    /* firing rate */
  double v;    /* self-inhibitory input (adaptation or fatigue property) */
} mtoka_osci_neuron_t;

mtoka_osci_neuron_t *mtoka_osci_neuron_init(mtoka_osci_neuron_t *neuron);
void mtoka_osci_neuron_destroy(mtoka_osci_neuron_t *neuron);


#endif /* __RHC_MATSUOKA_OSCILLATOR_H__ */
