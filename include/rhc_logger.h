#ifndef __RHC_LOGGER_H__
#define __RHC_LOGGER_H__

#include "rhc_misc.h"

typedef struct{
  char filename[BUFSIZ];
  FILE* fp;
} logger_t;

#define logger_filename(self) (self)->filename

logger_t *logger_init(logger_t *self, const char *filename);
void logger_destroy(logger_t *self);

FILE* logger_open(logger_t *self, const char *filename);
void logger_close(logger_t *self);

#endif /* __RHC_LOGGER_H__ */
