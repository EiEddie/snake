#include <display/tui.h>


void display_buf_refresh() {
	_dbuf.begin.x =
	    (int)(COLS - _dtgt.field->wight * 2 - 2) / 2;
	_dbuf.begin.y =
	    (int)(LINES - _dtgt.field->height - 3) / 2;

	struct val_t hsec = *list_tail(&_dtgt.snake->body);
	_dbuf.snk_head = _dbuf.snk_head_prev =
	    pnt_move(hsec.pos, hsec.dir, (int)(hsec.len - 1));

	_dbuf.snk_tail_prev =
	    list_head(&_dtgt.snake->body)->pos;

	_dbuf.food = _dbuf.food_prev = _dtgt.field->food;

	_dbuf.snk_len = _dtgt.snake->len;
}

int display_init(struct field_t* field,
                 struct snake_t* snake) {
	int err = 0;

	_dtgt.field = field;
	_dtgt.snake = snake;
	display_buf_refresh();

	setlocale(LC_ALL, "");

	// 初始化
	initscr();

	// 禁止字符回显
	if((err = noecho()))
		return err;

	// 光标不可见
	if((err = curs_set(0)))
		return err;

	return 0;
}

int display_free(void) {
	_dtgt.field = NULL;
	_dtgt.snake = NULL;
	return endwin();
}

int display_test(size_t width, size_t height) {
	if(width * 2 + 2 > COLS)
		return -1;
	else if(height + 2 + 1 > LINES)
		return -2;
	else
		return 0;
}

void display_flush(void) {
	display_buf_refresh();

	// 打印边框
	int x = _dbuf.begin.x;
	int y = _dbuf.begin.y;

	move(y, x);
	printw(ICON_WALL_DOWN_RIGHT);
	for(int _ = 0; _ < _dtgt.field->wight * 2; _++)
		printw(ICON_WALL_HOR);
	printw(ICON_WALL_DOWN_LEFT);
	y++;

	for(int i = 0; i < _dtgt.field->height; i++) {
		move(y, x);
		printw(ICON_WALL_VER);
		for(int j = 0; j < _dtgt.field->wight * 2; j++)
			printw(" ");
		printw(ICON_WALL_VER);
		y++;
	}

	move(y, x);
	printw(ICON_WALL_UP_RIGHT);
	for(int _ = 0; _ < _dtgt.field->wight * 2; _++)
		printw(ICON_WALL_HOR);
	printw(ICON_WALL_UP_LEFT);
	y++;

	// 显示得分
	move(y, x);
	printw("len: %zu", _dtgt.snake->len);
	y++;

	// 绘制蛇身
	struct iter_t iter;
	pos_t tmp = {};
	iter_init(&iter, &_dtgt.snake->body);
	do {
		struct val_t val = iter_val(&iter);
		for(size_t i = 0; i < val.len; i++) {
			tmp = pnt_move(val.pos, val.dir, (int)i);
			mvprintw(tmp.y + _dbuf.begin.y + 1,
			         tmp.x * 2 + _dbuf.begin.x + 1,
			         ICON_SNAKE_BODY);
		}
	} while(!iter_next(&iter));
	mvprintw(tmp.y + _dbuf.begin.y + 1,
	         tmp.x * 2 + _dbuf.begin.x + 1,
	         ICON_SNAKE_HEAD);

	// 绘制食物
	mvprintw(_dtgt.field->food.y + _dbuf.begin.y + 1,
	         _dtgt.field->food.x * 2 + _dbuf.begin.x + 1,
	         ICON_FOOD);

	// 刷新屏幕
	refresh();
}

void display_update(void) {
#ifdef _DEBUG_
	mvprintw(0, 0,
	         "len:%zu,dir:%d,"
	         "fdist:%d,cdist:%d,"
	         "food:(%d,%d),"
	         "tail:(%d,%d),\n",
	         _dtgt.snake->len, _dtgt.snake->dir,
	         _dtgt.snake->food_dist,
	         _dtgt.snake->collision_dist,
	         _dtgt.field->food.x, _dtgt.field->food.y,
	         list_head(&_dtgt.snake->body)->pos.x,
	         list_head(&_dtgt.snake->body)->pos.y);
#endif //_DEBUG_

	// 食物
	if(_dbuf.food.x != _dtgt.field->food.x
	   || _dbuf.food.y != _dtgt.field->food.y) {
		_dbuf.food_prev = _dbuf.food;
		_dbuf.food = _dtgt.field->food;
		mvprintw(_dbuf.food_prev.y + _dbuf.begin.y + 1,
		         _dbuf.food_prev.x * 2 + _dbuf.begin.x + 1,
		         "  ");
		mvprintw(_dbuf.food.y + _dbuf.begin.y + 1,
		         _dbuf.food.x * 2 + _dbuf.begin.x + 1,
		         ICON_FOOD);
	}

	// 蛇头
	struct val_t hsec = *list_tail(&_dtgt.snake->body);
	_dbuf.snk_head =
	    pnt_move(hsec.pos, hsec.dir, (int)(hsec.len - 1));
	mvprintw(_dbuf.snk_head_prev.y + _dbuf.begin.y + 1,
	         _dbuf.snk_head_prev.x * 2 + _dbuf.begin.x + 1,
	         ICON_SNAKE_BODY);
	mvprintw(_dbuf.snk_head.y + _dbuf.begin.y + 1,
	         _dbuf.snk_head.x * 2 + _dbuf.begin.x + 1,
	         ICON_SNAKE_HEAD);
	_dbuf.snk_head_prev = _dbuf.snk_head;

	// 蛇尾
	if(_dbuf.snk_tail_prev.x
	       != list_head(&_dtgt.snake->body)->pos.x
	   || _dbuf.snk_tail_prev.y
	       != list_head(&_dtgt.snake->body)->pos.y) {
		if(_dbuf.snk_tail_prev.x != _dbuf.snk_head.x
		   || _dbuf.snk_tail_prev.y != _dbuf.snk_head.y)
			mvprintw(
			    _dbuf.snk_tail_prev.y + _dbuf.begin.y + 1,
			    _dbuf.snk_tail_prev.x * 2 + _dbuf.begin.x
			        + 1,
			    "  ");
		_dbuf.snk_tail_prev =
		    list_head(&_dtgt.snake->body)->pos;
	}

	// 分数
	if(_dbuf.snk_len != _dtgt.snake->len)
		mvprintw(_dbuf.begin.y + 2 + _dtgt.field->height,
		         _dbuf.begin.x + strlen("len: "), "%zu",
		         _dtgt.snake->len);

	refresh();
}
