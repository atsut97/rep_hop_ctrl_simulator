#ifndef __RHC_MODEL_H__
#define __RHC_MODEL_H__

#include "rhc_misc.h"

typedef struct{
  double m;
} model_t;

#define model_mass(self) ( (self)->m )

#define model_set_mass(self,m) ( model_mass(self) = (m) )

model_t *model_init(model_t *self, double m);
void model_destroy(model_t *self);

double model_calc_acc(double m, double fz, double fe);
#define model_acc(self,fz,fe) model_calc_acc( (self)->m, fz, fe )

#endif /* __RHC_MODEL_H__ */
