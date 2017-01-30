#ifndef __RHC_VEC_H__
#define __RHC_VEC_H__

#include "rhc_misc.h"

typedef struct{
  uint size;
  double *e;
} _rhcVec;

typedef _rhcVec* rhcVec;

#define rhcVecSize(v) ( v->size )

rhcVec rhcVecCreate(uint size);
void rhcVecDestroy(rhcVec v);

#endif /* __RHC_VEC_H__ */
