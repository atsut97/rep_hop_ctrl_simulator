#include "rhc_vec.h"

rhcVec rhcVecCreate(unsigned int size)
{
  rhcVec v;

  if( ( v = (_rhcVec *)malloc( sizeof(_rhcVec) ) ) == NULL ){
    fprintf( stderr, "cannot allocate memory\n" );
    return NULL;
  }
  if( ( v->e = (double *)malloc( sizeof(double) * size ) ) == NULL ){
    fprintf( stderr, "cannot allocate memory\n" );
    return NULL;
  }
  v->size = size;
  return v;
}
