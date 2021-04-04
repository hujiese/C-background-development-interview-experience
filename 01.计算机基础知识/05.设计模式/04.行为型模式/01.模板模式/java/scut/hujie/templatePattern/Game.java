package scut.hujie.templatePattern;

public abstract class Game {
	abstract void initialize();

	abstract void startPlay();

	abstract void endPlay();

	// template
	public final void play() {

		//initialize game
		initialize();

		//start game 
		startPlay();

		//end game
		endPlay();
	}
}
