#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define MAX_bomb 25


int FIELD_WIDTH = 21;
int FIELD_HIGHT = 21;

bool selected;								//初期駒をセットしたか(true)していないか(false)
bool hit;

enum class MODE { MENU, GAMEOVER, GAME, GAMECREARE, STOP, RESET };
//enum class MODE { GAME, MENU, GAMEOVER, GAMECREARE, STOP };
enum class BOMB { SPECIAL, NORMAL, LONG_ };
enum class STATE { NON, WALL, GRASS, NORMAL_BOM, LONG_BOM, SPECIAL_BOM, ENEMY, SET };

MODE mode;
BOMB bom;									//爆弾の種類
STATE state[21][21];						//盤面の状態を格納する配列
STATE tmp[21][21];							//盤面の状態を一時記憶する配列

int cursorX = 1, cursorY = 1;				//カーソル(キャラクター)の位置

bool direction[16] = { false };

const char* alert = " 初期位置を選択してください[p]\n";

time_t display_span;
time_t enemy_span;

int numof_normal = 25;
int numof_special = 5;
int numof_long = 5;

void judg_bombhit(int x, int y);

/*********************************************************************************************/
//爆弾bombクラス

class bomb {
public:
	int number = 0;						//爆弾配列の要素
	int range = 0;						//爆発の範囲
	bool timer_set[MAX_bomb] = {};		//起爆タイマーをセットしたか否か
	bool set[MAX_bomb];					//爆弾を設置したか否か
	int  set_x[MAX_bomb] = {};			//起爆するx座標
	int  set_y[MAX_bomb] = {};			//起爆するy座標
	time_t timer[MAX_bomb] = {};		//爆弾設置時の時間
	STATE type = {};					//起爆する爆弾のタイプ
	void set_bomb(int num);						//設置から起爆、消滅までの関数
	void explode(int x, int y, int range);		//起爆
	void release(int x, int y);					//消滅
};

void bomb::explode(int x, int y, int range) {
	if (cursorX == x && cursorY == y) {
		hit = true;
	}
	for (int i = 1; state[x + i][y] != STATE::WALL && i <= range; i++) {
		judg_bombhit(x + i, y);
		state[x + i][y] = type;
	}
	for (int i = 1; state[x - i][y] != STATE::WALL && i <= range; i++) {
		judg_bombhit(x - i, y);
		state[x - i][y] = type;
	}
	for (int i = 1; state[x][y + i] != STATE::WALL && i <= range; i++) {
		judg_bombhit(x, y + i);
		state[x][y + i] = type;
	}
	for (int i = 1; state[x][y - i] != STATE::WALL && i <= range; i++) {
		judg_bombhit(x, y - i);
		state[x][y - i] = type;
	}
}

void bomb::release(int x, int y) {
	for (int i = 0; state[x + i][y] == type && i <= range; i++) {
		state[x + i][y] = STATE::NON;
	}
	for (int i = 1; state[x - i][y] == type && i <= range; i++) {
		state[x - i][y] = STATE::NON;
	}
	for (int i = 1; state[x][y + i] == type && i <= range; i++) {
		state[x][y + i] = STATE::NON;
	}
	for (int i = 1; state[x][y - i] == type && i <= range; i++) {
		state[x][y - i] = STATE::NON;
	}
}

void bomb::set_bomb(int num) {
	state[set_x[num]][set_y[num]] = type;
	if (timer_set[num] == false) {
		timer[num] = time(NULL);
		timer_set[num] = true;
	}
	if (timer[num] + 1 < time(NULL)) {
		state[set_x[num]][set_y[num]] = type;
		explode(set_x[num], set_y[num], range);
		//set[number] = false;
	}
	if (timer[num] + 2 < time(NULL) && type != STATE::LONG_BOM) {
		//release処理
		release(set_x[num], set_y[num]);
		set[num] = false;
	}
	else if (timer[num] + 3 < time(NULL) && type == STATE::LONG_BOM) {
		release(set_x[num], set_y[num]);
		set[num] = false;
	}
}

bomb normal;
bomb special;
bomb long_;

/*********************************************************************************************/
//敵e_nemクラス

class e_nem {
public:
	int point_x[16] = {};
	int point_y[16] = {};
	bool life[16] = {};			//false:生存//true:死亡
	void get_point();
	void move();
};

