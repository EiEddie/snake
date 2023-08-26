#include <ctrl/kbd.h>


int ctrl_init(void) {
	int err = 0;

	// 初始化
	initscr();

	// 禁用行缓冲
	if((err = raw()))
		return err;

	// 设定非阻塞读取: 阻塞时长为 0
	timeout(0);

	// 启用键盘功能键
	if((err = keypad(stdscr, TRUE)))
		return err;

	return 0;
}

int ctrl_free(void) {
	return endwin();
}
