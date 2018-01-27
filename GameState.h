// David LaCroix
// 11/3/2012
// Mancala AI Project - GameState Class
//
#pragma once

#define PLAYER1 1
#define PLAYER2 0

#include <stdlib.h>
#include <iostream>

using namespace std;

class GameState 
{
private:		
	bool isP1Turn;					//is true when it is player 1's turn
	char stores[2];					//The number of pebbles in each players store (0->P1, 1->P2)
	char pits[12];					//The number of pebbles in each pit. pit[0] is in top left and the count proceeds clockwise around the board.
	
	void endTurn();														//ends the players turn
	void setPit(bool player, int pit, int value);						//sets the value of a pit
	void setStore(bool player, int value);								//sets the value of a store
	void addPebble(bool player, int pit);								//add a pebble to a pit
	void addPebble(bool player);										//add a pebble to a store
	void copy(const GameState &state);									//conform to specified gamestate.

public:
	GameState();														//constructor for the default gamestate -> a new game
	GameState(const GameState &copy);
	GameState(bool isP1Turn, const char* stores, const char* pits);		//constructor for a specified gamestate

	bool move(int choice);												//makes a legal move
	bool tryMove (GameState &state, int choice) const;					//show the results of a potential move

	void printState() const;													//prints the state of the game

	//get functions
	bool getTurn() const;												//returns thes state of isP1Turn
	char getPit(bool player, char pit) const;							//returns the number of pebbles in a pit. valid pits are 0-5.
	char getStore(bool player) const;									//returns the number of pebbles in a store
	int getP1Lead() const;												//returns the difference between player1's store and player2's store
	bool isGameOver() ;//returns whether or not the game is over

};

//
//default constructor
//
GameState::GameState()										
	:isP1Turn(true)
{
	setStore(PLAYER1, 0);
	setStore(PLAYER2, 0);
	
	for(int i=0; i<6; i++)
	{
		setPit(PLAYER1, i, 4);
		setPit(PLAYER2, i, 4);
	}
};

//
//constructor
//
GameState::GameState(bool playerTurn, const char* stores, const char* pits)
	:isP1Turn(playerTurn)
{
	setStore(PLAYER1, stores[PLAYER1]);
	setStore(PLAYER2, stores[PLAYER2]);

	for(int i=0; i<6; i++)
	{
		setPit(PLAYER1, i, pits[i]);
		setPit(PLAYER2, i, pits[6+i]);
	}
};

//
//copy constructor
//
GameState::GameState(const GameState &copy)
	:isP1Turn(copy.getTurn())
{
	setStore(PLAYER1, copy.getStore(PLAYER1));
	setStore(PLAYER2, copy.getStore(PLAYER2));

	for(int i=0; i<6; i++) 
	{
		setPit(PLAYER1, i, copy.getPit(PLAYER1, i));
		setPit(PLAYER2, i, copy.getPit(PLAYER2, i));
	}

};

//
//make a valid move
//
//preconditions: none
//
//postconditions:	0 is returned on invalid move
//					1 is returned if move was completed. The GameState is updated accordingly
bool GameState::move(int choice)
{
	GameState result = GameState();
	if(tryMove(result, choice))
	{
		copy(result);
		return 1;
	}

	cerr << "Invalid move: choice = " << choice << endl;
	char c;
	cin >> c;
	return 0;
};

//
//try a valid move
//
//preconditions: none
//
//postconditions:	0 is returned on invalid move
//					1 is returned if move was completed. The GameState passed by reference is updated accordingly.
bool GameState::tryMove(GameState &state, int choice) const
{
	//declare variables
	bool side;								//true if P1's side of board is being used.
	bool inStore = false;					//true if the last pebble was dropped in the players store.
	int pebbles;							//the number of pebbles the player is holding.
	int pit;								//the current pit being used.
		

	//validate input
	if(choice > 5 || choice < 0)
		return 0;
	
	//validate move choice
	pebbles = getPit(getTurn(), choice);
	if(!pebbles)
		return 0;
	
	//copy the current gamestate
	state = GameState(*this);	

	//
	//move those pebbles
	//
	pit = choice;
	side = getTurn();

	//remove pebbles from pit player is playing
	state.setPit(getTurn(), choice, 0);
	while(pebbles > 0)	
	{
		while(pebbles > 0 && pit > 0)
		{
			pit--;
			pebbles--;
			state.addPebble(getTurn(), pit);
		}
		if(pebbles > 0)
		{
			pebbles--;
			state.addPebble(getTurn());
			inStore = true;
			if(pebbles > 0)
			{
				//change side of board
				side = !side;
				pit = 6;
				//this is not the last pit
				inStore = false;
			}
		}
		while( pebbles > 0 && pit > 0)	//tests right to left (if out of pits won't subtract pebble)
		{
			pit--;
			pebbles--;
			state.addPebble(!getTurn(), pit);
		}
		
		//skip the opponents pit
		if(pebbles > 0)
		{
			//change side of board
			side = !side;
			pit = 6;
		}
	}

	//if the last pebble was dropped on the players side and not in his store
	//AND there were no pebbles in that pit
	if( !inStore && state.getPit(getTurn(), pit) == 1 && side == getTurn())
	{
		//steal the oppents pebbles on the other side
		state.setStore(getTurn(), state.getStore(getTurn()) + 1 + state.getPit(!getTurn(), 5-pit));
		state.setPit(!getTurn(), 5-pit, 0);
		state.setPit(getTurn(), pit, 0);
	}

	//change turns if the last pebble dropped was not in the players store
	if(!inStore)
	{
		state.endTurn();
	}


	return 1;
};

