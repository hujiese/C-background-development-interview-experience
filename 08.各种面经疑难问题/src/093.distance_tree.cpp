#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

struct BinaryTreeNode
{
	int m_dbValue;
	BinaryTreeNode* m_pLeft;
	BinaryTreeNode* m_pRight;
};

int helper(BinaryTreeNode * root, int &depth)
{
	if (root == NULL)
	{
		depth = 0;
		return 0;
	}
	int ld, rd;
	int maxleft = helper(root->m_pLeft, ld);
	int maxright = helper(root->m_pRight, rd);
	depth = max(ld, rd) + 1;
	return max(maxleft, max(maxright, ld + rd));
}

int maxDistance(BinaryTreeNode* root)
{
	int depth;
	return helper(root, depth);
}

void printTreeInLevelWithN(BinaryTreeNode* pTree)
{
	if (pTree == nullptr)
		return;

	queue<BinaryTreeNode*> nodes;
	nodes.push(pTree);
	int toBePrint = 1;
	int prePrint = 0;
	while (!nodes.empty())
	{
		BinaryTreeNode* node = nodes.front();
		nodes.pop();
		cout << node->m_dbValue << " ";
		toBePrint--;

		if (node->m_pLeft != nullptr)
		{
			++prePrint;
			nodes.push(node->m_pLeft);
		}

		if (node->m_pRight != nullptr)
		{
			++prePrint;
			nodes.push(node->m_pRight);
		}

		if (toBePrint == 0)
		{
			toBePrint = prePrint;
			cout << endl;
		}
	}
}

int main(void)
{
	BinaryTreeNode node1;
	node1.m_dbValue = 5;

	BinaryTreeNode node2;
	node2.m_dbValue = 3;

	BinaryTreeNode node3;
	node3.m_dbValue = 7;

	BinaryTreeNode node4;
	node4.m_dbValue = 2;

	BinaryTreeNode node5;
	node5.m_dbValue = 4;

	BinaryTreeNode node6;
	node6.m_dbValue = 6;

	BinaryTreeNode node7;
	node7.m_dbValue = 8;

	node1.m_pLeft = &node2;
	node1.m_pRight = &node3;
	node2.m_pLeft = &node4;
	node2.m_pRight = &node5;
	//node3.m_pLeft = &node6;
	//node3.m_pRight = &node7;
	node3.m_pLeft = nullptr;
	node3.m_pRight = nullptr;

	node4.m_pLeft = nullptr;
	node4.m_pRight = nullptr;
	node5.m_pLeft = nullptr;
	node5.m_pRight = nullptr;
	node6.m_pLeft = nullptr;
	node6.m_pRight = nullptr;
	node7.m_pLeft = nullptr;
	node7.m_pRight = nullptr;

	printTreeInLevelWithN(&node1);
	cout << endl;

	int depth = 0;
	cout << helper(&node1, depth) << endl; // 3
	return 0;
}