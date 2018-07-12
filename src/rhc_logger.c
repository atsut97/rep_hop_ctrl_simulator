#include "rhc_logger.h"
#include "rhc_string.h"

logger_t *logger_init(logger_t *self, const char *filename)
{
  if(filename) {
    /* open file */
    logger_open(self, filename);
  } else {
    /* supposed to use stdout */
    string_copy("", self->filename);
    self->fp = NULL;
  }
  return self;
}

void logger_destroy(logger_t *self)
{
  if( self->fp )
    fclose(self->fp);
  self->fp = NULL;
}

FILE* logger_open(logger_t *self, const char *filename)
{
  string_copy(filename, self->filename);
  self->fp = fopen(filename, "w");
  if(!self->fp) {
    RUNTIME_ERR(filename);
    return NULL;
  }
  return self->fp;
}
