## 适配器模式 ##

Adapter模式也叫适配器模式，是构造型模式之一，通过Adapter模式可以改变已有类（或外部类）的接口形式。

### 一、角色和职责 ###

![](https://i.imgur.com/TGJmESH.png)

将一个类的接口转换成客户希望的另外一个接口。使得原本由于接口不兼容而不能一起工作的那些类可以一起工作。

### 二、案例 ###
```cpp
#include <iostream>
using namespace std;

// Current18 
// Current220
// Adapter 

class  Current18v
{
public:
	virtual void useCurrent18v() = 0;
};

class  Current220v
{
public:
	void useCurrent220v()
	{
		cout << "我是220v 欢迎使用" << endl;
	}
};

class Adapter : public Current18v
{
public:
	Adapter(Current220v *current) :m_current(current)
	{
	}

	virtual void useCurrent18v()
	{
		cout << "适配器 适配 220v " << endl;
		m_current->useCurrent220v();
	}
private:
	Current220v *m_current;
};


int main(void)
{
	Current220v		*current220v = new Current220v();
	Adapter			*adapter = new Adapter(current220v);

	adapter->useCurrent18v();

	delete current220v ;
	delete adapter;

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/m6NypZW.png)

### 三、案例扩展 ###

有一个 MediaPlayer 接口和一个实现了 MediaPlayer 接口的实体类 AudioPlayer。默认情况下，AudioPlayer 可以播放 mp3 格式的音频文件。

还有另一个接口 AdvancedMediaPlayer 和实现了 AdvancedMediaPlayer 接口的实体类。该类可以播放 vlc 和 mp4 格式的文件。

想要让 AudioPlayer 播放其他格式的音频文件。为了实现这个功能，需要创建一个实现了 MediaPlayer 接口的适配器类 MediaAdapter，并使用 AdvancedMediaPlayer 对象来播放所需的格式。

AudioPlayer 使用适配器类 MediaAdapter 传递所需的音频类型，不需要知道能播放所需格式音频的实际类。AdapterPatternDemo，演示类使用 AudioPlayer 类来播放各种格式。

![](https://i.imgur.com/i6KSYNS.jpg)