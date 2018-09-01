#include "rhc_vec_ring.h"

void vec_ring_init(vec_ring_t *self, int dim, int size)
{
  register int i;

  self->head = 0;
  self->size = 0;
  self->dim = dim;
  self->max = size;
  if( ( vec_ring_buf(self) = nalloc( vec_t, size ) ) == NULL ){
    ALLOC_ERR();
    return;
  }
  for( i=0; i<vec_ring_capacity(self); i++ ){
    vec_ring_buf(self)[i] = vec_create( vec_ring_dim(self) );
  }
}

void vec_ring_destroy(vec_ring_t *self)
{
  register int i;

  for( i=0; i<vec_ring_capacity(self); i++ ){
    vec_destroy( vec_ring_buf(self)[i] );
  }
  sfree( vec_ring_buf(self) );
}

void vec_ring_push(vec_ring_t *self, vec_t v)
{
  if( !vec_ring_empty(self) )
    self->head++;
  if( !vec_ring_full(self) )
    self->size++;
  vec_copy( v, vec_ring_head(self) );
}

vec_t vec_ring_pop(vec_ring_t *self)
{
  vec_t ret;

  ret = vec_ring_head(self);
  if( !vec_ring_empty(self) )
    self->size--;
  if( !vec_ring_empty(self) )
    self->head--;
  return ret;
}

bool vec_ring_empty(vec_ring_t *self)
{
  return ( vec_ring_size(self) == 0 );
}

bool vec_ring_full(vec_ring_t *self)
{
  return ( vec_ring_size(self) == vec_ring_capacity(self) );
}
