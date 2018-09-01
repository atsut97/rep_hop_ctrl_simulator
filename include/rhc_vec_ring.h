#ifndef __RHC_VEC_RING_H__
#define __RHC_VEC_RING_H__

#include "rhc_vec.h"

typedef struct {
  vec_t *buf;
  int head;
  int max;
} vec_ring_t;

#define vec_ring_buf(self)   (self)->buf
#define vec_ring_capacity(self) (self)->max
#define vec_ring_size(self)     (self)->head

#define vec_ring_head(self)     vec_ring_buf(self)[(self)->head]

void vec_ring_init(vec_ring_t *self, int size);
void vec_ring_destroy(vec_ring_t *self);

bool vec_ring_empty(vec_ring_t *self);
bool vec_ring_full(vec_ring_t *self);

void vec_ring_reset(vec_ring_t *self);

void vec_ring_push(vec_ring_t *self, vec_t v);
void vec_ring_pop(vec_ring_t *self);

#endif /* __RHC_VEC_RING_H__ */
