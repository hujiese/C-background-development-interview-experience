#pragma once
#include "Game.h"
#include <iostream>
using namespace std;

class Football :
	public Game
{
public:
	Football();
	void initialize();
	void startPlay();
	void endPlay();
	virtual ~Football();
};

