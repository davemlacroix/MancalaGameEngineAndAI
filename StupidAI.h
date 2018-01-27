// David LaCroix
// 11/3/2012
// Mancala AI Project - Abstract AI Class
//
#pragma once

#define PLAYER1 1
#define PLAYER2 0

#include <string>
#include <stdlib.h>
#include <iostream>
#include "AI.h"
#include "GameState.h"

using namespace std;

class StupidAI: public AI
{
private:
	static char name[30];
	GameState ResultState;
public:
	StupidAI();
	int findMove(GameState State);
	const static char *StupidAI::getName();
};


char StupidAI::name[30] = "StupidAI";

//
//constructor
//
StupidAI::StupidAI()
{
	ResultState = GameState();
}

//
//move finding algorithm
//
int StupidAI::findMove(GameState State)
{
	int move = 6;
	
	while(move-- > 0)
	{
		if(State.tryMove(ResultState, move))
		{
			return move;
		}
	}	

	return -1;
}

const char *StupidAI::getName()
 {
	return name;
 }