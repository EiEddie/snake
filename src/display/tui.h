#ifndef SNAKE_SRC_DISPLAY_TUI_H_
#define SNAKE_SRC_DISPLAY_TUI_H_

#include <ncurses.h>
#include <locale.h>

#include <core/core.h>


static struct display_target_t {
	struct field_t* field;
	struct snake_t* snake;
} _dtgt = {NULL, NULL};

int display_init(struct field_t* field,
                 struct snake_t* snake);

int display_free(void);

/**
 * \brief 测试当前终端大小能否正常显示
 * \retval -1 宽度不足
 * \retval -2 高度不足
 */
int display_test(size_t width, size_t height);

void display_flush(void);

#endif //SNAKE_SRC_DISPLAY_TUI_H_
