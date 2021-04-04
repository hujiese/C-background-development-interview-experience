#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct BinaryTreeNode
{
	int m_dbValue;
	BinaryTreeNode* m_pLeft;
	BinaryTreeNode* m_pRight;
};

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
			prePrint = 0;
			cout << endl;
		}
	}
}

int getNodeLevel(BinaryTreeNode* pTree, BinaryTreeNode* target)
{
	if (pTree == nullptr)
		return -1;

	int level = 0;
	queue<BinaryTreeNode*> nodes;
	nodes.push(pTree);
	int toBePrint = 1;
	int prePrint = 0;
	while (!nodes.empty())
	{
		BinaryTreeNode* node = nodes.front();
		nodes.pop();
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

			++level;
			toBePrint = prePrint;
			prePrint = 0;
			cout << endl;
			if (node == target)
				break;
		}

		if (node == target)
		{
			++level;
			break;
		}
	}
	return level;
}
int main(void)
{
	BinaryTreeNode node1;
	node1.m_dbValue = 1;

	BinaryTreeNode node2;
	node2.m_dbValue = 2;

	BinaryTreeNode node3;
	node3.m_dbValue = 3;

	BinaryTreeNode node4;
	node4.m_dbValue = 4;

	BinaryTreeNode node5;
	node5.m_dbValue = 5;

	BinaryTreeNode node6;
	node6.m_dbValue = 6;

	BinaryTreeNode node7;
	node7.m_dbValue = 7;

	BinaryTreeNode node8;
	node8.m_dbValue = 8;

	BinaryTreeNode node9;
	node9.m_dbValue = 9;

	BinaryTreeNode node10;
	node10.m_dbValue = 10;

	BinaryTreeNode node11;
	node11.m_dbValue = 11;

	node1.m_pLeft = &node2;
	node1.m_pRight = &node3;
	node2.m_pLeft = &node4;
	node2.m_pRight = &node5;
	node3.m_pLeft = &node6;
	node3.m_pRight = &node7;

	node4.m_pLeft = &node8;
	node4.m_pRight = &node9;
	node5.m_pLeft = nullptr;
	node5.m_pRight = nullptr;
	node6.m_pLeft = &node10;
	node6.m_pRight = nullptr;
	node7.m_pLeft = nullptr;
	node7.m_pRight = &node11;
	node8.m_pLeft = nullptr;
	node8.m_pRight = nullptr;
	node9.m_pLeft = nullptr;
	node9.m_pRight = nullptr;
	node10.m_pLeft = nullptr;
	node10.m_pRight = nullptr;
	node11.m_pLeft = nullptr;
	node11.m_pRight = nullptr;
	printTreeInLevelWithN(&node1);
	cout << endl;

	cout << getNodeLevel(&node1, &node7) << endl;
	return 0;
}