#include <iostream>
#include <vector>
using namespace std;

class App
{
public:
	App();
	static App* theApp;
};

// error LNK2001: 无法解析的外部符号 "public: static class App * App::theApp"
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