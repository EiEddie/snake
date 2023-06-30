#include <core/body.h>


pos_t pnt_move(pos_t pnt, int dir, int step) {
	dir = (dir + 4) % 4;

	pos_t ans = pnt;
	if(dir & 1)
		ans.y -= ((dir & 0b10) - 1) * step;
	else
		ans.x -= ((dir & 0b10) - 1) * step;

	return ans;
}


void snake_init(struct snake_t *snake, size_t wight,
                size_t height) {
	srand(time(NULL));
	pos_t begin_pnt = {rand() % wight, rand() % height};

	// 使蛇向中心方向走, 避免开局撞墙
	pos_t offset = {(int)wight / 2 - begin_pnt.x,
	                (int)height / 2 - begin_pnt.y};
	int dir = abs(offset.x) < abs(offset.y);
	if(dir)
		dir |= (offset.y < 0) << 1;
	else
		dir |= (offset.x < 0) << 1;

	snake->wight = wight;
	snake->height = height;
	snake->dist =
	    // 此类型的最大值
	    (1 << (8 * sizeof(snake->dist) - 1)) - 1;
	snake->len = 1;
	snake->dir = dir;
	list_init(&snake->body);
	struct val_t val = {begin_pnt, dir, 1};
	// 蛇头储存在链表的尾部
	list_push_back(&snake->body, val);

	snake_set_dist(snake);
}

void snake_free(struct snake_t *snake) {
	snake->len = 0;
	list_free(&snake->body);
}

void snake_set_dist(struct snake_t *snake) {
	struct iter_t iter;
	iter_init(&iter, &snake->body);
	snake->dist =
	    // 此类型的最大值
	    (1 << (8 * sizeof(snake->dist) - 1)) - 1;

	// 与自身的碰撞检测
	pos_t snake_pos = list_tail(&snake->body).pos;
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
			snake->dist =
			    min(snake->dist,
			        abs(section.pos.y - snake_pos.y));
		else
			// 左右移动
			snake->dist =
			    min(snake->dist,
			        abs(section.pos.x - snake_pos.x));
	} while(!iter_next(&iter));

	// 与墙壁的碰撞检测
	switch(snake->dir) {
	case 0:
		snake->dist =
		    min(snake->dist, snake->wight - snake_pos.x);
		break;
	case 1:
		snake->dist =
		    min(snake->dist, snake->height - snake_pos.y);
		break;
	case 2:
		snake->dist = min(snake->dist, snake_pos.x + 1);
		break;
	case 3:
		snake->dist = min(snake->dist, snake_pos.y + 1);
		break;
	}
}
