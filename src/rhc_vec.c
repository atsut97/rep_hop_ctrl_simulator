#include "rhc_vec.h"

static vec_t _vec_set_elem_vlist(vec_t v, va_list args);
vec_t _vec_set_elem_vlist(vec_t v, va_list args)
{
  register size_t i;

  for( i=0; i<vec_size(v); i++ )
    vec_set_elem( v, i, (double)va_arg( args, double ) );
  return v;
}

vec_t vec_set_elem_list(vec_t v, ... )
{
  va_list args;

  va_start( args, v );
  _vec_set_elem_vlist( v, args );
  va_end( args );
  return v;
}

vec_t vec_create(size_t size)
{
  vec_t v;

  if( ( v = nalloc( _vec_t, 1 ) ) == NULL ){
    eprintf( "cannot allocate memory\n" );
    return NULL;
  }
  if( size > 0 ){
    if( ( v->elem = nalloc( double, size ) ) == NULL ){
      eprintf( "cannot allocate memory\n" );
      return NULL;
    }
  } else
    v->elem = NULL;
  v->size = size;
  return v;
}

vec_t vec_create_list(size_t size, ... )
{
  vec_t v;
  va_list args;

  if( ( v = vec_create( size ) ) == NULL )
    return NULL;
  va_start( args, size );
  _vec_set_elem_vlist( v, args );
  va_end( args );
  return v;
}

vec_t vec_create_array(size_t size, double array[])
{
  vec_t v;

  if( ( v = vec_create( size ) ) == NULL )
    return NULL;
  memcpy( vec_buf( v ), array, sizeof(double)*size );
  return v;
}

void vec_destroy(vec_t v)
{
  if( vec_buf(v) )
    sfree( vec_buf(v) );
  sfree( v );
}

vec_t vec_add(vec_t v1, vec_t v2, vec_t v)
{
  size_t i;

  for( i=0; i<vec_size(v1); i++ )
    vec_set_elem( v, i, vec_elem(v1,i) + vec_elem(v2,i) );
  return v;
}
