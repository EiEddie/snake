#include <gtest/gtest.h>

extern "C" {
#include <core/core.h>
}


TEST(core, gen_food_outer) {
	field_t field{};
	field_init(&field, 10, 10);
	snake_t snake{};
	// 手动生成
	{
		snake.field = &field;
		snake.collision_dist = INT_MAX;
		snake.food_dist = INT_MAX;
		snake.food_cnt = 0;
		list_init(&snake.body);

		pos_t pos_3rd = {3, 2};
		struct val_t third = {pos_3rd, 1, 3};
		list_push_back(&snake.body, third);

		pos_t pos_2nd = {4, 4};
		struct val_t second = {pos_2nd, 0, 2};
		list_push_back(&snake.body, second);

		pos_t pos_1st = {5, 5};
		struct val_t first = {pos_1st, 1, 2};
		list_push_back(&snake.body, first);

		snake.dir = list_tail(&snake.body)->dir;
		snake.len = snake_len(&snake);
	}


	int nums[] = {0, 19, 23, 36, 51, 55, 84};
	set_num(nums, 7);
	pos_t poses[] = {{0, 0}, {9, 1}, {6, 2}, {2, 4},
	                 {6, 6}, {0, 7}, {9, 9}};

	for(int i = 0; i < 7; i++) {
		(void)gen_food(&field, &snake);
		EXPECT_EQ(field.food.x, poses[i].x)
		    << "when i is: " << i << '\n';
		EXPECT_EQ(field.food.y, poses[i].y)
		    << "when i is: " << i << '\n';
	}

	snake_free(&snake);
}


TEST(core, gen_food_inner) {
	field_t field{};
	field_init(&field, 3, 5);
	snake_t snake{};
	// 手动生成
	{
		snake.field = &field;
		snake.collision_dist = INT_MAX;
		snake.food_dist = INT_MAX;
		snake.food_cnt = 0;
		list_init(&snake.body);

		pos_t pos_3rd = {0, 0};
		struct val_t third = {pos_3rd, 1, 3};
		list_push_back(&snake.body, third);

		pos_t pos_2nd = {1, 2};
		struct val_t second = {pos_2nd, 0, 2};
		list_push_back(&snake.body, second);

		pos_t pos_1st = {2, 3};
		struct val_t first = {pos_1st, 1, 2};
		list_push_back(&snake.body, first);

		snake.dir = list_tail(&snake.body)->dir;
		snake.len = snake_len(&snake);
	}

	int nums[] = {0, 1, 2, 3, 4, 5, 6, 7};
	set_num(nums, 8);
	pos_t poses[] = {{1, 0}, {2, 0}, {1, 1}, {2, 1},
	                 {0, 3}, {1, 3}, {0, 4}, {1, 4}};

	for(int i = 0; i < 7; i++) {
		(void)gen_food(&field, &snake);
		EXPECT_EQ(field.food.x, poses[i].x)
		    << "when i is: " << i << '\n';
		EXPECT_EQ(field.food.y, poses[i].y)
		    << "when i is: " << i << '\n';
	}

	snake_free(&snake);
}
