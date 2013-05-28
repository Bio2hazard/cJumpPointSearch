#include <stdlib.h>

#include "heap.h"
#include "display.h"
#include "jps_grid.h"

int cmp(struct open_list *one, struct open_list *two)
{
	int a, b;

	if (one->list_node == NULL)
		a = 0;
	else
		a = one->list_node->f;

	if (two->list_node == NULL)
		b = 0;
	else
		b = two->list_node->f;

	return a - b;
}

struct open_list *ol_new()
{
	struct open_list *newlist = (struct open_list *) malloc(sizeof(struct open_list));
	malloc_count++; /* [ Malloc Count ] */
	newlist->right = newlist;
	newlist->left = newlist;
	newlist->list_node = NULL;
	return newlist;
}

void ol_clean(struct open_list *head)
{
	if (head != NULL) {
		struct open_list *pos = head;
		struct open_list *tmp = head;
		do {
			tmp = pos->right;
			free(pos);
			malloc_count--; /* [ Malloc Count ] */
			pos = tmp;
		} while (pos != head);
	}
}

struct open_list *ol_insert_right(struct open_list *list, struct node *data)
{
	struct open_list *newlist = (struct open_list *) malloc(sizeof(struct open_list));
	malloc_count++;         /* [ Malloc Count ] */
	newlist->list_node = data;
	newlist->left = list;
	newlist->right = list->right;
	list->right = newlist;
	newlist->right->left = newlist;
	return newlist;
}

struct open_list *ol_del_free(struct open_list *list)
{
	struct open_list *res = list->left;
	list->right->left = list->left;
	list->left->right = list->right;
	free(list);
	malloc_count--; /* [ Malloc Count ] */
	return res;
}

struct open_list *ol_listsort(struct open_list *list)
{
	struct open_list *p, *q, *e, *tail, *oldhead;
	int insize, nmerges, psize, qsize, i;

	/*
	 * Silly special case: if `list' was passed in as NULL, return
	 * NULL immediately.
	 */
	if (!list)
		return NULL;

	insize = 1;

	while (1) {
		p = list;
		oldhead = list;                /* only used for circular linkage */
		list = NULL;
		tail = NULL;

		nmerges = 0; /* count number of merges we do in this pass */

		while (p) {
			nmerges++; /* there exists a merge to be done */
			/* step `insize' places along from p */
			q = p;
			psize = 0;
			for (i = 0; i < insize; i++) {
				psize++;
				q = (q->right == oldhead ? NULL : q->right);
				if (!q) break;
			}

			/* if q hasn't fallen off end, we have two lists to merge */
			qsize = insize;

			/* now we have two lists; merge them */
			while (psize > 0 || (qsize > 0 && q)) {

				/* decide whether next element of merge comes from p or q */
				if (psize == 0) {
					/* p is empty; e must come from q. */
					e = q;
					q = q->right;
					qsize--;
					if (q == oldhead) q = NULL;
				} else if (qsize == 0 || !q) {
					/* q is empty; e must come from p. */
					e = p;
					p = p->right;
					psize--;
					if (p == oldhead) p = NULL;
				} else if (cmp(p, q) <= 0) {
					/* First element of p is lower (or same);
					 * e must come from p. */
					e = p;
					p = p->right;
					psize--;
					if (p == oldhead) p = NULL;
				} else {
					/* First element of q is lower; e must come from q. */
					e = q;
					q = q->right;
					qsize--;
					if (q == oldhead) q = NULL;
				}

				/* add the next element to the merged list */
				if (tail) {
					tail->right = e;
				} else {
					list = e;
				}
				/* Maintain reverse pointers in a doubly linked list. */
				e->left = tail;
				tail = e;
			}

			/* now p has stepped `insize' places along, and q has too */
			p = q;
		}

		tail->right = list;
		list->left = tail;

		/* If we have done only one merge, we're finished. */
		if (nmerges <= 1) /* allow for nmerges==0, the empty list case */
			return list;

		/* Otherwise repeat, merging lists twice the size */
		insize *= 2;
	}
}
