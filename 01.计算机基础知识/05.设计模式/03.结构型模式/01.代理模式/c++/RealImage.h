#pragma once
#include "Image.h"
#include <string>
#include <iostream>
using namespace std;

class RealImage :
	public Image
{
public:
	RealImage();
	RealImage(string fileName);
	virtual void display();
	virtual ~RealImage();

protected:
	string fileName;
	void loadFromDisk(string fileName);
};

