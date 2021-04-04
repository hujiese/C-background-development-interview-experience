#pragma once
#include <string>
using namespace std;

class MediaPlayer
{
public:
	MediaPlayer();
	virtual void play(string audioType, string fileName) = 0;
	virtual ~MediaPlayer();
};

