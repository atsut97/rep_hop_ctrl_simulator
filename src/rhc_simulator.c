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

void simulator_run(simulator_t *self, vec_t p0, double time, double dt)
{
  simulator_set_state( self, p0 );
  while( simulator_time(self) < time ){
    simulator_update( self, 0.0, dt );
  }
}
