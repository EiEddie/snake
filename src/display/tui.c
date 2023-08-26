#include <display/tui.h>


int display_init(struct field_t* field,
                 struct snake_t* snake) {
	int err = 0;

	_dtgt.field = field;
	_dtgt.snake = snake;

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
	//	clear();

	int x_bg = (int)(COLS - _dtgt.field->wight * 2 - 2) / 2;
	int y_bg = (int)(LINES - _dtgt.field->height - 3) / 2;

	// 打印边框
	int x = x_bg;
	int y = y_bg;

	move(y, x);
	printw("┌");
	for(int _ = 0; _ < _dtgt.field->wight * 2; _++)
		printw("─");
	printw("┐");
	y++;

	for(int i = 0; i < _dtgt.field->height; i++) {
		move(y, x);
		printw("│");
		for(int j = 0; j < _dtgt.field->wight * 2; j++)
			printw(" ");
		printw("│");
		y++;
	}

	move(y, x);
	printw("└");
	for(int _ = 0; _ < _dtgt.field->wight * 2; _++)
		printw("─");
	printw("┘");
	y++;

	// 显示得分
	move(y, x);
	//	printw("len: %zu", _dtgt.snake->len);
	pos_t snake_pos = pnt_move(
	    (*list_tail(&_dtgt.snake->body)).pos,
	    (*list_tail(&_dtgt.snake->body)).dir,
	    (int)((*list_tail(&_dtgt.snake->body)).len - 1));
	printw(
	    "len: %zu, fd: %d, cd: %d, cod: (%d,%d), fcod: (%d,%d)              ",
	    _dtgt.snake->len, _dtgt.snake->food_dist,
	    _dtgt.snake->collision_dist, snake_pos.x,
	    snake_pos.y, _dtgt.field->food.x,
	    _dtgt.field->food.y);
	y++;

	// 绘制蛇身
	struct iter_t iter;
	iter_init(&iter, &_dtgt.snake->body);
	do {
		struct val_t val = iter_val(&iter);
		for(size_t i = 0; i < val.len; i++) {
			pos_t tmp = pnt_move(val.pos, val.dir, (int)i);
			mvprintw(tmp.y + y_bg + 1, tmp.x * 2 + x_bg + 1,
			         "🟩");
		}
	} while(!iter_next(&iter));

	// 绘制食物
	mvprintw(_dtgt.field->food.y + y_bg + 1,
	         _dtgt.field->food.x * 2 + x_bg + 1, "💖");

	// 刷新屏幕
	refresh();
}
