#include "rhc_ctrl.h"

ctrl_events_t *ctrl_events_init(ctrl_events_t *self)
{
  ctrl_events_event(self, apex).t = 0;
  ctrl_events_event(self, apex).z = 0;
  ctrl_events_event(self, touchdown).t = 0;
  ctrl_events_event(self, touchdown).z = 0;
  ctrl_events_event(self, bottom).t = 0;
  ctrl_events_event(self, bottom).z = 0;
  ctrl_events_event(self, liftoff).t = 0;
  ctrl_events_event(self, liftoff).z = 0;
  self->phase = initial;
  ctrl_events_phi(self) = 0;
  ctrl_events_n(self) = 0;
  return self;
}

void ctrl_events_destroy(ctrl_events_t *self)
{
  ctrl_events_init( self );
}

ctrl_t *ctrl_init(ctrl_t *self, cmd_t *cmd, model_t *model)
{
  self->cmd = cmd;
  self->model = model;
  self->fz = 0;
  self->_update = ctrl_update_default;
  self->_destroy = ctrl_destroy_default;
  self->_header = ctrl_header_default;
  self->_writer = ctrl_writer_default;
  self->prp = NULL;
  ctrl_n( self ) = 0;
  ctrl_phi( self ) = 0;
  return self;
}

void ctrl_destroy_default(ctrl_t *self)
{
  self->cmd = NULL;
  self->model = NULL;
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

bool ctrl_is_in_extension(ctrl_t *self, vec_t p)
{
  return ( vec_elem(p,1) >= 0 ) && ( vec_elem(p,0) <= ctrl_z0(self) );
}

double ctrl_calc_sqr_v0(double z0, double zd)
{
  return 2.0 * G * ( zd - z0 );
}

ctrl_t *ctrl_update_default(ctrl_t *self, double t, vec_t p)
{
  double phi;

  phi = ctrl_calc_phi( ctrl_z0(self), ctrl_zd(self), ctrl_zb(self), p );
  if( ctrl_phi( self ) < 0 && phi >= 0 ) ctrl_n(self)++;
  ctrl_phi( self ) = phi;
  return self;
}

void ctrl_header_default(FILE *fp, void *util)
{
  fprintf( fp, "\n" );
}

void ctrl_writer_default(FILE *fp, ctrl_t *self, void *util)
{
  fprintf( fp, "\n" );
}

complex_t *ctrl_calc_phase_complex(double z0, double zd, double zb, vec_t p, complex_t *c)
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

  ctrl_calc_phase_complex( z0, zd, zb, p, &c );
  return complex_arg( &c );
}
