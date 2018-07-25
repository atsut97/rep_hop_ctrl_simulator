#include "rhc_simulator.h"

simulator_t *simulator_init(simulator_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model)
{
  simulator_cmd( self )   = cmd;
  simulator_ctrl( self )  = ctrl;
  simulator_model( self ) = model;
  simulator_time( self )  = 0;
  simulator_state( self ) = vec_create( 2 );
  simulator_set_fe( self, 0 );
  ode_assign( &self->ode, rk4 );
  ode_init( &self->ode, 2, simulator_dp );
  self->n_trial = 0;
  return self;
}

void simulator_destroy(simulator_t *self)
{
  simulator_cmd( self )   = NULL;
  simulator_ctrl( self )  = NULL;
  simulator_model( self ) = NULL;
  simulator_time( self )  = 0;
  vec_destroy( simulator_state( self ) );
  simulator_state( self ) = NULL;
  simulator_set_fe( self, 0 );
  if( self->ode._ws )
    ode_destroy( &self->ode );
  self->n_trial = 0;
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

void simulator_update(simulator_t *self, double fe, double dt)
{
  ode_update( &self->ode, simulator_time(self), simulator_state(self), dt, self );
  simulator_inc_time( self, dt );
}

void simulator_run(simulator_t *self, vec_t p0, double time, double dt, logger_t *logger, void *util)
{
  simulator_set_state( self, p0 );
  while( simulator_time(self) < time ){
    if( logger )
      simulator_dump( self, logger, util );
    simulator_update( self, 0.0, dt );
  }
  simulator_inc_trial( self );
}

void simulator_header_default(FILE *fp, void *util)
{
  fprintf( fp, "t,z,vz,az,fz,fe,z0,zd,zb,n,phi,m\n");
}

void simulator_writer_default(FILE *fp, simulator_t *s, void *util)
{
  vec_t state = simulator_state(s);
  model_t *model = simulator_model(s);
  ctrl_t *ctrl = simulator_ctrl(s);
  fprintf( fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
           simulator_time(s),
           vec_elem(state,0), vec_elem(state,1),
           model_acc(model),
           ctrl_fz(ctrl), simulator_fe(s),
           ctrl_z0(ctrl), ctrl_zd(ctrl), ctrl_zb(ctrl),
           ctrl_n(ctrl), ctrl_phi(ctrl),
           model_mass(model) );
}

void simulator_set_default_logger(simulator_t *self, logger_t *logger)
{
  logger_register( logger, simulator_header_default, simulator_writer_default );
}

void simulator_dump(simulator_t *self, logger_t *logger, void *util)
{
  logger_write( logger, self, util );
}

void simulator_dump_header(simulator_t *self, logger_t *logger, void *util)
{
  logger_write_header( logger, NULL );
}

void simulator_dump_data(simulator_t *self, logger_t *logger, void *util)
{
  logger_write_data( logger, self, util );
}
