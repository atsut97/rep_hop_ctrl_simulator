#include "rhc_ctrl_raibert.h"
#include "rhc_ctrl.h"
#include "rhc_misc.h"
#include "rhc_model.h"

static double _ctrl_raibert_calc_fz_unit_full_nonlinear(ctrl_t *self, double t, vec_t p);
static double _ctrl_raibert_calc_fz_unit_simplified_nonlinear(ctrl_t *self, double t, vec_t p);
static double _ctrl_raibert_calc_fz_unit_full_linear(ctrl_t *self, double t, vec_t p);
static double _ctrl_raibert_calc_fz_unit_simplified_linear(ctrl_t *self, double t, vec_t p);

ctrl_t *ctrl_raibert_create(ctrl_t *self, cmd_t *cmd, model_t *model, enum ctrl_raibert_types type)
{
  ctrl_init( self, cmd, model );
  self->_update = ctrl_raibert_update;
  self->_destroy = ctrl_raibert_destroy;
  self->_header = ctrl_raibert_header;
  self->_writer = ctrl_raibert_writer;

  if( ( self->prp = nalloc( ctrl_raibert_prp, 1 ) ) == NULL ){
    ALLOC_ERR();
    return NULL;
  }
  ((ctrl_raibert_prp *)(self->prp))->type = type;
  (ctrl_raibert_end_of_thrust(self)).t = 0;
  (ctrl_raibert_end_of_thrust(self)).z = 0;
  (ctrl_raibert_end_of_thrust(self)).v = 0;
  ((ctrl_raibert_prp *)(self->prp))->is_in_thrust = false;
  return self;
}

void ctrl_raibert_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
}

ctrl_t *ctrl_raibert_update(ctrl_t *self, double t, vec_t p)
{
  /* Note that ctrl_events_update() is called in
   * ctrl_update_default() */
  ctrl_update_default( self, t, p );
  ctrl_raibert_update_events( self, t, p );
  self->fz = ctrl_raibert_calc_fz( self, t, p );
  return self;
}

void ctrl_raibert_header(FILE *fp, void *util)
{
  fprintf( fp, ",type,delta,tau,gamma,yeta1,zr,mu,in_th,et_t,et_z,et_v\n" );
}

void ctrl_raibert_writer(FILE *fp, ctrl_t *self, void *util)
{
  fprintf( fp, ",%d,%f,%f,%f,%f,%f,%f,%d,%f,%f,%f\n",
           ctrl_raibert_type(self),
           ctrl_raibert_delta(self),
           ctrl_raibert_tau(self),
           ctrl_raibert_gamma(self),
           ctrl_raibert_yeta1(self),
           ctrl_raibert_zr(self),
           ctrl_raibert_mu(self),
           ctrl_raibert_is_in_thrust(self),
           ctrl_raibert_end_of_thrust(self).t,
           ctrl_raibert_end_of_thrust(self).z,
           ctrl_raibert_end_of_thrust(self).v );
}

ctrl_t *ctrl_raibert_set_params(ctrl_t *self, double delta, double tau, double gamma, double yeta1, double zr, double mu)
{
  ctrl_raibert_set_delta( self, delta );
  ctrl_raibert_set_tau( self, tau );
  ctrl_raibert_set_gamma( self, gamma );
  ctrl_raibert_set_yeta1( self, yeta1 );
  ctrl_raibert_set_zr( self, zr );
  ctrl_raibert_set_mu( self, mu );
  return self;
}

void ctrl_raibert_update_events(ctrl_t *self, double t, vec_t p)
{
  double tb;  /* time at bottom */
  bool is_in_thrust = false;

  /* check if current state is in thrust phase */
  ctrl_events_update( ctrl_events(self), t, p, ctrl_cmd(self) );
  if( ctrl_phase_in( self, extension ) ){
    tb = ctrl_events_at( self, bottom )->t;
    if( ( tb <= t + TOL ) && ( t + TOL < tb + ctrl_raibert_delta(self) ) ){
      /* we need to keep previous phase intact here, we're going to
       * update current thrust phase at the end. */
      is_in_thrust = true;
    }
  }

  /* update end-of-thrust event */
  if( is_in_thrust || ctrl_raibert_is_in_thrust(self) ){
    ctrl_raibert_end_of_thrust(self).t = t;
    ctrl_raibert_end_of_thrust(self).z = vec_elem(p, 0);
    ctrl_raibert_end_of_thrust(self).v = vec_elem(p, 1);
  }
  /* update thrust phase */
  ctrl_raibert_get_prp(self)->is_in_thrust = is_in_thrust;
}

