#ifndef __RHC_CTRL_REP_HOP_STAND_H__
#define __RHC_CTRL_REP_HOP_STAND_H__

#include "rhc_ctrl.h"

enum ctrl_rep_hop_stand_types{
  none = -1,
  no_soft_landing = 0,
  soft_landing_discont_rho,
  soft_landing_cont_rho,
};

typedef struct{
  enum ctrl_rep_hop_stand_types type;
  cmd_t params;
  double q1, q2;
  double vh, vm;
  double r, sqr_gamma, f_gamma;
  double phase;
} ctrl_rep_hop_stand_prp;

#define ctrl_rep_hop_stand_get_prp(self)   ( (ctrl_rep_hop_stand_prp*)((self)->prp) )
#define ctrl_rep_hop_stand_type(self)      ( ctrl_rep_hop_stand_get_prp(self)->type )
#define ctrl_rep_hop_stand_q1(self)        ( ctrl_rep_hop_stand_get_prp(self)->q1 )
#define ctrl_rep_hop_stand_q2(self)        ( ctrl_rep_hop_stand_get_prp(self)->q2 )
#define ctrl_rep_hop_stand_vh(self)        ( ctrl_rep_hop_stand_get_prp(self)->vh )
#define ctrl_rep_hop_stand_vm(self)        ( ctrl_rep_hop_stand_get_prp(self)->vm )
#define ctrl_rep_hop_stand_r(self)         ( ctrl_rep_hop_stand_get_prp(self)->r )
#define ctrl_rep_hop_stand_sqr_gamma(self) ( ctrl_rep_hop_stand_get_prp(self)->sqr_gamma )
#define ctrl_rep_hop_stand_f_gamma(self)   ( ctrl_rep_hop_stand_get_prp(self)->f_gamma )
#define ctrl_rep_hop_stand_phase(self)     ( ctrl_rep_hop_stand_get_prp(self)->phase )

#define ctrl_rep_hop_stand_rho(self)         ( ctrl_cmd(self)->rep_hop_stand.rho )
#define ctrl_rep_hop_stand_k(self)           ( ctrl_cmd(self)->rep_hop_stand.k )
#define ctrl_rep_hop_stand_set_rho(self,val) ( ctrl_rep_hop_stand_rho(self) = (val) )
#define ctrl_rep_hop_stand_set_k(self,val)   ( ctrl_rep_hop_stand_k(self) = (val) )

#define ctrl_rep_hop_stand_params(self)     ( &(ctrl_rep_hop_stand_get_prp(self)->params) )
#define ctrl_rep_hop_stand_params_za(self)  ( ctrl_rep_hop_stand_params(self)->za )
#define ctrl_rep_hop_stand_params_zh(self)  ( ctrl_rep_hop_stand_params(self)->zh )
#define ctrl_rep_hop_stand_params_zm(self)  ( ctrl_rep_hop_stand_params(self)->zm )
#define ctrl_rep_hop_stand_params_zb(self)  ( ctrl_rep_hop_stand_params(self)->zb )
#define ctrl_rep_hop_stand_params_rho(self) ( ctrl_rep_hop_stand_params(self)->rep_hop_stand.rho )

double ctrl_rep_hop_stand_calc_q1(double zh, double zm, double g);
double ctrl_rep_hop_stand_calc_r(double zm, double zb);
double ctrl_rep_hop_stand_calc_sqr_gamma(vec_t p, double zh, double zm, double zb);
#define ctrl_rep_hop_stand_calc_gamma(zh) sqrt( ctrl_rep_hop_stand_calc_sqr_gamma( zh ) )
double ctrl_rep_hop_stand_calc_za(double zh, double zm, double zb);
double ctrl_rep_hop_stand_calc_zh(double za, double zm, double zb);
double ctrl_rep_hop_stand_calc_zm(double za, double zh, double zb);
double ctrl_rep_hop_stand_calc_zb(double za, double zh, double zm);
double ctrl_rep_hop_stand_calc_vm(double zh, double zm, double zb);

ctrl_t *ctrl_rep_hop_stand_create(ctrl_t *self, cmd_t *cmd, model_t *model, enum ctrl_rep_hop_stand_types type);
void ctrl_rep_hop_stand_destroy(ctrl_t *self);
ctrl_t *ctrl_rep_hop_stand_update(ctrl_t *self, double t, vec_t p);
void ctrl_rep_hop_stand_header(FILE *fp, void *util);
void ctrl_rep_hop_stand_writer(FILE *fp, ctrl_t *self, void *util);
double ctrl_rep_hop_stand_calc_fz(ctrl_t *self, vec_t p);


#endif /* __RHC_CTRL_REP_HOP_STAND_H__ */
