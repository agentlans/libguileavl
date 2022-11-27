#include "common.h"
#include "pbst.h"

void guile_bst_finalizer(void* tree) {
	pbst_destroy((struct pbst_table*) tree, NULL);
}

#define bst_TO_SCM(x) scm_from_pointer((x), guile_bst_finalizer)
#define SCM_TO_bst(x) ((struct pbst_table*) scm_to_pointer(x))
#define TRAV(x) ((struct pbst_traverser*) scm_to_pointer(x))

SCM guile_bst_create(SCM compare) {
	return bst_TO_SCM(pbst_create(
		guile_comparison_func, 
		compare,
		&guile_allocator));
}

SCM guile_bst_copy(SCM tbl) {
	return bst_TO_SCM(pbst_copy(
		SCM_TO_bst(tbl), 
		guile_copy_func,
		guile_item_func,
		&guile_allocator));
}

SCM guile_bst_count(SCM tree) {
	return scm_from_unsigned_integer(pbst_count(SCM_TO_bst(tree)));
}

// Returns NULL pointer if not found, otherwise returns the item
SCM guile_bst_find(SCM tree, SCM item) {
	RETURN_ITEM(pbst_find(SCM_TO_bst(tree), item));
}

SCM guile_bst_insert(SCM tree, SCM item) {
	RETURN_ITEM(pbst_insert(SCM_TO_bst(tree), item));
}

SCM guile_bst_delete(SCM tree, SCM item) {
	RETURN_ITEM(pbst_delete(SCM_TO_bst(tree), item));
}

SCM guile_bst_replace(SCM tree, SCM item) {
	RETURN_ITEM(pbst_replace(SCM_TO_bst(tree), item));
}

// Returns a new traverser
SCM guile_bst_t_new(SCM tree) {
	struct pbst_traverser* trav;
	trav = guile_malloc(&guile_allocator, sizeof(struct pbst_traverser));
	pbst_t_init(trav, SCM_TO_bst(tree));
	return TO_SCM(trav);
}

/*SCM guile_bst_t_init(SCM trav, SCM tree) {
	pbst_t_init(TRAV(trav), SCM_TO_bst(tree));
	return SCM_UNSPECIFIED;
}*/

SCM guile_bst_t_first(SCM trav, SCM tree) {
	RETURN_ITEM(pbst_t_first(TRAV(trav), SCM_TO_bst(tree)));
}

SCM guile_bst_t_last(SCM trav, SCM tree) {
	RETURN_ITEM(pbst_t_last(TRAV(trav), SCM_TO_bst(tree)));
}

SCM guile_bst_t_find(SCM trav, SCM tree, SCM item) {
	RETURN_ITEM(pbst_t_find(TRAV(trav), SCM_TO_bst(tree), item));
}

SCM guile_bst_t_insert(SCM trav, SCM tree, SCM item) {
	RETURN_ITEM(pbst_t_insert(TRAV(trav), SCM_TO_bst(tree), item));
}

SCM guile_bst_t_copy(SCM trav, SCM src_trav) {
	RETURN_ITEM(pbst_t_copy(TRAV(trav), TRAV(src_trav)));
}

SCM guile_bst_t_next(SCM trav) {
	RETURN_ITEM(pbst_t_next(TRAV(trav)));
}

SCM guile_bst_t_prev(SCM trav) {
	RETURN_ITEM(pbst_t_prev(TRAV(trav)));
}

SCM guile_bst_t_cur(SCM trav) {
	RETURN_ITEM(pbst_t_cur(TRAV(trav)));
}

SCM guile_bst_t_replace(SCM trav, SCM new_item) {
	RETURN_ITEM(pbst_t_replace(TRAV(trav), new_item));
}

void init_guile_bst()
{
	// bst tree
	scm_c_define_gsubr("bst", 1, 0, 0, guile_bst_create);
	scm_c_define_gsubr("bst-copy", 1, 0, 0, guile_bst_copy);
	scm_c_define_gsubr("bst-count", 1, 0, 0, guile_bst_count);
	scm_c_define_gsubr("bst-find", 2, 0, 0, guile_bst_find);
	scm_c_define_gsubr("bst-insert!", 2, 0, 0, guile_bst_insert);
	scm_c_define_gsubr("bst-delete!", 2, 0, 0, guile_bst_delete);
	scm_c_define_gsubr("bst-replace!", 2, 0, 0, guile_bst_replace);
	// bst tree traversers
	scm_c_define_gsubr("bstt", 1, 0, 0, guile_bst_t_new);
	scm_c_define_gsubr("bstt-set-first!", 2, 0, 0, guile_bst_t_first);
	scm_c_define_gsubr("bstt-set-last!", 2, 0, 0, guile_bst_t_last);
	scm_c_define_gsubr("bstt-copy", 2, 0, 0, guile_bst_t_copy);
	scm_c_define_gsubr("bstt-find", 3, 0, 0, guile_bst_t_find);
	scm_c_define_gsubr("bstt-insert!", 3, 0, 0, guile_bst_t_insert);
	scm_c_define_gsubr("bstt-next!", 1, 0, 0, guile_bst_t_next);
	scm_c_define_gsubr("bstt-prev!", 1, 0, 0, guile_bst_t_prev);
	scm_c_define_gsubr("bstt-get", 1, 0, 0, guile_bst_t_cur);
	scm_c_define_gsubr("bstt-set!", 2, 0, 0, guile_bst_t_replace);
}

// gcc guile.c pbst.c `pkg-config --cflags --libs guile-3.0`
