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

class HumanPlayerAI: public AI
{
private:
	static char name[30];
	GameState ResultState;
public:
	HumanPlayerAI();
	int findMove(GameState State);
	const static char *getName();
};


char HumanPlayerAI::name[30] = "HumanPlayerAI";

//
//constructor
//
HumanPlayerAI::HumanPlayerAI()
{
	ResultState = GameState();
}

//
//algorithm for next move
//
int HumanPlayerAI::findMove(GameState State)
{
	int choice;
	while (true)
	{
		GameState Results = GameState();
		State.printState();
		cout << "Enter your next move (0 is closest to store, 5 is furthest from store):\n";

		cin >> choice;
		cin.ignore(250, '\n');

		if(choice >= 0 && choice <= 5)
		{
			if(State.tryMove(Results, choice))
			{
				return choice;
			}
			else
			{
				cout << "Invalid Move\n";
			}
		}
	}

	//i don't know how you would get here but...
	return -1;
}

const char *HumanPlayerAI::getName()
 {
	return name;
 }