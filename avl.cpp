#include "avl.hpp"
#include <algorithm>
using namespace std;

AVLTree::AVLTree() {
    root = nullptr;
}

AVLTree::~AVLTree() {
    freeNodes(root);
}

int AVLTree::height(AVLNode* n) {
    if (n == nullptr) return 0;
    return n->height;
}

int AVLTree::getBalance(AVLNode* n) {
    if (n == nullptr) return 0;
    return height(n->left) - height(n->right);
}

AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

AVLNode* AVLTree::insert(AVLNode* node, const Post &post) {
    // Step 1: normal BST insert
    if (node == nullptr)
        return new AVLNode(post);

    if (post.timestamp < node->data.timestamp)
        node->left = insert(node->left, post);
    else if (post.timestamp > node->data.timestamp)
        node->right = insert(node->right, post);
    else
        return node; // duplicate timestamp, ignore

    // Step 2: update height
    node->height = 1 + max(height(node->left), height(node->right));

    // Step 3: get balance
    int balance = getBalance(node);

    // Step 4: 4 rotation cases
    // Left Left
    if (balance > 1 && post.timestamp < node->left->data.timestamp)
        return rotateRight(node);

    // Right Right
    if (balance < -1 && post.timestamp > node->right->data.timestamp)
        return rotateLeft(node);

    // Left Right
    if (balance > 1 && post.timestamp > node->left->data.timestamp) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left
    if (balance < -1 && post.timestamp < node->right->data.timestamp) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void AVLTree::addPost(const std::string &text, long long ts) {
    Post newPost(text, ts);
    root = insert(root, newPost);
}

void AVLTree::collectReverse(AVLNode* n, vector<Post> &out, int &remaining) {
    if (n == nullptr || remaining == 0) return;

    collectReverse(n->right, out, remaining);  // go right first (most recent)

    if (remaining > 0) {
        out.push_back(n->data);
        remaining--;
    }

    collectReverse(n->left, out, remaining);
}

vector<Post> AVLTree::getRecentPosts(int count) {
    vector<Post> results;
    int remaining = (count < 0) ? INT32_MAX : count;
    collectReverse(root, results, remaining);
    return results;
}

void AVLTree::freeNodes(AVLNode* n) {
    if (n == nullptr) return;
    freeNodes(n->left);
    freeNodes(n->right);
    delete n;
}