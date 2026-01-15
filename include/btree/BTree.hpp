#pragma once

#include <string>
#include "btree/BTreeNode.hpp"
#include <ostream>


class BTree {
public:
    explicit BTree(int m);   // order m: max number of children per node
    ~BTree();

    BTree(const BTree&) = delete;
    BTree& operator=(const BTree&) = delete;

    bool search(int key) const;
    void insert(int key);
    void exportDot(const std::string& filename) const;

   

private:
    int m_;        // max children
    int t_;        // minimum degree = ceil(m/2)
    BTreeNode* root_;

    static void destroyRec(BTreeNode* node);
    static void exportDotRec(const BTreeNode* node, std::ostream& out);

};
