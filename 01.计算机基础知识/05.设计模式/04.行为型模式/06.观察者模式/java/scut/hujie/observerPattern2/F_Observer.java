package scut.hujie.observerPattern2;

import scut.hujie.observerPattern2.Observer;

public class F_Observer extends Observer {
	
	@Override
	public void update(String msg) {
		System.out.println(F_Observer.class.getName() + " : " + msg);
	}
}
