#pragma once
#include <iostream>
using namespace std;

template <typename T>
class _List
{
private:
	class Node
	{
	public:
		Node(T input)
		{
			data = input;
			next = nullptr;
			prev = nullptr;
		}
		Node()
		{
			data = 0;
			next = nullptr;
			prev = nullptr;
		}
		void operator=(T input)
		{
			data = input;
		}
		friend ostream& operator<<(ostream& os, const Node& node);
		T data;
		Node* next;
		Node* prev;
	};
	Node* tail;
	Node* head;
	Node* fin;
	int listsize;
public:
	class _Iterator
	{
	private:
		Node* current;
	public:
		_Iterator(Node* node = 0) :current(node) {}
		_Iterator& operator++()
		{
			current = current->next;
			return *this;
		}
		_Iterator& operator--()
		{
			current = current->prev;
			return *this;
		}
		Node* operator&()
		{
			return current;
		}
		T& operator*()
		{
			return current->data;
		}
		bool operator!=(const _Iterator& ref)
		{
			return current != ref.current;
		}
		bool operator==(const _Iterator& ref)
		{
			return current == ref.current;
		}
	};
	friend ostream& operator<<(ostream& os, const Node& node)
	{
		os<<node.data<<endl;
		return os;
	}
	_List()
	{
		listsize = 0;
		head = nullptr;
		tail = nullptr;
		fin = new Node();
	}
	int size()
	{
		return listsize;
	}
	bool is_empty()
	{
		return listsize <= 0;
	}
	T front_data()
	{
		return head->data;
	}
	T back_data()
	{
		return fin->data;
	}
	void Push_front(T input)
	{
		if (listsize == 0)
		{
			head = new Node(input);
			tail = head;
			head->next = fin;
			head->prev = fin;
			fin->prev = head;
			fin->next = head;
		}
		else
		{
			head->prev = new Node(input);
			head->prev->next = head;
			head = head->prev;
			head->prev = fin;
			fin->next = head;
		}
		listsize++;
	}
	void Push_back(T input)
	{
		if (listsize == 0)
		{
			head = new Node(input);
			tail = head;
			head->prev = fin;
			head->next = fin;
			fin->prev = head;
			fin->next = head;
		}
		else
		{
			tail->next = new Node(input);
			tail->next->prev = tail;
			tail->next->next = fin;
			tail = tail->next;
			fin->prev = tail;
		}
		listsize++;
	}
	T Pop_front()
	{
		if (listsize <= 0)
			return 0;
		T temp = head->data;
		if (listsize == 1)
		{
			delete head;
			head = NULL;
			tail = NULL;
		}
		else
		{
			head->next->prev = fin;
			fin->next = head->next;
			delete head;
			head = fin->next;
		}
		listsize--;
		return temp;
	}
	T Pop_back()
	{
		if (listsize <= 0)
			return 0;
		T temp = tail->data;
		if (listsize == 1)
		{
			delete head;
			head = NULL;
			tail = NULL;
		}
		else
		{
			tail->prev->next = fin;
			fin->prev = tail->prev;
			delete tail;
			tail = fin->prev;
		}
		listsize--;
		return temp;
	}
	void insert(_Iterator point, T input)
	{
		Node* node = new Node(input);
		node->prev = (&point)->prev;
		node->next = &point;
		(&point)->prev->next = node;
		(&point)->prev = node;
		if (&point == head)head = node;
		if (&point == tail)tail = node;
		listsize++;
	}
	void erase(_Iterator point)
	{
		(&point)->next->prev = (&point)->prev;
		(&point)->prev->next = (&point)->next;
		if (&point == head)head = (&point)->next;
		if (&point == tail)tail = (&point)->prev;
		delete (&point);
		//point = NULL;
		listsize--;
	}
	void erase(T input)
	{
		_Iterator* itr_array = new _Iterator[listsize];
		int find = 0;
		for (_Iterator itr = begin(); itr != end(); ++itr)
		{
			if (*itr == input)
			{
				itr_array[find++] = itr;
			}
		}
		for (int i = 0; i < find; ++i)
		{
			erase(itr_array[i]);
		}
		delete itr_array;
		listsize -= find;
	}
	void Clear()
	{
		Node** node_array = new Node*[listsize];
		int index = 0;
		for (_Iterator itr = begin(); itr != end(); ++itr)
		{
			node_array[index++] = &itr;
		}
		for (int i = 0; i < listsize; ++i)
		{
			//주의:T 데이터가 *형일때 data에 대한 메모리해제는 따로 해주기.
			cout << node_array[i]->data << "delete" << endl;
			delete node_array[i];
		}
		listsize = 0;
	}
	_Iterator begin()
	{
		return _Iterator(head);
	}
	_Iterator end()
	{
		return _Iterator(fin);
	}
	Node& operator[](int index)
	{
		int check = 0;
		if (listsize <= index)
		{
			cout << "인덱스 초과" << endl;
			Node* temp = nullptr;
			return *temp;
		}
		for (_Iterator itr = begin(); itr != end(); ++itr)
		{
			if (check == index)
			{
				Node* temp = &itr;
				return *temp;
			}
			check++;
		}
	}
};
