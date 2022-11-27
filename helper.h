#ifndef _GUILEHELPER
#define _GUILEHELPER

#include <libguile.h>

// Creates and returns a Guile foreign variable type
SCM make_type(const char *name);
// Checks whether x's type matches guile_type
void assert_type(SCM guile_type, SCM x);
// Returns SCM object from a C pointer
SCM make_obj(SCM guile_type, void *obj);
// Returns the SCM object's C pointer
void* get_pointer(SCM x);

// Conversion
unsigned int to_unsigned(SCM x);
SCM from_unsigned(unsigned int x);

#endif