#include "Test.h"

int main(void)
{
    size_t size;
    Mem _memory(100);
    Tree my_tree(_memory);

    Tree::Iterator *my_iter = my_tree.newIterator();
    
    int data[6] = {0, 1, 2, 3, 4, 5};
    my_tree.insert(my_iter, 0, &data[0], sizeof(int));

    my_tree.insert(my_iter, 0, &data[1], sizeof(int));
    my_tree.insert(my_iter, 1, &data[2], sizeof(int));

    my_iter->goToChild(0);
    my_tree.insert(my_iter, 0, &data[3], sizeof(int));
    //my_tree.remove(my_iter);
    //my_tree.remove(my_iter);
    my_iter->goToNext();
    my_iter->goToNext();
    my_iter->goToParent();
    my_iter->goToChild(1);
    my_tree.remove(my_iter);
    my_iter->goToParent();

    my_tree.printChilds(my_iter);
    delete my_iter;

    /* int elements = 100;
    Test my_test(&my_tree, elements);
    int *data = my_test.insertElements(false);
    my_test.removeOddElements(false);
    my_test.isOddElementsDeleted(data, true);
    free(data);
    data = nullptr; */
    return 0;
}