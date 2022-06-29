#include "Test.h"

int *Test::insertElements(bool to_print) {
    int local_count = this->elements;
    Tree::Iterator *tree_iter = this->tree->newIterator();

    int *data = (int *)malloc(sizeof(int) * (this->elements + 1));
    for (int i = 0; i < this->elements + 1; i++) {
        data[i] = i;
    }

    this->tree->insert(tree_iter, 0, &data[0], sizeof(int));
    local_count--;

    for (int i = 0; i < local_count; i++) {
        this->tree->insert(tree_iter, 0, &data[i + 1], sizeof(int));
    }

    int count_elems = 0;
    size_t size;
    while (tree_iter->hasNext()) {
        if (static_cast<int *>(tree_iter->getElement(size))) {
            count_elems++;
            tree_iter->goToNext();
        }
    }
    count_elems++;
    if (count_elems == this->elements) {
        std::cout << "[+] " << this->elements << " elements were added successfully!" << std::endl;
    }else {
        std::cout << "[-] Error: Not all items have been added!" << std::endl;
    }

    if (to_print) {
        tree_iter->goToParent();
        this->tree->printChilds(tree_iter);
    }
    delete tree_iter;
    return data;
}

void Test::removeOddElements(bool to_print) {
    Tree::Iterator *tree_iter = this->tree->newIterator();
    int count = 0;
    while (count != this->elements / 2) {
        size_t size;
        int *value = static_cast<int *>(tree_iter->getElement(size));
        if (*value % 2 != 0) {
            this->tree->remove(tree_iter);
            count++;
        }else {
            tree_iter->goToNext();
        }
    }
    std::cout << "[ODD REMOVER] Odd elements were deleted." << std::endl;
    if (to_print) {
        tree->printChilds(tree_iter);
    }
    delete tree_iter;
}

void Test::isOddElementsDeleted(int *data, bool to_print) {
    for (int i = 0; i < this->elements; i++) {
        if (data[i] % 2 != 0) {
            Tree::Iterator *value = this->tree->find(&data[i], sizeof(data[i]));
            if (value) {
                std::cout << "[ODD ELEMENTS CHECKER] Error: The odd value was found!" << std::endl;
                delete value;
                break;
            }else {
                continue;
            }
        }else {
            continue;
        }
    }
    std::cout << "[ODD ELEMENTS CHECKER] The odd values weren't found." << std::endl;
    if (to_print) {
        Tree::Iterator *my_iter = this->tree->newIterator();
        std::cout << std::endl << std::endl;
        this->tree->printChilds(my_iter);
        delete my_iter;
    }
}