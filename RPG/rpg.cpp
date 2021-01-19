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

//===============================================
//マップ配列を扱う時、x,y座標が逆になることに注意
//===============================================


void disp_local() {
	system("cls");
	for (int y = 0; y < FIELD_HIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (x == cursorX && y == cursorY) {
				printf("＠");
			}
			else if (map_local[y][x] == 0) {
				printf("・");
			}
			else if (map_local[y][x] == 1) {
				printf("Λ");
			}
		}
		printf("\n");
	}
}

bool move_local(int x, int y) {
	if (x<0 || x>FIELD_WIDTH - 1 || y<0 || y>FIELD_HIGHT - 1) {     //マップの外にはいけない
		return false;
	}
	if (map_local[y][x] == 1) {										//山には行けない
		return false;
	}
	else
		return true;
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
					case 'w':if (move_local(cursorX, cursorY - 1) == true) { cursorY--; }; break;
					case 'a':if (move_local(cursorX - 1, cursorY) == true) { cursorX--; }; break;
					case 's':if (move_local(cursorX, cursorY + 1) == true) { cursorY++; }; break;
					case 'd':if (move_local(cursorX + 1, cursorY) == true) { cursorX++; }; break;
					}
					disp_local();
				}
			}
		}
	}
}
