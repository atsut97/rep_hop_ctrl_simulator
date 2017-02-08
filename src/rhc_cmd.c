#include "rhc_cmd.h"

cmd_t *cmd_init(cmd_t *self)
{
  self->zd = 0;
  self->z0 = 0;
  self->zb = 0;
  return self;
}

void cmd_destroy(cmd_t *self)
{
  cmd_init( self );
}
