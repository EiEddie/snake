#ifndef SNAKE_SRC_DBG_DEBUG_H_
#define SNAKE_SRC_DBG_DEBUG_H_

#include <stddef.h>


#ifdef _TEST_
// 在测试时代替随机数
#define rand get_num
#endif //_TEST_

static struct {
	int* numv;
	size_t numc;
	size_t index;
} _num = {NULL, 0, 0};


/**
 * \brief 设定函数 `get_num` 的值
 *
 * 用于在测试时代替随机数
 *
 * \param numv 数组, 生成时将循环返回此数组的值
 * \param numc 数组个数
 */
void set_num(int numv[], size_t numc);

/**
 * \brief 在测试时代替随机数
 */
int get_num();


#endif //SNAKE_SRC_DBG_DEBUG_H_
