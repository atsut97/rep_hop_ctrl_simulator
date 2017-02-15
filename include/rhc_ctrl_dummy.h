#ifndef RHC_CTRL_DUMMY_H
#define RHC_CTRL_DUMMY_H

#include "rhc_ctrl.h"

typedef struct{
  double k;
} ctrl_dummy_prp;

ctrl_t *ctrl_dummy_create(ctrl_t *self, cmd_t *cmd, double k);
void ctrl_dummy_destroy(ctrl_t *self);
ctrl_t *ctrl_dummy_update(ctrl_t *self, double t, vec_t p);

#endif /* RHC_CTRL_DUMMY_H */
