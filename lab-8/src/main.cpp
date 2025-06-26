#include <iostream>

struct TreeNode {
    int key;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;

    TreeNode(int key) : key(key), left(nullptr), right(nullptr), parent(nullptr) {}
};

TreeNode* findPreorderSuccessor(TreeNode* node) {
    if (!node) return nullptr;
    if (node->left) return node->left;
    if (node->right) return node->right;
    TreeNode* successor = nullptr;
    TreeNode* ancestor = node->parent;

    while (ancestor) {
        if (ancestor->left == node) {
            successor = ancestor->right;
            break;
        }
        if (ancestor->parent && ancestor->parent->left == ancestor && ancestor->parent->right) {
            successor = ancestor->parent->right;
            break;
        }
        node = ancestor;
        ancestor = ancestor->parent;
    }

    return successor;
}

TreeNode* insertNode(TreeNode* root, int key) {
    if (root == nullptr) {
        return new TreeNode(key);
    }
    if (key < root->key) {
        root->left = insertNode(root->left, key);
        root->left->parent = root;
    }
    else if (key > root->key) {
        root->right = insertNode(root->right, key);
        root->right->parent = root;
    }
    return root;
}

TreeNode* findRightmostDescendant(TreeNode* node) {
    if (!node) return nullptr;
    while (node->right) {
        node = node->right;
    }
    return node;
}

TreeNode* findPreorderPredecessor(TreeNode* root, TreeNode* n) {
    if (n == root) return nullptr;
    TreeNode* parent = n->parent;
    if (parent->left == nullptr || parent->left == n) return parent;
    TreeNode* curr = parent->left;
    while (curr->right != nullptr) curr = curr->right;
    return curr;
}

TreeNode* findLeftmostChild(TreeNode* node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

TreeNode* postorderSuccessor(TreeNode* root, TreeNode* n) {
    if (n == root) return nullptr;
    TreeNode* parent = n->parent;
    if (parent->right == nullptr || parent->right == n) return parent;
    TreeNode* curr = parent->right;
    while (curr->left != nullptr) curr = curr->left;
    return curr;
}

TreeNode* postorderPredecessor(TreeNode* root, TreeNode* n) {
    if (n->right) return n->right;
    if (n->left) return n->left;
    TreeNode* curr = n, * parent = curr->parent;
    while (parent != nullptr && parent->left == curr) {
        curr = curr->parent;
        parent = parent->parent;
    }
    if (parent == nullptr) return nullptr;

    return parent->left;
}

int main() {

    TreeNode* root = new TreeNode(10);
    root = insertNode(root, 6);
    root = insertNode(root, 12);
    root = insertNode(root, 8);
    root = insertNode(root, 7);
    root = insertNode(root, 5);

    return 0;
}
