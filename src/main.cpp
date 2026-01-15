

//AVL main

/*#include <iostream>
#include "avl/AVLTree.hpp"
/*
int main() {
    std::cout << "MAIN EDIT CONFIRMED\n";
    AVLTree t;
    t.insert(10);
    t.insert(20);
    t.insert(30);

    std::cout << "ABOUT TO EXPORT DOT\n";
    t.exportDot("avl_rr_rotation.dot");
    std::cout << "DONE EXPORT DOT\n";
    return 0;
}
*/
/*

#include <iostream>
#include "avl/AVLTree.hpp"

int main() {
    AVLTree t;

    
    int values[] = { 10, 20, 30, 40, 50, 25 };
    for (int v : values) {
        t.insert(v);
    }

    
    t.exportDot("avl_after_inserts.dot");

    
    t.remove(40);

    
    t.exportDot("avl_after_delete.dot");

    return 0;
}
*/




#include <iostream>
#include "btree/BTree.hpp"

int main() {
    BTree bt(5); // order m = 5
    int bvals[] = { 10, 20, 5, 6, 12, 30, 7, 17 };
    for (int v : bvals) bt.insert(v);

    bt.exportDot("btree_after_inserts.dot");



    std::cout << "BTree search 6: " << (bt.search(6) ? "true" : "false") << "\n";
    std::cout << "BTree search 99: " << (bt.search(99) ? "true" : "false") << "\n";
    return 0;
}







