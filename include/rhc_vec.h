#ifndef __RHC_VEC_H__
#define __RHC_VEC_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct{
  unsigned int size;
  double *e;
} _rhcVec;

typedef _rhcVec* rhcVec;

#define rhcVecSize(v) ( v->size )

rhcVec rhcVecCreate(unsigned int size);
void rhcVecDestroy(rhcVec v);

#endif /* __RHC_VEC_H__ */
