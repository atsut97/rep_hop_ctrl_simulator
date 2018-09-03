#include "rhc_simulator.h"

simulator_t *simulator_init(simulator_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model)
{
  simulator_cmd( self )   = cmd;
  simulator_ctrl( self )  = ctrl;
  simulator_model( self ) = model;
  simulator_time( self )  = 0;
  simulator_step( self )  = 0;
  simulator_state( self ) = vec_create( 2 );
  simulator_set_fe( self, 0 );
  ode_assign( &self->ode, rk4 );
  ode_init( &self->ode, 2, simulator_dp );
  self->n_trial = 0;
  simulator_set_update_fp( self, simulator_update );
  simulator_set_dump_fp( self, simulator_dump );
  simulator_set_tag( self, "" );
  return self;
}

void simulator_destroy(simulator_t *self)
{
  simulator_cmd( self )   = NULL;
  simulator_ctrl( self )  = NULL;
  simulator_model( self ) = NULL;
  simulator_time( self )  = 0;
  simulator_step( self )  = 0;
  vec_destroy( simulator_state( self ) );
  simulator_state( self ) = NULL;
  simulator_set_fe( self, 0 );
  if( self->ode._ws )
    ode_destroy( &self->ode );
  self->n_trial = 0;
  simulator_set_tag( self, "" );
}

void simulator_set_update_fp(simulator_t *self, bool (*update_fp)(simulator_t*, double, double, void*))
{
  self->update_fp = update_fp;
}

void simulator_set_dump_fp(simulator_t *self, void (*dump_fp)(simulator_t*, logger_t*, void*))
{
  self->dump_fp = dump_fp;
}

char *simulator_set_tag(simulator_t *self, const char* tag)
{
  string_copy( tag, self->tag );
  return self->tag;
}

char *simulator_update_default_tag(simulator_t *self) {
  char default_tag[BUFSIZ];

  sprintf( default_tag, "sc%05d", simulator_n_trial(self) );
  return simulator_set_tag( self, default_tag );
}

bool simulator_has_default_tag(simulator_t *self) {
  int len;

  len = string_len( simulator_tag(self) );
  if( len == 0 ) return true;
  else if( len < 5 ) return false;

  const char *c = &simulator_tag(self)[len-5];
  if( string_is_digit( c ) && atoi( c ) == simulator_n_trial(self) - 1 )
    return true;
  return false;
}

vec_t simulator_dp(double t, vec_t x, void *util, vec_t v)
{
  simulator_t *sim = util;

  ctrl_update( sim->ctrl, t, x );
  model_update( sim->model, ctrl_fz(sim->ctrl), 0 );
  vec_set_elem( v, 0, vec_elem(x,1) );
  vec_set_elem( v, 1, model_acc( sim->model ) );
  return v;
}

void simulator_reset(simulator_t *self)
{
  simulator_time( self ) = 0;
  simulator_step( self ) = 0;
}

bool simulator_update(simulator_t *self, double fe, double dt, void *util)
{
  ode_update( &self->ode, simulator_time(self), simulator_state(self), dt, self );
  return true;
}

void simulator_update_time(simulator_t *self, double dt)
{
  simulator_inc_step( self );
  simulator_time(self) = simulator_step(self) * dt;
}

void simulator_run(simulator_t *self, vec_t p0, double time, double dt, logger_t *logger, void *util)
{
  simulator_reset( self );
  simulator_set_state( self, p0 );
  if( simulator_has_default_tag( self ) ){
    simulator_update_default_tag( self );
  }
  while( simulator_time(self) < time ){
    self->dump_fp( self, logger, util );
    self->update_fp( self, 0.0, dt, util );
    simulator_update_time( self, dt );
  }
  simulator_inc_trial( self );
}

void simulator_header_default(FILE *fp, simulator_t *s, void *util)
{
  fprintf( fp, "tag,t,z,vz,az,fz,fe,z0,zd,zb,n,phi,m" );
  ctrl_header( fp, s->ctrl, util );
}

void simulator_writer_default(FILE *fp, simulator_t *s, void *util)
{
  vec_t state = simulator_state(s);
  model_t *model = simulator_model(s);
  ctrl_t *ctrl = simulator_ctrl(s);
  fprintf( fp, "%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
           simulator_tag(s),
           simulator_time(s),
           vec_elem(state,0), vec_elem(state,1),
           model_acc(model),
           ctrl_fz(ctrl), simulator_fe(s),
           ctrl_z0(ctrl), ctrl_zd(ctrl), ctrl_zb(ctrl),
           ctrl_n(ctrl), ctrl_phi(ctrl),
           model_mass(model) );
  ctrl_writer( fp, s->ctrl, util );
}

void simulator_set_default_logger(simulator_t *self, logger_t *logger)
{
  logger_register( logger, simulator_header_default, simulator_writer_default );
}

void simulator_dump(simulator_t *self, logger_t *logger, void *util)
{
  if( logger )
    logger_write( logger, self, util );
}

void simulator_dump_header(simulator_t *self, logger_t *logger, void *util)
{
  logger_write_header( logger, self, NULL );
}

void simulator_dump_data(simulator_t *self, logger_t *logger, void *util)
{
  logger_write_data( logger, self, util );
}
