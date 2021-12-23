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
