#pragma once
#include <string>
using namespace std;
class Memento
{
public:
	explicit Memento();
	Memento(string state);
	string getState() const;
	virtual ~Memento();

private:
	string state;
};

