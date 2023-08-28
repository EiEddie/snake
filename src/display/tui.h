#ifndef SNAKE_SRC_DISPLAY_TUI_H_
#define SNAKE_SRC_DISPLAY_TUI_H_

#include <ncurses.h>
#include <locale.h>
#include <string.h>

#include <core/core.h>
#include <display/icon.h>

static struct display_target_t {
	struct field_t* field;
	struct snake_t* snake;
} _dtgt = {NULL, NULL};

static struct {
	pos_t begin;

	pos_t snk_head;
	pos_t snk_head_prev;

	pos_t snk_tail_prev;

	pos_t food;
	pos_t food_prev;

	size_t snk_len;
} _dbuf;

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

void display_update(void);

#endif //SNAKE_SRC_DISPLAY_TUI_H_
