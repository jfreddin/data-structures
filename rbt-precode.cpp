#include <bits/stdc++.h>

using namespace std;

enum Color { RED, BLACK };

class Node {
public:
    int key;
    Color color;
    Node *left, *right, *parent;

    static Node* NIL;

    Node(int key, Color color = RED) {
        this->key = key;
        this->color = color;
        left = right = parent = nullptr;
    }

    static void initNil() {
        if (NIL == nullptr) {
            NIL = new Node(0, BLACK);
            NIL->left = NIL->right = NIL->parent = NIL;
        }
    }

    bool isNil() {
        return this == NIL;
    }

    Node* searchNode(int k) {
        Node* cur = this;
        while (!cur->isNil() && k != cur->key) {
            cur = (k < cur->key) ? cur->left : cur->right;
        }
        return cur->isNil() ? nullptr : cur;
    }

    Node* minimum() {
        Node* cur = this;
        while (!cur->left->isNil()) cur = cur->left;
        return cur;
    }

    void leftRotate(Node*& root) {
        Node* y = right;
        right = y->left;
        if (!y->left->isNil()) y->left->parent = this;
        y->parent = parent;
        if (parent->isNil()) root = y;
        else if (this == parent->left) parent->left = y;
        else parent->right = y;
        y->left = this;
        parent = y;
    }

    void rightRotate(Node*& root) {
        Node* y = left;
        left = y->right;
        if (!y->right->isNil()) y->right->parent = this;
        y->parent = parent;
        if (parent->isNil()) root = y;
        else if (this == parent->right) parent->right = y;
        else parent->left = y;
        y->right = this;
        parent = y;
    }

    void insertNode(Node*& root, int key) {
        initNil();
        Node* z = new Node(key, RED);
        z->left = z->right = z->parent = NIL;

        Node* y = NIL;
        Node* x = root;

        while (!x->isNil()) {
            y = x;
            if (z->key < x->key) x = x->left;
            else if (z->key > x->key) x = x->right;
            else { delete z; return; }
        }

        z->parent = y;
        if (y->isNil()) root = z;
        else if (z->key < y->key) y->left = z;
        else y->right = z;

        z->insertFixup(root);
    }

    void insertFixup(Node*& root) {
        Node* z = this;
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        z->leftRotate(root);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    z->parent->parent->rightRotate(root);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        z->rightRotate(root);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    z->parent->parent->leftRotate(root);
                }
            }
        }
        root->color = BLACK;
    }

    static void transplant(Node*& root, Node* u, Node* v) {
        if (u->parent->isNil()) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    void deleteNode(Node*& root, int key) {
        Node* z = root ? root->searchNode(key) : nullptr;
        if (z == nullptr) return;

        Node* y = z;
        Node* x;
        Color yOriginalColor = y->color;

        if (z->left->isNil()) {
            x = z->right;
            transplant(root, z, z->right);
        } else if (z->right->isNil()) {
            x = z->left;
            transplant(root, z, z->left);
        } else {
            y = z->right->minimum();
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(root, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(root, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == BLACK) {
            x->deleteFixup(root);
        }
    }

    void deleteFixup(Node*& root) {
        Node* x = this;
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    x->parent->leftRotate(root);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        w->rightRotate(root);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    x->parent->leftRotate(root);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    x->parent->rightRotate(root);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        w->leftRotate(root);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    x->parent->rightRotate(root);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }
};

Node* Node::NIL = nullptr;

void inorder(Node* root, vector<int>& out) {
    if (root->isNil()) return;
    inorder(root->left, out);
    out.push_back(root->key);
    inorder(root->right, out);
}

int main() {
    Node::initNil();
    Node* root = Node::NIL;

    vector<int> keys = {10, 18, 7, 15, 16, 30, 25, 40, 60, 2, 17};

    for (int k : keys) {
        root->insertNode(root, k);
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