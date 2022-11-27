#include "pavl.h"
#include <assert.h>

struct pavl_node *make_tree(void **arr, struct pavl_node *parent, int start,
                            int end, struct libavl_allocator *alloc);
int label_height(struct pavl_node *node);
int get_height_label(struct pavl_node *node);
void label_balance(struct pavl_node *node);

#define LTREE(x) ((x)->pavl_link[0])
#define RTREE(x) ((x)->pavl_link[1])

inline int max(int a, int b) {
  if (a > b)
    return a;
  else
    return b;
}

// Returns an AVL tree from an array sorted with the comparison function
struct pavl_table *
pavl_create_tree_from_list(void **arr, size_t n, pavl_comparison_func *compare,
                           void *param, struct libavl_allocator *allocator) {

  struct pavl_table *tree;

  assert(compare != NULL);

  if (allocator == NULL)
    allocator = &pavl_allocator_default;

  tree = allocator->libavl_malloc(allocator, sizeof *tree);
  if (tree == NULL)
    return NULL;

  tree->pavl_root = NULL;
  tree->pavl_compare = compare;
  tree->pavl_param = param;
  tree->pavl_alloc = allocator;
  tree->pavl_count = n;

  // Make the root node of the tree
  tree->pavl_root = make_tree(arr, NULL, 0, n, allocator);
  // Find the heights of each node
  label_height(tree->pavl_root);
  // Rewrite the labels to their balance factors.
  label_balance(tree->pavl_root);
  // Return AVL tree
  return tree;
}

// Returns pointer to the root node of a balanced binary tree containing
// the elements of arr.
struct pavl_node *make_tree(void **arr, struct pavl_node *parent, int start,
                            int end, struct libavl_allocator *alloc) {
  if (start >= end) {
    return NULL;
  } else {
    int pivot = (start + end) / 2;
    struct pavl_node *node;
    node = alloc->libavl_malloc(alloc, sizeof(struct pavl_node));
    if (node == NULL) {
      return NULL;
    }
    LTREE(node) = make_tree(arr, node, start, pivot, alloc);
    RTREE(node) = make_tree(arr, node, pivot + 1, end, alloc);
    node->pavl_parent = parent;
    node->pavl_data = arr[pivot];
    return node;
  }
}

// Finds the height of the node and saves it
// the pavl_balance slot (temporary until label_balance is called)
int label_height(struct pavl_node *node) {
  if (node == NULL) {
    return 0;
  } else {
    // label children with their heights
    int lh = label_height(LTREE(node));
    int rh = label_height(RTREE(node));
    int h = max(lh, rh) + 1;
    node->pavl_balance = h;
    return h;
  }
}

int get_height_label(struct pavl_node *node) {
  if (node == NULL) {
    return 0;
  } else {
    return node->pavl_balance;
  }
}

// Label the nodes with the correct balance factor
void label_balance(struct pavl_node *node) {
  if (node != NULL) {
    int lh = get_height_label(LTREE(node));
    int rh = get_height_label(RTREE(node));
    node->pavl_balance = rh - lh;
    // Label this node's child nodes
    label_balance(LTREE(node));
    label_balance(RTREE(node));
  }
}
