#include "rhc_ctrl.h"

ctrl_t *ctrl_init(ctrl_t *self, cmd_t *cmd)
{
  self->cmd = cmd;
  return self;
}

void ctrl_destroy(ctrl_t *self)
{
  self->cmd = NULL;
}
