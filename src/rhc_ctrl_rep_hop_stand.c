#include "rhc_ctrl_rep_hop_stand.h"

ctrl_t *ctrl_rep_hop_stand_create(ctrl_t *self, cmd_t *cmd, model_t *model, enum ctrl_rep_hop_stand_types type)
{
  ctrl_rep_hop_stand_prp *prp;

  ctrl_init( self, cmd, model );
  self->_update = ctrl_rep_hop_stand_update;
  self->_destroy = ctrl_rep_hop_stand_destroy;
  self->_header = ctrl_rep_hop_stand_header;
  self->_writer = ctrl_rep_hop_stand_writer;

  if( ( self->prp = nalloc( ctrl_rep_hop_stand_prp, 1 ) ) == NULL ){
    ALLOC_ERR();
    return NULL;
  }
  prp = self->prp;
  prp->type = type;
  cmd_copy( cmd, ctrl_rep_hop_stand_params(self) );
  prp->q1 = 0;
  prp->q2 = 0;
  prp->vh = 0;
  prp->vm = 0;
  prp->r = 0;
  prp->sqr_gamma = 0;
  prp->f_gamma = 0;
  prp->phase = 0;
  return self;
}

void ctrl_rep_hop_stand_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
}

double ctrl_rep_hop_stand_calc_q1(double zh, double zm, double g)
{
  return sqrt( g / ( zh - zm ) );
}

double ctrl_rep_hop_stand_calc_r(double zm, double zb)
{
  return 0;
}

double ctrl_rep_hop_stand_calc_sqr_gamma(vec_t p, double zh, double zm, double zb)
{
  return 0;
}

double ctrl_rep_hop_stand_calc_za(double zh, double zm, double zb)
{
  return 0;
}

double ctrl_rep_hop_stand_calc_zh(double za, double zm, double zb)
{
  return 0;
}

double ctrl_rep_hop_stand_calc_zm(double za, double zh, double zb)
{
  return 0;
}

double ctrl_rep_hop_stand_calc_zb(double za, double zh, double zm)
{
  return 0;
}

double ctrl_rep_hop_stand_calc_vm(double zh, double zm, double zb)
{
  return 0;
}

ctrl_t *ctrl_rep_hop_stand_update(ctrl_t *self, double t, vec_t p){}
void ctrl_rep_hop_stand_header(FILE *fp, void *util){}
void ctrl_rep_hop_stand_writer(FILE *fp, ctrl_t *self, void *util){}
double ctrl_rep_hop_stand_calc_fz(ctrl_t *self, vec_t p)
{
  return 0;
}
