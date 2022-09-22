#include "rhc_ctrl_mtoka.h"
#include "rhc_mtoka_osci.h"

ctrl_t *ctrl_mtoka_create(ctrl_t *self, cmd_t *cmd, model_t *model)
{
  ctrl_init( self, cmd, model );
  self->_update = ctrl_mtoka_update;
  self->_destroy = ctrl_mtoka_destroy;
  self->_header = ctrl_mtoka_header;
  self->_writer = ctrl_mtoka_writer;

  if( ( self->prp = nalloc( ctrl_mtoka_prp, 1 ) ) == NULL ){
    ALLOC_ERR();
    return NULL;
  }
  mtoka_osci_init( ctrl_mtoka_osci(self), 2 );
  return self;
}

void ctrl_mtoka_destroy(ctrl_t *self)
{
  if( self->prp )
    mtoka_osci_destroy( ctrl_mtoka_osci(self) );
  sfree( self->prp );
  ctrl_destroy_default( self );
}

ctrl_t *ctrl_mtoka_update(ctrl_t *self, double t, vec_t p)
{
  return self;
}

void ctrl_mtoka_header(FILE *fp, void *util)
{}

void ctrl_mtoka_writer(FILE *fp, ctrl_t *self, void *util)
{}
