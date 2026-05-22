#ifndef AVL_HPP
#define AVL_HPP

#include <string>
#include <vector>

struct Post {
    std::string content;
    long long timestamp;

    Post(const std::string &text, long long ts) {
        content = text;
        timestamp = ts;
    }
};

struct AVLNode {
    Post data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const Post &p) : data(p) {
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* n);
    int getBalance(AVLNode* n);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* insert(AVLNode* node, const Post &post);
    void collectReverse(AVLNode* n, std::vector<Post> &out, int &remaining);
    void freeNodes(AVLNode* n);

public:
    AVLTree();
    ~AVLTree();
    void addPost(const std::string &text, long long ts);
    std::vector<Post> getRecentPosts(int count);
};

#endif