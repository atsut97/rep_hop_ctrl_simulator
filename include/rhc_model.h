#ifndef __RHC_MODEL_H__
#define __RHC_MODEL_H__

#include "rhc_vec.h"

typedef struct{
  double m;
} model_t;

#define model_mass(self) ( (self)->m )

model_t *model_init(model_t *self, double m);
void model_destroy(model_t *self);

#endif /* __RHC_MODEL_H__ */
