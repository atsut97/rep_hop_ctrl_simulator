#include "rhc_ctrl_regulator.h"

cmd_t *ctrl_regulator_cmd_init(ctrl_t *self, cmd_t *cmd)
{
  cmd_default_init( cmd );
  cmd->regulator.q1 = 1;
  cmd->regulator.q2 = 1;
  return cmd;
}

ctrl_t *ctrl_regulator_create(ctrl_t *self, cmd_t *cmd, model_t *model)
{
  ctrl_regulator_prp *prp;

  ctrl_init( self, cmd, model );
  ctrl_regulator_cmd_init( self, cmd );
  self->_update = ctrl_regulator_update;
  self->_destroy = ctrl_regulator_destroy;
  self->_header = ctrl_regulator_header;
  self->_writer = ctrl_regulator_writer;

  if( ( self->prp = nalloc( ctrl_regulator_prp, 1 ) ) == NULL ){
    ALLOC_ERR();
    return NULL;
  }
  prp = self->prp;
  prp->xi = 0;
  return self;
}

void ctrl_regulator_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
}

ctrl_t *ctrl_regulator_update(ctrl_t *self, double t, vec_t p)
{
  ctrl_regulator_prp *prp;

  ctrl_update_default( self, t, p );
  prp = self->prp;
  prp->xi = ctrl_regulator_calc_xi( ctrl_z0(self) );
  self->fz = ctrl_regulator_calc_fz( self, p );
  if( ctrl_is_in_flight( self, p ) ) self->fz = 0;
  return self;
}

void ctrl_regulator_header(FILE *fp, void *util)
{
  fprintf( fp, ",q1,q2,xi\n" );
}

void ctrl_regulator_writer(FILE *fp, ctrl_t *self, void *util)
{
  fprintf( fp, ",%f,%f,%f\n",
           ctrl_regulator_q1(self), ctrl_regulator_q2(self),
           ctrl_regulator_xi(self) );
}

double ctrl_regulator_calc_sqr_xi(double z0)
{
  return G / z0;
}

double ctrl_regulator_calc_fz(ctrl_t *self, vec_t p)
{
  double z0, q1, q2, m;
  double xi2;
  double k1, k2;

  z0 = ctrl_z0(self);
  q1 = ctrl_regulator_q1(self);
  q2 = ctrl_regulator_q2(self);
  m = ctrl_model(self)->m;
  xi2 = ctrl_regulator_calc_sqr_xi( z0 );
  k1 = -m * xi2 * q1 * q2;
  k2 = -m * sqrt(xi2) * ( q1 + q2 );
  return k1 * ( vec_elem( p, 0 ) - z0 ) + k2 * vec_elem( p, 1 ) + m * G;
}
