#ifndef __RHC_CTRL_H__
#define __RHC_CTRL_H__

#include "rhc_cmd.h"
#include "rhc_complex.h"
#include "rhc_model.h"
#include "rhc_vec.h"

typedef struct {
  /* Events are determined by the current position and velocity and
   * the highest position when standing (z0). */
  struct _ctrl_events_tuple_t {
    double t, z, v;
  } ctrl_events_tuple_t;
  struct _ctrl_events_tuple_t apex;
  struct _ctrl_events_tuple_t touchdown;
  struct _ctrl_events_tuple_t bottom;
  struct _ctrl_events_tuple_t liftoff;

  /* Phases are determined by the current phase value, i.e. phi. Note
   * that which phase an event belongs to can not be identified
   * uniquely due to digitalization. It means that, for instances, a
   * bottom event may be updated during extension phase. */
  enum _ctrl_events_phases_t {
    invalid=-1, falling=0, compression, extension, rising,
  } _ctrl_events_phases_t;
  enum _ctrl_events_phases_t phase;
  double phi;  /* stores the calculated phase value */
  int n;       /* counts how many it reaches apex */
} ctrl_events_t;

ctrl_events_t *ctrl_events_init(ctrl_events_t *self);
ctrl_events_t *ctrl_events_update(ctrl_events_t *self, double t, vec_t p, cmd_t *cmd);
void ctrl_events_destroy(ctrl_events_t *self);

#define ctrl_events_event(self,event) ( (self)->event )
#define ctrl_events_apex_t(self)      ( ctrl_events_event(self,apex).t )
#define ctrl_events_apex_z(self)      ( ctrl_events_event(self,apex).z )
#define ctrl_events_apex_v(self)      ( ctrl_events_event(self,apex).v )
#define ctrl_events_touchdown_t(self) ( ctrl_events_event(self,touchdown).t )
#define ctrl_events_touchdown_z(self) ( ctrl_events_event(self,touchdown).z )
#define ctrl_events_touchdown_v(self) ( ctrl_events_event(self,touchdown).v )
#define ctrl_events_bottom_t(self)    ( ctrl_events_event(self,bottom).t )
#define ctrl_events_bottom_z(self)    ( ctrl_events_event(self,bottom).z )
#define ctrl_events_bottom_v(self)    ( ctrl_events_event(self,bottom).v )
#define ctrl_events_liftoff_t(self)   ( ctrl_events_event(self,liftoff).t )
#define ctrl_events_liftoff_z(self)   ( ctrl_events_event(self,liftoff).z )
#define ctrl_events_liftoff_v(self)   ( ctrl_events_event(self,liftoff).v )

bool ctrl_events_is_in_falling(ctrl_events_t *self);
bool ctrl_events_is_in_compression(ctrl_events_t *self);
bool ctrl_events_is_in_extension(ctrl_events_t *self);
bool ctrl_events_is_in_rising(ctrl_events_t *self);
bool ctrl_events_is_in_flight(ctrl_events_t *self);

#define ctrl_events_phase(self) ( (self)->phase )
#define ctrl_events_phi(self)   ( (self)->phi )
#define ctrl_events_n(self)     ( (self)->n )
complex_t *ctrl_events_calc_phase_complex(double z0, double zd, double zb, vec_t p, complex_t *c);
double ctrl_events_calc_phi(double z0, double zd, double zb, vec_t p);


typedef struct _ctrl_t{
  cmd_t *cmd;
  model_t *model;
  double fz;
  void *prp;
  ctrl_events_t events;
  struct _ctrl_t* (*_update)(struct _ctrl_t*,double,vec_t);
  void (*_destroy)(struct _ctrl_t*);
  void (*_header)(FILE*, void*);
  void (*_writer)(FILE*, struct _ctrl_t*, void*);
} ctrl_t;

#define ctrl_cmd(self)    ((ctrl_t*)self)->cmd
#define ctrl_model(self)  ((ctrl_t*)self)->model
#define ctrl_fz(self)     ((ctrl_t*)self)->fz
#define ctrl_z0(self)     ctrl_cmd(self)->z0
#define ctrl_zd(self)     ctrl_cmd(self)->zd
#define ctrl_zb(self)     ctrl_cmd(self)->zb
#define ctrl_n(self)      ctrl_events_n( ctrl_events(self) )
#define ctrl_phi(self)    ctrl_events_phi( ctrl_events(self) )

#define ctrl_phase(self)            ctrl_events_phase( ctrl_events(self) )
#define ctrl_phase_in(self, phase)  ctrl_events_is_in_##phase( ctrl_events(self) )
#define ctrl_events(self)           ( &((ctrl_t*)self)->events )
#define ctrl_events_at(self, event) ctrl_events_event( ctrl_events(self), event )

#define ctrl_update(self,t,p)     ((ctrl_t*)self)->_update( self, t, p )
#define ctrl_destroy(self)        ((ctrl_t*)self)->_destroy( self )
#define ctrl_header(fp,self,util) ((ctrl_t*)self)->_header( fp, util )
#define ctrl_writer(fp,self,util) ((ctrl_t*)self)->_writer( fp, self, util )

ctrl_t *ctrl_init(ctrl_t *self, cmd_t *cmd, model_t *model);

double ctrl_calc_sqr_v0(double z0, double zd);
#define ctrl_calc_v0(z0,zd) sqrt( ctrl_calc_sqr_v0( z0, zd ) )
#define ctrl_sqr_v0(self) ctrl_calc_sqr_v0( ctrl_z0(self), ctrl_zd(self) )
#define ctrl_v0(self) sqrt( ctrl_sqr_v0( self ) )

ctrl_t *ctrl_update_default(ctrl_t *self, double t, vec_t p);
void ctrl_destroy_default(ctrl_t *self);
void ctrl_header_default(FILE *fp, void *util);
void ctrl_writer_default(FILE *fp, ctrl_t *self, void *util);

#endif /* __RHC_CTRL_H__ */
