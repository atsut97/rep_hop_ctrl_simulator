#ifndef __RHC_CTRL_ARL_H__
#define __RHC_CTRL_ARL_H__

#include "rhc_ctrl.h"

enum ctrl_arl_types{
  none = -1,
  ahmadi2007 = 0,
  michalska1997 = 1,
};

typedef struct {
  enum ctrl_arl_types type; /* controller type */
  double c;
} ctrl_arl_prp;

ctrl_t *ctrl_arl_create(ctrl_t *self, cmd_t *cmd, model_t *model, enum ctrl_arl_types type);
void ctrl_arl_destroy(ctrl_t *self);
ctrl_t *ctrl_arl_update(ctrl_t *self, double t, vec_t p);
void ctrl_arl_header(FILE *fp, void *util);
void ctrl_arl_writer(FILE *fp, ctrl_t *self, void *util);

#define ctrl_arl_k(self)       ( ctrl_cmd(self)->arl.k )
#define ctrl_arl_beta(self)    ( ctrl_cmd(self)->arl.beta )
#define ctrl_arl_get_prp(self) ( (ctrl_arl_prp*)((self)->prp) )
#define ctrl_arl_type(self)    ( ctrl_arl_get_prp(self)->type )
#define ctrl_arl_c(self)       ( ctrl_arl_get_prp(self)->c )

#define ctrl_arl_set_k(self,val)     ( ctrl_arl_k(self) = (val) )
#define ctrl_arl_set_beta(self,val)  ( ctrl_arl_beta(self)   = (val) )

ctrl_t *ctrl_arl_set_params(ctrl_t *self, double k, double beta);

double ctrl_arl_calc_fz(ctrl_t *self, double t, vec_t p);

#endif /* __RHC_CTRL_ARL_H__ */
