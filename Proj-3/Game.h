#ifndef _GAME_H_
#define _GAME_H_

#include "Pieces.h"

class Game
{
public:
	Board brd;
	int activePl;
	Game(){}
	Game(int size){
		brd = Board(size,"textures/Border.jpg","textures/Border.jpg");
		activePl = 0;
	}
};


#endif
