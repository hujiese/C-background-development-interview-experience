#pragma once
#include "Game.h"
#include <iostream>
using namespace std;

class Cricket :
	public Game
{
public:
	Cricket();
	void initialize();
	void startPlay();
	void endPlay();
	virtual ~Cricket();
};

