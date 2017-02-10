#include "rhc_ctrl.h"

ctrl_t *ctrl_init(ctrl_t *self, cmd_t *cmd)
{
  self->cmd = cmd;
  self->fz = 0;
  return self;
}

void ctrl_destroy(ctrl_t *self)
{
  self->cmd = NULL;
}

bool ctrl_is_in_flight(ctrl_t *self, vec_t p)
{
  return ( vec_elem(p,0) > self->cmd->z0 );
}

bool ctrl_is_in_compression(ctrl_t *self, vec_t p)
{
  return ( vec_elem(p,1) < 0 ) && ( vec_elem(p,0) <= self->cmd->z0 );
}

bool ctrl_is_in_decompression(ctrl_t *self, vec_t p)
{
  return ( vec_elem(p,1) >= 0 ) && ( vec_elem(p,0) <= self->cmd->z0 );
}
