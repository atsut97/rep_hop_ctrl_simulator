#ifndef __RHC_VEC_H__
#define __RHC_VEC_H__

#include "rhc_misc.h"

typedef struct{
  size_t size;
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
vec_t vec_clear(vec_t v);

/* create, destroy a vector */
vec_t vec_create(size_t size);
vec_t vec_create_list(size_t size, ... );
vec_t vec_create_array(size_t size, double array[]);
void vec_destroy(vec_t v);

/* copy */
vec_t vec_copy(vec_t src, vec_t dst);

/* clone */
vec_t vec_clone(vec_t src);

/* arithmatic operations */
vec_t vec_add(vec_t v1, vec_t v2, vec_t v);
vec_t vec_sub(vec_t v1, vec_t v2, vec_t v);
vec_t vec_mul(vec_t v1, double k, vec_t v);
vec_t vec_div(vec_t v1, double k, vec_t v);
vec_t vec_cat(vec_t v1, double k, vec_t v2, vec_t v);

/* check equality */
bool vec_match(vec_t v1, vec_t v2);
bool vec_equal(vec_t v1, vec_t v2);
bool vec_near(vec_t v1, vec_t v2, double tol);

/* inner product */
double vec_dot(vec_t v1, vec_t v2);

/* output */
void vec_f_write(FILE *fp, vec_t v);
#define vec_write(v) vec_f_write( stdout, v )

#endif /* __RHC_VEC_H__ */
