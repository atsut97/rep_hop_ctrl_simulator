#ifndef __RHC_VEC_H__
#define __RHC_VEC_H__

#include "rhc_misc.h"

typedef struct{
  uint size;
  double *elem;
} _vec_t;

typedef _vec_t * vec_t;

/* vector size */
#define vec_size(v)      ( (v)->size )
#define vec_set_size(v,s) ( vec_size(v) = (s) )

/* get/set a vector element */
#define vec_buf(v)         (v)->elem
#define vec_elem(v,n)      vec_buf(v)[n]
#define vec_set_elem(v,n,e) ( vec_elem(v,n) = (e) )
vec_t vec_set_elem_list(vec_t v, ... );

/* create, destroy a vector */
vec_t vec_create(uint size);
vec_t vec_create_list(uint size, ... );
void vec_destroy(vec_t v);

#endif /* __RHC_VEC_H__ */
