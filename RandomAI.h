// David LaCroix
// 11/3/2012
// Mancala AI Project - Abstract AI Class
//
#pragma once

#define PLAYER1 1
#define PLAYER2 0

#include <stdlib.h>
#include <iostream>
#include <random>
#include "AI.h"
#include "GameState.h"

using namespace std;

class RandomAI: public AI
{
private:
	static char name[30];
	GameState ResultState;
public:
	RandomAI(int seed = 0);
	int findMove(GameState State);
	const static char *getName();
};


char RandomAI::name[30] = "RandomAI";

//
//constructor
//
RandomAI::RandomAI(int seed)
{
	ResultState = GameState();
	srand(seed);
}

//
//algorithm for next move
//
int RandomAI::findMove(GameState State)
{
	//could be more efficient
	while (true)
	{
		int randMove = rand()%6;	

		if(State.tryMove(ResultState, randMove))
		{
			return randMove;
			break;
		}
	}
	//this wasn't truly random =/
	/*for(int i=0; i<6; i++)
	{

		if(State.tryMove(ResultState, randMove))
		{
			return randMove;
		}
		
		randMove++;
		if(randMove > 5)
			randMove = 0;
	}*/

	//there are no possible moves, the game is over
	return -1;
}

const char *RandomAI::getName()
 {
	return name;
 }