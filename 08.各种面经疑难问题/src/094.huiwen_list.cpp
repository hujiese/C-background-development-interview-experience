#include <iostream>
using namespace std;

struct ListNode
{
	int m_nValue;
	ListNode* m_pNext;
};

void AddToTail(ListNode* pHead, int value)
{
	ListNode* pNode = new ListNode;
	pNode->m_nValue = value;
	pNode->m_pNext = nullptr;

	if (pHead == nullptr)
	{
		pHead = pNode;
	}
	else
	{
		ListNode *temp = pHead;
		while (temp->m_pNext != nullptr)
		{
			temp = temp->m_pNext;
		}
		temp->m_pNext = pNode;
	}

}

void PrintListNode(ListNode* pHead)
{
	if (pHead == nullptr)
	{
		return;
	}
	else
	{
		cout << pHead->m_nValue << endl;

		ListNode* pNode = pHead;
		while (pNode->m_pNext != nullptr)
		{
			cout << pNode->m_pNext->m_nValue << endl;
			pNode = pNode->m_pNext;
		}
	}

}

bool isHeiWen(ListNode* head)
{
	if (head == nullptr)
		return false;

	bool flag = true;
	ListNode* pNode = head;
	int length = 0;
	while (pNode)
	{
		++length;
		pNode = pNode->m_pNext;
	}
	//cout << length << endl;
	//PrintListNode(head);
	ListNode* pre = nullptr;
	ListNode* curr = head;
	ListNode* next = nullptr;
	for (int i = 0; i < length / 2; ++i)
	{
		next = curr->m_pNext;
		curr->m_pNext = pre;
		pre = curr;
		curr = next;
	}

	//PrintListNode(curr);
	ListNode* head1 = pre;
	ListNode* head2 = curr;
	if (length % 2 == 1)
		head2 = head2->m_pNext;
	//PrintListNode(head1);
	//cout << "----" << endl;
	//PrintListNode(head2);

	while (head1 && head2)
	{
		if (head1->m_nValue != head2->m_nValue)
		{
			flag = false;
			break;
		}
		head1 = head1->m_pNext;
		head2 = head2->m_pNext;
	}
	return flag;
}

int main(void)
{
	ListNode* list = new ListNode;
	list->m_nValue = 1;
	list->m_pNext = nullptr;

	AddToTail(list, 2);
	AddToTail(list, 3);
	AddToTail(list, 4);
	//AddToTail(list, 4);
	AddToTail(list, 3);
	AddToTail(list, 2);
	AddToTail(list, 1);
	//PrintListNode(list);
	cout << isHeiWen(list) << endl;
	return 0;
}