#include <iostream>
#include <vector>
using namespace std;

void printResult(vector<int>& vec)
{
	for (int i : vec)
		cout << i << " ";
	cout << endl;
}

void solveCore(int* array, int target, vector<vector<int>>& result, vector<int>& temp, int index, int len)
{
	if (target == 0)
	{
		result.push_back(temp);
		return;
	}

	for (int i = index; i < len; ++i)
	{
		int current = target - array[i];
		if (current < 0)
			return;
		temp.push_back(array[i]);
		solveCore(array, current, result, temp, i, len);
		temp.pop_back();
	}
}

int compare(const void* num1, const void* num2)
{
	return *(int*)num1 - *(int*)num2;
}

void solve(int* array, int target, vector<vector<int>>& result, int len)
{
	if (array == nullptr || len <= 0)
		return;
	qsort(array, len, sizeof(int), compare);
	vector<int> temp;
	solveCore(array, target, result, temp, 0, len);

}

int main()
{
	int A[4] = { 3, 2, 6, 7 };
	int x = 9;
	vector<vector<int>> result;
	int sum = 0;
	solve(A, x, result, 4);
	for (vector<int> vec : result)
	{
		for (int i : vec)
			cout << i << " ";
		cout << endl;
	}
	return 0;
}