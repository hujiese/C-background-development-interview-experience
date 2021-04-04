#include <iostream>
#include <string>
using namespace std;

int main(void)
{
	string str = "PATZJUJZTACCBCC";
	// 这里我们需要用一个二维数组dp来作为备忘录，记录子问题的结果，以便重复的计算。这也是动态规划的精髓所在。不过这种做法的算法复杂度也是O(n^2)
	int result[15][15] = { 0 }; // result[i][j] 保存字符串下标 i-j 之间字符串是否为回文，如果为1则表示 str.sbustr(i, j)部分为回文

	for (int i = 0; i < 15; ++i)
		result[i][i] = 1;

	int max = 1;
	int flag = 0;
	// 这种方法十分粗暴，依次找长度为L = 3、4、5...15长度的回文
	for (int L = 3; L < 15; ++L)
	{
		for (int i = 0; i + L - 1 < 15; ++i) // L为窗口大小（回文长度），第一个窗口内的数为 0-2 ，不断移动窗口，找到长度为L的回文并标记
		{
			int j = i + L - 1;
			if (str[i] == str[j] && result[i + 1][j - 1] == 1)
			{
				result[i][j] = 1;
				max = L;
				flag = i; // 记录最长回文的起始位置，结束位置为i + L - 1
			}
		}
	}

	cout << "最长回文长度为 : " << max << endl; // 9
	cout << "最长回文为: " << str.substr(flag, flag + max - 1) << endl; // ATZJUJZTA
	return 0;
}