#include <stdlib.h>
#include <random>
#include <iostream>
#include <fstream>

#include "GameState.h"
#include "AI.h"
#include "RandomAI.h"
#include "SimpleAI.h"
#include "StupidAI.h"
#include "MyAI.h"
#include "GameManager.h"
#include "ReadFileAI.h"
#include "HumanPlayerAI.h"
#include "DescisionTreeAI.h"


using namespace std;

#define SEED 0
#define GAMES_DEEP 3 //between 0 and 8 ONLY

AI *chooseAI(bool player, char *name, int arg3);

int main( int argc, char *argv[] )
{
	//the list of potential AI's
	
	//declare variables
	int numGames = 1;
	int gamesPlayed = 0;
	int result;
	int player1Wins = 0;
	int player2Wins = 0;
	int ties = 0;
	int errors = 0;
	long int totMoves = 0;
	long int totTurns = 0;
	ofstream log;
	log.open("log.dat", ios::app);
	//
	//grab arguments from command line
	//
	
	//validate number of arguments passed
	if(argc < 3)
	{
		cout << "Not enough arguments, this program takes:\n";
		cout << "Player1AI Player2AI NumberOfGames(optional)\n";
		return -1;
	}

	//check for game # specification
	if(argc > 5)
	{
		numGames = atoi(argv[5]);
	}

	//setup those AI
	AI *Player1 = chooseAI(PLAYER1, argv[1], atoi(argv[3]));	
	AI *Player2 = chooseAI(PLAYER2, argv[2], atoi(argv[4]));

	if(&Player1 == NULL || &Player2 == NULL)
		return -1;
	
	GameManager TheGame = GameManager(*Player1, *Player2);
	GameState Results = GameState();
	//
	//Play VS a Random Player or a human
	//
if(!strcmp(argv[1], "random") || !strcmp(argv[2], "random") || !strcmp(argv[1], "human") || !strcmp(argv[2], "human"))
	{
		while(gamesPlayed++ < numGames)
		{
			TheGame.newGame();

			result = TheGame.playGame();

			if(result == PLAYER1)
			{
				player1Wins++;
			}
			else if(result == PLAYER2)
			{
				player2Wins++;
			}
			else if(result == 2)
			{
				ties++;
			}
			else
			{
				cerr << "Something went wrong this game... " << gamesPlayed << endl;
				errors++;
			}
					
			cout << "Game " << gamesPlayed << ": result = " << result << endl;
			totMoves += TheGame.getNMoves();
			totTurns += TheGame.getNTurns();
		}
	}
	//
	//vs nonrandom and nonhuman
	//
	else
	{
		log.close();
		int moves[8] = {0,0,0,0,0,0,0,0};
		int moveNum = 0;
		GameState Hold = GameState();
		GameManager GameI = GameManager(*Player1, *Player2);

		while(moves[0] < 6)
		{
			//reset move number
			TheGame.newGame();
			
			for(moveNum=0; moveNum<GAMES_DEEP; moveNum++)
			{
				//add a game
				numGames++;

				if(TheGame.getState().tryMove(Hold, moves[moveNum]))
				{
					//make a move
					TheGame.playMove(moves[moveNum]);

					//play out the rest of the game
					GameI = GameManager(TheGame);
					result = GameI.playGame();
					gamesPlayed++;

					if(result == PLAYER1)
					{
						player1Wins++;
					}
					else if(result == PLAYER2)
					{
						player2Wins++;
					}
					else if(result == 2)
					{
						ties++;
					}
					else
					{
						cerr << "Something went wrong this game... " << gamesPlayed << endl;
						errors++;
					}

					cout << "Game " << gamesPlayed << ": total losses = " << player2Wins << endl;
					totMoves += GameI.getNMoves();
					totTurns += GameI.getNTurns();

				}
				else
				{
					//invalid move -- start over
					moves[moveNum]++;
					while(moveNum > 0)
					{
						if(moves[moveNum] > 5)
						{
							moves[moveNum] = 0;
							moves[moveNum-1]++;
						}
						moveNum--;
					}
					break;
				}
					if(moveNum == GAMES_DEEP-1)
					{
						moves[moveNum]++;
						while(moveNum > 0)
						{
							if(moves[moveNum] > 5)
							{
								moves[moveNum] = 0;
								moves[moveNum-1]++;
							}
							moveNum--;
						}
						break;
					}
			}

		}
	}



	//
	//output results
	//
	log.close();
	log.open("log.dat", ios::app);

	log << argv[1] << argv[3] << " v. " << argv[2] << argv[4] << endl;
	log << gamesPlayed-1 << " games were played.\n";
	log << "Player 1 Won " << player1Wins << " times.\n";
	log << "Player 2 Won " << player2Wins << " times.\n";
	log << "There were " << ties << " ties.\n";
	log << "There were " << errors << " this game\n\n";
	
	//log << "An average of " << float(totMoves)/(gamesPlayed-1) << " moves were made each game.\n";
	//log << "An average of " << float(totTurns)/(gamesPlayed-1) << " turns were taken each game.\n";
	log << endl << endl;

	log.close();

	return 0;
}

//
//AI Picker
//
AI *chooseAI(bool player, char *name, int arg3)
{
	if(!strcmp(name, "random"))
	{
		return new RandomAI();
	}
	else if(!strcmp(name, "simple"))
	{
		return new SimpleAI();
	}
	else if(!strcmp(name, "stupid"))
	{
		return new StupidAI();
	}
	else if(!strcmp(name, "human"))
	{
		return new HumanPlayerAI();
	}
	else if (!strcmp(name, "readfile"))
	{
		return new ReadFileAI("testSet.dat");
	}
	else if (!strcmp(name, "dtree"))
	{
		return new DescisionTreeAI(player, arg3);
	}
	else if (!strcmp(name, "myai"))
	{
		return new MyAI(player, arg3);
	}
	else if (!strcmp(name, "iterator"))
	{
		return new HumanPlayerAI();
	}
	else
	{
		cout <<"Invalid AI choice " << name << ".\n";
		return NULL;
	}
}

//
//game iterator
//
