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
  double sqr_R_des;
  double sqr_R;
  double delta;
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

#define ctrl_arl_set_k(self,val)     ( ctrl_arl_k(self) = (val) )
#define ctrl_arl_set_beta(self,val)  ( ctrl_arl_beta(self) = (val) )

ctrl_t *ctrl_arl_set_params(ctrl_t *self, double k, double beta);

double ctrl_arl_calc_sqr_R_des(double m, double k, double zh, double za, double g);
double ctrl_arl_calc_sqr_R(vec_t p, double m, double k, double zh, double g);
double ctrl_arl_calc_delta(vec_t p, double m, double k, double beta, double zh, double za, double g);
#define ctrl_arl_sqr_R_des(self) \
  ( ctrl_arl_calc_sqr_R_des( ctrl_model(self)->m, ctrl_arl_k(self), ctrl_zh(self), ctrl_za(self), ctrl_model(self)->gravity ) )
#define ctrl_arl_sqr_R(self,p) \
  ( ctrl_arl_calc_sqr_R( p, ctrl_model(self)->m, ctrl_arl_k(self), ctrl_zh(self), ctrl_model(self)->gravity ) )
#define ctrl_arl_delta(self,p) \
  ( ctrl_arl_calc_delta( p, ctrl_model(self)->m, ctrl_arl_k(self), ctrl_arl_beta(self), ctrl_zh(self), ctrl_za(self), ctrl_model(self)->gravity ) )

double ctrl_arl_calc_fz(ctrl_t *self, double t, vec_t p);

#endif /* __RHC_CTRL_ARL_H__ */
