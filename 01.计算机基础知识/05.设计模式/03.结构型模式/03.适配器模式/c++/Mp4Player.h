#pragma once
#include "AdvancedMediaPlayer.h"
#include <iostream>
using namespace std;

class Mp4Player :
	public AdvancedMediaPlayer
{
public:
	Mp4Player();
	virtual void playVlc(string fileName);
	virtual void playMp4(string fileName);
	virtual ~Mp4Player();
};

