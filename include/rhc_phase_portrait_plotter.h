#ifndef __RHC_PHASE_PORTRAIT_PLOTTER_H__
#define __RHC_PHASE_PORTRAIT_PLOTTER_H__

#include "rhc_simulator.h"
#include "rhc_vec_list.h"
#include "rhc_vec_ring.h"

typedef struct {
  int dim;
  vec_t pmin;
  vec_t pmax;
  int *n_sc;
  vec_list_t p0_list;
  vec_ring_t point_buf; /* circular buffer of points */

  cmd_t *cmd;
  ctrl_t *ctrl;
  model_t *model;
  logger_t *logger;
  simulator_t simulator;

  vec_t _a, _b;  /* workspace */
} ppp_t;

#define ppp_dim(self)       (self)->dim
#define ppp_cmd(self)       (self)->cmd
#define ppp_ctrl(self)      (self)->ctrl
#define ppp_model(self)     (self)->model
#define ppp_min(self)       (self)->pmin
#define ppp_max(self)       (self)->pmax
#define ppp_n_sc(self,ax)   (self)->n_sc[ax]
#define ppp_p0_list(self)   ( &(self)->p0_list )
#define ppp_point_buf(self) ( &(self)->point_buf )
#define ppp_logger(self)    (self)->logger
#define ppp_simulator(self) ( &(self)->simulator )

#define ppp_p0_list_head(self) vec_list_head( ppp_p0_list(self) )
#define ppp_p0_list_tail(self) vec_list_tail( ppp_p0_list(self) )

ppp_t *ppp_init(ppp_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model, logger_t *logger);
void ppp_destroy(ppp_t *self);

void ppp_set_lim(ppp_t *self, vec_t pmin, vec_t pmax);
void ppp_set_lim_xy(ppp_t *self, double xmin, double xmax, double ymin, double ymax);
void ppp_set_n_sc(ppp_t *self, int *n_sc);
void ppp_set_n_sc_xy(ppp_t *self, int n_sc_x, int n_sc_y);
vec_t ppp_push_p0(ppp_t *self, vec_t p0);

void ppp_generate_edge_points(ppp_t *self);

bool ppp_simulator_update(simulator_t *self, double fe, double dt, void *util);
void ppp_run(ppp_t *self, double max_time, double dt);

#endif /* __RHC_PHASE_PORTRAIT_PLOTTER_H__ */
