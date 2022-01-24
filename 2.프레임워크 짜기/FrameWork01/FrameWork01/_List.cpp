#include "_List.h"
#pragma region _Iterator 함수
template <typename T>
_List<T>::_Iterator::_Iterator(Node* node)
{
	current = node;
}
template <typename T>
_List<T>::_Iterator& _List<T>::_Iterator::operator++()
{
	current = current->next;
	return *this;
}
template<typename T>
_List<T>::_Iterator& _List<T>::_Iterator::operator--()
{
	current = current->prev;
	return *this;
}
template<typename T>
_List<T>::Node* _List<T>::_Iterator::operator&()
{
	return current;
}
template<typename T>
T& _List<T>::_Iterator::operator*()
{
	return current->data;
}
template <typename T>
bool _List<T>::_Iterator::operator!=(const _Iterator& ref)
{
	return current != ref.current;
}
template <typename T>
bool _List<T>::_Iterator::operator==(const _Iterator& ref)
{
	return current == ref.current;
}
#pragma endregion

#pragma region _List 함수
template<typename T>
_List<T>::_List()
{
	data = 0;
	head = nullptr;
	tail = nullptr;
	fin = new Node();
}
template <typename T>
int _List<T>::size()
{
	return listsize;
}
template<typename T>
bool _List<T>::is_empty()
{
	return listsize <= 0;
}
template<typename T>
T _List<T>::front_data()
{
	return head->data;
}
template<typename T>
T _List<T>::back_data()
{
	return fin->data;
}
template<typename T>
void _List<T>::Push_front(T input)
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
template <typename T>
void _List<T>::Push_back(T input)
{
	if (listsize == 0)
	{
		head = Node(input);
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
template<typename T>
T _List<T>::Pop_front()
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
template<typename T>
T _List<T>::Pop_back()
{
	if (listsize <= 0)
		return 0;
	T temp = tail->data;
	if (listsize == 1)
	{
		delete haed;
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
template <typename T>
void _List<T>::insert(_Iterator point, T data)
{
		Node* node = new Node(data);
		node->prev = point->current->prev;
		node->next = point->current;
		point->current->prev->next = node;
		point->current->prev = node;
		if (point->current == head)head = node;
		if (point->current == tail)tail = node;
		listsize++;
}
template<typename T>
void _List<T>::erase(_Iterator point)
{
	point->current->next->prev = point->current->prev;
	point->current->prev->next = point->current->next;
	if (point->current == head)head = point->current->next;
	if (point->current == tail)tail = point->current->prev;
	delete point->current;
	point->current = NULL;
}
template<typename T>
void _List<T>::erase(T data)
{
	for(_Iterator itr = begin(); itr != end(); itr++)
	{
		if (itr->current->data == data)
		{
			erase(itr);
		}
	}
}
template<typename T>
_List<T>::_Iterator _List<T>::begin()
{
	return _Iterator(head);
}
template<typename T>
_List<T>::_Iterator _List<T>::end()
{
	return _Iterator(fin);
}
#pragma endregion 