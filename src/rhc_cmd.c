#include "rhc_cmd.h"

static const cmd_t __empty_cmd;

cmd_t *cmd_init(cmd_t *self)
{
  /* This function initializes the struct that the pointer self points
   * to with zeros. This supposes that the object __empty_cmd is
   * zero-initialized as static objects with no initializers are
   * implicitly initialized with zeros. */
  *self = __empty_cmd;
  return self;
}

cmd_t *cmd_default_init(cmd_t *self)
{
  cmd_init( self );
  self->za = 0.28;
  self->z0 = 0.26;
  self->zb = 0.24;
  return self;
}

void cmd_destroy(cmd_t *self)
{
  cmd_init( self );
}
