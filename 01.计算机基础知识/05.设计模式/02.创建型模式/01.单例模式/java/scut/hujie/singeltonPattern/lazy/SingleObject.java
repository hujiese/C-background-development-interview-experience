package scut.hujie.singeltonPattern.lazy;

public class SingleObject {
	// ���� SingleObject ��һ������
	private static SingleObject instance = null;

	// �ù��캯��Ϊ private����������Ͳ��ᱻʵ����
	private SingleObject() {
	}

	// ��ȡΨһ���õĶ���
	public static SingleObject getInstance() {
		if(instance == null) {
			instance = new SingleObject();
		}
		return instance;
	}

	public void showMessage() {
		System.out.println("Hello World!");
	}
}
