package scut.hujie.observerPattern2;

import java.util.ArrayList;
import java.util.List;

public class Subject {
	private List<Observer> observers = new ArrayList<>(); // ״̬�ı�

	public void setMsg(String msg) {
		notifyAll(msg);
	}

	// ����
	public void addAttach(Observer observer) {
		observers.add(observer);
	}

	// ֪ͨ���ж��ĵĹ۲���
	private void notifyAll(String msg) {
		for (Observer observer : observers) {
			observer.update(msg);
		}
	}
}
