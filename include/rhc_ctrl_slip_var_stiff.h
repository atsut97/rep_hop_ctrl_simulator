#ifndef __RHC_CTRL_SLIP_VAR_STIFF_H__
#define __RHC_CTRL_SLIP_VAR_STIFF_H__

#include "rhc_ctrl.h"

typedef struct{
  double k;
} ctrl_slip_var_stiff_prp;

ctrl_t *ctrl_slip_var_stiff_create(ctrl_t *self, cmd_t *cmd, model_t *model);
void ctrl_slip_var_stiff_destroy(ctrl_t *self);
ctrl_t *ctrl_slip_var_stiff_update(ctrl_t *self, double t, vec_t p);
void ctrl_slip_var_stiff_header(FILE *fp, void *util);
void ctrl_slip_var_stiff_writer(FILE *fp, ctrl_t *self, void *util);

double ctrl_slip_var_stiff_calc_stiffness_decomp(vec_t p, double m, double z0, double zd, double zb);
double ctrl_slip_var_stiff_calc_stiffness_comp(vec_t p, double m, double z0, double zd, double zb);
double ctrl_slip_var_stiff_calc_stiffness(vec_t p, double m, double z0, double zd, double zb);
#define ctrl_slip_var_stiff_stiffness(self,p) ctrl_slip_var_stiff_calc_stiffness( p, ctrl_model(self)->m, ctrl_z0(self), ctrl_zd(self), ctrl_zb(self) )

#endif /* __RHC_CTRL_SLIP_VAR_STIFF_H__ */
