#include <iostream>
#include <queue>

using namespace std;

struct BinaryTreeNode
{
	int m_dbValue;
	BinaryTreeNode* m_pLeft;
	BinaryTreeNode* m_pRight;
};

// 解法一
bool isAFullTree(BinaryTreeNode* root)
{
	if (root == nullptr)
		return true;

	bool flag = false;
	queue<BinaryTreeNode*> nodes;
	nodes.push(root);
	while (!nodes.empty())
	{
		BinaryTreeNode* node = nodes.front();

		if (flag && (node->m_pLeft != nullptr || node->m_pRight != nullptr)) // 如果已经出现了一个左右孩子为空的节点，但后续节点还有左右孩子，返回false
			return false;

		if (node->m_pLeft == nullptr && node->m_pRight == nullptr) // 出现第一个左右孩子为空的节点，那么后续节点都应该左右孩子为空
		{
			if (!flag)
				flag = true;
		}
		nodes.pop();
		if (node->m_pLeft == nullptr && node->m_pRight != nullptr) // 如果节点的左孩子为空，右孩子不为空，那么为false
			return false;
			

		if (node->m_pLeft != nullptr)
			nodes.push(node->m_pLeft);
		if (node->m_pRight != nullptr)
			nodes.push(node->m_pRight);
	}
	return true;
}

//// 解法二 
///*
//利用队列先将根节点入队,只要当前节点不为NULL,先从队列front()得到一个节点并将吹对节点的左右孩子入队,如果存在只有一个孩子的情况,用NULL来代替那个孩子;
//如果当前节点为空了 则判断队列是否为NULL；
//
//如果队列也NULL了 那么此棵树就是完全二叉树；
//
//队还不为NULL那么就直接front出队 如果当前出队的节点是NULL节点 则证明不是完全二叉树。
//*/
//bool isAFullTree2(BinaryTreeNode* root)
//{
//	if (root == nullptr)
//		return false;
//
//	BinaryTreeNode* node = root;
//	queue<BinaryTreeNode*> treeNodes;
//	treeNodes.push(node);
//	while (node)
//	{
//		if (node)
//		{
//			treeNodes.push(node->m_pLeft);
//			treeNodes.push(node->m_pRight);
//			treeNodes.pop();
//		}
//		node = treeNodes.front();
//	}
//
//	while (!treeNodes.empty())
//	{
//		BinaryTreeNode* node = treeNodes.front();
//		treeNodes.pop();
//		if (!node)
//			return false;
//	}
//	return true;
//}

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
	node3.m_pLeft = nullptr;
	node3.m_pRight = &node7;
	//node3.m_pRight = nullptr;

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

	cout << isAFullTree(&node1) << endl;
	return 0;
}