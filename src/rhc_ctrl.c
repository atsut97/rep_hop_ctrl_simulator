#include "rhc_ctrl.h"
#include "rhc_misc.h"

static enum _ctrl_events_phases_t _ctrl_events_determine_phase(double phi);
static void _ctrl_events_update_event(ctrl_events_t *self, enum _ctrl_events_phases_t phase, double t, vec_t p, cmd_t *cmd);

ctrl_events_t *ctrl_events_init(ctrl_events_t *self)
{
  ctrl_events_apex(self)->t = 0;
  ctrl_events_apex(self)->z = 0;
  ctrl_events_apex(self)->v = 0;
  ctrl_events_touchdown(self)->t = 0;
  ctrl_events_touchdown(self)->z = 0;
  ctrl_events_touchdown(self)->v = 0;
  ctrl_events_bottom(self)->t = 0;
  ctrl_events_bottom(self)->z = 0;
  ctrl_events_bottom(self)->v = 0;
  ctrl_events_liftoff(self)->t = 0;
  ctrl_events_liftoff(self)->z = 0;
  ctrl_events_liftoff(self)->v = 0;
  self->phase = invalid;
  ctrl_events_phi(self) = 0;
  ctrl_events_n(self) = 0;
  self->is_updated = false;
  return self;
}

void ctrl_events_destroy(ctrl_events_t *self)
{
  ctrl_events_init( self );
}

complex_t *ctrl_events_calc_phase_complex(double z0, double zd, double zb, vec_t p, complex_t *c)
{
  double z, v, v0;

  z = vec_elem( p, 0 );
  v = vec_elem( p, 1 );
  v0 = ctrl_calc_v0( z0, zd );
  complex_init( c, (z-z0)/(z0-zb), -v/v0 );
  return c;
}

double ctrl_events_calc_phi(double z0, double zd, double zb, vec_t p)
{
  complex_t c;

  ctrl_events_calc_phase_complex( z0, zd, zb, p, &c );
  return complex_arg( &c );
}

bool ctrl_events_is_in_falling(ctrl_events_t *self)
{
  return ctrl_events_phase(self) == falling;
}

bool ctrl_events_is_in_compression(ctrl_events_t *self)
{
  return ctrl_events_phase(self) == compression;
}

bool ctrl_events_is_in_extension(ctrl_events_t *self)
{
  return ctrl_events_phase(self) == extension;
}

bool ctrl_events_is_in_rising(ctrl_events_t *self)
{
  return ctrl_events_phase(self) == rising;
}

bool ctrl_events_is_in_flight(ctrl_events_t *self)
{
  /* Flight phase is the union of rising and falling phases. */
  return ctrl_events_is_in_rising( self ) || ctrl_events_is_in_falling( self );
}

enum _ctrl_events_phases_t _ctrl_events_determine_phase(double phi)
{
  if( ( 0 <= phi ) && ( phi < PI_2 ) ){
    /* Falling phase includes apex event, but not touchdown. */
    return falling;
  } else if( ( PI_2 <= phi ) && ( phi < PI ) ){
    /* Compression phase includes touchdown event, but not bottom. */
    return compression;
  } else if( ( -PI <= phi ) && ( phi < -PI_2 ) ){
    /* Extension phase includes bottom event, but not lift-off. */
    return extension;
  } else if( ( -PI_2 <= phi ) && ( phi < 0 ) ){
    /* Rising phase includes lift-off event, but not apex. */
    return rising;
  } else {
    return invalid;
  }
}

void _ctrl_events_update_event(ctrl_events_t *self, enum _ctrl_events_phases_t phase, double t, vec_t p, cmd_t *cmd)
{
  double z, v;

  z = vec_elem( p, 0 );
  v = vec_elem( p, 1 );
  if( phase == rising || ctrl_events_is_in_rising( self ) ){
    if( ctrl_events_is_in_extension( self ) ||
        z >= ctrl_events_apex(self)->z ){
      ctrl_events_set_apex( self, t, z, v );
    }
  }
  if( phase == falling || ctrl_events_is_in_falling( self ) ){
    if( ctrl_events_is_in_rising( self ) ||
        v <= ctrl_events_touchdown(self)->v ){
      ctrl_events_set_touchdown( self, t, z, v );
    }
  }
  if( phase == compression || ctrl_events_is_in_compression( self ) ){
    if( ctrl_events_is_in_falling( self ) ||
        z <= ctrl_events_bottom(self)->z ){
      ctrl_events_set_bottom( self, t, z, v );
    }
  }
  if( phase == extension || ctrl_events_is_in_extension( self ) ){
    if( ctrl_events_is_in_compression( self ) ||
        v >= ctrl_events_liftoff(self)->v ){
      ctrl_events_set_liftoff( self, t, z, v );
    }
  }
}

ctrl_events_t *ctrl_events_update(ctrl_events_t *self, double t, vec_t p, cmd_t *cmd)
{
  double phi;
  enum _ctrl_events_phases_t phase;

  if( ctrl_events_is_updated(self) ) return self;
  phi = ctrl_events_calc_phi( cmd->z0, cmd->zd, cmd->zb, p );
  if( ctrl_events_phi( self ) < 0 && phi >= 0 ) ctrl_events_n(self)++;
  phase = _ctrl_events_determine_phase( phi );
  _ctrl_events_update_event( self, phase, t, p, cmd );
  ctrl_events_phi(self) = phi;
  ctrl_events_phase(self) = phase;
  ctrl_events_is_updated(self) = true;
  return self;
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
  ctrl_events_init( ctrl_events( self ) );
  return self;
}

void ctrl_destroy_default(ctrl_t *self)
{
  self->cmd = NULL;
  self->model = NULL;
  self->prp = NULL;
  ctrl_events_destroy( ctrl_events( self ) );
}

double ctrl_calc_sqr_v0(double z0, double zd)
{
  return 2.0 * G * ( zd - z0 );
}

ctrl_t *ctrl_update_default(ctrl_t *self, double t, vec_t p)
{
  ctrl_events_update_next( ctrl_events( self ) );
  ctrl_events_update( ctrl_events( self ), t, p, ctrl_cmd( self ) );
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
