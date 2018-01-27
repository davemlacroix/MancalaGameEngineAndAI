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

class MyAI: public AI
{
private:
	bool player;
	static char name[30];
	GameState ResultState;
	int depth;													//how deep in the descision tree the AI will go
public:
	MyAI(bool whichPlayer, int turnsAhead);
	int findMove(GameState State);
	const static char *getName();

	int searchMoves(GameState State, int move, int depth);		//the recursive emulation of a descision tree
	int getDepth();
};


char MyAI::name[30] = "MyAI";

//
//constructor
//
MyAI::MyAI(bool whichPlayer, int turnsAhead)
{
	ResultState = GameState();
	depth = turnsAhead;
	player = whichPlayer;
}

//
//algorithm for next move
//
int MyAI::findMove(GameState State)
{
	
	int results[6];
	GameState Results = GameState();		//hold onto ret

	//if you can go again, do that first
	for(int i=0; i<6; i++)
	{
		if(State.tryMove(Results, i))
		{
			if(State.getTurn() == Results.getTurn())
				{
			return i;

			}
		}
	}
	
	//check all possible moves
	for(int i=0; i<6; i++)
	{
		if(State.getPit(State.getTurn(), i) != 0)
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

	for(int i=0;  i<6; i++)
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
int MyAI::searchMoves(GameState State, int move, int depth)
{
	int results[6];
	GameState Results = GameState(State);		//hold onto ret

	State.move(move);

	if(State.getTurn() == Results.getTurn())
	{	Results.printState();
	State.printState();
	}

	if(State.isGameOver())
	{
			if(State.getStore(player) > State.getStore(!player))
			{
			//you win
				return 99;
			}
			else if(State.getStore(player) < State.getStore(!player))
			{
			//you lose
				return -99;
			}
			else
			{
				return 0;
			}
	}
	
	//time to check the results
	if(depth == 0)
	{
		return (State.getStore(player) - State.getStore(!player));
	}

	//if you can go again, do that first
	bool goAgain = 1;
	while(goAgain)
	{
		goAgain = 0;
		for(int i=0; i<6; i++)
		{
			if(State.tryMove(Results, i))
			{
				if(State.getTurn() == Results.getTurn())
				{

				State.move(i);
				goAgain = 1;
				break;
				}
			}
		}
	}
		if(State.isGameOver())
	{
			if(State.getStore(player) > State.getStore(!player))
			{
			//you win
				return 99;
			}
			else if(State.getStore(player) < State.getStore(!player))
			{
			//you lose
				return -99;
			}
			else
			{
				return 0;
			}
	}
	//check all possible moves
	for(int i=0; i<6; i++)
	{
		if(State.getPit(State.getTurn(), i) != 0)
		{
			results[i] = searchMoves(State, i, depth-1);
		}
		else
		{
			//should never be picked
			results[i] = -9999*(State.getTurn() == player) + 999;				
		}

	}
	
	//priming
	int bestValue = results[0];
	int choice = 0;

	if(State.getTurn() == player)
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

const char *MyAI::getName()
 {
	return name;
 }
int MyAI::getDepth()
{
	return depth;
}
