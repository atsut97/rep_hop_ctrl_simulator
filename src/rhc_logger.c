#include "rhc_logger.h"
#include "rhc_string.h"

logger_t *logger_init(logger_t *self, const char *filename)
{
  string_copy(filename, self->filename);
  return self;
}

void logger_destroy(logger_t *self)
{
}

