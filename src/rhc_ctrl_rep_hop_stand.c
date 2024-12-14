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
  prp->vm = 0;
  prp->phi = 0;
  return self;
}

void ctrl_rep_hop_stand_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
}

void ctrl_rep_hop_stand_header(FILE *fp, void *util)
{
  fprintf( fp, ",type,rho,k,q1,q2,vm,zm_phi,p_za,p_zh,p_zm,p_zb,p_rho\n" );
}

void ctrl_rep_hop_stand_writer(FILE *fp, ctrl_t *self, void *util)
{
  fprintf( fp, ",%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
           ctrl_rep_hop_stand_type(self),
           ctrl_rep_hop_stand_rho(self),
           ctrl_rep_hop_stand_k(self),
           ctrl_rep_hop_stand_q1(self),
           ctrl_rep_hop_stand_q2(self),
           ctrl_rep_hop_stand_vm(self),
           ctrl_rep_hop_stand_phi(self),
           ctrl_rep_hop_stand_params_za(self),
           ctrl_rep_hop_stand_params_zh(self),
           ctrl_rep_hop_stand_params_zm(self),
           ctrl_rep_hop_stand_params_zb(self),
           ctrl_rep_hop_stand_params_rho(self) );
 }

double ctrl_rep_hop_stand_calc_q1(double zh, double zm, double g)
{
  return sqrt( g / ( zh - zm ) );
}

double ctrl_rep_hop_stand_calc_r(double zm, double zb)
{
  double r;

  r = zm - zb;
  if( istiny(r) )
    RUNTIME_WARN( "rhc_ctrl_rep_hop_stand: zm and zb are too close!\n" );
  return r;
}

double ctrl_rep_hop_stand_calc_sqr_vm(double zh, double zm, double zb, double g)
{
  double r;

  r = ctrl_rep_hop_stand_calc_r( zm, zb );
  return g * r * r / ( zh - zm );
}

double ctrl_rep_hop_stand_calc_sqr_gamma(vec_t p, double zh, double zm, double zb, double g)
{
  double z, v, r, sqr_r, sqr_vm;

  z = vec_elem( p, 0 );
  v = vec_elem( p, 1 );
  r = ctrl_rep_hop_stand_calc_r( zm, zb );
  sqr_r = r * r;
  sqr_vm = ctrl_rep_hop_stand_calc_sqr_vm( zh, zm, zb, g );
  return ( z - zm ) * ( z - zm ) / sqr_r + v * v / sqr_vm;
}

double ctrl_rep_hop_stand_calc_za(double zh, double zm, double zb)
{
  return 0.5 * ( ( zm - zb ) * ( zm - zb ) / ( zh - zm ) + zh + zm );
}

double ctrl_rep_hop_stand_calc_zh(double za, double zm, double zb)
{
  if( ( za + zb - 2.0 * zm ) < 0.0 ) {
    RUNTIME_ERR( "rhc_ctrl_rep_hop_stand: za > 2*zm - zb must be satisfied\n" );
    exit( 1 );
  }
  return za - sqrt( ( za - zb ) * ( za + zb - 2.0 * zm ) );
}

double ctrl_rep_hop_stand_calc_zm(double za, double zh, double zb)
{
  return 0.5 * ( ( za + zb - ( za - zh ) * ( za - zh ) / ( za - zb ) ) );
}

double ctrl_rep_hop_stand_calc_zb(double za, double zh, double zm)
{
  return zm - sqrt( ( zh - zm ) * ( 2.0 * za - zh - zm ) );
}

ctrl_t *ctrl_rep_hop_stand_update_params(ctrl_t *self)
{
  cmd_t *params;
  double zb, zm;

  params = ctrl_rep_hop_stand_params(self);
  cmd_copy( ctrl_cmd(self), params );
  if( ctrl_rep_hop_stand_rho(self) > 0 ){
    zb = ctrl_rep_hop_stand_calc_zb( ctrl_za(self), ctrl_zh(self), ctrl_zm(self) );
    if( ctrl_zb(self) < zb ){
      params->zb = zb;
    } else{
      zm = ctrl_rep_hop_stand_calc_zm( ctrl_za(self), ctrl_zh(self), ctrl_zb(self) );
      params->zm = zm;
    }
  }
  return self;
}

ctrl_t *ctrl_rep_hop_stand_update(ctrl_t *self, double t, vec_t p)
{
  ctrl_update_default( self, t, p );

  if( ctrl_phase_in( self, flight ) ){
    self->fz = 0;
  } else{
    self->fz = ctrl_rep_hop_stand_calc_fz( self, p );
  }
  return self;
}

double ctrl_rep_hop_stand_calc_fz(ctrl_t *self, vec_t p)
{
  double zh, zm, zb, rho, k;
  double g, k1, q1, gamma, f_gamma, unit_fz;

  zh = ctrl_rep_hop_stand_params_zh( self );
  zm = ctrl_rep_hop_stand_params_zm( self );
  zb = ctrl_rep_hop_stand_params_zb( self );
  rho = ctrl_rep_hop_stand_params_rho( self );
  k = ctrl_rep_hop_stand_k( self );
  g = model_gravity( ctrl_model(self) );
  k1 = g / ( zh - zm );
  q1 = ctrl_rep_hop_stand_calc_q1( zh, zm, g );
  gamma = ctrl_rep_hop_stand_calc_gamma( p, zh, zm, zb, g );
  f_gamma = 1.0 - rho * exp( k * ( 1.0 - gamma ) );
  unit_fz = -2.0 * q1 * f_gamma * vec_elem(p,1) - k1 * ( vec_elem(p,0) - zm ) + g;
  return model_mass( ctrl_model( self ) ) * unit_fz;
}