//
//get players turn
//
bool GameState::getTurn() const
{
	return isP1Turn;
};

//
//get the value of a pit
//
char GameState::getPit(bool player, char pit) const
{
	if (pit < 0 || pit > 5)
	{
		cerr << "Invalid argument for funtion: char GameState::getPit(bool player, char pit) const\n";
		cerr << "player = " << player << endl;
		cerr << "pit = " << pit << endl;

		return -1;
	}
	else
	{
		return pits[player*6 + pit];
	}
};
	
//
//get the value of a store
//
char GameState::getStore(bool player) const
{
	return stores[player];
};

//
//end a players turn
//
void GameState::endTurn()
{
		isP1Turn = !isP1Turn;
}

//
//Set the value of a pit
//
void GameState::setPit(bool player, int pit, int value)
{
	if(pit < 0 || pit > 5 || value < 0 || value > 48)
	{
		cerr << "Invalid argument for function: void GameState::setPit(bool player, int pit, char value)\n";
		cerr << "player = " << player << endl;
		cerr << "pit = " << pit << endl;
		cerr << "value = " << value << endl;
	}
	else
	{
		pits[player*6 + pit] = value;
	}
};

//
//set the value of a store
//
void GameState::setStore(bool player, int value)
{
	if (value < 0 || value > 48)
	{
		cerr << "Invalid argument for funtion: void GameState::setStore(bool player, char value)\n";
		cerr << "player = " << player << endl;
		cerr << "value = " << value << endl;
	}
	else
	{
		stores[player] = value;
	}
};

//
//add a pebble to a pit
//
void GameState::addPebble(bool player, int pit)
{
	if (pit < 0 || pit > 5)
	{
		cerr << "Invalid argument for funtion: void GameState::addPebble(bool player, int pit)\n";
		cerr << "player = " << player << endl;
		cerr << "pit = " << pit << endl;
	}
	else
	{
		setPit(player, pit, getPit(player, pit) + 1);
	}
};

//
//add a pebble to a store
//
void GameState::addPebble(bool player)
{
	setStore(player, getStore(player) + 1);
};

//
//copy another gamestate
//
void GameState::copy(const GameState &tcopy)
{
	isP1Turn = tcopy.getTurn();

	this->setStore(PLAYER1, tcopy.getStore(PLAYER1));
	this->setStore(PLAYER2, tcopy.getStore(PLAYER2));

	for(int i=0; i<6; i++) 
	{
		this->setPit(PLAYER1, i, tcopy.getPit(PLAYER1, i));
		this->setPit(PLAYER2, i, tcopy.getPit(PLAYER2, i));
	}
}
//
//returns the difference between player1's store and player2's store
//
int GameState::getP1Lead() const
{
	return (getStore(PLAYER1) - getStore(PLAYER2));
}

//
//returns 1 if game is over
//
bool GameState::isGameOver()
{
	int pitsumP1 = 0;
	int pitsumP2 = 0;


	//are all of one players pits empty?
	for(int i=0; i<6; i++)
	{
		pitsumP1 += getPit(PLAYER1, i);
		pitsumP2 += getPit(PLAYER2, i);
	}

	if(pitsumP1 == 0 || pitsumP2 == 0)
	{
		//clean up pits
		for(int i=0; i<6; i++)
		{
			setStore(PLAYER1, getStore(PLAYER1) + getPit(PLAYER1, i));
			setPit(PLAYER1, i,  0);
			setStore(PLAYER2, getStore(PLAYER2) + getPit(PLAYER2, i));
			setPit(PLAYER2, i, 0);
		}
		return 1;
	}

	//does someone have more than half of the pits?
	if(getStore(PLAYER1) > 24 || getStore(PLAYER2) > 24)
	{
		return 1;
	}
	//cout << "Total pebbles in pits " << pitsumP1 << "+" << pitsumP2 << "and total is " << pitsumP1 + pitsumP2 + getStore(PLAYER1) + getStore(PLAYER2) << endl;
	return 0;
}

//
//prints the state of the game
//

void GameState::printState() const
{
	char carray[30];
	cout << "Current Board State:\n    ";
	for(int i=0; i<6; i++)
	{
		cout << "(" << _itoa(getPit(PLAYER2, i),carray,10) << ")  ";
	}
	cout << endl;

	cout << "(" << _itoa(getStore(PLAYER2),carray,10) << ")                             (";
	cout << _itoa(getStore(PLAYER1),carray,10) << ")\n";
	
	cout << "    ";
	for(int i=5; i>=0; i--)
	{
		cout << "(" << _itoa(getPit(PLAYER1, i),carray,10) << ")  ";
	}
	cout << endl;
}