#include "AudioPlayer.h"


AudioPlayer::AudioPlayer()
{
}

void AudioPlayer::play(string audioType, string fileName)
{
	//���� mp3 �����ļ�������֧��
	if (audioType == "mp3"){
		cout << "Playing mp3 file. Name: " << fileName << endl;
	}
	//mediaAdapter �ṩ�˲��������ļ���ʽ��֧��
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
