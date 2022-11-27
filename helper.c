#include <stdint.h>
#include "helper.h"

// Creates and returns a Guile foreign variable type
SCM make_type(const char *name) {
  SCM guile_type_name = scm_from_utf8_symbol(name);
  SCM slots = scm_list_1(scm_from_utf8_symbol("pointer"));
  return scm_make_foreign_object_type(guile_type_name, slots, NULL);
}

// Checks whether x's type matches guile_type
void assert_type(SCM guile_type, SCM x) {
  scm_assert_foreign_object_type(guile_type, x);
}

// Returns SCM object from a C pointer
SCM make_obj(SCM guile_type, void *obj) {
  return scm_make_foreign_object_1(guile_type, obj);
}

// Returns the SCM object's C pointer
void* get_pointer(SCM x) {
  return scm_foreign_object_ref(x, 0);
}

// Conversion
unsigned int to_unsigned(SCM x) {
  return scm_to_unsigned_integer(x, 0, UINT_MAX);
}

SCM from_unsigned(unsigned int x) {
  return scm_from_unsigned_integer(x);
}