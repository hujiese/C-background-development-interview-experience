#pragma once
#include "DrawAPI.h"
#include <iostream>
using namespace std;

class GreenCircle :
	public DrawAPI
{
public:
	GreenCircle();
	virtual void drawCircle(int radius, int x, int y);
	virtual ~GreenCircle();
};

