#include "rhc_string.h"

char *string_copy(const char *s, char *d) {
  return strcpy(d, s);
}

int string_len(const char *s) {
  return strlen( s );
}
