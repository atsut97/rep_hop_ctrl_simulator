#ifndef __RHC_VEC_LIST_H__
#define __RHC_VEC_LIST_H__

#include "rhc_vec.h"

typedef struct _vec_list_node_t{
  vec_t v;
  struct _vec_list_node_t *next;
} vec_list_node_t;

#define vec_list_node_data(self) ( (self)->v )
#define vec_list_node_next(self) ( (self)->next )

vec_list_node_t *vec_list_node_init(vec_list_node_t *self);
void vec_list_node_destroy(vec_list_node_t *self);
vec_list_node_t *vec_list_node_insert_next(vec_list_node_t *self, vec_list_node_t *new);
vec_list_node_t *vec_list_node_delete_next(vec_list_node_t *self);
vec_t vec_list_node_set_data(vec_list_node_t *self, vec_t v);

typedef struct _vec_list_t{
  size_t num;
  vec_list_node_t root;
} vec_list_t;

#define vec_list_num(self)  ( (self)->num )
#define vec_list_root(self) ( &(self)->root )
#define vec_list_tail(self) vec_list_node_next( vec_list_root(self) )
#define vec_list_inc(self)  ( vec_list_num(self)++ )
#define vec_list_dec(self)  ( vec_list_num(self)-- )

vec_list_t *vec_list_init(vec_list_t *self);
void vec_list_destroy(vec_list_t *self);
vec_list_node_t *vec_list_insert_next(vec_list_t *self, vec_list_node_t *node, vec_list_node_t *new);
#define vec_list_insert_tail(self,n) vec_list_insert_next( self, vec_list_root(self), (n) )
vec_list_node_t *vec_list_delete_next(vec_list_t *self, vec_list_node_t *node);
#define vec_list_delete_tail(self) vec_list_delete_next( self, vec_list_root(self) )

#endif /* __RHC_VEC_LIST_H__ */
