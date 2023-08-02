/**
 * \brief 只应在调试时被外部使用的函数
 */

#ifndef SNAKE_SRC_CORE_DEBUG_H_
#define SNAKE_SRC_CORE_DEBUG_H_

#include <core/snake.h>


pos_t gen_food_outer(int w, int h, pos_t near, pos_t far);

pos_t gen_food_inner(pos_t near, pos_t far,
                     struct snake_t* snake);

int gen_food(struct field_t* field, struct snake_t* snake);


void snake_set_collision_dist(struct snake_t* snake);

void snake_set_food_dist(struct snake_t* snake);


#endif //SNAKE_SRC_CORE_DEBUG_H_
