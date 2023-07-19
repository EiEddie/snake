#include <core/snake.h>


pos_t pnt_move(pos_t pnt, int dir, int step) {
	dir = (dir + 4) % 4;

	pos_t ans = pnt;
	if(dir & 1)
		ans.y -= ((dir & 0b10) - 1) * step;
	else
		ans.x -= ((dir & 0b10) - 1) * step;

	return ans;
}


void field_init(struct field_t *field, size_t wight,
                size_t height) {
	field->wight = wight;
	field->height = height;
	field->food.x = field->food.y = 0;
}


void snake_init(struct snake_t *snake,
                struct field_t *field) {
	srand(time(NULL));
	pos_t begin_pnt = {rand() % field->wight,
	                   rand() % field->height};

	// 使蛇向中心方向走, 避免开局撞墙
	pos_t offset = {(int)field->wight / 2 - begin_pnt.x,
	                (int)field->height / 2 - begin_pnt.y};
	int dir = abs(offset.x) < abs(offset.y);
	if(dir)
		dir |= (offset.y < 0) << 1;
	else
		dir |= (offset.x < 0) << 1;

	snake->field = field;
	snake->collision_dist = INT_MAX;
	snake->food_dist = INT_MAX;
	snake->food_cnt = 0;
	snake->len = 1;
	snake->dir = dir;
	list_init(&snake->body);
	struct val_t val = {begin_pnt, dir, 1};
	// 蛇头储存在链表的尾部
	list_push_back(&snake->body, val);

	snake_set_collision_dist(snake);
	snake_set_food_dist(snake);
}

void snake_free(struct snake_t *snake) {
	snake->len = 0;
	list_free(&snake->body);
}

void snake_set_collision_dist(struct snake_t *snake) {
	struct iter_t iter;
	iter_init(&iter, &snake->body);
	snake->collision_dist = INT_MAX;

	// 与自身的碰撞检测
	pos_t snake_pos = (*list_tail(&snake->body)).pos;
	do {
		struct val_t section = iter_val(&iter);

		// 若此节与蛇头不垂直
		if(!((section.dir + snake->dir) % 2))
			continue;

		// 若此节不在蛇移动方向上
		if((snake->dir & 1
		        // 上下移动
		        ? section.pos.y < snake_pos.y
		        // 左右移动
		        : section.pos.x < snake_pos.x)
		   ^ (snake->dir & 0b10))
			continue;

		pos_t section_bg =
		    pnt_move(section.pos, section.dir + 2, 1);
		pos_t section_ed = pnt_move(
		    section.pos, section.dir, (int)section.len - 1);

		// 若此节不在蛇头正前方
		if(snake->dir & 1) {
			// 上下移动
			if(min(section_bg.x, section_ed.x) > snake_pos.x
			   || max(section_bg.x, section_ed.x)
			       < snake_pos.x)
				continue;
		} else {
			// 左右移动
			if(min(section_bg.y, section_ed.y) > snake_pos.y
			   || max(section_bg.y, section_ed.y)
			       < snake_pos.y)
				continue;
		}

		if(snake->dir & 1)
			// 上下移动
			snake->collision_dist =
			    min(snake->collision_dist,
			        abs(section.pos.y - snake_pos.y));
		else
			// 左右移动
			snake->collision_dist =
			    min(snake->collision_dist,
			        abs(section.pos.x - snake_pos.x));
	} while(!iter_next(&iter));

	// 与墙壁的碰撞检测
	switch(snake->dir) {
	case 0:
		snake->collision_dist =
		    min(snake->collision_dist,
		        snake->field->wight - snake_pos.x);
		break;
	case 1:
		snake->collision_dist =
		    min(snake->collision_dist,
		        snake->field->height - snake_pos.y);
		break;
	case 2:
		snake->collision_dist =
		    min(snake->collision_dist, snake_pos.x + 1);
		break;
	case 3:
		snake->collision_dist =
		    min(snake->collision_dist, snake_pos.y + 1);
		break;
	}
}

void snake_set_food_dist(struct snake_t *snake) {
	snake->food_dist = INT_MAX;
	pos_t snake_pos = (*list_tail(&snake->body)).pos;

	// 若食物不在蛇移动方向上
	if((snake->dir & 1
	        // 上下移动
	        ? snake->field->food.y < snake_pos.y
	        // 左右移动
	        : snake->field->food.x < snake_pos.x)
	   ^ (snake->dir & 0b10))
		return;

	// 若食物不在蛇头正前方
	if(snake->dir & 1 ? snake->field->food.x != snake_pos.x
	                  : snake->field->food.y != snake_pos.y)
		return;

	if(snake->dir & 1)
		// 上下移动
		snake->food_dist =
		    abs(snake->field->food.y - snake_pos.y);
	else
		// 左右移动
		snake->food_dist =
		    abs(snake->field->food.x - snake_pos.x);
}

int snake_eat(struct snake_t *snake) {
	if(snake->food_dist <= 0) {
		snake->food_cnt++;
		// TODO: 生成新的食物
		snake_set_food_dist(snake);
		return 0;
	} else {
		return -1;
	}
}

int snake_move(struct snake_t *snake, int dir) {
	if((dir + snake->dir) % 2 != 0) {
		// 给定的方向与蛇移动方向不平行
		// 即蛇将要转向
		// 设定新的蛇头所在的节

		/** \brief snake_head */
		struct val_t shead = *list_tail(&snake->body);
		pos_t new_shead_pos = pnt_move(
		    shead.pos, shead.dir, (int)(shead.len - 1));
		new_shead_pos = pnt_move(new_shead_pos, dir, 1);

		struct val_t new_shead = {new_shead_pos, dir, 1};
		list_push_back(&snake->body, new_shead);

		snake_set_food_dist(snake);
		snake_set_collision_dist(snake);
	}

	if(snake->collision_dist <= 1)
		return -1;

	snake->food_dist--;
	snake->collision_dist--;

	if((dir + snake->dir) % 2 == 0)
		// 蛇直行
		// 蛇头所在节长度加 1
		list_tail(&snake->body)->len++;

	if(snake_eat(snake) == 0) {
		snake->len++;
		return 0;
	}

	// 蛇未吃到食物
	// 蛇尾所在的节长度减 1, 表现为蛇整体前移一格
	list_head(&snake->body)->len--;
	if(list_head(&snake->body)->len <= 0)
		// 蛇尾所在的节长度为 0, 删除它
		list_pop_front(&snake->body);

	return 0;
}
