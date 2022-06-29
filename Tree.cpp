#include "Tree.h"

// Developer mode
#include <iostream>

void Tree::printChilds(Tree::Iterator *curr_pos) {
    if (!tree_elements) return;
    std::cout << "[Tree Elements]: " << this->tree_elements << std::endl; 
    std::cout << "Parent value: " << *(int *)curr_pos->current_pos->elem << " | childs: " << curr_pos->current_pos->child_count << " | Parent address: " << curr_pos->current_pos << std::endl;
    size_t size = sizeof(int);
    if (curr_pos->current_pos->child_count != 0) {
        List::Iterator *my_childs = curr_pos->current_pos->childs->newIterator();
        while (true) {
            void *elem = my_childs->getElement(size);
            Tree::Node *my_node = (Tree::Node *)elem;
            std::cout << "Child value: " << *(int *)my_node->elem << " | Parent: " << my_node->parent << " | Childs: " << my_node->childs << " | Value size: " << my_node->size << " | Childs count: " << my_node->child_count << std::endl;
            if (my_childs->hasNext()) {
                my_childs->goToNext();
            }else {
                break;
            }
        }
        delete my_childs;
    }
}

// ---end----

void *Tree::Iterator::getElement(size_t &size) {
    size = current_pos->size;
    return current_pos->elem;
}

Tree::Node* Tree::Iterator::findNextChild(Tree::Node *cur_pos) {
    if (!cur_pos->parent) {
        return NULL;
    }
    Tree::Node *current_node = cur_pos;
    Tree::Node *parent = cur_pos->parent;
    List::Iterator *list_iter = parent->childs->newIterator();
    size_t size;
    int current_node_child_index = 0;
    while (static_cast<Tree::Node *>(list_iter->getElement(size)) != current_node) {
        list_iter->goToNext();
        current_node_child_index++;
    }
    if (parent->child_count > current_node_child_index + 1) {

        list_iter->goToNext();
        return static_cast<Tree::Node *>(list_iter->getElement(size));
    }else {
        this->findNextChild(parent);
    }
}

bool Tree::Iterator::myGoToNext(int hasNext) {
    Tree::Node *tmp_pos = this->current_pos;
    if (tmp_pos->child_count > 0) {
        size_t size;
        List::Iterator *list_iter = tmp_pos->childs->newIterator();
        Tree::Node *first_child = static_cast<Tree::Node *>(list_iter->getElement(size));
        tmp_pos = first_child;
        delete list_iter;
        if (!hasNext) {
            this->current_pos = tmp_pos;
        }
        return true;
    }else {
        Tree::Node *next_elem = this->findNextChild(current_pos);
        if (next_elem) {
            if (!hasNext) {
                this->current_pos = next_elem;
            }else {
                return true;
            }
        }else {
            return false;
        }
    }
}

bool Tree::Iterator::hasNext() {
    return this->myGoToNext(1);
}

void Tree::Iterator::goToNext() {
    this->myGoToNext(0);
}

bool Tree::Iterator::equals(Container::Iterator* right) {
    if (dynamic_cast<Tree::Iterator*>(right)->current_pos == this->current_pos) {
        return true;
    }
    return false;
}

bool Tree::Iterator::goToParent() {
    if (!this->current_pos) {
        return false;
    }
    if (!this->current_pos->parent) {
        return false;
    }
    this->current_pos = this->current_pos->parent;
    return true;
}

bool Tree::Iterator::goToChild(int child_index) {
    size_t size;
    List::Iterator *childs = current_pos->childs->newIterator();
    for (int i = 0; i < child_index; i++) {
        if (childs->hasNext()) {
            childs->goToNext();
        }else {
            delete childs;
            return false;
        }
    }
    current_pos = static_cast<Node *>(childs->getElement(size));
    delete childs;
    return true;
}

Tree::Iterator *Tree::newIterator() {
    return new Tree::Iterator(this->root);
}

int Tree::size() {
    return this->tree_elements;
}

size_t Tree::max_bytes() {
    return _memory.size();
}

Tree::Node *Tree::allocData(Tree::Iterator *my_iter, int child_index, void *elem, size_t size) {
    Tree::Node *new_data = new (_memory.allocMem(sizeof(Node))) Node(size, my_iter->current_pos);
    new_data->elem = elem;
    new_data->childs = (List *)_memory.allocMem(sizeof(List));

    // инициализация списка
    List my_list(_memory);
    memcpy(new_data->childs, &my_list, sizeof(List));
    return new_data;
}

