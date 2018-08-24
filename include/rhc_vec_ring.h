#ifndef __RHC_VEC_RING_H__
#define __RHC_VEC_RING_H__

#include "rhc_vec.h"

typedef struct {
  vec_t *buffer;
  int head, tail;
  int max;
} vec_ring_t;

#define vec_ring_buffer(self)   (self)->buffer
#define vec_ring_capacity(self) (self)->max
#define vec_ring_size(self)     ( (self)->head - (self)->tail )

void vec_ring_init(vec_ring_t *self, int size);
void vec_ring_destroy(vec_ring_t *self);

void vec_ring_reset(vec_ring_t *self);

void vec_ring_push_front(vec_ring_t *self, vec_t v);
void vec_ring_push_back(vec_ring_t *self, vec_t v);

void vec_ring_pop_front(vec_ring_t *self);
void vec_ring_pop_back(vec_ring_t *self);

bool vec_ring_empty(vec_ring_t *self);
bool vec_ring_full(vec_ring_t *self);

#endif /* __RHC_VEC_RING_H__ */
