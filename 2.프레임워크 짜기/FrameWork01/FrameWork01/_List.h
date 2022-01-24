#pragma once
template <typename T>
class _List
{
private:
	struct Node
	{
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
		T data;
		Node* next;
		Node* prev;
	};
public:
	Node* tail;
	Node* head;
	Node* fin;
	int listsize;

	class _Iterator
	{
	private:
		Node* current;
	public:
		_Iterator(Node* node=0);
		_Iterator& operator++();
		_Iterator& operator--();
		Node* operator&();
		T& operator*();
		bool operator!=(const _Iterator& ref);
		bool operator==(const _Iterator& ref);
	};
	_List();
	int size();
	bool is_empty();
	T front_data();
	T back_data();
	void Push_front(T input);
	void Push_back(T input);
	T Pop_front();
	T Pop_back();
	void insert(_Iterator point,T input);
	void erase(_Iterator point);
	void erase(T data);
	_Iterator begin();
	_Iterator end();
};

