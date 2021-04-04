#include <iostream>
#include <string>
using namespace std;

int main(void)
{
	string str = "PATZJUJZTACCBCC";
	// ����������Ҫ��һ����ά����dp����Ϊ����¼����¼������Ľ�����Ա��ظ��ļ��㡣��Ҳ�Ƕ�̬�滮�ľ������ڡ����������������㷨���Ӷ�Ҳ��O(n^2)
	int result[15][15] = { 0 }; // result[i][j] �����ַ����±� i-j ֮���ַ����Ƿ�Ϊ���ģ����Ϊ1���ʾ str.sbustr(i, j)����Ϊ����

	for (int i = 0; i < 15; ++i)
		result[i][i] = 1;

	int max = 1;
	int flag = 0;
	// ���ַ���ʮ�ֱִ��������ҳ���ΪL = 3��4��5...15���ȵĻ���
	for (int L = 3; L < 15; ++L)
	{
		for (int i = 0; i + L - 1 < 15; ++i) // LΪ���ڴ�С�����ĳ��ȣ�����һ�������ڵ���Ϊ 0-2 �������ƶ����ڣ��ҵ�����ΪL�Ļ��Ĳ����
		{
			int j = i + L - 1;
			if (str[i] == str[j] && result[i + 1][j - 1] == 1)
			{
				result[i][j] = 1;
				max = L;
				flag = i; // ��¼����ĵ���ʼλ�ã�����λ��Ϊi + L - 1
			}
		}
	}

	cout << "����ĳ���Ϊ : " << max << endl; // 9
	cout << "�����Ϊ: " << str.substr(flag, flag + max - 1) << endl; // ATZJUJZTA
	return 0;
}