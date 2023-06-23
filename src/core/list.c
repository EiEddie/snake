#include <core/list.h>


void list_init(struct list_t* list) {
	list->len = 0;
	list->head = list->tail = NULL;
}

void list_free(struct list_t* list) {
	struct node_t* self = list->head;
	while(self != NULL) {
		struct node_t* tmp = self;
		self = self->prev;
		free(tmp);
	}

	list->head = list->tail = NULL;
	list->len = 0;
}

void list_push(struct list_t* list, struct val_t val) {
	if(list->len == 0) {
		list->head = list->tail =
		    malloc(sizeof(struct node_t));
	} else {
		list->tail = (list->tail->prev =
		                  malloc(sizeof(struct node_t)));
	}
	list->tail->val = val;
	list->tail->prev = NULL;
	list->len++;
}

void list_pop_front(struct list_t* list) {
	struct node_t* tmp = list->head;
	list->head = list->head->prev;
	free(tmp);
	list->len--;
}


void iter_init(struct iter_t* iter, struct list_t* list) {
	iter->list = list;
	iter->self = list->head;
}

int iter_next(struct iter_t* iter) {
	if(iter->self->prev == NULL)
		return -1;

	iter->self = iter->self->prev;
	return 0;
}

struct val_t iter_val(struct iter_t* iter) {
	return iter->self->val;
}
