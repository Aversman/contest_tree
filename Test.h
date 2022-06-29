#pragma once
#include "Mem.h"
#include "Tree.h"

class Test {
    Tree *tree;
    int elements;
public:
    Test(Tree *tree, int elements): tree(tree), elements(elements) {}
    int *insertElements(bool to_print);
    void removeOddElements(bool to_print);
    void isOddElementsDeleted(int *data, bool to_print);
};