bool ctrl_raibert_is_in_thrust(ctrl_t *self)
{
  return ctrl_raibert_get_prp(self)->is_in_thrust;
}

double _ctrl_raibert_calc_fz_unit_full_nonlinear(ctrl_t *self, double t, vec_t p) {
  double fz_unit;
  double tau, gamma, yeta1;

  tau = ctrl_raibert_tau(self);
  gamma = ctrl_raibert_gamma(self);
  yeta1 = ctrl_raibert_yeta1(self);
  if( ctrl_phase_in(self, compression) ) {
    fz_unit = yeta1 / vec_elem(p,0) - gamma * vec_elem(p,1);
  } else if( ctrl_raibert_is_in_thrust(self) ) {
    fz_unit = tau - gamma * vec_elem(p,1);
  } else if( ctrl_phase_in(self, extension) ) {
    fz_unit = tau * ctrl_raibert_end_of_thrust_z(self) / vec_elem(p,0) - gamma * vec_elem(p,1);
  } else {
    fz_unit = 0;
  }
  return fz_unit;
}

double _ctrl_raibert_calc_fz_unit_simplified_nonlinear(ctrl_t *self, double t, vec_t p) {
  double fz_unit;
  double tau, yeta1;

  tau = ctrl_raibert_tau(self);
  yeta1 = ctrl_raibert_yeta1(self);
  if( ctrl_phase_in(self, compression) ) {
    fz_unit = yeta1 / vec_elem(p,0) + model_gravity( ctrl_model(self) );
  } else if( ctrl_phase_in(self, extension) ) {
    fz_unit = tau * ctrl_events_at(self, bottom)->z / vec_elem(p,0) + model_gravity( ctrl_model(self) );
  } else {
    fz_unit = 0;
  }
  return fz_unit;
}

double _ctrl_raibert_calc_fz_unit_full_linear(ctrl_t *self, double t, vec_t p) {
  double fz_unit;
  double tau, gamma, yeta1, zr;

  tau = ctrl_raibert_tau(self);
  gamma = ctrl_raibert_gamma(self);
  yeta1 = ctrl_raibert_yeta1(self);
  zr = ctrl_raibert_zr(self);
  if( ctrl_phase_in(self, compression) ) {
    fz_unit = yeta1 * ( zr - vec_elem(p,0) ) - gamma * vec_elem(p,1);
  } else if( ctrl_raibert_is_in_thrust(self) ) {
    fz_unit = tau - gamma * vec_elem(p,1);
  } else if( ctrl_phase_in(self, extension) ) {
    fz_unit = tau * ( zr - vec_elem(p,0) ) / ( zr - ctrl_raibert_end_of_thrust_z(self) ) - gamma * vec_elem(p,1);
  } else {
    fz_unit = 0;
  }
  return fz_unit;
}

double _ctrl_raibert_calc_fz_unit_simplified_linear(ctrl_t *self, double t, vec_t p) {
  double fz_unit;
  double tau, gamma, yeta1, zr;

  tau = ctrl_raibert_tau(self);
  gamma = ctrl_raibert_gamma(self);
  yeta1 = ctrl_raibert_yeta1(self);
  zr = ctrl_raibert_zr(self);
  if( ctrl_phase_in(self, compression) ) {
    fz_unit = yeta1 * ( zr - vec_elem(p,0) ) - gamma * vec_elem(p,1);
  } else if( ctrl_raibert_is_in_thrust(self) ) {
    fz_unit = tau - gamma * vec_elem(p,1);
  } else if( ctrl_phase_in(self, extension) ) {
    fz_unit = yeta1 * ( zr - vec_elem(p,0) ) - gamma * vec_elem(p,1);
  } else {
    fz_unit = 0;
  }
  return fz_unit;
}

double ctrl_raibert_calc_fz(ctrl_t *self, double t, vec_t p) {
  double fz_unit;

  /* ctrl_raibert_update_events() must be called before calculating fz
   * to determine the current phase. */
  switch( ctrl_raibert_type(self) ) {
    case full_nonlinear:
      fz_unit = _ctrl_raibert_calc_fz_unit_full_nonlinear( self, t, p );
      break;
    case simplified_nonlinear:
      fz_unit = _ctrl_raibert_calc_fz_unit_simplified_nonlinear( self, t, p );
      break;
    case full_linear:
      fz_unit = _ctrl_raibert_calc_fz_unit_full_linear( self, t, p );
      break;
    case simplified_linear:
      fz_unit = _ctrl_raibert_calc_fz_unit_simplified_linear( self, t, p );
      break;
    default:
      fz_unit = 0;
      break;
  }
  return ctrl_model(self)->m * fz_unit;
}
