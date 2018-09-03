#ifndef __RHC_VEC_RING_H__
#define __RHC_VEC_RING_H__

#include "rhc_vec.h"

typedef struct {
  vec_t *buf;     /* pointer to array */
  int dim;        /* dimension of vector */
  int head;       /* head position */
  int size;       /* current size */
  int max;        /* maximum size (capacity) */
} vec_ring_t;

#define vec_ring_buf(self)      (self)->buf
#define vec_ring_dim(self)      (self)->dim
#define vec_ring_capacity(self) (self)->max
#define vec_ring_size(self)     (self)->size

#define vec_ring_index(self,index) ( (index) % vec_ring_capacity(self) )
#define vec_ring_head_index(self)  (self)->head
#define vec_ring_tail_index(self)  vec_ring_index(self, (self)->head - (self)->size + (self)->max + 1)
#define vec_ring_head(self)        vec_ring_buf(self)[vec_ring_head_index(self)]
#define vec_ring_tail(self)        vec_ring_buf(self)[vec_ring_tail_index(self)]

void vec_ring_init(vec_ring_t *self, int dim, int size);
void vec_ring_destroy(vec_ring_t *self);

bool vec_ring_empty(vec_ring_t *self);
bool vec_ring_full(vec_ring_t *self);

void vec_ring_reset(vec_ring_t *self);

void vec_ring_push(vec_ring_t *self, vec_t v);
vec_t vec_ring_pop(vec_ring_t *self);

#endif /* __RHC_VEC_RING_H__ */
