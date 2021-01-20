#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>




namespace name_build {

	class BUILD {
	public:
		void rest();
		void weapon();
	};

}


namespace enemy_disp {
	class ENEMY {
	public:
		void section(int i);
		void battle_disp();
		void monster1();
		void monster2();
		void monster3();
	};
}