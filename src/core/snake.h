#ifndef SNAKE_SRC_CORE_SNAKE_H_
#define SNAKE_SRC_CORE_SNAKE_H_

#include <core/list.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>


#define min(x, y) ((x) > (y) ? (y) : (x))
#define max(x, y) ((x) > (y) ? (x) : (y))


pos_t pnt_move(pos_t pnt, int dir, int step);


struct field_t {
	size_t wight, height;
	pos_t food;
};

void field_init(struct field_t* field, size_t wight,
                size_t height);


struct snake_t {
	struct field_t* field;

	int collision_dist;
	int food_dist;

	int food_cnt;

	int dir;
	size_t len;
	struct list_t body;
};

/**
 * \warning 使用前需设定随机数种子
 */
void snake_init(struct snake_t* snake,
                struct field_t* field);

void snake_free(struct snake_t* snake);

/**
 * \brief 移动与转向
 *
 * 蛇身的移动, 长度变化, 碰撞判断
 * 蛇头转向, 重设与障碍和食物的距离
 *
 * \retval -1 发生碰撞
 */
int snake_move(struct snake_t* snake, int dir);

/**
 * \brief 吃掉食物
 *
 * \retval -1 未发现食物
 */
int snake_eat(struct snake_t* snake);


#endif //SNAKE_SRC_CORE_SNAKE_H_
