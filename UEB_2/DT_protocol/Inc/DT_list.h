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




#endif /* INC_DT_LIST_H_ */
