#include "AudioPlayer.h"


AudioPlayer::AudioPlayer()
{
}

void AudioPlayer::play(string audioType, string fileName)
{
	//播放 mp3 音乐文件的内置支持
	if (audioType == "mp3"){
		cout << "Playing mp3 file. Name: " << fileName << endl;
	}
	//mediaAdapter 提供了播放其他文件格式的支持
	else if (audioType == "mp4" || audioType == "vlc")
	{
		mediaAdapter.reset(new MediaAdapter(audioType));
		mediaAdapter->play(audioType, fileName);
	}
	else
	{
		cout << "Invalid media. " << audioType << " format not supported" << endl;
	}
}

AudioPlayer::~AudioPlayer()
{
}
