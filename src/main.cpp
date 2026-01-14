#include <iostream>
#include "avl/AVLTree.hpp"


int main() {
    AVLTree t;

    int vals[] = { 10, 20, 30, 40, 50, 25 };
    for (int v : vals) t.insert(v);
  t.exportDot("avl.dot");


    std::cout << "Search 25: " << (t.search(25) ? "true" : "false") << "\n";
    std::cout << "Search 99: " << (t.search(99) ? "true" : "false") << "\n";
    std::cout << "Valid AVL: " << (t.isValidAVL() ? "true" : "false") << "\n";

    t.remove(40);
    std::cout << "After remove 40, search 40: " << (t.search(40) ? "true" : "false") << "\n";
    std::cout << "Valid AVL: " << (t.isValidAVL() ? "true" : "false") << "\n";

    return 0;
}
