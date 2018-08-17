#ifndef __RHC_PHASE_PORTRAIT_PLOTTER_H__
#define __RHC_PHASE_PORTRAIT_PLOTTER_H__

#include "rhc_simulator.h"

typedef struct {
  int dim;
  vec_t pmin;
  vec_t pmax;
  int *n_sc;

  cmd_t *cmd;
  ctrl_t *ctrl;
  model_t *model;
} ppp_t;

#define ppp_dim(self)      (self)->dim
#define ppp_cmd(self)      (self)->cmd
#define ppp_ctrl(self)     (self)->ctrl
#define ppp_model(self)    (self)->model
#define ppp_min(self)      (self)->pmin
#define ppp_max(self)      (self)->pmax
#define ppp_n_sc(self,ax)  (self)->n_sc[ax]

ppp_t *ppp_init(ppp_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model);
void ppp_destroy(ppp_t *self);

#endif /* __RHC_PHASE_PORTRAIT_PLOTTER_H__ */
