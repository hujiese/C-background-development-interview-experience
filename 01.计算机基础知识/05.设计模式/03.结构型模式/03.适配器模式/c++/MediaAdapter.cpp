#include "MediaAdapter.h"


MediaAdapter::MediaAdapter()
{
}

MediaAdapter::MediaAdapter(string audioType)
{
	if (audioType == "vlc"){
		advancedMusicPlayer.reset(new VlcPlayer());
	}
	else if (audioType == "mp4"){
		advancedMusicPlayer.reset(new Mp4Player());
	}
}

void MediaAdapter::play(string audioType, string fileName)
{
	if (audioType == "vlc"){
		advancedMusicPlayer->playVlc(fileName);
	}
	else if (audioType == "mp4"){
		advancedMusicPlayer->playMp4(fileName);
	}
}

MediaAdapter::~MediaAdapter()
{
}
