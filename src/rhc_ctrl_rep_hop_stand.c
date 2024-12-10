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
  prp->vh = 0;
  prp->vm = 0;
  prp->r = 0;
  prp->sqr_gamma = 0;
  prp->f_gamma = 0;
  prp->phase = 0;
  return self;
}

void ctrl_rep_hop_stand_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
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

ctrl_t *ctrl_rep_hop_stand_update(ctrl_t *self, double t, vec_t p){
  return self;
}
void ctrl_rep_hop_stand_header(FILE *fp, void *util){}
void ctrl_rep_hop_stand_writer(FILE *fp, ctrl_t *self, void *util){}
double ctrl_rep_hop_stand_calc_fz(ctrl_t *self, vec_t p)
{
  return 0;
}
