#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define MAX_bomb 25


int FIELD_WIDTH = 21;
int FIELD_HIGHT = 21;

bool selected;								//��������Z�b�g������(true)���Ă��Ȃ���(false)
bool hit;

enum class MODE { GAME, MENU, GAMEOVER, GAMECREARE };
enum class BOMB { SPECIAL, NORMAL, LONG_ };
enum class STATE { NON, WALL, GRASS, NORMAL_BOM, LONG_BOM, SPECIAL_BOM, ENEMY, SET };

MODE mode;
BOMB bom;									//���e�̎��
STATE state[21][21];						//�Ֆʂ̏�Ԃ��i�[����z��
STATE tmp[21][21];							//�Ֆʂ̏�Ԃ��ꎞ�L������z��

int cursorX = 1, cursorY = 1;				//�J�[�\��(�L�����N�^�[)�̈ʒu

bool direction[16] = { false };

const char* alert = " �����ʒu��I�����Ă�������[p]\n";

time_t display_span;
time_t enemy_span;

int numof_normal = 25;
int numof_special = 5;
int numof_long = 5;

void judg_bombhit(int x, int y);

/*********************************************************************************************/
//���ebomb�N���X

class bomb {
public:
	int number = 0;						//���e�z��̗v�f
	int range = 0;						//�����͈̔�
	bool timer_set[MAX_bomb] = {};		//�N���^�C�}�[���Z�b�g�������ۂ�
	bool set[MAX_bomb];					//���e��ݒu�������ۂ�
	int  set_x[MAX_bomb] = {};			//�N������x���W
	int  set_y[MAX_bomb] = {};			//�N������y���W
	time_t timer[MAX_bomb] = {};		//���e�ݒu���̎���
	STATE type = {};					//�N�����锚�e�̃^�C�v
	void set_bomb(int num);						//�ݒu����N���A���ł܂ł̊֐�
	void explode(int x, int y, int range);		//�N��
	void release(int x, int y);					//����
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
		//release����
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
//�Ge_nem�N���X

class e_nem {
public:
	int point_x[16] = {};
	int point_y[16] = {};
	bool life[16] = {};			//false:����//true:���S
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
	for (int i = 0; i < FIELD_HIGHT; i++) {					//�c�����̕ǂ��Z�b�g
		state[0][i] = STATE::WALL;
		state[FIELD_WIDTH - 1][i] = STATE::WALL;
	}
	for (int i = 1; i < FIELD_WIDTH - 1; i++) {				//�������̕ǂ��Z�b�g
		state[i][0] = STATE::WALL;
		state[i][21 - 1] = STATE::WALL;
	}
	for (int i = 1; i < FIELD_HIGHT / 2; i++) {				//�͂����̏����ǂ��Z�b�g
		for (int j = 1; j < FIELD_WIDTH / 2; j++) {
			state[j * 2][i * 2] = STATE::WALL;
		}
	}
	for (int i = 0; i < 60; i++) {							//�͂����̏�����Q�����Z�b�g
		randomA[i] = rand() % 21;
		randomB[i] = rand() % 21;
		if (state[randomA[i]][randomB[i]] == STATE::NON) {
			state[randomA[i]][randomB[i]] = STATE::GRASS;
		}
		else
			i--;											//���łɃI�u�W�F�N�g���Z�b�g����Ă�����m�[�J��
	}
}

void subfield() {
	printf("\n");
	if (bom == BOMB::NORMAL) {
		printf("  �� �m�[�}���@�@�c��%d\n", numof_normal);
		printf("     �X�y�V����  �c��%d\n", numof_special);
		printf("     �����O      �c��%d\n", numof_long);
	}
	else if (bom == BOMB::SPECIAL) {
		printf("  �@ �m�[�}���@�@�c��%d\n", numof_normal);
		printf("  �� �X�y�V����  �c��%d\n", numof_special);
		printf("     �����O      �c��%d\n", numof_long);
	}
	else if (bom == BOMB::LONG_) {
		printf("     �m�[�}���@�@�c��%d\n", numof_normal);
		printf("     �X�y�V����  �c��%d\n", numof_special);
		printf("  �� �����O      �c��%d\n", numof_long);
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

			//�J�[�\���ݒ�ƔՖʂ̋L��
			if (state[x][y] != STATE::SET) {
				tmp[x][y] = state[x][y];
			}
			if (x == cursorX && y == cursorY) {
				state[x][y] = STATE::SET;
			}

			//�e�_�Astate�ɉ������A�X�L�A�[�g��`��
			if (state[x][y] == STATE::NON) {
				printf("  ");
			}
			else if (state[x][y] == STATE::WALL) {
				printf("��");
			}
			else if (state[x][y] == STATE::GRASS) {
				printf("��");
			}
			else if (state[x][y] == STATE::SET) {
				printf("��");
			}
			else if (state[x][y] == STATE::NORMAL_BOM) {
				printf("�Z");
			}
			else if (state[x][y] == STATE::SPECIAL_BOM) {
				printf("��");
			}
			else if (state[x][y] == STATE::LONG_BOM) {
				printf("��");
			}
			else if (state[x][y] == STATE::ENEMY) {
				printf("��");
			}

			//�N��
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
				if (display_span < time(NULL)) { //��b���Ƃɕ`��
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
						if (selected == false && state[cursorX][cursorY] == STATE::NON) { //�����Ȃ��ʒu��'p':�����S�}��u��
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
					}
					display();
				}


				if (selected == false) {
					alert = " �����ʒu��I�����Ă�������[p]\n";
				}
				else
					alert = "\n";

				if (hit == true) {
					mode = MODE::MENU;
				}

				if (mode != MODE::GAME) {
					//�������������s��
					break;
				}
			}
		}
		else if (mode == MODE::MENU) {
			while (1) {
				system("cls");
				printf("���j���[");
			}
		}
		else if (mode == MODE::GAMECREARE) {

		}
		else if (mode == MODE::GAMEOVER) {

		}
	}
}



