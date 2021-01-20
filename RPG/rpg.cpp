#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "field_map.h"

#define FIELD_WIDTH 60
#define FIELD_HIGHT 29


enum class MODE { GAME, MODE, SAVE, LOAD }; MODE mode;
enum class MAP { LOCAL, EAST1 }; MAP map;
enum class BUILD_LOCAL { NON, HOME, CAVE1, TOWER }; BUILD_LOCAL build_local;
enum class CHARA { ENEMY1, ENEMY2, ENEMY3 }; CHARA chara;

int cursorX = 30;
int cursorY = 15;

//===============================================
//�}�b�v�z����������Ax,y���W���t�ɂȂ邱�Ƃɒ���
//===============================================

//================================================================================================================
//================================================================================================================
//���[�J���}�b�v


//�}�b�v
void disp_local_non() {
	system("cls");
	for (int y = 0; y < FIELD_HIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (x == cursorX && y == cursorY) {
				printf("��");
			}
			else if (map_local_non[y][x] == 0) {
				printf("  ");
			}
			else if (map_local_non[y][x] == 1) {
				printf("��");
			}
			else if (map_local_non[y][x] == 2) {
				printf("��");
			}
			else if (map_local_non[y][x] == 3) {
				printf(" |");
			}
			else if (map_local_non[y][x] == 4) {
				printf("| ");
			}
			else if (map_local_non[y][x] == 5) {
				printf("__");
			}
		}
		printf("\n");
	}
}
bool move_local_non(int x, int y) {
	if (x<0 || x>FIELD_WIDTH - 1 || y<0 || y>FIELD_HIGHT - 1) {     //�}�b�v�̊O�ɂ͂����Ȃ�
		return false;
	}
	else if (x == 30 && y == 14) {									//home�֓���
		build_local = BUILD_LOCAL::HOME;
		cursorX = 29;												//home�̏����ʒu
		cursorY = 27;
		return false;
	}
	else if (map_local_non[y][x] == 1 || map_local_non[y][x] == 3 || map_local_non[y][x] == 4 || map_local_non[y][x] == 5) {		//�R�A��1�A��2�ɂ͍s���Ȃ�
		return false;
	}
	else
		return true;
}
//=============================================================================================================
//�z�[��
void disp_local_home() {
	system("cls");
	for (int y = 0; y < FIELD_HIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (x == cursorX && y == cursorY) {
				printf("��");
			}
			else if (map_local_home[y][x] == 0) {
				printf("  ");
			}
			else if (map_local_home[y][x] == 1) {
				printf("��");
			}
		}
		printf("\n");
	}
}

bool move_local_home(int x, int y) {
	if (x<0 || x>FIELD_WIDTH - 1 || y<0 || y>FIELD_HIGHT - 1) {     //�}�b�v�̊O�ɂ͂����Ȃ�
		return false;
	}
	else if ((x == 28 || x==29 || x==30 ) && y == 28) {				//���̊O�֏o��
		build_local = BUILD_LOCAL::NON;
		cursorX = 30;
		cursorY = 15;
		return false;
	}
	else if (map_local_home[y][x] == 1) {								//�ǂ̊O�ɂ͍s���Ȃ�
		return false;
	}
	else
		return true;
}



//================================================================================================================





int main() {
	while (1){
		if (mode == MODE::GAME) {
			time_t replot = time(NULL) - 5;
			while (1) {
				if (replot  + 5 < time(NULL)) {
					replot = time(NULL);
					if (map == MAP::LOCAL) {
						if (build_local == BUILD_LOCAL::NON) {
							disp_local_non();
						}
						else if (build_local == BUILD_LOCAL::HOME) {
							disp_local_home();
						}
					}
				}

				if (_kbhit()) {
					if (map == MAP::LOCAL) {
						if (build_local == BUILD_LOCAL::NON) {
							switch (_getch()) {
							case 'w':if (move_local_non(cursorX, cursorY - 1) == true) { cursorY--; }; break;
							case 'a':if (move_local_non(cursorX - 1, cursorY) == true) { cursorX--; }; break;
							case 's':if (move_local_non(cursorX, cursorY + 1) == true) { cursorY++; }; break;
							case 'd':if (move_local_non(cursorX + 1, cursorY) == true) { cursorX++; }; break;
							}
						}
						else if (build_local == BUILD_LOCAL::HOME) {
							switch (_getch()) {
							case 'w':if (move_local_home(cursorX, cursorY - 1) == true) { cursorY--; }; break;
							case 'a':if (move_local_home(cursorX - 1, cursorY) == true) { cursorX--; }; break;
							case 's':if (move_local_home(cursorX, cursorY + 1) == true) { cursorY++; }; break;
							case 'd':if (move_local_home(cursorX + 1, cursorY) == true) { cursorX++; }; break;
							}
						}
					}





					//�L�[���͂��������瑦���ɍĕ`��
					if (map == MAP::LOCAL) {
						if (build_local == BUILD_LOCAL::NON) {
							disp_local_non();
						}
						else if (build_local == BUILD_LOCAL::HOME) {
							disp_local_home();
						}
					}
				}
			}
		}
	}
}
