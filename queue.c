#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */

    if (!q)
        return;

    list_ele_t *next = NULL;
    while (q->head) {
        next = q->head->next;  //紀錄下一個
        free(q->head->value);  // free string
        free(q->head);         // free list element
        q->head = next;        // next
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;

    if (!q)
        return false;

    /* TODO: What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (newh)
        newh->value = malloc(sizeof(char) * (strlen(s) + 1));  // allocate
    else
        return false;

    if (!newh->value) {
        free(newh);
        return false;
    }

    memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));  // copy string

    newh->next = q->head;
    q->head = newh;


    if (!q->tail) {
        q->tail = newh;
        newh->next = NULL;
    }
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newt = malloc(sizeof(list_ele_t));

    if (!newt)
        return false;


    newt->value = malloc(sizeof(char) * sizeof(strlen(s)) + 1);

    if (!newt->value) {
        free(newt);
        return false;
    }

    memset(newt->value, '\0', strlen(s) + 1);
    strncpy(newt->value, s, strlen(s));

    if (!q->tail)
        q->head = q->tail = newt;
    else {
        q->tail->next = newt;
        q->tail = newt;
    }

    newt->next = NULL;
    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q)
        return false;

    list_ele_t *rmh = q->head;

    if (!rmh)
        return false;

    q->head = q->head->next;

    if (sp && rmh->value) {
        size_t msgLength = strlen(rmh->value);
        memset(sp, 0, bufsize);
        if (msgLength < bufsize)
            strncpy(sp, rmh->value, msgLength);
        else
            strncpy(sp, rmh->value, bufsize - 1);
    }
    free(rmh->value);
    free(rmh);
    q->size--;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    list_ele_t *reverse(list_ele_t * cur, list_ele_t * *head);

    if (!q || !q->head)
        return;

    if (!q->head->next)
        return;

    q->tail = q->head;
    list_ele_t *tmp;
    reverse(q->head->next, &tmp)->next = q->head;
    q->head = tmp;
    q->tail->next = NULL;
}

list_ele_t *reverse(list_ele_t *cur, list_ele_t **head)
{
    if (cur->next) {
        reverse(cur->next, head)->next = cur;
    } else {
        *head = cur;
    }
    return cur;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;

    void mergesort(list_ele_t **);

    mergesort(&q->head);
    list_ele_t *tmp = q->head;
    while (tmp->next) {
        tmp = tmp->next;
    }
    q->tail = tmp;
}

void mergesort(list_ele_t **head)
{
    void merge(list_ele_t *, list_ele_t *, list_ele_t **);
    /* check if bigger than 2*/
    if (!*head || !(*head)->next)
        return;

    /* init */
    list_ele_t *t = *head;
    list_ele_t *r = (*head)->next;

    /* fast-slow pointer */
    while (r && r->next) {
        t = t->next;
        r = r->next->next;
    }

    /* splite into two list*/
    r = t->next;
    t->next = NULL;
    t = *head;

    /* recursive */
    mergesort(&t);
    mergesort(&r);

    /* merge */
    merge(t, r, head);

    return;
}


void merge(list_ele_t *l1, list_ele_t *l2, list_ele_t **head)
{
    list_ele_t *cur = NULL;
    list_ele_t *tmp = NULL;
    *head = NULL;
    /* compare two node val */
    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) < 0) {
            tmp = l1;
            l1 = l1->next;
        } else {
            tmp = l2;
            l2 = l2->next;
        }
        if (!*head)
            *head = cur = tmp;
        else {
            cur->next = tmp;
            cur = tmp;
        }
    }

    /* if l1 or l2 exist */
    if (l1 || l2)
        if (l1)
            cur->next = l1;
        else
            cur->next = l2;
    else
        cur->next = NULL;


    return;
}
