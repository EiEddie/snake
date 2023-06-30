#ifndef SNAKE_SRC_CORE_LIST_H_
#define SNAKE_SRC_CORE_LIST_H_

#include <stddef.h>
#include <malloc.h>


typedef struct {
	int x, y;
} pos_t;


struct val_t {
	/** \brief 节的位置 */
	pos_t pos;
	/** \brief 节的方向 */
	int dir;
	/** \biref 节的长度 */
	size_t len;
};

struct node_t {
	struct val_t val;
	struct node_t* prev;
};

struct list_t {
	size_t len;
	struct node_t* head;
	struct node_t* tail;
};

void list_init(struct list_t* list);

void list_free(struct list_t* list);

void list_push_back(struct list_t* list, struct val_t val);

void list_pop_front(struct list_t* list);

struct val_t list_head(struct list_t* list);

struct val_t list_tail(struct list_t* list);


struct iter_t {
	struct list_t* list;
	struct node_t* self;
};

void iter_init(struct iter_t* iter, struct list_t* list);

/**
 * \retval -1 已到达末尾
 */
int iter_next(struct iter_t* iter);

struct val_t iter_val(struct iter_t* iter);

#endif //SNAKE_SRC_CORE_LIST_H_
