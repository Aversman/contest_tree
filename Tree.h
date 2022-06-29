#pragma once
#include "Mem.h"
#include "TreeAbstract.h"
#include "List.h"

class Tree: AbstractTree {
    class Node {
    public:
        void *elem;
        size_t size;
        int child_count;
        Node *parent;
        List *childs;
        Node(): elem(NULL), size(0), parent(NULL), childs(NULL), child_count(0) {}
        Node(size_t size, Node *parent): size(size), parent(parent), child_count(0) {}
    };
    // кол-во элементов дерева
    int tree_elements = 0;
    Node *root;
public:
    class Iterator: public AbstractTree::Iterator {
        Node *current_pos;
        bool myGoToNext(int hasNext);
        Node *findNextChild(Tree::Node *cur_pos);
        friend class Tree;
    public:
        Iterator(): current_pos(NULL) {}
        Iterator(Node *ptr): current_pos(ptr) {}
        ~Iterator();
        void *getElement(size_t &size);
        bool hasNext();
        void goToNext();
        bool equals(Container::Iterator *right);

        // TreeAbstract
        bool goToParent();
        bool goToChild(int child_index);
    };

    Tree(MemoryManager &mem): AbstractTree(mem) {
        this->_memory = mem;
        root = new (_memory.allocMem(sizeof(Node))) Node();
    }
    ~Tree();
    int size();
    size_t max_bytes();
    Tree::Iterator* find(void *elem, size_t size);
    Iterator* newIterator();
    void remove(Container::Iterator *iter);
    void clear();
    bool empty();

    // TreeAbstract
    int insert(AbstractTree::Iterator *iter, int child_index, void *elem, size_t size);
    bool remove(AbstractTree::Iterator *iter, int leaf_only);

    // developer mode
    void printChilds(Tree::Iterator *current_pos);
    // developer mode end
private:
    Node *allocData(Tree::Iterator *my_iter, int child_index, void *elem, size_t size);
    // переходит в самый конец дерева по левому краю
    Tree::Node *goToLastChild(Tree::Node *node);
    // рекурсивно удаляет всех детей (и детей детей) до указанной ноды
    bool recursionNodeRemoving(Tree::Node *main_parent);
};