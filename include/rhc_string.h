#ifndef __RHC_STRING_H__
#define __RHC_STRING_H__

#include "rhc_defs.h"
#include <string.h>

char *string_copy(const char *s, char *d);
int string_len(const char *s);
char *string_reverse(const char *s, char *d);
bool string_is_digit(const char *s);
bool string_starts_with(const char *s, const char *prefix);
bool string_ends_with(const char *s, const char *suffix);

#endif /* __RHC_STRING_H__ */
