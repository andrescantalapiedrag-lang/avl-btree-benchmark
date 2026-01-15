#pragma once

#include <vector>

class BTreeNode {
public:
    BTreeNode(int t, bool leaf);

    
    bool search(int key) const;

private:
    int t_;                        // minimum degree
    bool leaf_;
    std::vector<int> keys_;        // sorted keys
    std::vector<BTreeNode*> ch_;   // children pointers (size = keys+1 when internal)

    
    void insertNonFull(int key);
    void splitChild(int i); 

    friend class BTree;
};
