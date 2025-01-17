#ifndef __RHC_CTRL_REP_HOP_STAND_H__
#define __RHC_CTRL_REP_HOP_STAND_H__

#include "rhc_ctrl.h"

enum ctrl_rep_hop_stand_types{
  rep_hop_stand_default = 0,
  soft_landing_piecewise,
  soft_landing_linear,
  no_update_params,
};

typedef struct{
  enum ctrl_rep_hop_stand_types type;
  double (*_calc_phi_ratio)(double);
  ctrl_t* (*_update_params)(ctrl_t *, vec_t);
  cmd_t params;
  double q1, q2;
  double vm;
  bool cushioning;
} ctrl_rep_hop_stand_prp;

#define ctrl_rep_hop_stand_get_prp(self)    ( (ctrl_rep_hop_stand_prp*)((self)->prp) )
#define ctrl_rep_hop_stand_type(self)       ( ctrl_rep_hop_stand_get_prp(self)->type )
#define ctrl_rep_hop_stand_q1(self)         ( ctrl_rep_hop_stand_get_prp(self)->q1 )
#define ctrl_rep_hop_stand_q2(self)         ( ctrl_rep_hop_stand_get_prp(self)->q2 )
#define ctrl_rep_hop_stand_vm(self)         ( ctrl_rep_hop_stand_get_prp(self)->vm )
#define ctrl_rep_hop_stand_cushioning(self) ( ctrl_rep_hop_stand_get_prp(self)->cushioning )
#define ctrl_rep_hop_stand_r(self)          ( ctrl_rep_hop_stand_get_prp(self)->r )
#define ctrl_rep_hop_stand_sqr_gamma(self)  ( ctrl_rep_hop_stand_get_prp(self)->sqr_gamma )
#define ctrl_rep_hop_stand_f_gamma(self)    ( ctrl_rep_hop_stand_get_prp(self)->f_gamma )
#define ctrl_rep_hop_stand_phi(self)        ( ctrl_rep_hop_stand_get_prp(self)->phi )

#define ctrl_rep_hop_stand_rho(self)         ( ctrl_cmd(self)->rep_hop_stand.rho )
#define ctrl_rep_hop_stand_k(self)           ( ctrl_cmd(self)->rep_hop_stand.k )
#define ctrl_rep_hop_stand_soft_landing(self) ( ctrl_cmd(self)->rep_hop_stand.soft_landing )
#define ctrl_rep_hop_stand_set_rho(self,val) ( ctrl_rep_hop_stand_rho(self) = (val) )
#define ctrl_rep_hop_stand_set_k(self,val)   ( ctrl_rep_hop_stand_k(self) = (val) )
#define ctrl_rep_hop_stand_enable_soft_landing(self) ( ctrl_rep_hop_stand_soft_landing(self) = true )
#define ctrl_rep_hop_stand_disable_soft_landing(self) ( ctrl_rep_hop_stand_soft_landing(self) = false )

#define ctrl_rep_hop_stand_params(self)     ( &(ctrl_rep_hop_stand_get_prp(self)->params) )
#define ctrl_rep_hop_stand_params_za(self)  ( ctrl_rep_hop_stand_params(self)->za )
#define ctrl_rep_hop_stand_params_zh(self)  ( ctrl_rep_hop_stand_params(self)->zh )
#define ctrl_rep_hop_stand_params_zm(self)  ( ctrl_rep_hop_stand_params(self)->zm )
#define ctrl_rep_hop_stand_params_zb(self)  ( ctrl_rep_hop_stand_params(self)->zb )
#define ctrl_rep_hop_stand_params_rho(self) ( ctrl_rep_hop_stand_params(self)->rep_hop_stand.rho )

double ctrl_rep_hop_stand_calc_q1(double zh, double zm, double g);
double ctrl_rep_hop_stand_calc_r(double zm, double zb);
double ctrl_rep_hop_stand_calc_sqr_vm(double zh, double zm, double zb, double g);
#define ctrl_rep_hop_stand_calc_vm(zh,zm,zb,g) sqrt( ctrl_rep_hop_stand_calc_sqr_vm( zh, zm, zb, g ) )
double ctrl_rep_hop_stand_calc_sqr_gamma(vec_t p, double zh, double zm, double zb, double g);
#define ctrl_rep_hop_stand_calc_gamma(p,zh,zm,zb,g) sqrt( ctrl_rep_hop_stand_calc_sqr_gamma( p, zh, zm, zb, g ) )
double ctrl_rep_hop_stand_calc_za(double zh, double zm, double zb);
double ctrl_rep_hop_stand_calc_zh(double za, double zm, double zb);
double ctrl_rep_hop_stand_calc_zm(double za, double zh, double zb);
double ctrl_rep_hop_stand_calc_zb(double za, double zh, double zm);
ctrl_t *ctrl_rep_hop_stand_update_params_default(ctrl_t *self, vec_t p);
ctrl_t *ctrl_rep_hop_stand_update_params_no_update(ctrl_t *self, vec_t p);

cmd_t *ctrl_rep_hop_stand_cmd_init(ctrl_t *self, cmd_t *cmd);
ctrl_t *ctrl_rep_hop_stand_create_with_type(ctrl_t *self, cmd_t *cmd, model_t *model, enum ctrl_rep_hop_stand_types type);
#define ctrl_rep_hop_stand_create(self, cmd, model) ctrl_rep_hop_stand_create_with_type( self, cmd, model, rep_hop_stand_default )
void ctrl_rep_hop_stand_destroy(ctrl_t *self);
ctrl_t *ctrl_rep_hop_stand_reset(ctrl_t *self, void *util);
ctrl_t *ctrl_rep_hop_stand_update(ctrl_t *self, double t, vec_t p);
void ctrl_rep_hop_stand_header(FILE *fp, void *util);
void ctrl_rep_hop_stand_writer(FILE *fp, ctrl_t *self, void *util);
double ctrl_rep_hop_stand_calc_fz(ctrl_t *self, vec_t p);


#endif /* __RHC_CTRL_REP_HOP_STAND_H__ */
