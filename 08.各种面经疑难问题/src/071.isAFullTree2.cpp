#include <iostream>
#include <queue>

using namespace std;

struct BinaryTreeNode
{
	int m_dbValue;
	BinaryTreeNode* m_pLeft;
	BinaryTreeNode* m_pRight;
};

// �ⷨһ
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

		if (flag && (node->m_pLeft != nullptr || node->m_pRight != nullptr)) // ����Ѿ�������һ�����Һ���Ϊ�յĽڵ㣬�������ڵ㻹�����Һ��ӣ�����false
			return false;

		if (node->m_pLeft == nullptr && node->m_pRight == nullptr) // ���ֵ�һ�����Һ���Ϊ�յĽڵ㣬��ô�����ڵ㶼Ӧ�����Һ���Ϊ��
		{
			if (!flag)
				flag = true;
		}
		nodes.pop();
		if (node->m_pLeft == nullptr && node->m_pRight != nullptr) // ����ڵ������Ϊ�գ��Һ��Ӳ�Ϊ�գ���ôΪfalse
			return false;
			

		if (node->m_pLeft != nullptr)
			nodes.push(node->m_pLeft);
		if (node->m_pRight != nullptr)
			nodes.push(node->m_pRight);
	}
	return true;
}

//// �ⷨ�� 
///*
//���ö����Ƚ����ڵ����,ֻҪ��ǰ�ڵ㲻ΪNULL,�ȴӶ���front()�õ�һ���ڵ㲢�����Խڵ�����Һ������,�������ֻ��һ�����ӵ����,��NULL�������Ǹ�����;
//�����ǰ�ڵ�Ϊ���� ���ж϶����Ƿ�ΪNULL��
//
//�������ҲNULL�� ��ô�˿���������ȫ��������
//
//�ӻ���ΪNULL��ô��ֱ��front���� �����ǰ���ӵĽڵ���NULL�ڵ� ��֤��������ȫ��������
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