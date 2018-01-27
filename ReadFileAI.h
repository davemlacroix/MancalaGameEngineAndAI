// David LaCroix
// 11/3/2012
// Mancala AI Project - Abstract AI Class
//
#pragma once

#define PLAYER1 1
#define PLAYER2 0

#include <fstream>
#include <string>
#include <stdlib.h>
#include <iostream>
#include "AI.h"
#include "GameState.h"


using namespace std;

class ReadFileAI: public AI
{
private:
	ifstream inFile;
	static char name[30];
	GameState ResultState;
public:
	ReadFileAI(char *fileName);
	~ReadFileAI();
	int findMove(GameState State);
	const static char *ReadFileAI::getName();
};


char ReadFileAI::name[30] = "ReadFileAI";

//
//constructor
//
ReadFileAI::ReadFileAI(char *fileName)
{
	ResultState = GameState();
	inFile.open(fileName);
}

//
//destructor
//

ReadFileAI::~ReadFileAI()
{
	inFile.close();
}

//
//move finding algorithm
//
int ReadFileAI::findMove(GameState State)
{
	char line[30];
	int result = 1;
	
	if(!inFile.eof())
	{
		inFile >> line;
		inFile.ignore(250, '\n');
	}
	else
	{
		cerr << "The file has ended\n";
	}
	while(result == 1)
	{
		result = 1;
		if(strcmp(line, "Game Over"))
		{
			cout << 5-atoi(line) << endl;
			result = State.tryMove(ResultState, (5-atoi(line)));
			if(result == 1)
				return 5-atoi(line);
		}
		else
		{
			if(!inFile.eof())
			{
				inFile >> line;
				inFile.ignore(250,'\n');
				cout << "The Game Should Be Over.\n";
				result = 1;
			}
			else
			{
				cerr << "The file has ended\n";
			}
		}
	}

	//the file was innacurate
	return -1;
}

const char *ReadFileAI::getName()
 {
	return name;
 }