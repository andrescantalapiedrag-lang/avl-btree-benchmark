#pragma once
#include "avl/AVLNode.hpp"
#include <string>
#include <ostream>



class AVLTree {
public:
    AVLTree();
    ~AVLTree();

    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    void exportDot(const std::string& filename) const;


    void insert(int key);
    void remove(int key);
    bool search(int key) const;

    // Optional helper for debugging / correctness
    bool isValidAVL() const;

private:
    AVLNode* root_;

    // Core helpers
    static int height(const AVLNode* n);
    static int balanceFactor(const AVLNode* n);
    static void updateHeight(AVLNode* n);

    static void exportDotRec(const AVLNode* node, std::ostream& out);
    static void exportNullChildren(const AVLNode* node, std::ostream& out);


    static AVLNode* rotateRight(AVLNode* y);
    static AVLNode* rotateLeft(AVLNode* x);

    static AVLNode* rebalance(AVLNode* n);

    static AVLNode* insertRec(AVLNode* node, int key);
    static AVLNode* removeRec(AVLNode* node, int key);
    static AVLNode* minValueNode(AVLNode* node);

    static bool searchRec(const AVLNode* node, int key);

    static void destroyRec(AVLNode* node);

    // Validation helpers
    static bool isBST(const AVLNode* node, long long minV, long long maxV);
    static bool isAVLHeightConsistent(const AVLNode* node, int& outHeight);
};
