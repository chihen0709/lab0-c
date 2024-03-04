#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *pre, *safe;
    list_for_each_entry_safe (pre, safe, head, list) {
        list_del(&pre->list);
        q_release_element(pre);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;
    element_t *ele = malloc(sizeof(element_t));
    if (!ele)
        return false;
    int length = sizeof(char) * (strlen(s) + 1);
    ele->value = malloc(length);
    if (!ele->value) {
        free(ele);
        return false;
    }
    strncpy(ele->value, s, length);
    list_add(&ele->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;
    element_t *ele = malloc(sizeof(element_t));
    if (!ele)
        return false;
    int length = sizeof(char) * (strlen(s) + 1);
    ele->value = malloc(length);
    if (!ele->value) {
        free(ele);
        return false;
    }
    strncpy(ele->value, s, length);
    list_add_tail(&ele->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *rev = list_first_entry(head, element_t, list);
    list_del_init(&rev->list);
    if (sp != NULL) {
        strncpy(sp, rev->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return rev;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *rev = list_last_entry(head, element_t, list);
    list_del_init(&rev->list);
    if (sp != NULL) {
        strncpy(sp, rev->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return rev;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int size = 0;
    struct list_head *ele;
    list_for_each (ele, head)
        ;
    size++;
    return size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    struct list_head *fast = head->next, *slow = fast;
    while ((fast != head) && (fast->next != head)) {
        fast = fast->next->next;
        slow = slow->next;
    }
    element_t *ele = list_entry(slow, element_t, list);
    list_del(slow);
    q_release_element(ele);
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    element_t *ele, *nex;
    bool is_dup = false;
    list_for_each_entry_safe (ele, nex, head, list) {
        if (ele->list.next != head && strcmp(ele->value, nex->value) == 0) {
            list_del(&ele->list);
            q_release_element(ele);
            is_dup = true;
        } else if (is_dup) {
            list_del(&ele->list);
            q_release_element(ele);
            is_dup = false;
        }
    }
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/

    if (!head || list_empty(head))
        return;
    struct list_head *ele = head->next;
    while (ele != head && ele->next != head) {
        struct list_head *t = ele;
        list_move(ele, t->next);
        ele = ele->next;
    }
}
/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *ele, *s, *t;
    list_for_each_safe (ele, s, head) {
        t = ele->next;
        ele->next = ele->prev;
        ele->prev = t;
    }
    t = head->next;
    head->next = head->prev;
    head->prev = t;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head))
        return;
    struct list_head *ele, *s, iter, *tmp_head = head;
    int i = 0;
    INIT_LIST_HEAD(&iter);
    list_for_each_safe (ele, s, head) {
        i++;
        if (i == k) {
            list_cut_position(&iter, tmp_head, s);
            q_reverse(&iter);
            list_splice_init(&iter, tmp_head);
            i = 0;
            tmp_head = s->prev;
        }
        // https://leetcode.com/problems/reverse-nodes-in-k-group/
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}
/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    struct list_head *ele = head->prev, *nex = ele->prev;
    while (ele != head) {
        if (strcmp(list_entry(nex, element_t, list)->value,
                   list_entry(ele, element_t, list)->value) < 0) {
            list_del(ele);
        } else {
            ele = nex;
        }
        ele = ele->prev;
    }

    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
