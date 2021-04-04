#pragma once
#include "AdvancedMediaPlayer.h"
#include <iostream>
using namespace std;

class VlcPlayer :
	public AdvancedMediaPlayer
{
public:
	VlcPlayer();
	virtual void playVlc(string fileName);
	virtual void playMp4(string fileName);
	virtual ~VlcPlayer();
};

