#ifndef __RHC_CTRL_MTOKA_H__
#define __RHC_CTRL_MTOKA_H__

#include "rhc_ctrl.h"

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

#endif /* __RHC_CTRL_MTOKA_H__ */
