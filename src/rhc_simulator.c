#include "rhc_simulator.h"

simulator_t *simulator_init(simulator_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model)
{
  simulator_cmd( self )   = cmd;
  simulator_ctrl( self )  = ctrl;
  simulator_model( self ) = model;
  simulator_time( self )  = 0;
  simulator_state( self ) = vec_create( 2 );
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
}
