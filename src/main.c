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

	while(1) {
		int tmp = 0;
		while(tmp != ERR) {
			tmp = getch();
			switch(tmp) {
			case KEY_LEFT:
			case 'a':
			case 'h':
				// LEFT
				snake_move(&snake, 2);
				break;
			case KEY_RIGHT:
			case 'd':
			case 'l':
				// RIGHT
				snake_move(&snake, 0);
				break;
			case KEY_DOWN:
			case 's':
			case 'j':
				// DOWN
				snake_move(&snake, 1);
				break;
			case KEY_UP:
			case 'w':
			case 'k':
				// UP
				snake_move(&snake, 3);
				break;
			case 'q':
			case '\003' /* ETX: ^C */:
				// Quit
				goto EXIT;
				break;
			case '\032' /* SUB: ^Z */:
				raise(SIGTSTP);
				break;
			default:
				break;
			}
		}

		display_flush();

		usleep(50 * 1000);
	}

EXIT:;
	display_free();
	ctrl_free();
	return 0;
}
