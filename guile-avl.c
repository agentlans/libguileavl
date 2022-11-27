#include "common.h"
#include "helper.h"
#include "pavl.h"

SCM avl;
SCM avlt;

/*
void guile_table_finalizer(void* tree) {
	pavl_destroy((struct pavl_table*) tree, NULL);
}
*/

#define AVL_SCM(x) make_obj(avl, x)
#define SCM_AVL(x) ((struct pavl_table*) get_pointer(x))
#define AVLT_SCM(x) make_obj(avlt, x)
#define SCM_AVLT(x) ((struct pavl_traverser*) get_pointer(x))
#define ASSERT_AVL(x) assert_type(avl, x)
#define ASSERT_AVLT(x) assert_type(avlt, x)

// My custom function to create an AVL tree from a sorted array
extern struct pavl_table *
pavl_create_tree_from_list(void **arr, size_t n, pavl_comparison_func *compare,
                           void *param, struct libavl_allocator *allocator);

SCM guile_avl_create(SCM compare) {
	return AVL_SCM(pavl_create(
		guile_comparison_func, 
		compare,
		&guile_allocator));
}

SCM guile_avl_create_from_array(SCM arr, SCM compare) {
	// Prepare the array
	scm_t_array_handle handle;
	scm_array_get_handle(arr, &handle);
	// Dimensions of the array
	scm_t_array_dim* dims = scm_array_handle_dims(&handle);
	size_t n = dims[0].ubnd - dims[0].lbnd + 1;
	// Elements of the array
	const SCM* scm_array = scm_array_handle_elements(&handle);
	void* void_array[n];
	for (int i = 0; i < n; ++i) {
		void_array[i] = scm_array[i];
	}
	// Create the table
	struct pavl_table* tbl;
	tbl = pavl_create_tree_from_list(
		void_array, n, guile_comparison_func,
		compare, &guile_allocator);
	// Release the array
	scm_array_handle_release(&handle);
	return AVL_SCM(tbl);
}

SCM guile_avl_copy(SCM tbl) {
	ASSERT_AVL(tbl);
	return AVL_SCM(pavl_copy(
		SCM_AVL(tbl), 
		guile_copy_func,
		guile_item_func,
		&guile_allocator));
}

SCM guile_avl_count(SCM tree) {
	return scm_from_unsigned_integer(pavl_count(SCM_AVL(tree)));
}

// Returns NULL pointer if not found, otherwise returns the item
SCM guile_avl_find(SCM tree, SCM item) {
	ASSERT_AVL(tree);
	RETURN_ITEM(pavl_find(SCM_AVL(tree), item));
}

SCM guile_avl_insert(SCM tree, SCM item) {
	ASSERT_AVL(tree);
	RETURN_ITEM(pavl_insert(SCM_AVL(tree), item));
}

SCM guile_avl_delete(SCM tree, SCM item) {
	ASSERT_AVL(tree);
	RETURN_ITEM(pavl_delete(SCM_AVL(tree), item));
}

SCM guile_avl_replace(SCM tree, SCM item) {
	ASSERT_AVL(tree);
	RETURN_ITEM(pavl_replace(SCM_AVL(tree), item));
}

// Returns a new traverser
SCM guile_avl_t_new(SCM tree) {
	ASSERT_AVL(tree);
	struct pavl_traverser* trav;
	trav = guile_malloc(&guile_allocator, sizeof(struct pavl_traverser));
	pavl_t_init(trav, SCM_AVL(tree));
	return AVLT_SCM(trav);
}

SCM guile_avl_t_first(SCM trav, SCM tree) {
	ASSERT_AVLT(trav);
	ASSERT_AVL(tree);
	RETURN_ITEM(pavl_t_first(SCM_AVLT(trav), SCM_AVL(tree)));
}

SCM guile_avl_t_last(SCM trav, SCM tree) {
	ASSERT_AVLT(trav);
	ASSERT_AVL(tree);
	RETURN_ITEM(pavl_t_last(SCM_AVLT(trav), SCM_AVL(tree)));
}

SCM guile_avl_t_find(SCM trav, SCM tree, SCM item) {
	ASSERT_AVLT(trav);
	ASSERT_AVL(tree);
	RETURN_ITEM(pavl_t_find(SCM_AVLT(trav), SCM_AVL(tree), item));
}

SCM guile_avl_t_insert(SCM trav, SCM tree, SCM item) {
	ASSERT_AVLT(trav);
	ASSERT_AVL(tree);
	RETURN_ITEM(pavl_t_insert(SCM_AVLT(trav), SCM_AVL(tree), item));
}

SCM guile_avl_t_copy(SCM trav, SCM src_trav) {
	ASSERT_AVLT(trav);
	ASSERT_AVLT(src_trav);
	RETURN_ITEM(pavl_t_copy(SCM_AVLT(trav), SCM_AVLT(src_trav)));
}

SCM guile_avl_t_next(SCM trav) {
	ASSERT_AVLT(trav);
	RETURN_ITEM(pavl_t_next(SCM_AVLT(trav)));
}

SCM guile_avl_t_prev(SCM trav) {
	ASSERT_AVLT(trav);
	RETURN_ITEM(pavl_t_prev(SCM_AVLT(trav)));
}

SCM guile_avl_t_cur(SCM trav) {
	ASSERT_AVLT(trav);
	RETURN_ITEM(pavl_t_cur(SCM_AVLT(trav)));
}

SCM guile_avl_t_replace(SCM trav, SCM new_item) {
	ASSERT_AVLT(trav);
	RETURN_ITEM(pavl_t_replace(SCM_AVLT(trav), new_item));
}

void init_guile_avl()
{
	// Initialize types
    avl = make_type("avl-tree");
    avlt = make_type("avl-traverser");
	// AVL tree
	scm_c_define_gsubr("avl", 1, 0, 0, guile_avl_create);
	scm_c_define_gsubr("avl-copy", 1, 0, 0, guile_avl_copy);
	scm_c_define_gsubr("avl-count", 1, 0, 0, guile_avl_count);
	scm_c_define_gsubr("avl-find", 2, 0, 0, guile_avl_find);
	scm_c_define_gsubr("avl-insert!", 2, 0, 0, guile_avl_insert);
	scm_c_define_gsubr("avl-delete!", 2, 0, 0, guile_avl_delete);
	scm_c_define_gsubr("avl-replace!", 2, 0, 0, guile_avl_replace);
	// AVL tree traversers
	scm_c_define_gsubr("avlt", 1, 0, 0, guile_avl_t_new);
	scm_c_define_gsubr("avlt-set-first!", 2, 0, 0, guile_avl_t_first);
	scm_c_define_gsubr("avlt-set-last!", 2, 0, 0, guile_avl_t_last);
	scm_c_define_gsubr("avlt-copy", 2, 0, 0, guile_avl_t_copy);
	scm_c_define_gsubr("avlt-find", 3, 0, 0, guile_avl_t_find);
	scm_c_define_gsubr("avlt-insert!", 3, 0, 0, guile_avl_t_insert);
	scm_c_define_gsubr("avlt-next!", 1, 0, 0, guile_avl_t_next);
	scm_c_define_gsubr("avlt-prev!", 1, 0, 0, guile_avl_t_prev);
	scm_c_define_gsubr("avlt-get", 1, 0, 0, guile_avl_t_cur);
	scm_c_define_gsubr("avlt-set!", 2, 0, 0, guile_avl_t_replace);
	// Create AVL tree from
	scm_c_define_gsubr("avl-from-array", 2, 0, 0, guile_avl_create_from_array);
}

// gcc guile.c pavl.c `pkg-config --cflags --libs guile-3.0`
