#ifndef __RHC_SIMULATOR_H__
#define __RHC_SIMULATOR_H__

#include "rhc_ode.h"
#include "rhc_cmd.h"
#include "rhc_ctrl.h"
#include "rhc_model.h"

typedef struct{
  double t;
  ode_t ode;
  vec_t p;
  cmd_t *cmd;
  ctrl_t *ctrl;
  model_t *model;
} simulator_t;

#define simulator_cmd(self)   (self)->cmd
#define simulator_ctrl(self)  (self)->ctrl
#define simulator_model(self) (self)->model

simulator_t *simulator_init(simulator_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model);
void simulator_destroy(simulator_t *self);

#endif /* __RHC_SIMULATOR_H__ */
