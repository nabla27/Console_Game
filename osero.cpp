#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//test

int FIELD_WIDTH = 10;
int FIELD_HEIGHT = 10;

int cursorX = 1;
int cursorY = 1;

int count_BK = 0;
int count_WH = 0;
int true_false = 0;

int wait_time = 40;
bool w_b = true;            //true:��//false:��

clock_t start;
clock_t limit;
clock_t paused_start;
clock_t paused_stop;
time_t limit_time;

const char* alert;

enum state{NON, BLACK, WHITE, WALL, SET};
enum class MODE{MENU, GAME, JUDG, PAUSE, RESET_M, RESET_G, OPTION};
enum class GAME_MODE{PARE, SINGLE};

state OR[14][14];
state tmp[14][14];											//��̏�ԋL��
bool t_f[14][14];											//��̑���//true����//false�Ȃ�
bool turn, paused;											//�^�[��//true:����//false:����
MODE mode;
GAME_MODE game_mode;

void judg_wall() {											//�ǂ̓����蔻��
	if (cursorX == 0) {
		cursorX = 1;
	}
	else if (cursorX == FIELD_WIDTH - 1) {
		cursorX = FIELD_WIDTH - 2;
	}
	if (cursorY == 0) {
		cursorY = 1;
	}
	else if (cursorY == FIELD_HEIGHT - 1) {
		cursorY = FIELD_HEIGHT - 2;
	}
}									

