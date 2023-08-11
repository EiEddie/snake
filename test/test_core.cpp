#include <gtest/gtest.h>

extern "C" {
#include <core/core.h>
}


#define GEN_SECTION(pos_x, pos_y, dir, len)     \
	do {                                        \
		pos_t __pos = {pos_x, pos_y};           \
		struct val_t __val = {__pos, dir, len}; \
		list_push_back(&snake.body, __val);     \
	} while(0)


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

		GEN_SECTION(3, 2, 1, 3);
		GEN_SECTION(4, 4, 0, 2);
		GEN_SECTION(5, 5, 1, 2);

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

		GEN_SECTION(0, 0, 1, 3);
		GEN_SECTION(1, 2, 0, 2);
		GEN_SECTION(2, 3, 1, 2);

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


TEST(core, dist1) {
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

		GEN_SECTION(1, 4, 0, 3);

		snake.dir = list_tail(&snake.body)->dir;
		snake.len = snake_len(&snake);
	}

	field.food = {0, 0};
	snake_set_collision_dist(&snake);
	snake_set_food_dist(&snake);
	ASSERT_EQ(snake.collision_dist, 7);
	ASSERT_EQ(snake.food_dist, INT_MAX);

	field.food = {7, 4};
	snake_set_collision_dist(&snake);
	snake_set_food_dist(&snake);
	ASSERT_EQ(snake.collision_dist, 7);
	ASSERT_EQ(snake.food_dist, 4);

	snake_free(&snake);
}


TEST(core, dist2) {
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

		GEN_SECTION(7, 4, 2, 3);
		GEN_SECTION(5, 5, 1, 2);
		GEN_SECTION(4, 6, 2, 5);
		GEN_SECTION(0, 5, 3, 2);
		GEN_SECTION(1, 4, 0, 3);

		snake.dir = list_tail(&snake.body)->dir;
		snake.len = snake_len(&snake);
	}

	field.food = {9, 4};
	snake_set_collision_dist(&snake);
	snake_set_food_dist(&snake);
	ASSERT_EQ(snake.collision_dist, 2);
	ASSERT_EQ(snake.food_dist, 6);

	snake_free(&snake);
}
