#pragma once
#include "DrawAPI.h"
#include <iostream>
using namespace std;

class RedCircle :
	public DrawAPI
{
public:
	RedCircle();
	virtual void drawCircle(int radius, int x, int y);
	virtual ~RedCircle();
};

