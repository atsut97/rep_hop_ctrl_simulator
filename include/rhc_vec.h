#ifndef __RHC_VEC_H__
#define __RHC_VEC_H__

#include "rhc_misc.h"

typedef struct{
  size_t size;
  double *elem;
} _vec_t;

typedef _vec_t * vec_t;

/* error messages */
#define VEC_ERR_SIZMIS  "size mismatch of vector"
#define VEC_ERR_ZERODIV "division by zero"

/* vector size */
#define vec_size(v)      ( (v)->size )
#define vec_set_size(v,s) ( vec_size(v) = (s) )

/* get/set a vector element */
#define vec_buf(v)         (v)->elem
#define vec_elem(v,n)      vec_buf(v)[n]
#define vec_set_elem(v,n,e) ( vec_elem(v,n) = (e) )
vec_t vec_set_elem_list(vec_t v, ... );
vec_t vec_clear(vec_t v);

/* create, destroy a vector */
vec_t vec_create(size_t size);
vec_t vec_create_list(size_t size, ... );
vec_t vec_create_array(size_t size, double array[]);
void vec_destroy(vec_t v);

/* arithmatic operations */
vec_t vec_add(vec_t v1, vec_t v2, vec_t v);
vec_t vec_sub(vec_t v1, vec_t v2, vec_t v);
vec_t vec_mul(vec_t v1, double k, vec_t v);
vec_t vec_div(vec_t v1, double k, vec_t v);

#endif /* __RHC_VEC_H__ */
