#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "field_map.h"

#define FIELD_WIDTH 60
#define FIELD_HIGHT 29


enum class MODE { GAME, MODE, SAVE, LOAD }; MODE mode;
enum class MAP { LOCAL, EAST1 }; MAP map;
enum class BUILD { HOME, CAVE1, TOWER }; BUILD build;
enum class CHARA { ENEMY1, ENEMY2, ENEMY3 }; CHARA chara;

int cursorX = 30;
int cursorY = 15;


void disp_local() {
	system("cls");
	for (int y = 0; y < FIELD_HIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (x == cursorX && y == cursorY) {
				printf("—");
			}
			else if (map_local[y][x] == 0) {
				printf("E");
			}
			else if (map_local[y][x] == 1) {
				printf("ƒ©");
			}
		}
		printf("\n");
	}
}



int main() {
	while (1){
		if (mode == MODE::GAME) {
			time_t replot = time(NULL) - 5;
			while (1) {
				if (replot  + 5 < time(NULL)) {
					replot = time(NULL);
					disp_local();
				}

				if (_kbhit()) {
					switch (_getch()) {
					case 'w':cursorY--; break;
					case 'a':cursorX--; break;
					case 's':cursorY++; break;
					case 'd':cursorX++; break;
					}
					disp_local();
				}
			}
		}
	}
}
