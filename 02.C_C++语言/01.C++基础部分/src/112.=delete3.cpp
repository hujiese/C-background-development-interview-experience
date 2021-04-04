#include <iostream>
using namespace std;

class Widget {
public:
	template<typename T>
	void ProcessPointer(T* ptr) {
		cout << typeid (T).name() << endl;
	}
private:
	void ProcessPointer(void*) {
		cout << "void" << endl;
	}
};

int main(int argc, char *argv[]) {
	Widget w;
	int* ip = NULL;
	w.ProcessPointer(ip);

	void* vp = NULL;
	w.ProcessPointer(vp); //1	IntelliSense:  ���� "Widget::ProcessPointer(void *)" (������ ��������:11) ���ɷ���

	return 0;
}