void e_nem::get_point() {
	int get_x, get_y;
	for (int i = 0; i < 16; i++) {
		get_x = rand() % FIELD_WIDTH;
		get_y = rand() % FIELD_HIGHT;
		if (state[get_x][get_y] == STATE::NON) {
			point_x[i] = get_x;
			point_y[i] = get_y;
			state[point_x[i]][point_y[i]] = STATE::ENEMY;
		}
		else
			i--;
	}
}

void e_nem::move() {
	for (int i = 0; i < 8; i++) {
		if (direction[i] == false && life[i] == false) {
			if (state[point_x[i] + 1][point_y[i]] == STATE::NON) {
				point_x[i]++;
				if (state[point_x[i]][point_y[i]] == STATE::SPECIAL_BOM || state[point_x[i]][point_y[i]] == STATE::NORMAL_BOM || state[point_x[i]][point_y[i]] == STATE::LONG_BOM) {
					life[i] = true;
				}
				state[point_x[i]][point_y[i]] = STATE::ENEMY;
				state[point_x[i] - 1][point_y[i]] = STATE::NON;
			}
			else if (state[point_x[i] - 1][point_y[i]] == STATE::NON) {
				state[point_x[i]][point_y[i]] = STATE::NON;
				point_x[i]--;
				state[point_x[i]][point_y[i]] = STATE::ENEMY;
				direction[i] = true;
			}
		}
		else if (direction[i] == true && life[i] == false) {
			if (state[point_x[i] - 1][point_y[i]] == STATE::NON) {
				point_x[i]--;
				if (state[point_x[i]][point_y[i]] == STATE::SPECIAL_BOM || state[point_x[i]][point_y[i]] == STATE::NORMAL_BOM || state[point_x[i]][point_y[i]] == STATE::LONG_BOM) {
					life[i] = true;
				}
				state[point_x[i]][point_y[i]] = STATE::ENEMY;
				state[point_x[i] + 1][point_y[i]] = STATE::NON;
			}
			else if (state[point_x[i] + 1][point_y[i]] == STATE::NON) {
				state[point_x[i]][point_y[i]] = STATE::NON;
				point_x[i]++;
				state[point_x[i]][point_y[i]] = STATE::ENEMY;
				direction[i] = false;
			}
		}
	}
	for (int i = 8; i < 16; i++) {
		if (direction[i] == false && life[i] == false) {
			if (state[point_x[i]][point_y[i] + 1] == STATE::NON) {
				point_y[i]++;
				if (state[point_x[i]][point_y[i]] == STATE::SPECIAL_BOM || state[point_x[i]][point_y[i]] == STATE::NORMAL_BOM || state[point_x[i]][point_y[i]] == STATE::LONG_BOM) {
					life[i] = true;
				}
				state[point_x[i]][point_y[i]] = STATE::ENEMY;
				state[point_x[i]][point_y[i] - 1] = STATE::NON;
			}
			else if (state[point_x[i]][point_y[i] - 1] == STATE::NON) {
				state[point_x[i]][point_y[i]] = STATE::NON;
				point_y[i]--;
				state[point_x[i]][point_y[i]] = STATE::ENEMY;
				direction[i] = true;
			}
		}
		else if (direction[i] == true && life[i] == false) {
			if (state[point_x[i]][point_y[i] - 1] == STATE::NON) {
				point_y[i]--;
				if (state[point_x[i]][point_y[i]] == STATE::SPECIAL_BOM || state[point_x[i]][point_y[i]] == STATE::NORMAL_BOM || state[point_x[i]][point_y[i]] == STATE::LONG_BOM) {
					life[i] = true;
				}
				state[point_x[i]][point_y[i]] = STATE::ENEMY;
				state[point_x[i]][point_y[i] + 1] = STATE::NON;
			}
			else if (state[point_x[i]][point_y[i] + 1] == STATE::NON) {
				state[point_x[i]][point_y[i]] = STATE::NON;
				point_y[i]++;
				state[point_x[i]][point_y[i]] = STATE::ENEMY;
				direction[i] = false;
			}
		}
	}
}

e_nem enemy;

/********************************************************************************************/

