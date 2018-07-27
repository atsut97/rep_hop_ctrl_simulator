#include "rhc_string.h"
#include <ctype.h>

char *string_copy(const char *s, char *d) {
  return strcpy(d, s);
}

int string_len(const char *s) {
  return strlen( s );
}

bool string_is_digit(const char *s) {
  register int i;

  for( i=0; i<string_len(s); i++ )
    if( isalpha( s[i] ) )
      return false;
  return true;
}
