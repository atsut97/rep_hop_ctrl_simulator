#include "rhc_ctrl_rep_hop_stand.h"

double _ctrl_rep_hop_stand_calc_phi_ratio_linear(double phi);
double _ctrl_rep_hop_stand_calc_phi_ratio_piecewise(double phi);

cmd_t *ctrl_rep_hop_stand_cmd_init(ctrl_t *self, cmd_t *cmd)
{
  cmd_default_init( cmd );
  ctrl_rep_hop_stand_set_rho( self, 0.0 );
  ctrl_rep_hop_stand_set_k( self, 4.0 );
  ctrl_rep_hop_stand_enable_soft_landing( self );
  return cmd;
}

ctrl_t *ctrl_rep_hop_stand_create_with_type(ctrl_t *self, cmd_t *cmd, model_t *model, enum ctrl_rep_hop_stand_types type)
{
  ctrl_rep_hop_stand_prp *prp;

  ctrl_init( self, cmd, model );
  ctrl_rep_hop_stand_cmd_init( self, cmd );
  self->_reset = ctrl_rep_hop_stand_reset;
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

  if( prp->type == no_update_params ){
    prp->_update_params = ctrl_rep_hop_stand_update_params_no_update;
    prp->_calc_phi_ratio = _ctrl_rep_hop_stand_calc_phi_ratio_piecewise;
  } else{
    prp->_update_params = ctrl_rep_hop_stand_update_params_default;
    if( prp->type == soft_landing_linear ){
      prp->_calc_phi_ratio = _ctrl_rep_hop_stand_calc_phi_ratio_linear;
    } else{
      prp->_calc_phi_ratio = _ctrl_rep_hop_stand_calc_phi_ratio_piecewise;
    }
  }
  cmd_copy( cmd, ctrl_rep_hop_stand_params(self) );
  prp->q1 = 0;
  prp->q2 = 0;
  prp->vm = 0;
  prp->cushioning = false;
  return self;
}

ctrl_t *ctrl_rep_hop_stand_reset(ctrl_t *self, void *util)
{
  ctrl_reset_default( self, util );
  ctrl_rep_hop_stand_cushioning(self) = false;
  return self;
}

void ctrl_rep_hop_stand_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
}

void ctrl_rep_hop_stand_header(FILE *fp, void *util)
{
  fprintf( fp, ",type,rho,k,soft_landing,q1,q2,vm,p_za,p_zh,p_zm,p_zb,p_rho" );
}