void create_WALL() {
	int randomA[60];
	int randomB[60];
	for (int i = 0; i < FIELD_HIGHT; i++) {					//縦方向の壁をセット
		state[0][i] = STATE::WALL;
		state[FIELD_WIDTH - 1][i] = STATE::WALL;
	}
	for (int i = 1; i < FIELD_WIDTH - 1; i++) {				//横方向の壁をセット
		state[i][0] = STATE::WALL;
		state[i][21 - 1] = STATE::WALL;
	}
	for (int i = 1; i < FIELD_HIGHT / 2; i++) {				//囲い内の初期壁をセット
		for (int j = 1; j < FIELD_WIDTH / 2; j++) {
			state[j * 2][i * 2] = STATE::WALL;
		}
	}
	for (int i = 0; i < 60; i++) {							//囲い内の初期障害物をセット
		randomA[i] = rand() % 21;
		randomB[i] = rand() % 21;
		if (state[randomA[i]][randomB[i]] == STATE::NON) {
			state[randomA[i]][randomB[i]] = STATE::GRASS;
		}
		else
			i--;											//すでにオブジェクトがセットされていたらノーカン
	}
}

void subfield() {
	printf("\n");
	if (bom == BOMB::NORMAL) {
		printf("  → ノーマル　　残り%d\n", numof_normal);
		printf("     スペシャル  残り%d\n", numof_special);
		printf("     ロング      残り%d\n", numof_long);
	}
	else if (bom == BOMB::SPECIAL) {
		printf("  　 ノーマル　　残り%d\n", numof_normal);
		printf("  → スペシャル  残り%d\n", numof_special);
		printf("     ロング      残り%d\n", numof_long);
	}
	else if (bom == BOMB::LONG_) {
		printf("     ノーマル　　残り%d\n", numof_normal);
		printf("     スペシャル  残り%d\n", numof_special);
		printf("  → ロング      残り%d\n", numof_long);
	}
	printf("point_x[0]=%d\n", enemy.point_x[0]);
	printf("point_y[0]=%d\n", enemy.point_y[0]);
	printf("state[point_x[0]][point_y[0]]==%d\n", state[enemy.point_x[0]][enemy.point_y[0]]);
}

void display() {
	system("cls");
	printf(alert);
	for (int y = 0; y < FIELD_HIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {

			//カーソル設定と盤面の記憶
			if (state[x][y] != STATE::SET) {
				tmp[x][y] = state[x][y];
			}
			if (x == cursorX && y == cursorY) {
				state[x][y] = STATE::SET;
			}

			//各点、stateに応じたアスキアートを描写
			if (state[x][y] == STATE::NON) {
				printf("  ");
			}
			else if (state[x][y] == STATE::WALL) {
				printf("■");
			}
			else if (state[x][y] == STATE::GRASS) {
				printf("□");
			}
			else if (state[x][y] == STATE::SET) {
				printf("◎");
			}
			else if (state[x][y] == STATE::NORMAL_BOM) {
				printf("〇");
			}
			else if (state[x][y] == STATE::SPECIAL_BOM) {
				printf("☆");
			}
			else if (state[x][y] == STATE::LONG_BOM) {
				printf("◇");
			}
			else if (state[x][y] == STATE::ENEMY) {
				printf("ζ");
			}

			//起爆
			for (int i = 1; i < MAX_bomb; i++) {
				if (normal.set[i] == true) {
					normal.set_bomb(i);
				}
				if (special.set[i] == true) {
					special.set_bomb(i);
				}
				if (long_.set[i] == true) {
					long_.set_bomb(i);
				}
			}
			state[x][y] = tmp[x][y];

		}
		printf("\n");
	}
	subfield();
}

void judg_bombhit(int x, int y) {
	if (state[x][y] == STATE::SET) {
		hit = true;
	}
}

bool judg_move(int x, int y) {
	if (state[x][y] == STATE::NON && selected == true) {
		return true;
	}
	else if (selected == false && x != 0 && y != 0 && x != FIELD_HIGHT - 1 && y != FIELD_WIDTH - 1) {
		return true;
	}
	else if ((state[x][y] == STATE::WALL || state[x][y] == STATE::GRASS) && selected == true) {
		return false;
	}
	else if (state[x][y] != STATE::WALL && state[x][y] != STATE::GRASS && state[x][y] != STATE::SET && state[x][y] != STATE::NON && selected == true) {
		hit = true;
		return true;
	}
	else
		return false;
}


