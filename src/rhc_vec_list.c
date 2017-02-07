#include "rhc_vec_list.h"

vec_list_node_t *vec_list_node_init(vec_list_node_t *self)
{
  self->v = NULL;
  self->next = self;
  return self;
}

void vec_list_node_destroy(vec_list_node_t *self)
{
  vec_destroy( vec_list_node_data(self) );
  self->v = NULL;
  self->next = NULL;
}

vec_list_node_t *vec_list_node_insert_next(vec_list_node_t *self, vec_list_node_t *new)
{
  new->next = self->next;
  self->next = new;
  return new;
}

vec_list_node_t *vec_list_node_delete_next(vec_list_node_t *self)
{
  vec_list_node_t *node;

  node = self->next;
  self->next = node->next;
  node->next = node;
  return node;
}

vec_t vec_list_node_set_data(vec_list_node_t *self, vec_t v)
{
  self->v = v;
  return v;
}

vec_list_t *vec_list_init(vec_list_t *self)
{
  vec_list_node_init( vec_list_root(self) );
  self->num = 0;
  return self;
}

void vec_list_destroy(vec_list_t *self)
{
  vec_list_node_t *node;

  while( !vec_list_is_empty(self) ){
    node = vec_list_delete_tail( self );
    vec_list_node_destroy( node );
  }
}

vec_list_node_t *vec_list_insert_next(vec_list_t *self, vec_list_node_t *node, vec_list_node_t *new)
{
  vec_list_inc( self );
  return vec_list_node_insert_next( node, new );
}

vec_list_node_t *vec_list_delete_next(vec_list_t *self, vec_list_node_t *node)
{
  vec_list_dec( self );
  return vec_list_node_delete_next( node );
}
