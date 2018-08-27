#include "rhc_vec_ring.h"

void vec_ring_init(vec_ring_t *self, int size)
{
  self->head = self->tail = 0;
  self->max = size;
}

void vec_ring_destroy(vec_ring_t *self)
{}

void vec_ring_push_front(vec_ring_t *self, vec_t v)
{
  self->head++;
}

void vec_ring_push_back(vec_ring_t *self, vec_t v)
{
  self->tail--;
}

void vec_ring_pop_front(vec_ring_t *self)
{
  self->head--;
}

void vec_ring_pop_back(vec_ring_t *self)
{
  self->tail++;
}

bool vec_ring_empty(vec_ring_t *self)
{
  return ( self->head == self->tail );
}

bool vec_ring_full(vec_ring_t *self)
{
  return ( vec_ring_size(self) == vec_ring_capacity(self) );
}
