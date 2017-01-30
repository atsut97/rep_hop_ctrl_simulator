#ifndef __RHC_VEC_H__
#define __RHC_VEC_H__

#include "rhc_misc.h"

typedef struct{
  uint size;
  double *elem;
} _rhcVec;

typedef _rhcVec * rhcVec;

/* vector size */
#define rhcVecSize(v)      ( (v)->size )
#define rhcVecSetSize(v,s) (rhcVecSize(v) = (s) )

/* get/set a vector element */
#define rhcVecBuf(v)         (v)->elem
#define rhcVecElem(v,n)      rhcVecBuf(v)[n]
#define rhcVecSetElem(v,n,e) ( rhcVecElem(v,n) = (e) )
rhcVec rhcVecSetElemList(rhcVec v, ... );

/* create, destroy a vector */
rhcVec rhcVecCreate(uint size);
rhcVec rhcVecCreateList(uint size, ... );
void rhcVecDestroy(rhcVec v);

#endif /* __RHC_VEC_H__ */
