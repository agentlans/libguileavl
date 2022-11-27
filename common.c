#include "common.h"

void * guile_malloc(struct libavl_allocator *allocator, size_t size) {
  assert (allocator != NULL && size > 0);
  return scm_gc_malloc(size, "");
}

void guile_free(struct libavl_allocator *allocator, void *block) {
  assert (allocator != NULL && block != NULL);
  //free(block); // not needed with a garbage collector
}

int guile_comparison_func(const void *a, const void *b, void *comp) {
	return scm_to_int8(scm_call_2((SCM) comp, (SCM) a, (SCM) b));
}

void* guile_copy_func(void* item, void* param) {
	return item;
}
void guile_item_func(void* item, void* param) {}
