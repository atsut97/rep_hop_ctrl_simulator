#include "rhc_model.h"

model_t *model_init(model_t *self, double m, cmd_t *cmd)
{
  self->m = m;
  self->cmd = cmd;
  return self;
}

void model_destroy(model_t *self)
{
  self->m = 0;
  self->cmd = NULL;
}
