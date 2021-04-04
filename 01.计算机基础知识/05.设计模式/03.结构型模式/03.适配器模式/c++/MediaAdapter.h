#pragma once
#include <memory>
#include "MediaPlayer.h"
#include "AdvancedMediaPlayer.h"
#include "VlcPlayer.h"
#include "Mp4Player.h"
using namespace std;

class MediaAdapter :
	public MediaPlayer
{
public:
	MediaAdapter();
	MediaAdapter(string audioType);
	void play(string audioType, string fileName);
	virtual ~MediaAdapter();

protected:
	shared_ptr<AdvancedMediaPlayer> advancedMusicPlayer;
};

