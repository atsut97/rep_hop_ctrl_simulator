#include "rhc_ctrl.h"

ctrl_t *ctrl_init(ctrl_t *self, cmd_t *cmd)
{
  self->cmd = cmd;
  self->fz = 0;
  self->_update = ctrl_update_default;
  self->_destroy = ctrl_destroy_default;
  self->prp = NULL;
  ctrl_n( self ) = 0;
  ctrl_phi( self ) = 0;
  return self;
}

void ctrl_destroy_default(ctrl_t *self)
{
  self->cmd = NULL;
  self->prp = NULL;
  ctrl_n( self ) = 0;
  ctrl_phi( self ) = 0;
}

bool ctrl_is_in_flight(ctrl_t *self, vec_t p)
{
  return ( vec_elem(p,0) > ctrl_z0(self) );
}

bool ctrl_is_in_compression(ctrl_t *self, vec_t p)
{
  return ( vec_elem(p,1) < 0 ) && ( vec_elem(p,0) <= ctrl_z0(self) );
}

bool ctrl_is_in_decompression(ctrl_t *self, vec_t p)
{
  return ( vec_elem(p,1) >= 0 ) && ( vec_elem(p,0) <= ctrl_z0(self) );
}

double ctrl_calc_sqr_v0(double z0, double zd)
{
  return 2.0 * G * ( zd - z0 );
}

ctrl_t *ctrl_update_default(ctrl_t *self, double t, vec_t p)
{
  /* dummy */
  return self;
}

complex_t *ctrl_calc_phase(double z0, double zd, double zb, vec_t p, complex_t *c)
{
  double z, v, v0;

  z = vec_elem( p, 0 );
  v = vec_elem( p, 1 );
  v0 = ctrl_calc_v0( z0, zd );
  complex_init( c, (z-z0)/(z0-zb), -v/v0 );
  return c;
}

double ctrl_calc_phi(double z0, double zd, double zb, vec_t p)
{
  complex_t c;

  ctrl_calc_phase( z0, zd, zb, p, &c );
  return complex_arg( &c );
}
