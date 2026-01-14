#pragma once

struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;

    explicit AVLNode(int k)
        : key(k), left(nullptr), right(nullptr), height(1) {}
};