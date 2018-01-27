// David LaCroix
// 11/3/2012
// Mancala AI Project - Abstract AI Class
//
#pragma once

#define PLAYER1 1
#define PLAYER2 0

#include <stdlib.h>
#include <iostream>

using namespace std;

class AI
{								
public:																
	virtual int findMove(GameState State) = 0;		//returns the AI's next move
};


