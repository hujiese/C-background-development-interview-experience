#pragma once
#include <iostream>
using namespace std;

class Expression
{
public:
	Expression();
	virtual bool interpret(string context) = 0;
	virtual ~Expression();
};

