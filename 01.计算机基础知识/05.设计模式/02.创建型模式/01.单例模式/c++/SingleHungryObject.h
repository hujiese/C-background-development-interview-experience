#pragma once
#include <iostream>
#include <memory>

using namespace std;

class SingleHungryObject
{
public:
	static shared_ptr<SingleHungryObject> getInstance();
	void showMessage();
	virtual ~SingleHungryObject();

private:
	SingleHungryObject();
	static shared_ptr<SingleHungryObject> instance;
};

