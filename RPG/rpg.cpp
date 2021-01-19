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




int main() {
	while (1){
		if (mode == MODE::GAME) {
			while (1) {
				time_t replot = time(NULL);
				if (replot < time(NULL)) {
					replot = time(NULL);
					system("cls");
					for (int y = 0; y < FIELD_HIGHT; y++) {
						for (int x = 0; x < FIELD_WIDTH; x++) {
							if (map_local[y][x] == 0) {
								printf("E");
							}
							else if (map_local[y][x] == 1) {
								printf("ƒ©");
							}
						}
						printf("\n");
					}
				}
			}
		}
	}
}