bool CHECK_WHITE(int check_x, int check_y) {						//��u����ʒu���m�F
	int count = 0;
	if (t_f[check_x][check_y] == false) {					
		if (OR[check_x - 1][check_y] == BLACK) {
			for (int j = 2; check_x - j >= 1;j++) {
				if (OR[check_x - j][check_y] == WHITE) {
					count++;
				}
				else if (OR[check_x - j][check_y] == NON) {
					break;
				}
			}
		}
		if (OR[check_x + 1][check_y] == BLACK) {
			for (int j = 2; check_x + j <= FIELD_WIDTH - 2; j++) {
				if (OR[check_x + j][check_y] == WHITE) {
					count++;
				}
				else if (OR[check_x + j][check_y] == NON) {
					break;
				}
			}
		}
		if (OR[check_x][check_y - 1] == BLACK) {
			for (int j = 2; check_y - j >= 1; j++) {
				if (OR[check_x][check_y - j] == WHITE) {
					count++;
				}
				else if (OR[check_x][check_y - j] == NON) {
					break;
				}
			}
		}
		if (OR[check_x][check_y + 1] == BLACK) {
			for (int j = 2; check_y + j <= FIELD_HEIGHT - 2; j++) {
				if (OR[check_x][check_y	+ j] == WHITE) {
					count++;
				}
				else if (OR[check_x][check_y + j] == NON) {
					break;
				}
			}
		}
		if (OR[check_x - 1][check_y - 1] == BLACK) {
			for (int j = 2; ((check_x - j >= 1) || (check_y - j >= 1)); j++) {
				if (OR[check_x - j][check_y - j] == WHITE) {
					count++;
				}
				else if (OR[check_x - j][check_y - j] == NON) {
					break;
				}
			}
		}
		if (OR[check_x + 1][check_y + 1] == BLACK) {
			for (int j = 2; ((check_x + j <= FIELD_WIDTH-2) || (check_y + j <= FIELD_HEIGHT-2)); j++) {
				if (OR[check_x + j][check_y + j] == WHITE) {
					count++;
				}
				else if (OR[check_x + j][check_y + j] == NON) {
					break;
				}
			}
		}
		if (OR[check_x - 1][check_y + 1] == BLACK) {
			for (int j = 2; ((check_x - j >= 1) || (check_y + j <= FIELD_HEIGHT - 2)); j++) {
				if (OR[check_x - j][check_y + j] == WHITE) {
					count++;
				}
				else if (OR[check_x - j][check_y + j] == NON) {
					break;
				}
			}
		}
		if (OR[check_x + 1][check_y - 1] == BLACK) {
			for (int j = 2; ((check_x + j <= FIELD_WIDTH - 2) || (check_y - j >= 1)); j++) {
				if (OR[check_x + j][check_y - j] == WHITE) {
					count++;
				}
				else if (OR[check_x + j][check_y - j] == NON) {
					break;
				}
			}
		}
		if (count >= 1) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool CHECK_BLACK(int check_x, int check_y) {						//��u����ʒu���m�F
	int count = 0;
	if (t_f[check_x][check_y] == false) {
		if (OR[check_x - 1][check_y] == WHITE) {
			for (int j = 2; check_x - j >= 1; j++) {
				if (OR[check_x - j][check_y] == BLACK) {
					count++;
				}
				else if (OR[check_x - j][check_y] == NON) {
					break;
				}
			}
		}
		if (OR[check_x + 1][check_y] == WHITE) {
			for (int j = 2; check_x + j <= FIELD_WIDTH - 2; j++) {
				if (OR[check_x + j][check_y] == BLACK) {
					count++;
				}
				else if (OR[check_x + j][check_y] == NON) {
					break;
				}
			}
		}
		if (OR[check_x][check_y - 1] == WHITE) {
			for (int j = 2; check_y - j >= 1; j++) {
				if (OR[check_x][check_y - j] == BLACK) {
					count++;
				}
				else if (OR[check_x][check_y - j] == NON) {
					break;
				}
			}
		}
		if (OR[check_x][check_y + 1] == WHITE) {
			for (int j = 2; check_y + j <= FIELD_HEIGHT - 2; j++) {
				if (OR[check_x][check_y + j] == BLACK) {
					count++;
				}
				else if (OR[check_x][check_y + j] == NON) {
					break;
				}
			}
		}
		if (OR[check_x - 1][check_y - 1] == WHITE) {
			for (int j = 2; ((check_x - j >= 1) || (check_y - j >= 1)); j++) {
				if (OR[check_x - j][check_y - j] == BLACK) {
					count++;
				}
				else if (OR[check_x - j][check_y - j] == NON) {
					break;
				}
			}
		}
		if (OR[check_x + 1][check_y + 1] == WHITE) {
			for (int j = 2; ((check_x + j <= FIELD_WIDTH - 2) || (check_y + j <= FIELD_HEIGHT - 2)); j++) {
				if (OR[check_x + j][check_y + j] == BLACK) {
					count++;
				}
				else if (OR[check_x + j][check_y + j] == NON) {
					break;
				}
			}
		}
		if (OR[check_x - 1][check_y + 1] == WHITE) {
			for (int j = 2; ((check_x - j >= 1) || (check_y + j <= FIELD_HEIGHT - 2)); j++) {
				if (OR[check_x - j][check_y + j] == BLACK) {
					count++;
				}
				else if (OR[check_x - j][check_y + j] == NON) {
					break;
				}
			}
		}
		if (OR[check_x + 1][check_y - 1] == WHITE) {
			for (int j = 2; ((check_x + j <= FIELD_WIDTH - 2) || (check_y - j >= 1)); j++) {
				if (OR[check_x + j][check_y - j] == BLACK) {
					count++;
				}
				else if (OR[check_x + j][check_y - j] == NON) {
					break;
				}
			}
		}
		if (count >= 1) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void REVERSE(int m, int n) {
	CHECK_WHITE(m, n);
	CHECK_BLACK(m, n);
	if (turn == true) {
		int right_B = 0;
		int left_B = 0;
		int up_B = 0;
		int down_B = 0;
		int left_up_B = 0;
		int left_down_B = 0;
		int right_up_B = 0;
		int right_down_B = 0;
		do {
			right_B++;
		} while (OR[m + right_B][n] == BLACK);
		if (OR[m + right_B][n] == WHITE) {
			t_f[m][n] = true;
			OR[m][n] = WHITE;
			for (int i = 1; i < right_B; i++) {
				OR[m + i][n] = WHITE;
				REVERSE(m + i, n);
			}
		}
		do {
			left_B++;
		} while (OR[m - left_B][n] == BLACK);
		if (OR[m - left_B][n] == WHITE) {
			t_f[m][n] = true;
			OR[m][n] = WHITE;
			for (int i = 1; i < left_B; i++) {
				OR[m - i][n] = WHITE;
				REVERSE(m - i, n);
			}
		}
		do {
			up_B++;
		} while (OR[m][n - up_B] == BLACK);
		if (OR[m][n - up_B] == WHITE) {
			t_f[m][n] = true;
			OR[m][n] = WHITE;
			for (int i = 1; i < up_B; i++) {
				OR[m][n - i] = WHITE;
				REVERSE(m, n - i);
			}
		}
		do {
			down_B++;
		} while (OR[m][n + down_B] == BLACK);
		if (OR[m][n + down_B] == WHITE) {
			t_f[m][n] = true;
			OR[m][n] = WHITE;
			for (int i = 1; i < down_B; i++) {
				OR[m][n + i] = WHITE;
				REVERSE(m, n + i);
			}
		}
		do {
			left_up_B++;
		} while (OR[m - left_up_B][n - left_up_B] == BLACK);
		if (OR[m - left_up_B][n - left_up_B] == WHITE) {
			t_f[m][n] = true;
			OR[m][n] = WHITE;
			for (int i = 1; i < left_up_B; i++) {
				OR[m - i][n - i] = WHITE;
				REVERSE(m - i, n - i);
			}
		}
		do {
			left_down_B++;
		} while (OR[m - left_down_B][n + left_down_B] == BLACK);
		if (OR[m - left_down_B][n + left_down_B] == WHITE) {
			t_f[m][n] = true;
			OR[m][n] = WHITE;
			for (int i = 1; i < left_down_B; i++) {
				OR[m - i][n + i] = WHITE;
				REVERSE(m - i, n + i);
			}
		}
		do {
			right_up_B++;
		} while (OR[m + right_up_B][n - right_up_B] == BLACK);
		if (OR[m + right_up_B][n - right_up_B] == WHITE) {
			t_f[m][n] = true;
			OR[m][n] = WHITE;
			for (int i = 1; i < right_up_B; i++) {
				OR[m + i][n - i] = WHITE;
				REVERSE(m + i, n - i);
			}
		}
		do {
			right_down_B++;
		} while (OR[m + right_down_B][n + right_down_B] == BLACK);
		if (OR[m + right_down_B][n + right_down_B] == WHITE) {
			t_f[m][n] = true;
			OR[m][n] = WHITE;
			for (int i = 1; i < right_down_B; i++) {
				OR[m + i][n + i] = WHITE;
				REVERSE(m + i, n + i);
			}
		}
	}
	if (turn == false) {
		int right_W = 0;
		int left_W = 0;
		int up_W = 0;
		int down_W = 0;
		int left_up_W = 0;
		int left_down_W = 0;
		int right_up_W = 0;
		int right_down_W = 0;
		do {
			right_W++;
		} while (OR[m + right_W][n] == WHITE);
		if (OR[m + right_W][n] == BLACK) {
			t_f[m][n] = true;
			OR[m][n] = BLACK;
			for (int i = 1; i < right_W; i++) {
				OR[m + i][n] = BLACK;
				REVERSE(m + i, n);
			}
		}
		do {
			left_W++;
		} while (OR[m - left_W][n] == WHITE);
		if (OR[m - left_W][n] == BLACK) {
			t_f[m][n] = true;
			OR[m][n] = BLACK;
			for (int i = 1; i < left_W; i++) {
				OR[m - i][n] = BLACK;
				REVERSE(m - i, n);
			}
		}
		do {
			up_W++;
		} while (OR[m][n - up_W] == WHITE);
		if (OR[m][n - up_W] == BLACK) {
			t_f[m][n] = true;
			OR[m][n] = BLACK;
			for (int i = 1; i < up_W; i++) {
				OR[m][n - i] = BLACK;
				REVERSE(m, n - i);
			}
		}
		do {
			down_W++;
		} while (OR[m][n + down_W] == WHITE);
		if (OR[m][n + down_W] == BLACK) {
			t_f[m][n] = true;
			OR[m][n] = BLACK;
			for (int i = 1; i < down_W; i++) {
				OR[m][n + i] = BLACK;
				REVERSE(m, n + i);
			}
		}
		do {
			left_up_W++;
		} while (OR[m - left_up_W][n - left_up_W] == WHITE);
		if (OR[m - left_up_W][n - left_up_W] == BLACK) {
			t_f[m][n] = true;
			OR[m][n] = BLACK;
			for (int i = 1; i < left_up_W; i++) {
				OR[m - i][n - i] = BLACK;
				REVERSE(m - i, n - i);
			}
		}
		do {
			left_down_W++;
		} while (OR[m - left_down_W][n + left_down_W] == WHITE);
		if (OR[m - left_down_W][n + left_down_W] == BLACK) {
			t_f[m][n] = true;
			OR[m][n] = BLACK;
			for (int i = 1; i < left_down_W; i++) {
				OR[m - i][n + i] = BLACK;
				REVERSE(m - i, n + i);
			}
		}
		do {
			right_up_W++;
		} while (OR[m + right_up_W][n - right_up_W] == WHITE);
		if (OR[m + right_up_W][n - right_up_W] == BLACK) {
			t_f[m][n] = true;
			OR[m][n] = BLACK;
			for (int i = 1; i < right_up_W; i++) {
				OR[m + i][n - i] = BLACK;
				REVERSE(m + i, n - i);
			}
		}
		do {
			right_down_W++;
		} while (OR[m + right_down_W][n + right_down_W] == WHITE);
		if (OR[m + right_down_W][n + right_down_W] == BLACK) {
			t_f[m][n] = true;
			OR[m][n] = BLACK;
			for (int i = 1; i < right_down_W; i++) {
				OR[m + i][n + i] = BLACK;
				REVERSE(m + i, n + i);
			}
		}
	}
}

void subfield() {											//�T�u�t�B�[���h�̏o��
	if (turn == true) {
		if (w_b == true) {
			printf("[�����̃^�[���ł�]��\n");
		}
		else
			printf("[�����̃^�[���ł�]�Z\n");
	}
	else if (turn == false) {
		if (w_b == true) {
			printf("[����̃^�[���ł�]�Z\n");
		}
		else
			printf("[����̃^�[���ł�]��\n");
	}
	printf("\n");
	printf(alert);
	printf("\n");
	printf("����̐�:"); printf("  %d\n", count_WH);
	printf("����̐�:"); printf("  %d\n", count_BK);
	printf("���v�:"); printf("  %d\n", true_false - (FIELD_HEIGHT+FIELD_WIDTH)*2 + 4);
	printf("\n");
	printf("�P�P�P�P�P�P�P�P�P�P�P\n");
	printf("w,a,s,d: �J�[�\���ړ�\n");
	printf("p      : ���z�u����\n");
	printf("k      : �X�L�b�v����\n");
	printf("m      : ���f���j���[\n");
	printf("______________________\n");
}

void COUNT(int m, int n) {									//��̐����J�E���g
	if (OR[m][n] == WHITE) {
		count_WH++;
	}
	else if (OR[m][n] == BLACK) {
		count_BK++;
	}
	if (t_f[m][n] == true) {
		true_false++;
	}
}

void display() {
	system("cls");									    //stdlib.h//��ʂ��N���A����R�}���h
	count_WH = 0;
	count_BK = 0;
	true_false = 0;
	if (game_mode == GAME_MODE::SINGLE) {
		printf("���[�h: 1�l       %d\n", limit);
	}
	else if (game_mode == GAME_MODE::PARE) {
		printf("���[�h: 2�l       %d\n", limit);
	}
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {

			judg_wall();                                //�ǂ̓����蔻��


			if (OR[x][y] != SET) {					//�J�[�\���ʒu�ȊO�̏�Ԃ��L��
				tmp[x][y] = OR[x][y];
			}
			if ((cursorX == x) && (cursorY == y)) {
				OR[x][y] = SET;
			}
			else if (((cursorX != x) || (cursorY != y)) && (t_f[x][y] == true)) {
				OR[x][y] = tmp[x][y];
			}
			else if (((cursorX != x) || (cursorY != y)) && (t_f[x][y] == false)) {
				OR[x][y] = NON;
			}


			COUNT(x, y);


			if (OR[x][y] == WALL) {
				printf("��");
			}
			else if (OR[x][y] == NON) {
				printf("�E");
			}
			else if (OR[x][y] == SET) {
				printf("��");
			}
			else if (OR[x][y] == WHITE) {
				if (w_b == true) {
					printf("��");
				}
				else
					printf("�Z");
			}
			else if (OR[x][y] == BLACK) {
				if (w_b == true) {
					printf("��");
				}
				else
					printf("��");
			}

		}
		printf("\n");
	}

	subfield();
	if ((count_BK == 0 && count_WH == true_false - (FIELD_HEIGHT + FIELD_WIDTH) * 2 + 4) || 
		(count_WH == 0 && count_BK == true_false - (FIELD_HEIGHT + FIELD_WIDTH) * 2 + 4)) {
		mode = MODE::JUDG;
	}

	alert = "\n";
}

int main() {
	mode = MODE::MENU;
	turn = true;
	while (1) {

		if (mode == MODE::MENU) {
			game_mode = GAME_MODE::SINGLE;
			int point = 0;
			while (1) {
				int point_menu = point % 3;
				system("cls");
				printf("\n");
				printf("  <�I�Z���@�Q�[��>\n\n\n");
				printf("���[�h�I��:");
				if (point_menu == 0) {
					printf("  �� 1�l\n");
					printf("                2�l\n");
					printf("                �I�v�V����\n");
				}
				else if (point_menu == 1) {
					printf("     1�l\n");
					printf("             �� 2�l\n");
					printf("                �I�v�V����\n");
				}
				else if (point_menu == 2) {
					printf("     1�l\n");
					printf("                2�l\n");
					printf("             �� �I�v�V����\n");
				}
				switch (_getch()) {
				case 'w': point--; break;
				case 's': point++; break;
				case '\r':
					if (point_menu == 0) {
						game_mode = GAME_MODE::SINGLE;
						mode = MODE::GAME;
					}
					else if (point_menu == 1) {
						game_mode = GAME_MODE::PARE;
						mode = MODE::GAME;
					}
					else if (point_menu == 2) {
						mode = MODE::OPTION;
					}
				}
				if (mode != MODE::MENU) {
					break;
				}
			}
		}

		else if (mode == MODE::GAME) {
			alert = "\n";

			OR[FIELD_WIDTH / 2 - 1][FIELD_HEIGHT / 2 - 1] = WHITE; t_f[FIELD_WIDTH / 2 - 1][FIELD_HEIGHT / 2 - 1] = true;						//������
			OR[FIELD_WIDTH / 2 - 1][FIELD_HEIGHT / 2] = BLACK; t_f[FIELD_WIDTH / 2 - 1][FIELD_HEIGHT / 2] = true;
			OR[FIELD_WIDTH / 2][FIELD_HEIGHT / 2 - 1] = BLACK; t_f[FIELD_WIDTH / 2][FIELD_HEIGHT / 2 - 1] = true;
			OR[FIELD_WIDTH / 2][FIELD_HEIGHT / 2] = WHITE; t_f[FIELD_WIDTH / 2][FIELD_HEIGHT / 2] = true;
			for (int j = 0; j < FIELD_WIDTH; j++) {					//���ǂ̐���
				OR[0][j] = WALL; t_f[0][j] = true;
				OR[FIELD_WIDTH - 1][j] = WALL; t_f[FIELD_WIDTH - 1][j] = true;
			}
			for (int j = 1; j < FIELD_HEIGHT; j++) {				//�c�ǂ̐���
				OR[j][0] = WALL; t_f[j][0] = true;
				OR[j][FIELD_HEIGHT - 1] = WALL; t_f[j][FIELD_HEIGHT - 1] = true;
			}
			if (paused == false) {
				start = clock() / 1000;
				paused_start = 0;
				paused_stop = 0;
			}
			else {
				paused_stop = clock() / 1000;
				paused = false;
				start = start + (paused_stop - paused_start);
				alert = "false\n";
			}
			time_t t = time(NULL);
			while (1) {
				if (t < time(NULL)) {
					t = time(NULL);
					limit = wait_time - ((clock() / 1000) - start);
					if (limit <= 0) {
						start = clock() / 1000;
						if (turn == true) {
							turn = false;
						}
						else {
							turn = true;
						}
					}
					display();
				}

				if (_kbhit()) {
					switch (_getch()) {
					case 'w':cursorY--; break;
					case 's':cursorY++; break;
					case 'a':cursorX--; break;
					case 'd':cursorX++; break;
					case 'p':
						if ((CHECK_WHITE(cursorX, cursorY) == true && turn == true) || CHECK_BLACK(cursorX, cursorY) == true && turn == false) {
							REVERSE(cursorX, cursorY);
							if (turn == true) {
								turn = false;
								start = clock() / 1000;
							}
							else {
								turn = true;
								start = clock() / 1000;
							}
						}
						else if (CHECK_BLACK(cursorX, cursorY) == false || CHECK_WHITE(cursorX, cursorY) == false) {
							alert = "���̏ꏊ�ɂ͒u���܂���\n";
						}
						break;
					case 'k':
						if (turn == true) {
							turn = false;
						}
						else if (turn == false && game_mode == GAME_MODE::PARE) {
							turn = true;
						}
						break;
					case 'm':
						mode = MODE::PAUSE;
						paused = true;
						paused_start = clock() / 1000;
						break;
					}
					display();
				}
				if (true_false - (FIELD_HEIGHT + FIELD_WIDTH) * 2 + 4 == (FIELD_HEIGHT - 2) * (FIELD_WIDTH - 2)) {			//�Ֆʂ����܂�����A�Q�[�����[�h���画�胂�[�h��
					mode = MODE::JUDG;
				}
				if (mode != MODE::GAME) {
					break;
				}
			}
		}

		else if (mode == MODE::JUDG) {
			while (1) {
				system("cls");
				printf("\n");
				printf("       <����>\n");
				printf("\n\n");
				if (w_b == true) {
					printf("    ����̐�:"); printf("  %d\n", count_WH);
					printf("    ����̐�:"); printf("  %d\n", count_BK);
					printf("\n\n");
					if (count_WH > count_BK) {
						printf("    ���Ȃ��̏���\n");
					}
					else {
						printf("     ����̏���\n");
					}
				}
				else if (w_b == false) {
					printf("    ����̐�:"); printf("  %d\n", count_BK);
					printf("    ����̐�:"); printf("  %d\n", count_WH);
					printf("\n\n");
					if (count_WH < count_BK) {
						printf("    ����̏���\n");
					}
					else {
						printf("     ���Ȃ��̏���\n");
					}
				}
				switch (_getch()) {
				case '\r':mode = MODE::RESET_M; break;
				}
				if (mode != MODE::JUDG) {
					break;
				}
			}
		}

		else if (mode == MODE::PAUSE) {
			int point = 0;
			while (1) {
				system("cls");
				int pause_point = point % 3;
				printf("\n");
				printf("      <���f���j���[>\n");
				printf("\n");
				printf(" ----------------------\n");
				if (pause_point == 0) {
					printf("|     �� �ĊJ          |\n");
					printf("|        ���Z�b�g      |\n");
					printf("|        ���j���[      |\n");
				}
				else if (pause_point == 1) {
					printf("|        �ĊJ          |\n");
					printf("|     �� ���Z�b�g      |\n");
					printf("|        ���j���[      |\n");
				}
				else if (pause_point == 2) {
					printf("|        �ĊJ          |\n");
					printf("|        ���Z�b�g      |\n");
					printf("|     �� ���j���[      |\n");
				}
				printf(" ----------------------\n");

				switch (_getch()) {
				case 'w':point--; break;
				case 's':point++; break;
				case '\r':
					if (pause_point == 0) {
						mode = MODE::GAME;
					}
					else if (pause_point == 1) {//���f>>���Z�b�g>>�Q�[��
						mode = MODE::RESET_G;
					}
					else if (pause_point == 2) {//���f>>���Z�b�g>>���j���[
						mode = MODE::RESET_M;
					}
					break;
				}

				if (mode != MODE::PAUSE) {
					break;
				}
			}
		}

		else if (mode == MODE::RESET_G || mode == MODE::RESET_M) {
			for (int i = 0; i < FIELD_HEIGHT; i++) {
				for (int j = 0; j < FIELD_WIDTH; j++) {
					OR[i][j] = NON;
					tmp[i][j] = NON;
					t_f[i][j] = false;
				}
			}
			cursorX = 1;
			cursorY = 1;
			turn = true;
			paused = false;
			if (mode == MODE::RESET_G) {
				mode = MODE::GAME;
			}
			else if (mode == MODE::RESET_M) {
				mode = MODE::MENU;
			}
		}

		else if (mode == MODE::OPTION) {                                   //��������//�����̋�F//�ՖʃT�C�Y
			int point_up = 0;
			int point_right = 0;
			const char* limTime = "40�b";
			const char* field = "8�~8";
			const char* WorB = "��";

			if (wait_time == 10) {
				limTime = "10�b";
			}
			else if (wait_time == 20) {
				limTime = "20�b";
			}
			else if (wait_time == 30) {
				limTime = "30�b";
			}
			else if (wait_time == 40) {
				limTime = "40�b";
			}
			else if (wait_time == 50) {
				limTime = "50�b";
			}
			else if (wait_time == 60) {
				limTime = "60�b";
			}
			if (FIELD_HEIGHT == 8) {
				field = "6�~6";
			}
			else if (FIELD_HEIGHT == 10) {
				field = "8�~8";
			}
			else if (FIELD_HEIGHT == 12) {
				field = "10�~10";
			}
			else if (FIELD_HEIGHT == 14) {
				field = "12�~12";
			}
			if (w_b == true) {
				WorB = "��";
			}
			else if (w_b == false) {
				WorB = "��";
			}

			while (1) {
				system("cls");
				printf("\n");
				printf("    <�I�v�V����>\n");
				printf("\n\n");
				if (point_up % 4 == 0) {
					if (point_right % 3 == 0) {
						printf("  �� [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 3 == 1) {
						printf("     [�����̋�F]    ������    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 3 == 2) {
						printf("     [�����̋�F]    �E����    ������                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
				}
				else if (point_up % 4 == 1) {
					if (point_right % 5 == 0) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("  �� [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 5 == 1) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    ��6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 5 == 2) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    ��8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 5 == 3) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    ��10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 5 == 4) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  ��12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
				}
				else if (point_up % 4 == 2) {
					if (point_right % 7 == 0) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("  �� [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 7 == 1) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    ��10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 7 == 2) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    ��20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 7 == 3) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    ��30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 7 == 4) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    ��40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 7 == 5) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    ��50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
					else if (point_right % 7 == 6) {
						printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
						printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
						printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    ��60�b            ["); printf(limTime); printf("]\n\n");
						printf("     back\n\n");
					}
				}
				else if (point_up % 4 == 3) {
					printf("     [�����̋�F]    �E����    �E����                                                    ["); printf(WorB); printf("]\n\n");
					printf("     [�ՖʃT�C�Y]    �E6�~6    �E8�~8    �E10�~10  �E12�~12                              ["); printf(field); printf("]\n\n");
					printf("     [ �҂����� ]    �E10�b    �E20�b    �E30�b    �E40�b    �E50�b    �E60�b            ["); printf(limTime); printf("]\n\n");
					printf("  �� back\n\n");
				}
				switch (_getch()) {
				case 'w':point_up--; break;
				case 's':point_up++; break;
				case 'd':point_right++; break;
				case 'a':point_right--; break;
				case '\r':
					if (point_up % 4 == 1) {
						if (point_right % 5 == 1) {
							field = "6�~6";
							FIELD_HEIGHT = 8;
							FIELD_WIDTH = 8;
						}
						else if (point_right % 5 == 2) {
							field = "8�~8";
							FIELD_HEIGHT = 10;
							FIELD_WIDTH = 10;
						}
						else if (point_right % 5 == 3) {
							field = "10�~10";
							FIELD_HEIGHT = 12;
							FIELD_WIDTH = 12;
						}
						else if (point_right % 5 == 4) {
							field = "12�~12";
							FIELD_HEIGHT = 14;
							FIELD_WIDTH = 14;
						}
					}
					else if (point_up % 4 == 2) {
						if (point_right % 7 == 1) {
							limTime = "10�b";
							wait_time = 10;
						}
						else if (point_right % 7 == 2) {
							limTime = "20�b";
							wait_time = 20;
						}
						else if (point_right % 7 == 3) {
							limTime = "30�b";
							wait_time = 30;
						}
						else if (point_right % 7 == 4) {
							limTime = "40�b";
							wait_time = 40;
						}
						else if (point_right % 7 == 5) {
							limTime = "50�b";
							wait_time = 50;
						}
						else if (point_right % 7 == 6) {
							limTime = "60�b";
							wait_time = 60;
						}
					}
					else if (point_up % 4 == 3) {
						mode = MODE::MENU;
					}
					else if (point_up % 4 == 0) {
						if (point_right % 3 == 1) {
							WorB = "��";
							w_b = true;
						}
						else if (point_right % 3 == 2) {
							WorB = "��";
							w_b = false;
						}
					}
					break;
				}
				if (mode != MODE::OPTION) {
					break;
				}

			}
		}
	}
}