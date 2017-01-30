#include "rhc_vec.h"

static rhcVec _rhcVecSetElemVList(rhcVec v, va_list args);

rhcVec _rhcVecSetElemVList(rhcVec v, va_list args)
{
  register uint i;

  for( i=0; i<rhcVecSize(v); i++ )
    rhcVecSetElem( v, i, (double)va_arg( args, double ) );
  return v;
}

rhcVec rhcVecSetElemList(rhcVec v, ... )
{
  va_list args;

  va_start( args, v );
  /* for( i=0; i<rhcVecSize(v); i++ ) */
  /*   rhcVecSetElem( v, i, (double)va_arg( args, double ) ); */
  _rhcVecSetElemVList( v, args );
  va_end( args );
  return v;
}

rhcVec rhcVecCreate(uint size)
{
  rhcVec v;

  if( ( v = rhcAlloc( _rhcVec, 1 ) ) == NULL ){
    eprintf( "cannot allocate memory\n" );
    return NULL;
  }
  if( ( v->elem = rhcAlloc( double, size ) ) == NULL ){
    eprintf( "cannot allocate memory\n" );
    return NULL;
  }
  v->size = size;
  return v;
}

rhcVec rhcVecCreateList(uint size, ... )
{
  rhcVec v;
  va_list args;

  if( ( v = rhcVecCreate( size ) ) == NULL )
    return NULL;
  va_start( args, size );
  _rhcVecSetElemVList( v, args );
  va_end( args );
  return v;
}

void rhcVecDestroy(rhcVec v)
{
  if( rhcVecBuf(v) )
    rhcFree( rhcVecBuf(v) );
  rhcFree( v );
}
