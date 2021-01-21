#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "field_map.h"
#include "build.h"




using namespace name_build;
using namespace enemy_disp;
BUILD build;
ENEMY enemy;

#define FIELD_WIDTH 60
#define FIELD_HIGHT 29


enum class MODE { GAME, MODE, SAVE, LOAD }; MODE mode;
enum class MAP { LOCAL, EAST1 }; MAP map;
enum class BUILD_LOCAL { NON, HOME, CAVE1, TOWER }; BUILD_LOCAL build_local;
enum class CHARA { ENEMY1, ENEMY2, ENEMY3 }; CHARA chara;

int cursorX = 30;
int cursorY = 15;

int count = 0;


/*新しい建物を追加時に行う変更点
disp関数(forで描写)を用意し、それに用いる配列マップを準備
建物内での当たり判定関数moveを追加
main関数内	5秒ごとの再描画設定
			キー入力分岐に追加
			キー入力後の即座に再描画を設定
建物に入る前のフィールドでその建物の当たり判定、初期座標の設定
*/


//===============================================
//マップ配列を扱う時、x,y座標が逆になることに注意
//===============================================
	


//================================================================================================================
//================================================================================================================
//ローカルマップ


//マップ
void disp_local_non() {
	system("cls");
	for (int y = 0; y < FIELD_HIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (x == cursorX && y == cursorY) {
				printf("＠");
			}
			else if (map_local_non[y][x] == 0) {
				printf("  ");
			}
			else if (map_local_non[y][x] == 1) {
				printf("Λ");
			}
			else if (map_local_non[y][x] == 2) {
				printf("町");
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
			else if (map_local_non[y][x] == 6) {
				printf("##");
			}
		}
		printf("\n");
	}
}
bool move_local_non(int x, int y) {
	if (x<0 || x>FIELD_WIDTH - 1 || y<0 || y>FIELD_HIGHT - 1) {     //マップの外にはいけない
		return false;
	}
	else if (x == 30 && y == 14) {									//homeへ入る
		build_local = BUILD_LOCAL::HOME;
		cursorX = 29;												//homeの初期位置
		cursorY = 27;
		return false;
	}
	else if (x == 5 && y == 25) {									//塔へ入る
		build_local = BUILD_LOCAL::TOWER;
		cursorX = 1;
		cursorY = 27;
		return false;
	}
	else if (map_local_non[y][x] == 1 || map_local_non[y][x] == 3 || map_local_non[y][x] == 4 || map_local_non[y][x] == 5) {		//山、壁1、壁2には行けない
		return false;
	}
	else
		return true;
}
//=============================================================================================================
//ホーム
void disp_local_home() {
	system("cls");
	for (int y = 0; y < FIELD_HIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (x == cursorX && y == cursorY) {
				printf("＠");
			}
			else if (map_local_home[y][x] == 0) {
				printf("  ");
			}
			else if (map_local_home[y][x] == 1) {
				printf("■");
			}
			else if (map_local_home[y][x] == 2) {
				printf("||");
			}
			else if (map_local_home[y][x] == 3) {
				printf("я");
			}
			else if (map_local_home[y][x] == 4) {
				printf(" |");
			}
			else if (map_local_home[y][x] == 5) {
				printf("| ");
			}
			else if (map_local_home[y][x] == 6) {
				printf("__");
			}
			else if (map_local_home[y][x] == 7) {
				printf("Ψ");
			}
		}
		printf("\n");
	}
}

bool move_local_home(int x, int y) {
	if (x<0 || x>FIELD_WIDTH - 1 || y<0 || y>FIELD_HIGHT - 1) {     //マップの外にはいけない
		return false;
	}
	else if ((x == 28 || x==29 || x==30 ) && y == 28) {				//町の外へ出る
		build_local = BUILD_LOCAL::NON;
		cursorX = 30;
		cursorY = 15;
		return false;
	}
	else if ((x == 10 || x == 11 || x == 12) && y == 23) {
		build.rest();
	}
	else if ((x == 6 || x == 7 || x == 8) && y == 10) {
		build.weapon();
	}
	else if (map_local_home[y][x] == 1 || map_local_home[y][x] == 3 || map_local_home[y][x] == 4 || map_local_home[y][x] == 5) {	//壁には行けない
		return false;
	}
	else
		return true;
}

