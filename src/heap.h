#ifndef __included_heap_h
#define __included_heap_h

extern int malloc_count;

/* Circular Doubly Linked List that contains a list of all open nodes ( nodes eligible for investigation ) */
struct open_list {
	struct open_list *left;
	struct open_list *right;
	struct node *list_node;
};

/* Comparison function. Used to determine which node should be examined next */
int cmp(struct open_list *one, struct open_list *two);

/* New list */
struct open_list *ol_new();

/* Clean list */
void ol_clean(struct open_list *head);

/* Add to list */
struct open_list *ol_insert_right(struct open_list *list, struct node *data);

/* Used to delete a entry & free the ressources */
struct open_list *ol_del_free(struct open_list *list);

/* Perform a mergesort on the list ( pass & receive the list's head ) */
struct open_list *ol_listsort(struct open_list *list);

#endif