void ctrl_rep_hop_stand_writer(FILE *fp, ctrl_t *self, void *util)
{
  fprintf( fp, ",%d,%f,%f,%d,%f,%f,%f,%f,%f,%f,%f,%f",
           ctrl_rep_hop_stand_type(self),
           ctrl_rep_hop_stand_rho(self),
           ctrl_rep_hop_stand_k(self),
           ctrl_rep_hop_stand_soft_landing(self),
           ctrl_rep_hop_stand_q1(self),
           ctrl_rep_hop_stand_q2(self),
           ctrl_rep_hop_stand_vm(self),
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
    RUNTIME_WARN( "rhc_ctrl_rep_hop_stand: zm and zb are too close!" );
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
  return za - sqrt( ( za - zb ) * ( za + zb - 2.0 * zm ) );
}

double ctrl_rep_hop_stand_calc_zm(double za, double zh, double zb)
{
  if( za > zh ){
    return 0.5 * ( ( za + zb - ( za - zh ) * ( za - zh ) / ( za - zb ) ) );
  } else{
    return 0.5 * ( za + zb );
  }
}

double ctrl_rep_hop_stand_calc_zb(double za, double zh, double zm)
{
  return zm - sqrt( ( zh - zm ) * ( 2.0 * za - zh - zm ) );
}

double _ctrl_rep_hop_stand_calc_phi_ratio_linear(double phi)
{
  double s;
  if( phi < 0.0 ) s = 1.0;
  else{
    s = (2.0 * phi) / PI - 1.0;
    if( s < 0.0 ) s = 0.0;
    else if( s > 1.0 ) s = 1.0;
  }
  return s;
}

double _ctrl_rep_hop_stand_calc_phi_ratio_piecewise(double phi)
{
  double s;
  if( phi < 0 ) s = 1.0;
  else s = 0.0;
  return s;
}

double ctrl_rep_hop_stand_calc_phi_based_param(ctrl_t *self, double phi, double v1, double v2)
{
  double s, v;

  s = ctrl_rep_hop_stand_get_prp(self)->_calc_phi_ratio( phi );
  v = v1 * ( 1.0 - s ) + v2 * s;
  return v;
}

ctrl_t *ctrl_rep_hop_stand_update_params_default(ctrl_t *self, vec_t p)
{
  cmd_t *params;
  double za, zb, zm, rho;
  double z_apex;

  params = ctrl_rep_hop_stand_params(self);
  cmd_copy( ctrl_cmd(self), params );

  if( ctrl_phase_in( self, flight ) ){
    /* Once the robot goes into flight, cushioning is enabled */
    ctrl_rep_hop_stand_cushioning(self) = true;
  } else if( ctrl_phase_in( self, extension ) ){
    /* Once the robot extends its body, cushioning is no more needed */
    ctrl_rep_hop_stand_cushioning(self) = false;
  }

  /* When soft landing is enabled, fix desired apex and rho if needed */
  if( ctrl_rep_hop_stand_soft_landing(self) ){
    if( istiny( ctrl_apex_z(self) ) || !ctrl_rep_hop_stand_cushioning(self) ){
      /* Use desired apex height is used as the robot goes into flight yet */
      z_apex = ctrl_za(self);
    } else{
      z_apex = ctrl_apex_z(self);
    }
    /* Fix the apex height to absorb the impact when touching down */
    za = ctrl_rep_hop_stand_calc_phi_based_param( self, ctrl_phi(self), z_apex, ctrl_za(self) );
    params->za = za;
    if( istiny( ctrl_rep_hop_stand_rho(self) ) ){
      /* Fix rho when the robot is in flight phase but it wants to stand */
      rho = ctrl_rep_hop_stand_calc_phi_based_param( self, ctrl_phi(self), 1.0, ctrl_rep_hop_stand_rho(self) );
      params->rep_hop_stand.rho = rho;
    }
  }

  /* When hopping or squatting, the crouching height is fixed so that
     it does not fall behind the kinematic lower limit */
  if( params->rep_hop_stand.rho > 0 ){
    zb = ctrl_rep_hop_stand_calc_zb( params->za, ctrl_zh(self), ctrl_zm(self) );
    if( ctrl_zb(self) < zb && ctrl_za(self) > ctrl_zh(self) ){
      /* The couching height will be over the kinematic limit */
      params->zb = zb;
    } else{
      /* The couching height falls behind the kinematic limit or squatting */
      zm = ctrl_rep_hop_stand_calc_zm( params->za, ctrl_zh(self), ctrl_zb(self) );
      params->zm = zm;
    }
  }
  return self;
}

ctrl_t *ctrl_rep_hop_stand_update_params_no_update(ctrl_t *self, vec_t p)
{
  cmd_t *params;
  double zm;

  params = ctrl_rep_hop_stand_params(self);
  cmd_copy( ctrl_cmd(self), params );
  zm = ctrl_rep_hop_stand_calc_zm( ctrl_za(self), ctrl_zh(self), ctrl_zb(self) );
  params->zm = zm;
  return self;
}

ctrl_t *ctrl_rep_hop_stand_update(ctrl_t *self, double t, vec_t p)
{
  ctrl_update_default( self, t, p );
  ctrl_rep_hop_stand_get_prp(self)->_update_params( self, p );
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
  ctrl_rep_hop_stand_prp *prp;

  prp = ctrl_rep_hop_stand_get_prp(self);
  zh = ctrl_rep_hop_stand_params_zh( self );
  zm = ctrl_rep_hop_stand_params_zm( self );
  zb = ctrl_rep_hop_stand_params_zb( self );
  rho = ctrl_rep_hop_stand_params_rho( self );
  k = ctrl_rep_hop_stand_k( self );
  g = model_gravity( ctrl_model(self) );
  k1 = g / ( zh - zm );
  q1 = ctrl_rep_hop_stand_calc_q1( zh, zm, g );
  prp->q1 = prp->q2 = q1;
  prp->vm = ctrl_rep_hop_stand_calc_vm( zh, zm, zb, g );
  gamma = ctrl_rep_hop_stand_calc_gamma( p, zh, zm, zb, g );
  f_gamma = 1.0 - rho * exp( k * ( 1.0 - gamma ) );
  unit_fz = -2.0 * q1 * f_gamma * vec_elem(p,1) - k1 * ( vec_elem(p,0) - zm ) + g;
  return model_mass( ctrl_model( self ) ) * unit_fz;
}
