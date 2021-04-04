package scut.hujie.observerPattern2;

import scut.hujie.observerPattern2.Observer;

public class S_Observer extends Observer {

	@Override
	public void update(String msg) {
		System.out.println(S_Observer.class.getName() + " : " + msg);
	}
}
