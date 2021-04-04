package scut.hujie.observerPattern2;

public class Main {

	public static void main(String[] args) {
		F_Observer fObserver = new F_Observer();
		S_Observer sObserver = new S_Observer();
		T_Observer tObserver = new T_Observer();
		Subject subject = new Subject();
		subject.addAttach(fObserver);
		subject.addAttach(sObserver);
		subject.addAttach(tObserver);
		subject.setMsg("msg change");
	}

}
