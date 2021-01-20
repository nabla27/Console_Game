#include "build.h"

using namespace name_build;
using namespace enemy_disp;

void BUILD::rest() {
	while (1) {
		system("cls");
		printf("rest\n");
	}
}

void BUILD::weapon() {
	while (1) {
		system("cls");
		printf("wepon shop\n");
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



void ENEMY::section(int i) {
	while (1) {
		printf("cls");
		if (i == 0) {
			monster1();
		}
		else if (i == 1) {
			monster2();
		}
		else if (i == 2) {
			monster3();
		}

		battle_disp();
	}
}


/*****************************************************************************************************/


void battle_disp() {
	printf("\n\n");
	printf("battle section display\n");
}