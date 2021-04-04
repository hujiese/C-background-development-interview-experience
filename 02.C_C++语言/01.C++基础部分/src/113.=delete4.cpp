#include <iostream>
using namespace std;

class Widget {
public:
	template<typename T>
	void ProcessPointer(T* ptr) {
		cout << typeid (T).name() << endl;
	}
	void ProcessPointer(void*) = delete;
};

int main(int argc, char *argv[]) {
	Widget w;
	int* ip = NULL;
	w.ProcessPointer(ip);

	void* vp = NULL;
	w.ProcessPointer(vp); //	1	IntelliSense:  �޷����� ���� "Widget::ProcessPointer(void *)" (������ ��������:10) -- ������ɾ���ĺ���
	return 0;
}