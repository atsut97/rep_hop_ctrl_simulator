#ifndef __RHC_CTRL_SLIP_H__
#define __RHC_CTRL_SLIP_H__

#include "rhc_ctrl.h"

typedef struct{
  double k;
} ctrl_slip_prp;

#define ctrl_slip_k(self) ( ((ctrl_slip_prp*)((self)->prp))->k )

ctrl_t *ctrl_slip_create(ctrl_t *self, cmd_t *cmd, model_t *model);
void ctrl_slip_destroy(ctrl_t *self);
ctrl_t *ctrl_slip_update(ctrl_t *self, double t, vec_t p);
void ctrl_slip_header(FILE *fp, void *util);
void ctrl_slip_writer(FILE *fp, ctrl_t *self, void *util);

double ctrl_slip_calc_stiffness(double m, double zh, double za, double zb);
#define ctrl_slip_stiffness(self) ctrl_slip_calc_stiffness( ctrl_model(self)->m, ctrl_zh(self), ctrl_za(self), ctrl_zb(self) )

#endif /* __RHC_CTRL_SLIP_H__ */
