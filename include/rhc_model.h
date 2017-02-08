#ifndef __RHC_MODEL_H__
#define __RHC_MODEL_H__

#include "rhc_vec.h"
#include "rhc_cmd.h"

typedef struct{
  double m;
  cmd_t *cmd;
} model_t;

#define model_mass(self) ( (self)->m )
#define model_cmd(self)  ( (self)->cmd )

model_t *model_init(model_t *self, double m, cmd_t *cmd);
void model_destroy(model_t *self);

#endif /* __RHC_MODEL_H__ */
