#include <core/snake.h>

#ifdef _DEBUG_
#include <dbg/debug.h>
#endif //_DEBUG_


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


/**
 * \param w 外部宽度
 * \param h 外部高度
 */
pos_t gen_food_outer(int w, int h, pos_t near, pos_t far) {
	pos_t ans = {};

	/** \brief 内部格子总数 */
	int inner_cnt =
	    (far.x - near.x + 1) * (far.y - near.y + 1);
	/** \brief 外部格子总数 */
	int outer_cnt = w * h - inner_cnt;

	int index = rand() % outer_cnt;
	if(index < near.x + near.y * w) {
		ans.x = index % w;
		ans.y = index / w;
	} else if(index > far.x + far.y * w - inner_cnt) {
		ans.x = (index + inner_cnt) % w;
		ans.y = (index + inner_cnt) / w;
	} else {
		// FIXME: O(1) 的算法实现
		for(int i = 0; i < far.y - near.y; i++) {
			int tmp =
			    index + (i + 1) * (far.x - near.x + 1);
			if(!((near.y + i) * w + far.x < tmp
			     && tmp < (near.y + i + 1) * w + near.x))
				continue;
			ans.x = tmp % w;
			ans.y = tmp / w;
		}
	}

	return ans;
}

pos_t gen_food_inner(pos_t near, pos_t far,
                     struct snake_t *snake) {
	pos_t ans = {};
	/** \brief 内部的宽高 */
	int w = far.x - near.x + 1, h = far.y - near.y + 1;
	int *stack = calloc(w * h, sizeof(int));

	// 在栈上标注所有蛇身占用的位置
	struct iter_t iter;
	iter_init(&iter, &snake->body);
	do {
		struct val_t val = iter_val(&iter);
		for(size_t i = 0; i < val.len; i++) {
			pos_t tmp = pnt_move(val.pos, val.dir, (int)i);
			stack[tmp.y * w + tmp.x] = 1;
		}
	} while(!iter_next(&iter));

	// 随机生成食物在内部且未被蛇身占用位置的序号
	int index = (int)(rand() % (w * h - snake->len));

	// 将上述序号映射到内部所有位置的序号
	for(int i = 0; i < w * h; i++) {
		if(stack[i] == 1)
			continue;
		if(index == 0) {
			ans.x = i % w;
			ans.y = i / w;
		}
		index--;
	}

	// 获得食物在整个网格, 即外部的位置
	ans.x = ans.x + near.x;
	ans.y = ans.y + near.y;

	free(stack);
	return ans;
}

/**
 * \brief 生成食物
 *
 * \warning 使用前需设定随机数种子
 * \retval -1 生成失败: 没有空间
 */
int gen_food(struct field_t *field, struct snake_t *snake) {
	// 检测蛇身范围, 即内部
	pos_t near = list_tail(&snake->body)->pos;
	pos_t far = near;

	struct iter_t iter;
	iter_init(&iter, &snake->body);
	do {
		pos_t tmp = iter_val(&iter).pos;
		near.x = min(near.x, tmp.x);
		near.y = min(near.y, tmp.y);
		far.x = max(far.x, tmp.x);
		far.y = max(far.y, tmp.y);
	} while(!iter_next(&iter));

	{
		/** \brief snake_head */
		struct val_t shead = *list_tail(&snake->body);
		pos_t tmp = pnt_move(shead.pos, shead.dir,
		                     (int)(shead.len - 1));
		near.x = min(near.x, tmp.x);
		near.y = min(near.y, tmp.y);
		far.x = max(far.x, tmp.x);
		far.y = max(far.y, tmp.y);
	}

	// 计数
	/** \brief 外部宽高 */
	int w = (int)field->wight, h = (int)field->wight;
	/** \brief 内部格子总数 */
	int inner_cnt =
	    (far.x - near.x + 1) * (far.y - near.y + 1);
	/** \brief 外部格子总数 */
	int outer_cnt = w * h - inner_cnt;

	if(outer_cnt <= 0) {
		if(inner_cnt <= snake->len) {
			// 空间不足, 退出
			return -1;
		} else {
			// 尝试在内部生成
			field->food = gen_food_inner(near, far, snake);
		}
	} else {
		// 尝试在外部生成
		field->food = gen_food_outer(w, h, near, far);
	}

	return 0;
}


