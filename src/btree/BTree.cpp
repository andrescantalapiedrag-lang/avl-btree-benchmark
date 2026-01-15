#include "btree/BTree.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>




BTreeNode::BTreeNode(int t, bool leaf)
    : t_(t), leaf_(leaf) {
    keys_.reserve(2 * t_ - 1);
    ch_.reserve(2 * t_);
}

bool BTreeNode::search(int key) const {
    // Find the first key >= key
    auto it = std::lower_bound(keys_.begin(), keys_.end(), key);
    int i = static_cast<int>(it - keys_.begin());

    if (it != keys_.end() && *it == key) return true;

    if (leaf_) return false;


    return ch_[i]->search(key);
}

void BTreeNode::splitChild(int i) {
    
    BTreeNode* y = ch_[i];
    assert(y && static_cast<int>(y->keys_.size()) == 2 * t_ - 1);

    
    BTreeNode* z = new BTreeNode(t_, y->leaf_);

    // Median key index in y is t_-1
    int medianIdx = t_ - 1;
    int medianKey = y->keys_[medianIdx];

   
    z->keys_.assign(y->keys_.begin() + medianIdx + 1, y->keys_.end());
    y->keys_.erase(y->keys_.begin() + medianIdx, y->keys_.end()); // removes median and above

    // Move children if not leaf: y has 2t children, split into t and t
    if (!y->leaf_) {
        // y currently has 2t_ children
        z->ch_.assign(y->ch_.begin() + t_, y->ch_.end());
        y->ch_.erase(y->ch_.begin() + t_, y->ch_.end());
    }

    // Insert new child z into this node right after y
    ch_.insert(ch_.begin() + i + 1, z);

    // Insert median key into this node's keys
    keys_.insert(keys_.begin() + i, medianKey);
}

void BTreeNode::insertNonFull(int key) {
    // Insert key into this node, assuming it is not full
    // If leaf: insert in sorted position
    if (leaf_) {
        auto it = std::lower_bound(keys_.begin(), keys_.end(), key);
        if (it != keys_.end() && *it == key) return; // ignore duplicates
        keys_.insert(it, key);
        return;
    }

    // Internal node: find child to descend into
    auto it = std::lower_bound(keys_.begin(), keys_.end(), key);
    int i = static_cast<int>(it - keys_.begin());

    // Duplicate check (if key equals a separator key, it's already in tree)
    if (it != keys_.end() && *it == key) return;

    // If the chosen child is full, split it first
    if (static_cast<int>(ch_[i]->keys_.size()) == 2 * t_ - 1) {
        splitChild(i);

        // After split, keys_[i] is the median that moved up.
        // Decide whether to descend into left child (i) or right child (i+1)
        if (key > keys_[i]) {
            i++;
        }
        else if (key == keys_[i]) {
            return; // became present as separator
        }
    }

    ch_[i]->insertNonFull(key);
}



static int ceil_div(int a, int b) {
    return (a + b - 1) / b;
}

BTree::BTree(int m)
    : m_(m), t_(ceil_div(m, 2)), root_(nullptr) {
    // m must be at least 3 for a meaningful B-tree (2 children is just a BST-like)
    assert(m_ >= 3);
    assert(t_ >= 2);
}

BTree::~BTree() {
    destroyRec(root_);
    root_ = nullptr;
}

void BTree::destroyRec(BTreeNode* node) {
    if (!node) return;
    for (BTreeNode* c : node->ch_) {
        destroyRec(c);
    }
    delete node;
}

bool BTree::search(int key) const {
    if (!root_) return false;
    return root_->search(key);
}

void BTree::insert(int key) {
    if (!root_) {
        root_ = new BTreeNode(t_, true);
        root_->keys_.push_back(key);
        return;
    }

    // If root is full, grow tree height by splitting root
    if (static_cast<int>(root_->keys_.size()) == 2 * t_ - 1) {
        BTreeNode* s = new BTreeNode(t_, false);
        s->ch_.push_back(root_);
        s->splitChild(0);
        root_ = s;
    }

    root_->insertNonFull(key);
}

void BTree::exportDot(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    out << "digraph BTree {\n";
    out << "  node [shape=record];\n";

    if (!root_) {
        out << "  empty [label=\"(empty)\"];\n";
        out << "}\n";
        return;
    }

    exportDotRec(root_, out);
    out << "}\n";
}

void BTree::exportDotRec(const BTreeNode* node, std::ostream& out) {
    if (!node) return;

    // Node label as record: |k0|k1|k2|
    out << "  n" << node << " [label=\"";
    for (size_t i = 0; i < node->keys_.size(); ++i) {
        out << "<f" << i << "> " << node->keys_[i];
        if (i + 1 < node->keys_.size()) out << " | ";
    }
    // If node has no keys (shouldn't happen normally), still print a placeholder
    if (node->keys_.empty()) out << "empty";
    out << "\"];\n";

    if (node->leaf_) return;

    // Children: connect parent to each child
    for (size_t i = 0; i < node->ch_.size(); ++i) {
        const BTreeNode* child = node->ch_[i];
        if (!child) continue;

        exportDotRec(child, out);
        out << "  n" << node << " -> n" << child << ";\n";
    }
}
