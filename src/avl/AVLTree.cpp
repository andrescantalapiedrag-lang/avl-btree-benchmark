#include "avl/AVLTree.hpp"
#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <functional>


AVLTree::AVLTree() : root_(nullptr) {}

AVLTree::~AVLTree() {
    destroyRec(root_);
    root_ = nullptr;
}

int AVLTree::height(const AVLNode* n) {
    return n ? n->height : 0;
}

int AVLTree::balanceFactor(const AVLNode* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

void AVLTree::updateHeight(AVLNode* n) {
    if (!n) return;
    n->height = 1 + std::max(height(n->left), height(n->right));
}

AVLNode* AVLTree::rotateRight(AVLNode* y) {
    //      y               x
    //     / \             / \
    //    x   T3   =>     T1  y
    //   / \                 / \
    //  T1  T2              T2  T3
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);
    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    //    x                 y
    //   / \               / \
    //  T1  y     =>      x  T3
    //     / \           / \
    //    T2 T3         T1 T2
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);
    return y;
}

AVLNode* AVLTree::rebalance(AVLNode* n) {
    if (!n) return n;

    updateHeight(n);
    int bf = balanceFactor(n);

    // Left heavy
    if (bf > 1) {
        // Left-Right case
        if (balanceFactor(n->left) < 0) {
            n->left = rotateLeft(n->left);
        }
        // Left-Left case
        return rotateRight(n);
    }

    // Right heavy
    if (bf < -1) {
        // Right-Left case
        if (balanceFactor(n->right) > 0) {
            n->right = rotateRight(n->right);
        }
        // Right-Right case
        return rotateLeft(n);
    }

    return n; // already balanced
}

AVLNode* AVLTree::insertRec(AVLNode* node, int key) {
    if (!node) return new AVLNode(key);

    if (key < node->key) {
        node->left = insertRec(node->left, key);
    }
    else if (key > node->key) {
        node->right = insertRec(node->right, key);
    }
    else {
        // Duplicate: ignore to keep set semantics (common in benchmarks)
        return node;
    }

    return rebalance(node);
}

AVLNode* AVLTree::minValueNode(AVLNode* node) {
    AVLNode* cur = node;
    while (cur && cur->left) cur = cur->left;
    return cur;
}

AVLNode* AVLTree::removeRec(AVLNode* node, int key) {
    if (!node) return nullptr;

    if (key < node->key) {
        node->left = removeRec(node->left, key);
    }
    else if (key > node->key) {
        node->right = removeRec(node->right, key);
    }
    else {
        // Node found
        if (!node->left || !node->right) {
            AVLNode* child = node->left ? node->left : node->right;
            delete node;
            return child; // might be nullptr
        }
        else {
            // Two children: replace with inorder successor
            AVLNode* succ = minValueNode(node->right);
            node->key = succ->key;
            node->right = removeRec(node->right, succ->key);
        }
    }

    return rebalance(node);
}

bool AVLTree::searchRec(const AVLNode* node, int key) {
    const AVLNode* cur = node;
    while (cur) {
        if (key < cur->key) cur = cur->left;
        else if (key > cur->key) cur = cur->right;
        else return true;
    }
    return false;
}

void AVLTree::destroyRec(AVLNode* node) {
    if (!node) return;
    destroyRec(node->left);
    destroyRec(node->right);
    delete node;
}

void AVLTree::insert(int key) {
    root_ = insertRec(root_, key);
}

void AVLTree::remove(int key) {
    root_ = removeRec(root_, key);
}

bool AVLTree::search(int key) const {
    return searchRec(root_, key);
}

// ---- Validation (optional but very helpful early on) ----

bool AVLTree::isBST(const AVLNode* node, long long minV, long long maxV) {
    if (!node) return true;
    if (node->key <= minV || node->key >= maxV) return false;
    return isBST(node->left, minV, node->key) && isBST(node->right, node->key, maxV);
}

bool AVLTree::isAVLHeightConsistent(const AVLNode* node, int& outHeight) {
    if (!node) { outHeight = 0; return true; }
    int hl = 0, hr = 0;
    if (!isAVLHeightConsistent(node->left, hl)) return false;
    if (!isAVLHeightConsistent(node->right, hr)) return false;

    int bf = hl - hr;
    int expectedH = 1 + std::max(hl, hr);
    if (bf < -1 || bf > 1) return false;
    if (node->height != expectedH) return false;

    outHeight = expectedH;
    return true;
}

bool AVLTree::isValidAVL() const {
    if (!isBST(root_,
        std::numeric_limits<long long>::min(),
        std::numeric_limits<long long>::max())) {
        return false;
    }
    int h = 0;
    return isAVLHeightConsistent(root_, h);
}

void AVLTree::exportDot(const std::string& filename) const {
    std::ofstream out(filename);

    if (!out.is_open()) {
        std::cerr << "ERROR: Cannot open file for writing: "
            << filename << std::endl;
        return;
    }

    std::cout << "Writing DOT file to: " << filename << std::endl;

    out << "digraph AVL {\n";
    out << "  node [shape=circle];\n";

    if (!root_) {
        out << "  empty [label=\"(empty)\"];\n";
        out << "}\n";
        return;
    }

    std::function<void(const AVLNode*)> dfs = [&](const AVLNode* n) {
        if (!n) return;

        out << "  n" << n << " [label=\"" << n->key << "\"];\n";

        if (n->left) {
            out << "  n" << n << " -> n" << n->left << ";\n";
            dfs(n->left);
        }
        if (n->right) {
            out << "  n" << n << " -> n" << n->right << ";\n";
            dfs(n->right);
        }
        };

    dfs(root_);
    out << "}\n";
    out.close();
}
