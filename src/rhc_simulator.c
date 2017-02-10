#include "rhc_simulator.h"

simulator_t *simulator_init(simulator_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model)
{
  self->cmd = cmd;
  self->ctrl = ctrl;
  self->model = model;
  return self;
}

void simulator_destroy(simulator_t *self)
{
  self->cmd = NULL;
  self->ctrl = NULL;
  self->model = NULL;
}
