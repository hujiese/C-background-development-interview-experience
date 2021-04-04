#pragma once
#include "DrawAPI.h"
#include <iostream>
#include <memory>
using namespace std;

class Shape
{
public:
	Shape();
	virtual void draw();
	virtual ~Shape();

protected:
	shared_ptr<DrawAPI> drawAPI;

	Shape(shared_ptr<DrawAPI> drawAPI);
};