int main() {
	srand((unsigned long)time(NULL));
	while (1) {
		if (mode == MODE::GAME) {
			create_WALL();
			enemy.get_point();
			display_span = time(NULL);
			enemy_span = time(NULL);
			while (1) {
				if (display_span < time(NULL)) { //一秒ごとに描写
					display_span = time(NULL);
					display();
				}
				if (enemy_span < time(NULL) && enemy_span + 1 >= time(NULL)) {
					enemy_span = time(NULL);
					enemy.move();
				}

				if (_kbhit()) {
					switch (_getch()) {
					case 'w':if (judg_move(cursorX, cursorY - 1) == true) { cursorY--; } break;
					case 'a':if (judg_move(cursorX - 1, cursorY) == true) { cursorX--; } break;
					case 's':if (judg_move(cursorX, cursorY + 1) == true) { cursorY++; } break;
					case 'd':if (judg_move(cursorX + 1, cursorY) == true) { cursorX++; } break;
					case 'p':
						if (selected == false && state[cursorX][cursorY] == STATE::NON) { //何もない位置で'p':初期ゴマを置く
							selected = true;
						}
						else if (selected == true && bom == BOMB::NORMAL && numof_normal > 0) {
							normal.number++;
							normal.set_x[normal.number] = cursorX;
							normal.set_y[normal.number] = cursorY;
							normal.type = STATE::NORMAL_BOM;
							normal.range = 1;
							normal.set[normal.number] = true;
							numof_normal--;
						}
						else if (selected == true && bom == BOMB::SPECIAL && numof_special > 0) {
							special.number++;
							special.set_x[special.number] = cursorX;
							special.set_y[special.number] = cursorY;
							special.type = STATE::SPECIAL_BOM;
							special.range = 20;
							special.set[special.number] = true;
							numof_special--;
						}
						else if (selected == true && bom == BOMB::LONG_ && numof_long > 0) {
							long_.number++;
							long_.set_x[long_.number] = cursorX;
							long_.set_y[long_.number] = cursorY;
							long_.type = STATE::LONG_BOM;
							long_.range = 2;
							long_.set[long_.number] = true;
							numof_long--;
						}

						break;
					case 'e':
						if (bom == BOMB::NORMAL) {
							bom = BOMB::SPECIAL;
						}
						else if (bom == BOMB::SPECIAL) {
							bom = BOMB::LONG_;
						}
						else if (bom == BOMB::LONG_) {
							bom = BOMB::NORMAL;
						}
					case 'q':mode = MODE::STOP; break;
					}
					display();
				}


				if (selected == false) {
					alert = " 初期位置を選択してください[p]\n";
				}
				else
					alert = "\n";

				if (hit == true) {
					mode = MODE::GAMEOVER;							//当たったらゲームオーバーモードに切り替え								
				}

				if (mode != MODE::GAME) {							//ゲームモードでなくなったら他のモードへ移行
					break;
				}
			}
		}
		else if (mode == MODE::MENU) {
			time_t retime = time(NULL) - 3;
			while (1) {
				if (retime + 3 < time(NULL)) {
					retime = time(NULL);
					system("cls");
					printf("■■■■■■■■■■■■■■■■■■■■■\n");
					printf("\n\n\n\n");
					printf("        ●～* ボンバーマン ●～*\n");
					printf("\n\n\n\n");
					printf("                 <Enter>\n");
					printf("\n\n\n");
					printf(" ________\n");
					printf("|操作説明|\n");
					printf("￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣\n");
					printf(" w,a,s,d :カーソル移動\n");
					printf("    p    :爆弾の設置\n");
					printf("    e    :爆弾の切り替え\n");
					printf("    q    :中断メニュー\n");
					printf("■■■■■■■■■■■■■■■■■■■■■\n");
				}
				if (_kbhit()) {
					switch (_getch()) {
					case '\r':mode = MODE::GAME; break;
					}
				}
				if (mode != MODE::MENU) {
					break;
				}
			}
		}

		else if (mode == MODE::GAMECREARE) {
			time_t retime = time(NULL) - 3;
			while (1) {
				if (retime + 3 < time(NULL)) {
					retime = time(NULL);
					system("cls");
					printf("■■■■■■■■■■■■■■■■■■■■■\n");
					printf("\n\n\n");
					printf("              ________________            \n");
					printf("              ゲーム　クリア!!　          \n");
					printf("              ￣￣￣￣￣￣￣￣            \n\n");
					printf(" (∩´∀｀)∩　              　ヽ(^。^)ノ \n");
					printf("\n\n\n\n\n\n\n\n");
					printf("                 <Enter>\n");
					printf("\n\n\n");
					printf("■■■■■■■■■■■■■■■■■■■■■\n");
				}
				if (_kbhit()) {
					switch (_getch()) {
					case '\r':mode = MODE::RESET; break;
					}
				}
				if (mode != MODE::GAMECREARE) {
					break;
				}
			}
		}

		else if (mode == MODE::GAMEOVER) {
			time_t retime = time(NULL) - 3;
			while (1) {
				if (retime + 3 < time(NULL)) {
					retime = time(NULL);
					system("cls");
					printf("■■■■■■■■■■■■■■■■■■■■■\n");
					printf("\n\n\n");
					printf("             __________________           \n");
					printf("             ゲーム　オーバー!!　         \n");
					printf("             ￣￣￣￣￣￣￣￣￣           \n\n");
					printf("　(;´･ω･)　              　　 　(ﾟДﾟ;) \n");
					printf("\n\n\n\n\n\n\n\n");
					printf("                 <Enter>\n");
					printf("\n\n\n");
					printf("■■■■■■■■■■■■■■■■■■■■■\n");
				}
				if (_kbhit()) {
					switch (_getch()) {
					case '\r':mode = MODE::RESET; break;
					}
				}
				if (mode != MODE::GAMEOVER) {
					break;
				}
			}
		}

		else if (mode == MODE::STOP) {
			time_t retime = time(NULL) - 3;
			int cursor = 0;                 //セレクト
			while (1) {
				if (retime + 3 < time(NULL)) {
					retime = time(NULL);
					system("cls");
					printf("■■■■■■■■■■■■■■■■■■■■■\n");
					printf("\n\n\n");
					printf("             __________________           \n");
					printf("               中断　メニュー　           \n");
					printf("             ￣￣￣￣￣￣￣￣￣           \n\n");
					printf(" (; ･`д･´)　              　　(´▽｀*) \n");
					printf("\n\n\n\n");
					if (cursor % 2 == 0) {
						printf("　　           　→ 再開 ←              \n");
						printf("      　        メニュー画面\n");
					}
					else if (cursor % 2 == 1) {
						printf("　　           　   再開                 \n");
						printf("      　     → メニュー画面 ←\n");
					}
					printf("\n\n");
					printf("                  <Enter>\n");
					printf("\n\n\n");
					printf("■■■■■■■■■■■■■■■■■■■■■\n");
				}
				if (_kbhit()) {
					switch (_getch()) {
					case '\r':
						if (cursor % 2 == 0) {
							mode = MODE::GAME;
						}
						else if (cursor % 2 == 1) {
							mode = MODE::RESET;
						}
					case 'w':cursor++; break;
					case 's':cursor++; break;
					}
					system("cls");
					printf("■■■■■■■■■■■■■■■■■■■■■\n");
					printf("\n\n\n");
					printf("             __________________           \n");
					printf("               中断　メニュー　           \n");
					printf("             ￣￣￣￣￣￣￣￣￣           \n\n");
					printf(" (; ･`д･´)　              　　(´▽｀*) \n");
					printf("\n\n\n\n");
					if (cursor % 2 == 0) {
						printf("　　           　→ 再開 ←              \n");
						printf("      　        メニュー画面\n");
					}
					else if (cursor % 2 == 1) {
						printf("　　           　   再開                 \n");
						printf("      　     → メニュー画面 ←\n");
					}
					printf("\n\n");
					printf("                  <Enter>\n");
					printf("\n\n\n");
					printf("■■■■■■■■■■■■■■■■■■■■■\n");
				}
				if (mode != MODE::STOP) {
					break;
				}
			}
		}

		else if (mode == MODE::RESET) {
			//パラメータリセット処理
			while (1) {
				system("cls");
				printf("リセット処理\n");
			}
		}
	}
}



