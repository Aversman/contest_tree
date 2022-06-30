#include <iostream>
#include "List.h"

 //Функции итератора:

 // Возврашает явно указатель на элемент, на который указывает итератор в данный момент.
 // Неявно возвращает размер данных.
 // Если итератор показывает за пределы контейнера (например, удален последний элемент), возвращает NULL.

void* List::ListIterator::getElement(size_t& size)
{
	size = current->size_node;

	return current->value;
}

// Возвращает true, если есть следующий элемент, иначе false.

bool List::ListIterator::hasNext()
{
  bool tmp = false;

  if (current != nullptr && current->next != nullptr)
  {
	  tmp = true;
  }

  return tmp;
}

// Переход к следующему элементу.

void List::ListIterator::goToNext()
{
	if (current == nullptr)
	{
		return;
	}

	current = current->next;
}

// проверка на равенство итераторов

bool List::ListIterator::equals(Container::Iterator* right)
{
	bool tmp = false;

	if (current == ((ListIterator*)(right))->current)
	{
		tmp = true;
	}

	return tmp;
}


//Функции списка:

// Добавление элемента в начало контейнера.
// В случае успешного добавления функция возвращает значение 0, в случае неудачи 1.


int List::push_front(void* elem, size_t elemSize)
{
	Node* new_front = new  (_memory.allocMem(sizeof(Node)))Node(root, elem, elemSize);
	root = new_front;
	counter_node++;	

	return 0;
}

// Удаление элемента из начала контейнера.

void List::pop_front()
{
	if (root == nullptr)
	{
		return;
	}

	Node* tmp_node = root;
	root = root->next;
	_memory.freeMem(tmp_node);
	counter_node--;
}

// Функция явно возвращает указатель на элемент, находящийся в начале контейнера.
// и не явно возвращает размер данных

void* List::front(size_t& size)
{

	size = root->size_node;

	return root->value;
}

// Добавление элемента в позицию, на которую указывает итератор iter.
// В случае успешного добавления функция возвращает значение 0, в случае неудачи 1.

int List::insert(AbstractList::Iterator* iter, void* elem, size_t elemSize)
{
	ListIterator* iterator = (ListIterator*)iter;
	Node* new_node = new  (_memory.allocMem(sizeof(Node)))Node(nullptr, elem, elemSize);

	Node* tmp_node;

	if (iterator->current == root)
	{
		tmp_node = root->next;
		root->next = new_node;
		new_node->next = tmp_node;

		//new_node->next = root;
		//root = new_node;

		counter_node++;
		return 0;
	}

	ListIterator* tmp_iterator = (ListIterator*)newIterator();
	tmp_iterator->current = root->next;
	//Node* prev = root;

	while (tmp_iterator->current != iterator->current && tmp_iterator->current != nullptr)
	{
		if (tmp_iterator->hasNext() != true)
		{
			_memory.freeMem(new_node);
			return 1;
		}

		tmp_iterator->goToNext();

		//prev = prev->next;
	}


	tmp_node = tmp_iterator->current->next;
	tmp_iterator->current->next = new_node;
	new_node->next = tmp_node;


	//prev->next = new_node;
	//new_node->next = tmp_iterator->current;

	counter_node++;
	return 0;
}

//Функции контейнера:

// Функция возвращает значение, равное количеству элементов в контейнере.

int List::size()
{
	return counter_node;
}

// Функция возвращает значение, равное максимальной вместимости контейнера в байтах.

size_t List::max_bytes()
{
	return _memory.maxBytes();
}

// Функция создает в динамической памяти итератор, указывающий на первый найденный
// в контейнере элемент. Если элемент не найден, возвращается пустой указатель.
// Удаление этого итератора должно делаться пользователем с помощью оператора delete.

List::Iterator* List::find(void* elem, size_t size)
{

	ListIterator* tmp_iterator= (ListIterator*)newIterator();
	tmp_iterator->current = root;

	while (tmp_iterator->current != nullptr)
	{
		if (size == tmp_iterator->current->size_node && tmp_iterator->current->value == elem)
		{
			return tmp_iterator;
		}

		tmp_iterator->goToNext();
	}

	return nullptr;
}

// Функция создает в динамической памяти итератор, указывающий на первый элемент
// контейнера. Если контейнер пустой, возвращается нулевой указатель.
// Удаление этого итератора должно делаться пользователем с помощью оператора delete.

List::Iterator* List::newIterator()
{

	ListIterator* iterator = new ListIterator();
	iterator->current = root;

	return iterator;
}

// Удаление элемента из позиции, на которую указывает итератор iter.
// После удаления итератор указывает на следующий за удаленным элемент.

void List::remove(Container::Iterator *iter)
{
	ListIterator* iterator = (ListIterator*)iter;

	if (iterator->current == root)
	{
		iterator->goToNext();
		pop_front();
		return;
	}

	ListIterator* tmp_iterator = (ListIterator*)newIterator();
	tmp_iterator->current = root->next;
	Node* prev = root;

	while (tmp_iterator->current != iterator->current)
	{
		tmp_iterator->goToNext();
		prev = prev->next;

		if (tmp_iterator->current == nullptr)
		{
			delete tmp_iterator;
			return;
		}
	}
	
	prev->next = iterator->current->next;
	
	iterator->goToNext();
	
	_memory.freeMem(tmp_iterator->current);

	delete tmp_iterator;

	counter_node--;
}
// Удаление всех элементов из контейнера.

void List::clear()
{
	Node* tmp_node = root;

	while (tmp_node != nullptr)
	{
		root = root->next;

		_memory.freeMem(tmp_node);

		tmp_node = root;
	}

	counter_node = 0;
}

// Если контейнер пуст возвращает true, иначе false

bool List::empty()
{
	return root == nullptr;
}

void List::print()
{
	ListIterator* it = (ListIterator*)(newIterator());
	for (int i = 0; i < size(); ++i)
	{
		cout << "Element " << i + 1 << ":" << endl;
		//for (int j = 0; j < it->current->size_node; ++j)
		int* tmp = (int*)it->current->value;
		cout << *(tmp)<<endl;
		//cout << endl;
		it->goToNext();
	}
}

