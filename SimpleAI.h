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

class SimpleAI: public AI
{
private:
	static char name[30];
	GameState ResultState;
public:
	SimpleAI();
	int findMove(GameState State);
	const static char *SimpleAI::getName();
};


char SimpleAI::name[30] = "SimpleAI";

//
//constructor
//
SimpleAI::SimpleAI()
{
	ResultState = GameState();
}

//
//move finding algorithm
//
int SimpleAI::findMove(GameState State)
{
	int move = -1;
	
	while(move++ < 5)
	{
		if(State.tryMove(ResultState, move))
		{
			return move;
		}
	}	

	return -1;
}

const char *SimpleAI::getName()
 {
	return name;
 }