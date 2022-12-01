/*
 * DT_list.h
 *
 *  Created on: Dec 1, 2022
 *      Author: marcwech
 */

#ifndef INC_DT_LIST_H_
#define INC_DT_LIST_H_

#include "stdlib.h"


typedef struct list_node {
  struct list_node *prev;
  struct list_node *next;
  void *val;
} list_node_t;

/*
 * list_t struct.
 */

typedef struct {
  list_node_t *head;
  list_node_t *tail;
  unsigned int len;
} list_t;

// Node prototypes.

list_node_t *
list_node_new(void *val);

// list_t prototypes.

list_t *
list_new(void);

list_node_t *
list_rpush(list_t *self, list_node_t *node);

list_node_t *
list_lpush(list_t *self, list_node_t *node);

list_node_t *
list_find(list_t *self, void *val);

list_node_t *
list_at(list_t *self, int index);

list_node_t *
list_rpop(list_t *self);

list_node_t *
list_lpop(list_t *self);

void
list_remove(list_t *self, list_node_t *node);

void
list_destroy(list_t *self);




#endif /* INC_DT_LIST_H_ */