//==============================================================================================================
//タワー
void disp_local_tower() {
	system("cls");
	for (int y = 0; y < FIELD_HIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			if (x == cursorX && y == cursorY) {
				printf("＠");
			}
			else if (map_local_tower[y][x] == 0) {
				printf("  ");
			}
			else if (map_local_tower[y][x] == 1) {
				printf("■");
			}
			else if (map_local_tower[y][x] == 2) {
				printf("町");
			}
			else if (map_local_tower[y][x] == 3) {
				printf(" |");
			}
			else if (map_local_tower[y][x] == 4) {
				printf("| ");
			}
			else if (map_local_tower[y][x] == 5) {
				printf("__");
			}
			else if (map_local_tower[y][x] == 6) {
				printf("##");
			}
		}
		printf("\n");
	}
}

bool move_local_tower(int x, int y) {
	if (map_local_tower[y][x] != 1) {
		return true;
	}
}



//================================================================================================================
//メイン関数
//===========




int main() {
	while (1){
		if (mode == MODE::GAME) {
			time_t replot = time(NULL) - 5;
			int battle_count = 10 + rand() % 20;
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
						else if (build_local == BUILD_LOCAL::TOWER) {
							disp_local_tower();
						}
					}
				}

				if (_kbhit()) {
					if (map == MAP::LOCAL) {
						if (build_local == BUILD_LOCAL::NON) {
							switch (_getch()) {
							case 'w':if (move_local_non(cursorX, cursorY - 1) == true) { cursorY--; count++; }; break;
							case 'a':if (move_local_non(cursorX - 1, cursorY) == true) { cursorX--; count++; }; break;
							case 's':if (move_local_non(cursorX, cursorY + 1) == true) { cursorY++; count++; }; break;
							case 'd':if (move_local_non(cursorX + 1, cursorY) == true) { cursorX++; count++; }; break;
							}
						}
						else if (build_local == BUILD_LOCAL::HOME) {
							switch (_getch()) {
							case 'w':if (move_local_home(cursorX, cursorY - 1) == true) { cursorY--; count++; }; break;
							case 'a':if (move_local_home(cursorX - 1, cursorY) == true) { cursorX--; count++; }; break;
							case 's':if (move_local_home(cursorX, cursorY + 1) == true) { cursorY++; count++; }; break;
							case 'd':if (move_local_home(cursorX + 1, cursorY) == true) { cursorX++; count++; }; break;
							}
						}
						else if (build_local == BUILD_LOCAL::TOWER) {
							switch (_getch()) {
							case 'w':if (move_local_tower(cursorX, cursorY - 1) == true) { cursorY--; count++; }; break;
							case 'a':if (move_local_tower(cursorX - 1, cursorY) == true) { cursorX--; count++; }; break;
							case 's':if (move_local_tower(cursorX, cursorY + 1) == true) { cursorY++; count++; }; break;
							case 'd':if (move_local_tower(cursorX + 1, cursorY) == true) { cursorX++; count++; }; break;
							}
						}
					}

					//キー入力があったら即座に再描画
					if (map == MAP::LOCAL) {
						if (build_local == BUILD_LOCAL::NON) {
							disp_local_non();
						}
						else if (build_local == BUILD_LOCAL::HOME) {
							disp_local_home();
						}
						else if (build_local == BUILD_LOCAL::TOWER) {
							disp_local_tower();
						}
					}

					//バトル
					if (count > battle_count && build_local == BUILD_LOCAL::NON) {
						count = 0;
						battle_count = 10 + rand() % 20;
						enemy.section(rand() % 3);
					}
					else if (count > battle_count && build_local != BUILD_LOCAL::NON) {
						count = 0;
						battle_count = 10 + rand() % 20;
					}

				}
			}
		}
	}
}
