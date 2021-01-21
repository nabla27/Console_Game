#include "build.h"

using namespace name_build;
using namespace enemy_disp;

void BUILD::rest() {
	bool exit = false;
	while (1) {
		system("cls");
		printf("rest\n");

		//テスト用
		if (_kbhit()) {
			switch (_getch()) {
			case '\r':exit = true; break;
			}
		}

		if (exit == true) {
			break;
		}
	}
}

void BUILD::weapon() {
	bool exit = false;
	while (1) {
		system("cls");
		printf("wepon shop\n");

		//テスト用
		if (_kbhit()) {
			switch (_getch()) {
			case '\r':exit = true; break;
			}
		}

		if (exit == true) {
			break;
		}
	}
}


/******************************************************************************************************/

void ENEMY::monster1() {
	printf("monster1\n");
}

void ENEMY::monster2() {
	printf("monster2\n");
}

void ENEMY::monster3() {
	printf("monster3\n");
}

void ENEMY::boss1() {
	printf("boss1\n");
}




void ENEMY::battle_disp() {
	printf("\n\n\n");
	printf("battle test display");
}


void ENEMY::section(int i) {
	bool exit = false;
	while (1) {
		system("cls");
		if (i == 0) {
			monster1();
		}
		else if (i == 1) {
			monster2();
		}
		else if (i == 2) {
			monster3();
		}
		else if (i == 3) {
			boss1();
		}

		battle_disp();

		//テスト用
		if (_kbhit()) {
			switch (_getch()) {
			case '\r':exit = true; break;
			}
		}

		if (exit == true) {
			break;
		}
	}
}


/*****************************************************************************************************/


