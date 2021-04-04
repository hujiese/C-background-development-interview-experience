#pragma once
#include <iostream>
#include <memory>

using namespace std;

class SingleLazyObject
{
public:
	static shared_ptr<SingleLazyObject> getInstance();
	void showMessage();
	virtual ~SingleLazyObject();

private:
	SingleLazyObject();
	static shared_ptr<SingleLazyObject> instance;
};