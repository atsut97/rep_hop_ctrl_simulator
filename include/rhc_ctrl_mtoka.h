#ifndef __RHC_CTRL_MTOKA_H__
#define __RHC_CTRL_MTOKA_H__

#include "rhc_ctrl.h"
#include "rhc_mtoka_osci.h"

typedef struct{
  mtoka_osci_t osci;
} ctrl_mtoka_prp;

ctrl_t *ctrl_mtoka_create(ctrl_t *self, cmd_t *cmd, model_t *model);
void ctrl_mtoka_destroy(ctrl_t *self);
ctrl_t *ctrl_mtoka_update(ctrl_t *self, double t, vec_t p);
void ctrl_mtoka_header(FILE *fp, void *util);
void ctrl_mtoka_writer(FILE *fp, ctrl_t *self, void *util);

#define ctrl_mtoka_rise_time_const(self)        ( ctrl_cmd(self)->mtoka.tau )
#define ctrl_mtoka_adapt_time_const(self)       ( ctrl_cmd(self)->mtoka.T )
#define ctrl_mtoka_mutual_inhibit_weights(self) ( ctrl_cmd(self)->mtoka.a )
#define ctrl_mtoka_steady_firing_rate(self)     ( ctrl_cmd(self)->mtoka.b )
#define ctrl_mtoka_firing_threshold(self)       ( ctrl_cmd(self)->mtoka.th )
#define ctrl_mtoka_feedback_gain(self)          ( ctrl_cmd(self)->mtoka.mu )
#define ctrl_mtoka_sensory_gain(self)           ( ctrl_cmd(self)->mtoka.rho )
#define ctrl_mtoka_saturation_gain(self)        ( ctrl_cmd(self)->mtoka.lam )
#define ctrl_mtoka_get_prp(self)                ( (ctrl_mtoka_prp*)((self)->prp) )
#define ctrl_mtoka_osci(self)                   ( &(ctrl_mtoka_get_prp(self)->osci) )

#define ctrl_mtoka_set_rise_time_const(self,val)        ( ctrl_mtoka_rise_time_const(self)        = (val) )
#define ctrl_mtoka_set_adapt_time_const(self,val)       ( ctrl_mtoka_adapt_time_const(self)       = (val) )
#define ctrl_mtoka_set_mutual_inhibit_weights(self,val) ( ctrl_mtoka_mutual_inhibit_weights(self) = (val) )
#define ctrl_mtoka_set_steady_firing_rate(self,val)     ( ctrl_mtoka_steady_firing_rate(self)     = (val) )
#define ctrl_mtoka_set_firing_threshold(self,val)       ( ctrl_mtoka_firing_threshold(self)       = (val) )
#define ctrl_mtoka_set_feedback_gain(self,val)          ( ctrl_mtoka_feedback_gain(self)          = (val) )
#define ctrl_mtoka_set_sensory_gain(self,val)           ( ctrl_mtoka_sensory_gain(self)           = (val) )
#define ctrl_mtoka_set_saturation_gain(self,val)        ( ctrl_mtoka_saturation_gain(self)        = (val) )

ctrl_t *ctrl_mtoka_set_params(ctrl_t *self, double tau, double T, double a, double b, double th, double mu, double rho, double lam);
ctrl_t *ctrl_mtoka_update_params(ctrl_t *self);

double ctrl_mtoka_calc_sensory_feedback(ctrl_t *self, double phase);
double ctrl_mtoka_calc_fz(ctrl_t *self, double t, vec_t p);

#endif /* __RHC_CTRL_MTOKA_H__ */
