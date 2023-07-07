#ifndef SNAKE_SRC_CORE_BODY_H_
#define SNAKE_SRC_CORE_BODY_H_

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

	int dir;
	size_t len;
	struct list_t body;
};

void snake_init(struct snake_t* snake,
                struct field_t* field);

void snake_free(struct snake_t* snake);

void snake_set_collision_dist(struct snake_t* snake);

void snake_set_food_dist(struct snake_t* snake);

/**
 * \brief 移动
 *
 * 蛇身的移动, 长度变化, 碰撞判断
 *
 * \retval -1 发生碰撞
 */
int snake_move(struct snake_t* snake);

/**
 * \brief 转向
 *
 * 蛇头转向, 重设与障碍和食物的距离
 *
 * \retval -1 给定的方向与蛇目前的移动方向平行
 */
int snake_veer(struct snake_t* snake, int dir);

/**
 * \brief 吃掉食物
 *
 * \retval -1 未发现食物
 */
int snake_eat(struct snake_t* snake);

// TODO: 以上三个方法
#endif //SNAKE_SRC_CORE_BODY_H_
