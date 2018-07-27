#include "rhc_ctrl_slip_var_stiff.h"

ctrl_t *ctrl_slip_var_stiff_create(ctrl_t *self, cmd_t *cmd, model_t *model)
{
  ctrl_slip_var_stiff_prp *prp;

  ctrl_init( self, cmd, model );
  self->_update = ctrl_slip_var_stiff_update;
  self->_destroy = ctrl_slip_var_stiff_destroy;
  self->_header = ctrl_slip_var_stiff_header;
  self->_writer = ctrl_slip_var_stiff_writer;

  if( ( self->prp = nalloc( ctrl_slip_var_stiff_prp, 1 ) ) == NULL ){
    ALLOC_ERR();
    return NULL;
  }
  prp = self->prp;
  prp->k = 0;
  return self;
}

void ctrl_slip_var_stiff_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
}

ctrl_t *ctrl_slip_var_stiff_update(ctrl_t *self, double t, vec_t p)
{

}

void ctrl_slip_var_stiff_header(FILE *fp, void *util)
{

}

void ctrl_slip_var_stiff_writer(FILE *fp, ctrl_t *self, void *util)
{

}

double ctrl_slip_var_stiff_calc_stifffness(double m, double z0, double zd, double zb)
{

}
