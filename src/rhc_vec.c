#include "rhc_vec.h"

static vec_t _vec_set_elem_vlist(vec_t v, va_list args);
vec_t _vec_set_elem_vlist(vec_t v, va_list args)
{
  register uint i;

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

vec_t vec_create(uint size)
{
  vec_t v;

  if( ( v = nalloc( _vec_t, 1 ) ) == NULL ){
    eprintf( "cannot allocate memory\n" );
    return NULL;
  }
  if( ( v->elem = nalloc( double, size ) ) == NULL ){
    eprintf( "cannot allocate memory\n" );
    return NULL;
  }
  v->size = size;
  return v;
}

vec_t vec_create_list(uint size, ... )
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

void vec_destroy(vec_t v)
{
  if( vec_buf(v) )
    sfree( vec_buf(v) );
  sfree( v );
}
