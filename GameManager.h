// David LaCroix
// 11/3/2012
// Mancala AI Project - playGame function
//
#pragma once

#define PLAYER1 1
#define PLAYER2 0

#include <stdlib.h>
#include <iostream>

#include "GameState.h"
#include "AI.h"
#include "randomAI.h"

using namespace std;


class GameManager
{
private:
	GameState TheGame;							//the current gamee state.
	AI *Player1;								//the AI for player 1.
	AI *Player2;								//the AI for player 2.

	//stat gathering
	int numMoves;
	int numTurns;
	
	GameState & useState();										//returns a refrence to TheGame
	AI & useAI(bool player);									//returns a refrence to specified players AI
	void setAI(bool player, AI &playerAI);						//sets specified players AI
	void addTurn();												//increments numTurns
	void addMove();												//increments numMoves
public:	
	GameManager(AI &player1, AI &player2);
	GameManager(AI &player1, AI &player2, GameState State, int moves, int turns);
	int playMove(int move = -1);								//plays out 1 move(1 = success, 0 = success,gameover, -1=error)
	int playGame();												//play out the game
	void newGame();												//starts a new game

	const AI &getAI(bool player) const;							//returns a constant refrence to specified AI
	GameState getState() const;									//returns a copy of the TheGame
	int getNMoves() const;										//returns number of moves this game
	int getNTurns() const;										//returns number of turns this game
};

//
// creates a standard mancala game
//
//preconditions: AI != NULL
//
//postconditions: none
GameManager::GameManager(AI &player1, AI &player2)
:	numMoves(0),
	numTurns(0)
{
	setAI(PLAYER1, player1);
	setAI(PLAYER2, player2);
	TheGame = GameState();
}

GameManager::GameManager(AI &player1, AI &player2, GameState State, int moves, int turns)
:	numMoves(moves),
	numTurns(turns)
{
	setAI(PLAYER1, player1);
	setAI(PLAYER2, player2);
	TheGame = State;
}

//
//set the AI for specified player
//
void GameManager::setAI(bool player, AI &playerAI)
{
	if(player == PLAYER1)
	{
		Player1 = &playerAI;
	}
	else
	{
		Player2 = &playerAI;
	}
}

//
//play out a move
//
int GameManager::playMove(int move)
{

	bool turn;							//only used for number of turns stat

	//to keep track of turns passed
	turn = getState().getTurn();

	//ask the AI or just move?
	if(move >= 0)
	{
		if(!useState().move(move))
		{
			cerr << "The Player1 AI made an invalid move\n";
			return -1;
		}
	}
	else
	{
		//make a move
		if(getState().getTurn() == PLAYER1)
		{
			//player 1 makes a move
			if(!useState().move(useAI(PLAYER1).findMove(getState())))
			{
				cerr << "The Player1 AI made an invalid move\n";
				return -1;
			}
		}
		else
		{
			//player 2 makes a move
			if(!useState().move(useAI(PLAYER2).findMove(getState())))
			{
				cerr << "The Player2 AI made an invalid move\n";
				return -1;
			}
		}

		if(turn != getState().getTurn())
			addTurn();

		addMove();

		if(getState().isGameOver())
		{
			//add one last turn?
			if(turn == getState().getTurn())
				addTurn();

			return 0;
		}
	}

	return 1;
}


//
//play out the game
//
int GameManager::playGame()
{
	//make sure the gaem isn't over
	if(getState().isGameOver())
	{
		cerr << "The game is alread over...\n";
		return -1;
	}

	//
	//play the game
	//
	int result = 1;
	while(result == 1)
	{
		//The best while statement ever ^_^
		result = playMove();
	}
	
	//check for abnormal return
	if(result == -1)
		return -1;

	//
	//determine the winner
	//
	if(getState().getStore(PLAYER1) > getState().getStore(PLAYER2))
	{
		//player1 wins
		return PLAYER1;
	}
	else if(getState().getStore(PLAYER1) < getState().getStore(PLAYER2))
	{
		//player2 wins
		return PLAYER2;
	}

	//it was a tie
	return 2;

};

//
// returns a constant refrence to the specified players AI
//
const AI &GameManager::getAI(bool player) const
{
	if(player == PLAYER1)
		return *Player1;
	return *Player2;
};

//
// returns a refrence to TheGame
//
GameState &GameManager::useState()
{
	return TheGame;
};

//
// returns a refrence to specified players AI
//
AI &GameManager::useAI(bool player)
{
	if(player == PLAYER1)
		return *Player1;
	return *Player2;
}

//
//returns a copy of the current game state
//
GameState GameManager::getState() const
{
	return TheGame;
}

//
//starts a new game
//
void GameManager::newGame()
{
	TheGame = GameState();
	numTurns = 0;
	numMoves = 0;
}

//
//increments number of turns
//
void GameManager::addTurn()
{
	numTurns++;
}

//
//increments number of moves
//
void GameManager::addMove()
{
	numMoves++;
}

//
//returns the number of turns played so far
//
int GameManager::getNTurns() const
{
	return numTurns;
}

//returns the number of moves made so far
int GameManager::getNMoves() const
{
	return numMoves;
}