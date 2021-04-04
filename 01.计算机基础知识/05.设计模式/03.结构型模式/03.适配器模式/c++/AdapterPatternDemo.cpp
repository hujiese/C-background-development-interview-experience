#include "MediaPlayer.h"
#include "AdvancedMediaPlayer.h"
#include "VlcPlayer.h"
#include "Mp4Player.h"
#include "MediaAdapter.h"
#include "AudioPlayer.h"
using namespace std;

int main(void)
{
	shared_ptr<AudioPlayer> audioPlayer(new AudioPlayer());

	audioPlayer->play("mp3", "beyond the horizon.mp3");
	audioPlayer->play("mp4", "alone.mp4");
	audioPlayer->play("vlc", "far far away.vlc");
	audioPlayer->play("avi", "mind me.avi");

	return 0;
}