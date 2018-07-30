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
