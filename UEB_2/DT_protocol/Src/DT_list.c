/*
 * DT_list.c
 *
 *  Created on: Dec 1, 2022
 *      Author: marcwech
 */

#include "DT_list.h"

/*
 * Allocate a new list_t. NULL on failure.
 */

list_t *
list_new(void) {
  list_t *self;
  if (!(self = malloc(sizeof(list_t))))
    return NULL;
  self->head = NULL;
  self->tail = NULL;
  self->len = 0;
  return self;
}

list_node_t *
list_node_new(void *val) {
  list_node_t *self;
  if (!(self = malloc(sizeof(list_node_t))))
    return NULL;
  self->prev = NULL;
  self->next = NULL;
  self->val = val;
  return self;
}

/*
 * Free the list.
 * @self: Pointer to the list
 */

void
list_destroy(list_t *self) {
  unsigned int len = self->len;
  list_node_t *next;
  list_node_t *curr = self->head;

  while (len--) {
    next = curr->next;
    free(curr);
    curr = next;
  }
  free(self);
}

/*
 * Append the given node to the list
 * and return the node, NULL on failure.
 * @self: Pointer to the list for popping node
 * @node: the node to push
 */

list_node_t *
list_rpush(list_t *self, list_node_t *node) {
  if (!node) return NULL;

  if (self->len) {
    node->prev = self->tail;
    node->next = NULL;
    self->tail->next = node;
    self->tail = node;
  } else {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node;
}

/*
 * Return / detach the last node in the list, or NULL.
 * @self: Pointer to the list for popping node
 */

list_node_t *
list_rpop(list_t *self) {
  if (!self->len) return NULL;

  list_node_t *node = self->tail;

  if (--self->len) {
    (self->tail = node->prev)->next = NULL;
  } else {
    self->tail = self->head = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Return / detach the first node in the list, or NULL.
 * @self: Pointer to the list for popping node
 */

list_node_t *
list_lpop(list_t *self) {
  if (!self->len) return NULL;

  list_node_t *node = self->head;

  if (--self->len) {
    (self->head = node->next)->prev = NULL;
  } else {
    self->head = self->tail = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Prepend the given node to the list
 * and return the node, NULL on failure.
 * @self: Pointer to the list for pushing node
 * @node: the node to push
 */

list_node_t *
list_lpush(list_t *self, list_node_t *node) {
  if (!node) return NULL;

  if (self->len) {
    node->next = self->head;
    node->prev = NULL;
    self->head->prev = node;
    self->head = node;
  } else {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node;
}

/*
 * Return the node associated to val or NULL.
 * @self: Pointer to the list for finding given value
 * @val: Value to find
 */

list_node_t *
list_find(list_t *self, uint8_t ID) {

  list_node_t *node = self->head;

  while (node->next == NULL) {
	 if(node->val->ID == ID)
		 return node;
  }
  return NULL;
}

/*
 * Return the node at the given index or NULL.
 * @self: Pointer to the list for finding given index
 * @index: the index of node in the list
 */

list_node_t *
list_at(list_t *self, int index) {

	if(self->len < (unsigned)index)
		return NULL;
	list_node_t *node = self->head;

	for(int i = 0; i < index; i++){
		node = node->next;
	}
	return node;
}

/*
 * Remove the given node from the list, freeing it and it's value.
 * @self: Pointer to the list to delete a node
 * @node: Pointer the node to be deleted
 */

void
list_remove(list_t *self, list_node_t *node) {
  if(node == NULL)
	  return;
  if(node == self->head){
	  node->next->prev = NULL;
	  self->head = node->next;
	  free(node);
	  return;
  }
  if(node == self->tail){
	  node->prev->next = NULL;
	  self->tail = node->prev;
	  free(node);
	  return;
  }
  node->next->prev = node->prev;
  node->prev->next = node->next;
  free(node);
  --self->len;
  return;
}
