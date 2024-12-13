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
  self->zh = 0.26;
  self->zm = 0.25;
  self->zb = 0.24;
  self->zmax = self->zh;
  self->zmin = self->zb;
  return self;
}

void cmd_destroy(cmd_t *self)
{
  cmd_init( self );
}

cmd_t *cmd_set(cmd_t *self, double za, double zh, double zm, double zb)
{
  self->za = za;
  self->zh = zh;
  self->zm = zm;
  self->zb = zb;
  return self;
}

cmd_t *cmd_set_limits(cmd_t *self, double zmax, double zmin)
{
  char msg[BUFSIZ];

  if( zmax < zmin ){
    sprintf( msg, "zmax must be higher than zmin: zmax=%f, zmin=%f\n", zmax, zmin );
    RUNTIME_ERR( msg );
  }
  self->zmax = zmax;
  self->zmin = zmin;
  return self;
}

cmd_t *cmd_copy(cmd_t *src, cmd_t *dst)
{
  memcpy(dst, src, sizeof(cmd_t));
  return dst;
}
