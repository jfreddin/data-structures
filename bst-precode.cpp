#include <bits/stdc++.h>

using namespace std;

class Node {
public:
    int key;
    Node *left, *right, *parent;

    Node(int key) {
        this->key = key;
        left = right = parent = nullptr;
    }

    void insertNode(Node*& root, int key) {
        Node* z = new Node(key);
        Node* y = nullptr;
        Node* x = root;

        while (x != nullptr) {
            y = x;
            if (z->key < x->key) x = x->left;
            else if (z->key > x->key) x = x->right;
            else { delete z; return; }
        }

        z->parent = y;
        if (y == nullptr) root = z;
        else if (z->key < y->key) y->left = z;
        else y->right = z;
    }

    Node* searchNode(int k) {
        Node* cur = this;
        while (cur != nullptr && cur->key != k) {
            cur = (k < cur->key) ? cur->left : cur->right;
        }
        return cur;
    }

    Node* minimum() {
        Node* cur = this;
        while (cur->left != nullptr) cur = cur->left;
        return cur;
    }

    Node* maximum() {
        Node* cur = this;
        while (cur->right != nullptr) cur = cur->right;
        return cur;
    }

    static void transplant(Node*& root, Node* u, Node* v) {
        if (u->parent == nullptr) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v != nullptr) v->parent = u->parent;
    }

    void deleteNode(Node*& root, int key) {
        Node* z = root ? root->searchNode(key) : nullptr;
        if (z == nullptr) return;

        if (z->left == nullptr) {
            transplant(root, z, z->right);
        } else if (z->right == nullptr) {
            transplant(root, z, z->left);
        } else {
            Node* y = z->right->minimum();
            if (y->parent != z) {
                transplant(root, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(root, z, y);
            y->left = z->left;
            y->left->parent = y;
        }

        delete z;
    }
};

void inorder(Node* root, vector<int>& out) {
    if (root == nullptr) return;
    inorder(root->left, out);
    out.push_back(root->key);
    inorder(root->right, out);
}

int main() {
    Node* root = nullptr;
    vector<int> keys = {10, 18, 7, 15, 16, 30, 25, 40, 60, 2, 17};

    for (int k : keys) {
        if (root == nullptr) {
            root = new Node(k);
        } else {
            root->insertNode(root, k);
        }
    }

    vector<int> out;
    inorder(root, out);
    for (int x : out) cout << x << " ";
    cout << "\n";

    int target = 15;
    Node* found = root->searchNode(target);
    cout << "search " << target << ": " << (found != nullptr) << "\n";

    root->deleteNode(root, 18);
    root->deleteNode(root, 30);

    out.clear();
    inorder(root, out);
    for (int x : out) cout << x << " ";
    cout << "\n";

    return 0;
}