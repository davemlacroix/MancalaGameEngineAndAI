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

class DescisionTreeAI: public AI
{
private:
	bool player;
	static char name[30];
	GameState ResultState;
	int depth;													//how deep in the descision tree the AI will go
public:
	DescisionTreeAI(bool whichPlayer, int turnsAhead);
	int findMove(GameState State);
	const static char *getName();

	int searchMoves(GameState State, int move, int depth);		//the recursive emulation of a descision tree
	int getDepth();
};


char DescisionTreeAI::name[30] = "DescisionTreeAI";

//
//constructor
//
DescisionTreeAI::DescisionTreeAI(bool whichPlayer, int turnsAhead)
{
	ResultState = GameState();
	depth = turnsAhead;
	player = whichPlayer;
}

//
//algorithm for next move
//
int DescisionTreeAI::findMove(GameState State)
{
	
	int results[6];
	GameState Results = GameState();		//hold onto ret
	
	//check all possible moves
	for(int i=0; i<6; i++)
	{
		if(State.tryMove(Results, i))
		{
			results[i] = searchMoves(State, i, getDepth()-1);
		}
		else
		{
			//should never be picked
			results[i] = -9999;				
		}

	}
	
	//priming
	int bestValue = results[0];
	int choice = 0;

	for(int i=1;  i<6; i++)
	{
		if(bestValue < results[i])
		{
			choice = i;
			bestValue = results[i];
		}
	}

	//return value of best choice for current player
	return choice;
}

//
//search for next move
//
int DescisionTreeAI::searchMoves(GameState State, int move, int depth)
{
	int results[6];
	GameState Results = GameState();		//hold onto ret
	GameState NewState = GameState();
	
	State.tryMove(NewState, move);
	
	
	if(NewState.isGameOver())
	{
			if(NewState.getStore(player) > NewState.getStore(!player))
			{
			//you win
				return 9999;
			}
			else if(NewState.getStore(player) > NewState.getStore(!player))
			{
			//you lose
				return -9999;
			}
	}

	//time to check the results
	if(depth == 0)
	{
		return (NewState.getStore(player) - NewState.getStore(!player));
	}

	
	//check all possible moves
	for(int i=0; i<6; i++)
	{
		if(NewState.tryMove(Results, i))
		{
			results[i] = searchMoves(Results, i, depth-1);
		}
		else
		{
			//should never be picked
			results[i] = -9999*(NewState.getTurn() == player) + 999;				
		}

	}
	
	//priming
	int bestValue = results[0];
	int choice = 0;

	if(NewState.getTurn() == player)
	{
		for(int i=1;  i<6; i++)
		{
			if(bestValue < results[i])
			{
				choice = i;
				bestValue = results[i];
			}
		}
	}
	else
	{
		for(int i=1;  i<6; i++)
		{
			if(bestValue > results[i])
			{
				choice = i;
				bestValue = results[i];
			}
		}
	}

	//return value of best choice for current player
	return bestValue;
}

const char *DescisionTreeAI::getName()
 {
	return name;
 }
int DescisionTreeAI::getDepth()
{
	return depth;
}
