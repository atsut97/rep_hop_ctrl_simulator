#include "rhc_cmd.h"

cmd_t *cmd_init(cmd_t *self)
{
  self->zd = 0;
  self->z0 = 0;
  self->zb = 0;
  return self;
}

cmd_t *cmd_default_init(cmd_t *self)
{
  cmd_init( self );
  self->zd = 0.28;
  self->z0 = 0.26;
  self->zb = 0.24;
  return self;
}

void cmd_destroy(cmd_t *self)
{
  cmd_init( self );
}
