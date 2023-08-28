#include <core/core.h>
#include <ctrl/ctrl.h>
#include <display/display.h>

#include <signal.h>
#include <unistd.h>


int main() {
	srand(time(NULL));

	struct field_t field;
	field_init(&field, 16, 16);

	struct snake_t snake;
	snake_init(&snake, &field);

	display_init(&field, &snake);
	ctrl_init();

	display_flush();

	unsigned long long cnt = 0;
	int unit_msec = 300;
	int loop_msec = 50;

	int dir = -1;
	while(1) {
		int tmp = 0;
		while(tmp != ERR) {
			tmp = getch();
			switch(tmp) {
			case KEY_LEFT:
			case 'a':
			case 'h':
				// LEFT
				dir = 2;
				break;
			case KEY_RIGHT:
			case 'd':
			case 'l':
				// RIGHT
				dir = 0;
				break;
			case KEY_DOWN:
			case 's':
			case 'j':
				// DOWN
				dir = 1;
				break;
			case KEY_UP:
			case 'w':
			case 'k':
				// UP
				dir = 3;
				break;
			case 'q':
			case '\003' /* ETX: ^C */:
				// Quit
				goto EXIT;
				break;
			case '\032' /* SUB: ^Z */:
				// Suspend
				raise(SIGTSTP);
				break;
			case 'r':
				// Refresh
				display_flush();
				break;
			default:
				break;
			}
		}

		if(cnt % (unit_msec / loop_msec) == 0) {
			if(snake_move(&snake,
			              dir == -1 ? snake.dir : dir))
				goto EXIT;
			else
				dir = -1;
			display_update();
		}

		usleep(loop_msec * 1000);
		cnt++;
	}

EXIT:;
	display_free();
	ctrl_free();
	return 0;
}
