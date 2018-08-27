#include "rhc_vec_ring.h"

void vec_ring_init(vec_ring_t *self, int size)
{
  self->head = 0;
  self->max = size;
}

void vec_ring_destroy(vec_ring_t *self)
{}

void vec_ring_push(vec_ring_t *self, vec_t v)
{
  self->head++;
}

void vec_ring_pop(vec_ring_t *self)
{
  self->head--;
}

bool vec_ring_empty(vec_ring_t *self)
{
  return ( self->head == 0 );
}

bool vec_ring_full(vec_ring_t *self)
{
  return ( vec_ring_size(self) == vec_ring_capacity(self) );
}