int Tree::insert(AbstractTree::Iterator *iter, int child_index, void *elem, size_t size) {
    if (child_index < 0) return 1;
    // инициализация корня
    if (this->tree_elements == 0) {
        this->tree_elements++;
        root->elem = elem;
        root->size = size;
        root->childs = (List *)_memory.allocMem(sizeof(List));

        // инициализация списка
        List my_list(_memory);
        memcpy(root->childs, &my_list, sizeof(List));
        return 0;
    }

    Tree::Iterator *my_iter = dynamic_cast<Tree::Iterator *>(iter);

    // случай, когда мы хотим добавить первого ребенка
    if (!my_iter->current_pos->child_count) {
        if (child_index == 0) {
            Tree::Node *new_data = allocData(my_iter, child_index, elem, size);
            my_iter->current_pos->childs->push_front(new_data, sizeof(new_data));
            my_iter->current_pos->child_count++;
            this->tree_elements++;
            return 0;
        }
        return 1;
    }

    // добавление остальных детей
    if (child_index > my_iter->current_pos->child_count) return 1;

    List::Iterator *childs = my_iter->current_pos->childs->newIterator();
    for (int i = 0; i < child_index - 1; i++) {
        if (childs->hasNext()) {
            childs->goToNext();
        }
    }

    Tree::Node *new_data = allocData(my_iter, child_index, elem, size);

    if (!my_iter->current_pos->childs->insert(childs, new_data, sizeof(new_data))) {
        delete childs;
        my_iter->current_pos->child_count++;
        this->tree_elements++;
        return 0;
    }
    delete childs;
    return 1;
}

Tree::Iterator *Tree::find(void *elem, size_t size) {
    Tree::Iterator *new_iter = this->newIterator();
    while (new_iter->current_pos->elem != elem) {
        if (new_iter->hasNext()) {
            new_iter->goToNext();
        }else {
            delete new_iter;
            return NULL;
        }
    }
    return new_iter;
}

void Tree::clear() {
    this->recursionNodeRemoving(root);
    root->childs = 0;
    root->child_count = 0;
    root->elem = nullptr;
    root->parent = NULL;
    root->size = 0;
    this->tree_elements = 0;
}

bool Tree::empty() {
    if (this->root == nullptr) {
        return true;
    }
    return false;
}

Tree::Node* Tree::goToLastChild(Tree::Node *node) {
    Tree::Node *my_cur_pos = node;
    if (my_cur_pos->child_count != 0) {
        size_t size;
        List::Iterator *list_iter = my_cur_pos->childs->newIterator();
        Tree::Node *new_node = static_cast<Tree::Node *>(list_iter->getElement(size));
        delete list_iter;
        this->goToLastChild(new_node);
    }else {
        return my_cur_pos;
    }
}

bool Tree::recursionNodeRemoving(Tree::Node *main_parent) {
    Tree::Node *last_child = this->goToLastChild(main_parent);
    if (main_parent != last_child) {
        _memory.freeMem(last_child->childs);
        _memory.freeMem(last_child);
        last_child->parent->childs->pop_front();
        last_child->parent->child_count--;
        this->tree_elements--;
        this->recursionNodeRemoving(main_parent);
    }else {
        return true;
    }
}

bool Tree::remove(AbstractTree::Iterator *iter, int leaf_only) {
    Tree::Iterator *my_iter = dynamic_cast<Tree::Iterator *>(iter);

    if (leaf_only && my_iter->current_pos->child_count > 0) {
        return false;
    }

    recursionNodeRemoving(my_iter->current_pos);
    if (my_iter->current_pos == root) {
        root->childs = 0;
        root->child_count = 0;
        root->elem = nullptr;
        root->parent = NULL;
        root->size = 0;
        this->tree_elements = 0;
    }else {
        size_t size;
        Tree::Node *delete_data = my_iter->current_pos;
        my_iter->current_pos = my_iter->current_pos->parent;
        List::Iterator *list_iter = my_iter->current_pos->childs->newIterator();
        while (static_cast<Tree::Node *>(list_iter->getElement(size)) != delete_data) {
            list_iter->goToNext();
        }
        Tree:: Node *test = static_cast<Tree::Node *>(list_iter->getElement(size));
        _memory.freeMem(delete_data->childs);
        _memory.freeMem(delete_data);
        my_iter->current_pos->childs->remove(list_iter);
        delete_data = nullptr;
        my_iter->current_pos->child_count--;
        this->tree_elements--;
        delete list_iter;
    }
    return true;
}

void Tree::remove(Container::Iterator *iter) {
    AbstractTree::Iterator *my_iter = dynamic_cast<AbstractTree::Iterator *>(iter);
    this->remove(my_iter, 0);
}

Tree::~Tree() {
    this->clear();
    delete this->root;
    this->root = NULL;
}

Tree::Iterator::~Iterator() {}