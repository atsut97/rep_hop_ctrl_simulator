#ifndef __RHC_SIMULATOR_H__
#define __RHC_SIMULATOR_H__

#include "rhc_ode.h"
#include "rhc_cmd.h"
#include "rhc_ctrl.h"
#include "rhc_model.h"

typedef struct{
  double t;
  ode_t ode;
  vec_t state;
  cmd_t *cmd;
  ctrl_t *ctrl;
  model_t *model;
} simulator_t;

#define simulator_cmd(self)   (self)->cmd
#define simulator_ctrl(self)  (self)->ctrl
#define simulator_model(self) (self)->model
#define simulator_time(self)  (self)->t
#define simulator_state(self) (self)->state

#define simulator_set_time(self,t)  ( simulator_time(self)  = (t) )
#define simulator_inc_time(self,dt) ( simulator_time(self) += (dt) )

simulator_t *simulator_init(simulator_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model);
void simulator_destroy(simulator_t *self);

void simulator_run(simulator_t *self, vec_t p0, double time);

#endif /* __RHC_SIMULATOR_H__ */
