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
  ctrl_slip_var_stiff_prp *prp;

  ctrl_update_default( self, t, p );
  prp = self->prp;
  prp->k = ctrl_slip_var_stiff_stiffness( self, p );
  self->fz = -prp->k * ( vec_elem(p,0) - ctrl_z0(self) );
  if( ctrl_is_in_flight( self, p ) ) self->fz = 0;
  return self;
}

void ctrl_slip_var_stiff_header(FILE *fp, void *util)
{
  fprintf( fp, ",k\n" );
}

void ctrl_slip_var_stiff_writer(FILE *fp, ctrl_t *self, void *util)
{
  ctrl_slip_var_stiff_prp *prp;

  prp = self->prp;
  fprintf( fp, ",%f\n", prp->k );
}

double ctrl_slip_var_stiff_calc_stiffness_decomp(vec_t p, double m, double z0, double zd, double zb)
{
  double numer = 2. * G * ( zd - vec_elem(p,0) ) - sqr( vec_elem(p,1) );
  double denom = sqr( z0 - vec_elem(p,0) );
  if( istiny( denom ) )
    return 0;
  return m * numer / denom;
}

double ctrl_slip_var_stiff_calc_stiffness_comp(vec_t p, double m, double z0, double zd, double zb)
{
  double numer = 2. * G * ( vec_elem(p,0) - zb ) + sqr( vec_elem(p,1) );
  double denom = ( 2.*z0 - ( vec_elem(p,0) + zb ) ) * ( vec_elem(p,0) - zb );
  return m * numer / denom;
}

double ctrl_slip_var_stiff_calc_stiffness(vec_t p, double m, double z0, double zd, double zb)
{
  if( vec_elem(p,1) > -1e-10 )
    return ctrl_slip_var_stiff_calc_stiffness_decomp( p, m, z0, zd, zb );
  else
    return ctrl_slip_var_stiff_calc_stiffness_comp( p, m, z0, zd, zb );
}
