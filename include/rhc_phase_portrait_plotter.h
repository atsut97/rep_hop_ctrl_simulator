#ifndef __RHC_PHASE_PORTRAIT_PLOTTER_H__
#define __RHC_PHASE_PORTRAIT_PLOTTER_H__

#include "rhc_simulator.h"
#include "rhc_vec_list.h"

typedef struct {
  int dim;
  vec_t pmin;
  vec_t pmax;
  int *n_sc;
  vec_list_t p0_list;

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
#define ppp_p0_list(self)  &(self)->p0_list

ppp_t *ppp_init(ppp_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model);
void ppp_destroy(ppp_t *self);

void ppp_set_lim(ppp_t *self, vec_t pmin, vec_t pmax);
void ppp_set_n_sc(ppp_t *self, int *n_sc);
vec_t ppp_push_p0(ppp_t *self, vec_t p0);

#endif /* __RHC_PHASE_PORTRAIT_PLOTTER_H__ */
