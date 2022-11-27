#ifndef _TREECOMMON
#define _TREECOMMON

#include <libguile.h>
#include <assert.h>

// The Guile custom types
//SCM bst;
//SCM bstt;

// Wrappers around libavl's functions to work with Guile

#define LIBAVL_ALLOCATOR
/* Memory allocator. */
struct libavl_allocator {
    void *(*libavl_malloc) (struct libavl_allocator *, size_t libavl_size);
    void (*libavl_free) (struct libavl_allocator *, void *libavl_block);
};

void * guile_malloc(struct libavl_allocator *allocator, size_t size);
void guile_free(struct libavl_allocator *allocator, void *block);

static struct libavl_allocator guile_allocator = {guile_malloc, guile_free};

int guile_comparison_func(const void *a, const void *b, void *comp);
void* guile_copy_func(void* item, void* param);
void guile_item_func(void* item, void* param);

#define TO_SCM(x) scm_from_pointer(x, NULL)
#define RETURN_ITEM(expr) \
	void* xxx = (expr); \
	if (!xxx) return TO_SCM(NULL); else return (SCM) xxx;

#endif
