#pragma once

#include "ListAbstract.h"
#include "Mem.h"

class List : public AbstractList
{
private:

	struct Node
	{
		void* value;
		size_t size_node;
		Node* next;

		Node(Node* tmp, void* value_tmp, size_t size_tmp) :next(tmp), value(value_tmp), size_node(size_tmp) {} 
	};

	Node* root = nullptr;

	int counter_node = 0;

public:

	List(MemoryManager &mm) : AbstractList(mm) {}

	class ListIterator : public AbstractList::Iterator
	{
		private:

			Node* current = nullptr;

		public:
			
			void* getElement(size_t& size);

			bool hasNext();

			void goToNext();

			bool equals(Container::Iterator* right);

			~ListIterator() = default;

			friend List;
	};

	int push_front(void* elem, size_t elemSize);

	void pop_front();

	void* front(size_t& size);

	int insert(AbstractList::Iterator* iter, void* elem, size_t elem_size) override;

	int size();

	size_t max_bytes();

	Container:: Iterator* find(void* elem, size_t size) override;

	Container:: Iterator* newIterator() override;

	void remove(Container::Iterator* iter);

	void clear();

	bool empty();

	void print();
};


