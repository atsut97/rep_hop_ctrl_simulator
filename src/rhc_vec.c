#include "rhc_vec.h"

rhcVec rhcVecCreate(uint size)
{
  rhcVec v;

  if( ( v = rhcAlloc( _rhcVec, 1 ) ) == NULL ){
    eprintf( "cannot allocate memory\n" );
    return NULL;
  }
  if( ( v->e = rhcAlloc( double, size ) ) == NULL ){
    eprintf( "cannot allocate memory\n" );
    return NULL;
  }
  v->size = size;
  return v;
}

void rhcVecDestroy(rhcVec v)
{
  if( v->e )
    rhcFree( v->e );
  rhcFree( v );
}
