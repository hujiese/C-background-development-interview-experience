#include <iostream>
#include <vector>
using namespace std;

class App
{
public:
	App();
	static App* theApp;
};

// error LNK2001: �޷��������ⲿ���� "public: static class App * App::theApp"
// App* App::theApp = nullptr;

App::App()
{
	theApp = this;
}

App* AfxGetApp()
{
	return App::theApp;
}

int main(void)
{
	App MyObj;
	return 0;
}