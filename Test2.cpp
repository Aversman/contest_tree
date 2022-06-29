#include "Test.h"

int *Test::insertElements(bool to_print) {
    int local_count = this->elements;
    Tree::Iterator *my_iter = this->tree->newIterator();

    int *data = (int *)malloc(sizeof(int) * (this->elements + 1));
    for (int i = 0; i < this->elements + 1; i++) {
        data[i] = i;
    }

    this->tree->insert(my_iter, 0, &data[0], sizeof(int));
    this->tree->insert(my_iter, 0, &data[1], sizeof(int));
    this->tree->insert(my_iter, 0, &data[2], sizeof(int));

    my_iter->goToChild(0);
    for (int i = 3; i < this->elements; i++) {
        if (data[i] % 2 != 0) {
            this->tree->insert(my_iter, 0, &data[i], sizeof(int));
            my_iter->goToChild(0);
        }else {
            continue;
        }
    }
    delete my_iter;
    my_iter = this->tree->newIterator();
    my_iter->goToChild(1);
    for (int i = 3; i < this->elements; i++) {
        if (data[i] % 2 == 0) {
            this->tree->insert(my_iter, 0, &data[i], sizeof(int));
            my_iter->goToChild(0);
        }else {
            continue;
        }
    }
    std::cout << "[+] " << this->elements << " elements were added successfully!" << std::endl;
    if (to_print) {
        this->tree->printChilds(my_iter);
    }
    delete my_iter;
    return data;
}

void Test::removeOddElements(bool to_print) {
    Tree::Iterator *my_iter = this->tree->newIterator();

    my_iter->goToChild(0);
    this->tree->remove(my_iter);
    std::cout << "[ODD REMOVER] Odd elements were deleted." << std::endl;
    if (to_print) {
        this->tree->printChilds(my_iter);
    }
    delete my_iter;
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
        this->tree->printChilds(my_iter);
        delete my_iter;
    }
}