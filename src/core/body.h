#ifndef SNAKE_SRC_CORE_BODY_H_
#define SNAKE_SRC_CORE_BODY_H_

#include <core/list.h>
#include <time.h>
#include <stdlib.h>


#define min(x, y) ((x) > (y) ? (y) : (x))
#define max(x, y) ((x) > (y) ? (x) : (y))


pos_t pnt_move(pos_t pnt, int dir, int step);


struct snake_t {
	size_t wight, height;
	/** \brief 蛇头距离障碍物的距离 */
	int dist;

	int dir;
	size_t len;
	struct list_t body;
};

void snake_init(struct snake_t* snake, size_t wight,
                size_t height);

void snake_free(struct snake_t* snake);

void snake_set_dist(struct snake_t* snake);

#endif //SNAKE_SRC_CORE_BODY_H_