void snake_set_collision_dist(struct snake_t *snake) {
	struct iter_t iter;
	iter_init(&iter, &snake->body);
	snake->collision_dist = INT_MAX;

	// 与自身的碰撞检测
	pos_t snake_pos =
	    pnt_move((*list_tail(&snake->body)).pos,
	             (*list_tail(&snake->body)).dir,
	             (int)((*list_tail(&snake->body)).len - 1));
	int snk_x = snake_pos.x;
	int snk_y = snake_pos.y;

	do {
		struct val_t sec = iter_val(&iter);
		int sec_x = sec.pos.x;
		int sec_y = sec.pos.y;

		// 若此节与蛇头不垂直
		if(!((sec.dir + snake->dir) % 2))
			continue;

		// 若此节不在蛇移动方向上
		// 且此节横或纵坐标与蛇头不重合
		if(snake->dir & 1) {
			// 上下移动
			if(((sec_y < snk_y) ^ !!(snake->dir & 0b10))
			   && (sec_y != snk_y))
				continue;
		} else {
			// 左右移动
			if(((sec_x < snk_x) ^ !!(snake->dir & 0b10))
			   && (sec_x != snk_x))
				continue;
		}

		pos_t sec_bg =
		    iter_ptr(&iter) == list_head(&snake->body)
		    ? sec.pos
		    : pnt_move(sec.pos, sec.dir + 2, 1);
		pos_t sec_ed =
		    pnt_move(sec.pos, sec.dir, (int)sec.len - 1);

		// 若此节不在蛇头正前方
		if(snake->dir & 1) {
			// 上下移动
			if(min(sec_bg.x, sec_ed.x) > snk_x
			   || max(sec_bg.x, sec_ed.x) < snk_x)
				continue;
		} else {
			// 左右移动
			if(min(sec_bg.y, sec_ed.y) > snk_y
			   || max(sec_bg.y, sec_ed.y) < snk_y)
				continue;
		}

		if(snake->dir & 1)
			// 上下移动
			snake->collision_dist = min(
			    snake->collision_dist, abs(sec_y - snk_y));
		else
			// 左右移动
			snake->collision_dist = min(
			    snake->collision_dist, abs(sec_x - snk_x));
	} while(!iter_next(&iter));

	// 与墙壁的碰撞检测
	int h = (int)snake->field->height;
	int w = (int)snake->field->wight;

	switch(snake->dir) {
	case 0:
		snake->collision_dist =
		    min(snake->collision_dist, w - snk_x);
		break;
	case 1:
		snake->collision_dist =
		    min(snake->collision_dist, h - snk_y);
		break;
	case 2:
		snake->collision_dist =
		    min(snake->collision_dist, snk_x + 1);
		break;
	case 3:
		snake->collision_dist =
		    min(snake->collision_dist, snk_y + 1);
		break;
	}
}

void snake_set_food_dist(struct snake_t *snake) {
	snake->food_dist = INT_MAX;
	pos_t snake_pos =
	    pnt_move((*list_tail(&snake->body)).pos,
	             (*list_tail(&snake->body)).dir,
	             (int)((*list_tail(&snake->body)).len - 1));
	int snk_x = snake_pos.x;
	int snk_y = snake_pos.y;
	int food_x = snake->field->food.x;
	int food_y = snake->field->food.y;

	// 若食物不在蛇移动方向上
	// 且此节横或纵坐标与蛇头不重合
	if(snake->dir & 1) {
		// 上下移动
		if(((food_y < snk_y) ^ !!(snake->dir & 0b10))
		   && (food_y != snk_y))
			return;
	} else {
		// 左右移动
		if(((food_x < snk_x) ^ !!(snake->dir & 0b10))
		   && (food_x != snk_x))
			return;
	}

	// 若食物不在蛇头正前方
	if(snake->dir & 1 ? food_x != snk_x : food_y != snk_y)
		return;

	if(snake->dir & 1)
		// 上下移动
		snake->food_dist = abs(food_y - snk_y);
	else
		// 左右移动
		snake->food_dist = abs(food_x - snk_x);
}

void snake_init(struct snake_t *snake,
                struct field_t *field) {
	pos_t begin_pnt = {(int)(rand() % field->wight),
	                   (int)(rand() % field->height)};

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

	gen_food(snake->field, snake);
	snake_set_collision_dist(snake);
	snake_set_food_dist(snake);
}

void snake_free(struct snake_t *snake) {
	snake->len = 0;
	list_free(&snake->body);
}

size_t snake_len(struct snake_t *snake) {
	size_t len = 0;
	struct iter_t iter;
	iter_init(&iter, &snake->body);
	do {
		len += iter_val(&iter).len;
	} while(!iter_next(&iter));
	return len;
}

int snake_eat(struct snake_t *snake) {
	if(snake->food_dist <= 0) {
		snake->food_cnt++;
		gen_food(snake->field, snake);
		snake_set_food_dist(snake);
		return 0;
	} else {
		return -1;
	}
}

int snake_move(struct snake_t *snake, int dir) {
	int snake_dir = snake->dir;

	if((dir + snake_dir) % 2 != 0) {
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

		snake->dir = dir;

		snake_set_food_dist(snake);
		snake_set_collision_dist(snake);
	}

	if((dir + snake_dir) % 2 == 0) {
		// 蛇直行

		// 更新距离
		snake->food_dist--;
		snake->collision_dist--;

		// 蛇头所在节长度加 1
		list_tail(&snake->body)->len++;
	}

	// 撞到障碍
	if(snake->collision_dist <= 0)
		return -1;

	if(snake_eat(snake) == 0) {
		snake->len++;
		return 0;
	}

	// 蛇未吃到食物
	// 蛇尾所在的节长度减 1, 表现为蛇整体前移一格
	{
		/** \brief snake_tail 的指针 */
		struct val_t *stail_ptr = list_head(&snake->body);
		stail_ptr->len--;
		if(stail_ptr->len <= 0)
			// 蛇尾所在的节长度为 0
			// 删除它
			list_pop_front(&snake->body);
		else
			// 蛇尾位置移动一格
			stail_ptr->pos =
			    pnt_move(stail_ptr->pos, stail_ptr->dir, 1);
	}

	return 0;
}
