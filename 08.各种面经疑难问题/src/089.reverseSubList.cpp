#include <iostream>

using namespace std;

struct ListNode
{
	int m_nValue;
	ListNode* m_pNext;
};

void addToTail(ListNode** pHead, int value)
{
	ListNode* node = new ListNode;
	node->m_nValue = value;
	node->m_pNext = nullptr;

	ListNode* pNode = *pHead;
	if (*pHead == nullptr)
		*pHead = node;
	else
	{
		while (pNode->m_pNext != nullptr)
		{
			pNode = pNode->m_pNext;
		}
		pNode->m_pNext = node;
	}
}

void printListNode(ListNode* pHead)
{
	if (pHead == nullptr)
		return;

	cout << pHead->m_nValue << endl;
	if (pHead->m_pNext != nullptr)
		printListNode(pHead->m_pNext);

}
ListNode* reverList(ListNode* pHead)
{
	if (pHead == nullptr)
		return nullptr;

	ListNode* pNewHead = nullptr;
	ListNode* pre = nullptr;
	ListNode* pNode = pHead;
	ListNode* next = nullptr;

	while (pNode != nullptr)
	{
		if (pNode->m_pNext == nullptr)
			pNewHead = pNode;

		next = pNode->m_pNext;

		pNode->m_pNext = pre;
		pre = pNode;
		pNode = next;
	}
	return pNewHead;
}

ListNode* revSubList(ListNode** pHead, int from, int to)
{
	if (pHead == nullptr)
		return nullptr;

	if (from >= to)
		return *pHead;

	ListNode* newHead = nullptr;
	ListNode* pNode = *pHead;
	ListNode* pre = nullptr;
	ListNode* next = nullptr;
	ListNode* start = nullptr;
	for (int i = 0; i < to; ++i)
	{
		if (i == from - 1)
			pre = pNode;

		if (i == from)
			start = pNode;

		pNode = pNode->m_pNext;
	}
	next = pNode->m_pNext;
	pNode->m_pNext = nullptr;
	newHead = reverList(start);

	if (pre != nullptr)
	{
		pre->m_pNext = newHead;
		newHead = pre;
	}
	start->m_pNext = next;

	return newHead;
}



int main(void)
{
	ListNode* list = nullptr;

	addToTail(&list, 1);
	addToTail(&list, 2);
	addToTail(&list, 3);
	addToTail(&list, 4);
	addToTail(&list, 5);
	addToTail(&list, 6);

	ListNode* head = revSubList(&list, 0, 5);
	printListNode(head);
	return 0;
}