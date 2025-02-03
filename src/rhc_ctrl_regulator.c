#include "rhc_ctrl_regulator.h"

cmd_t *ctrl_regulator_cmd_init(ctrl_t *self, cmd_t *cmd)
{
  cmd_default_init( cmd );
  cmd->regulator.q1 = 2;
  cmd->regulator.q2 = 3;
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
  prp->xi = ctrl_regulator_calc_xi( ctrl_zh(self), model_gravity( ctrl_model(self) ) );
  self->fz = ctrl_regulator_calc_fz( self, p );
  if( ctrl_phase_in( self, flight ) ) self->fz = 0;
  if( self->fz < 0 ) self->fz = 0;
  return self;
}

void ctrl_regulator_header(FILE *fp, void *util)
{
  fprintf( fp, ",q1,q2,xi" );
}

void ctrl_regulator_writer(FILE *fp, ctrl_t *self, void *util)
{
  fprintf( fp, ",%f,%f,%f",
           ctrl_regulator_q1(self), ctrl_regulator_q2(self),
           ctrl_regulator_xi(self) );
}

double ctrl_regulator_calc_sqr_xi(double zh, double g)
{
  return g / zh;
}

double ctrl_regulator_calc_fz(ctrl_t *self, vec_t p)
{
  double zh, q1, q2, m;
  double xi2;
  double k1, k2;

  zh = ctrl_zh(self);
  q1 = ctrl_regulator_q1(self);
  q2 = ctrl_regulator_q2(self);
  m = ctrl_model(self)->m;
  xi2 = ctrl_regulator_calc_sqr_xi( zh, model_gravity( ctrl_model(self) ) );
  k1 = -m * xi2 * q1 * q2;
  k2 = -m * sqrt(xi2) * ( q1 + q2 );
  return k1 * ( vec_elem( p, 0 ) - zh ) + k2 * vec_elem( p, 1 ) + m * model_gravity( ctrl_model(self) );
}
