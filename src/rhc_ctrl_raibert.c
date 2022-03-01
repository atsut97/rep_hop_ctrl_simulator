#include "rhc_ctrl_raibert.h"
#include "rhc_ctrl.h"
#include "rhc_misc.h"

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
  return self;
}

void ctrl_raibert_header(FILE *fp, void *util)
{

}

void ctrl_raibert_writer(FILE *fp, ctrl_t *self, void *util)
{

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

  ctrl_events_update( ctrl_events(self), t, p, ctrl_cmd(self) );
  ctrl_raibert_get_prp(self)->is_in_thrust = false;
  if( ctrl_phase_in( self, extension ) ){
    tb = ctrl_events_at( self, bottom ).t;
    if( ( tb <= t ) && ( t < tb + ctrl_raibert_delta(self) ) ){
      ctrl_raibert_get_prp(self)->is_in_thrust = true;
    }
  }
}

bool ctrl_raibert_is_in_thrust(ctrl_t *self)
{
  return ctrl_raibert_get_prp(self)->is_in_thrust;
}

double ctrl_raibert_calc_fz(ctrl_t *self, double t, vec_t p) {
  return 0;
}
