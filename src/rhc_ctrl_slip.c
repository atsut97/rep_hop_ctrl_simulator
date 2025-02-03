#include "rhc_ctrl_slip.h"

ctrl_t *ctrl_slip_create(ctrl_t *self, cmd_t *cmd, model_t *model)
{
  ctrl_slip_prp *prp;

  ctrl_init( self, cmd, model );
  self->_update = ctrl_slip_update;
  self->_destroy = ctrl_slip_destroy;
  self->_header = ctrl_slip_header;
  self->_writer = ctrl_slip_writer;

  if( ( self->prp = nalloc( ctrl_slip_prp, 1 ) ) == NULL ){
    ALLOC_ERR();
    return NULL;
  }
  prp = self->prp;
  prp->k = 0;
  return self;
}

void ctrl_slip_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
}

ctrl_t *ctrl_slip_update(ctrl_t *self, double t, vec_t p)
{
  ctrl_slip_prp *prp;

  ctrl_update_default( self, t, p );
  prp = self->prp;
  prp->k = ctrl_slip_stiffness( self );
  self->fz = -prp->k * ( vec_elem(p,0) - ctrl_zh(self) );
  if( ctrl_phase_in( self, flight ) ) self->fz = 0;
  if( self->fz < 0 ) self->fz = 0;
  return self;
}

void ctrl_slip_header(FILE *fp, void *util)
{
  fprintf( fp, ",k" );
}

void ctrl_slip_writer(FILE *fp, ctrl_t *self, void *util)
{
  ctrl_slip_prp *prp;

  prp = self->prp;
  fprintf( fp, ",%f", prp->k );
}

double ctrl_slip_calc_stiffness(double m, double zh, double za, double zb, double g)
{
  return 2. * m * g * ( za - zb ) / sqr( zh - zb );
}
