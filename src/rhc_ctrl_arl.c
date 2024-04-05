#include "rhc_ctrl_arl.h"
#include "rhc_ctrl.h"

double ctrl_arl_calc_c(double k, double m);
double ctrl_arl_calc_sqr_R(double m, double k, double z0, double zd);

ctrl_t *ctrl_arl_create(ctrl_t *self, cmd_t *cmd, model_t *model, enum ctrl_arl_types type)
{
  ctrl_init( self, cmd, model );
  self->_update = ctrl_arl_update;
  self->_destroy = ctrl_arl_destroy;
  self->_header = ctrl_arl_header;
  self->_writer = ctrl_arl_writer;

  if ( ( self->prp = nalloc( ctrl_arl_prp, 1 ) ) == NULL ){
    ALLOC_ERR();
    return NULL;
  }
  ((ctrl_arl_prp *)(self->prp))->type = type;
  ((ctrl_arl_prp *)(self->prp))->c = 0;
  return self;
}

void ctrl_arl_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
}

ctrl_t *ctrl_arl_set_params(ctrl_t *self, double k, double beta)
{
  ctrl_arl_set_k( self, k );
  ctrl_arl_set_beta( self, beta );
  return self;
}

ctrl_t *ctrl_arl_update(ctrl_t *self, double t, vec_t p)
{
  double beta, c, R2, delta, l, v, R2_actual;

  ctrl_update_default( self, t, p );
  beta = ctrl_arl_beta(self);
  c = ctrl_arl_calc_c( ctrl_arl_k(self), ctrl_model(self)->m );
  R2 = ctrl_arl_calc_sqr_R( ctrl_model(self)->m, ctrl_arl_k(self), ctrl_z0(self), ctrl_zd(self));
  l = vec_elem(p,0) - ctrl_z0(self);
  v = vec_elem(p,1);
  R2_actual = (l + G / c) * (l + G / c) + v * v / c;
  delta = beta * v * l * (R2_actual - R2);
  self->fz = -ctrl_arl_k(self) * l + ctrl_model(self)->m * delta;
  if( ctrl_phase_in( self, flight ) ) self->fz = 0;
  return self;
}

void ctrl_arl_header(FILE *fp, void *util)
{
  fprintf( fp, ",type,k,beta,c\n" );
}

void ctrl_arl_writer(FILE *fp, ctrl_t *self, void *util)
{
  fprintf( fp, ",%d,%f,%f,%f\n",
           ctrl_arl_type(self),
           ctrl_arl_k(self),
           ctrl_arl_beta(self),
           ctrl_arl_c(self) );
}

double ctrl_arl_calc_sqr_R(double m, double k, double z0, double zd)
{
  double sqr_v0 = ctrl_calc_sqr_v0(z0, zd);
  double c = ctrl_arl_calc_c(k, m);
  double G_c = G / c;
  return G_c * G_c + sqr_v0 / c;
}


double ctrl_arl_calc_c(double k, double m)
{
  return k / m;
}

double ctrl_arl_calc_fz(ctrl_t *self, double t, vec_t p)
{
  return 0;
}
