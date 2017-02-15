#include "rhc_simulator.h"

simulator_t *simulator_init(simulator_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model)
{
  simulator_cmd( self )   = cmd;
  simulator_ctrl( self )  = ctrl;
  simulator_model( self ) = model;
  simulator_time( self )  = 0;
  simulator_state( self ) = vec_create( 2 );
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
  ode_destroy( &self->ode );
}

vec_t simulator_dp(double t, vec_t x, void *util, vec_t v)
{
  return v;
}

void simulator_update(simulator_t *self, vec_t p, double fe, double dt)
{
  simulator_inc_time( self, dt );
}

void simulator_run(simulator_t *self, vec_t p0, double time)
{
  simulator_time( self ) = 10;
}
