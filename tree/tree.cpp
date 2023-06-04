#include "tree.h"
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <queue>

Tree::Tree() : root(nullptr) {}

Tree::~Tree() {
    destroyTree(root);
}

void Tree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

const Tree::Node* Tree::getRoot() const {
    return root;
}

void Tree::insert(double value) {
    root = insertNode(root, value);
}

Tree::Node* Tree::insertNode(Node* node, double value) const {
    if (node == nullptr) {
        return new Node(value);
    }

    if (value < node->data) {
        node->left = insertNode(node->left, value);
    } else if (value > node->data) {
        node->right = insertNode(node->right, value);
    }

    return node;
}

double Tree::findMin() const {
    if (root == nullptr) {
        throw std::runtime_error("The tree is empty");
    }

    Node* minNode = findMinNode(root);
    return minNode->data;
}

Tree::Node* Tree::findMinNode(Node* node) const {
    if (node == nullptr) {
        return nullptr;
    } else if (node->left == nullptr) {
        return node;
    }

    return findMinNode(node->left);
}


void preOrderTraversal(const Tree::Node* node) {
    if (node == nullptr) {
        return;
    }

    std::cout << node->data << " ";
    preOrderTraversal(node->left);
    preOrderTraversal(node->right);
}

void inOrderTraversal(const Tree::Node* node) {
    if (node == nullptr) {
        return;
    }

    inOrderTraversal(node->left);
    std::cout << node->data << " ";
    inOrderTraversal(node->right);
}


void postOrderTraversal(const Tree::Node* node) {
    if (node == nullptr) {
        return;
    }

    postOrderTraversal(node->left);
    postOrderTraversal(node->right);
    std::cout << node->data << " ";
}

int Tree::getHeight(const Node* node) const {
    if (node == nullptr) {
        return 0;
    }

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    return 1 + std::max(leftHeight, rightHeight);
}

int Tree::getBalanceFactor(const Node* node) const {
    if (node == nullptr) {
        return 0;
    }

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    return leftHeight - rightHeight;
}

Tree::Node* Tree::rotateLeft(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    return newRoot;
}

Tree::Node* Tree::rotateRight(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    return newRoot;
}

Tree::Node* Tree::balanceNode(Node* node) {
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1) {
        if (getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }

    if (balanceFactor < -1) {
        if (getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }

    return node;
}

Tree::Node* Tree::balanceTree() {
    std::queue<Node*> nodeQueue;
    std::vector<Node*> nodes;

    if (root != nullptr) {
        nodeQueue.push(root);
    }

    while (!nodeQueue.empty()) {
        Node* node = nodeQueue.front();
        nodeQueue.pop();
        nodes.push_back(node);

        if (node->left != nullptr) {
            nodeQueue.push(node->left);
        }
        if (node->right != nullptr) {
            nodeQueue.push(node->right);
        }
    }

    std::sort(nodes.begin(), nodes.end(), [](const Node* a, const Node* b) {
        return a->data < b->data;
    });

    root = balanceTreeHelper(nodes, 0, nodes.size() - 1);

    return root;
}

Tree::Node* Tree::balanceTreeHelper(std::vector<Node*>& nodes, int start, int end) {
    if (start > end) {
        return nullptr;
    }

    int mid = start + (end - start) / 2;
    Node* node = nodes[mid];

    node->left = balanceTreeHelper(nodes, start, mid - 1);
    node->right = balanceTreeHelper(nodes, mid + 1, end);

    return node;
}

//Вертикальная печать дерева
void printVerticalTree(const Tree::Node* node, int level, std::map<int, std::vector<double>>& levelMap) {
    if (node == nullptr) {
        return;
    }

    levelMap[level].push_back(node->data);

    printVerticalTree(node->left, level - 1, levelMap);
    printVerticalTree(node->right, level + 1, levelMap);
}

void printTreeVerticalOrder(const Tree::Node* root) {
    std::map<int, std::vector<double>> levelMap;

    printVerticalTree(root, 0, levelMap);

    for (const auto& levelPair : levelMap) {
        std::cout << "Level " << levelPair.first << ": ";
        for (const auto& value : levelPair.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

//Горизонтальная печать дерева
void printTreeHorizontal(const Tree::Node* root) {
    if (root == nullptr) {
        return;
    }

    std::queue<const Tree::Node*> nodeQueue;
    nodeQueue.push(root);

    while (!nodeQueue.empty()) {
        int levelSize = nodeQueue.size();

        for (int i = 0; i < levelSize; i++) {
            const Tree::Node* currentNode = nodeQueue.front();
            nodeQueue.pop();

            std::cout << currentNode->data << " ";

            if (currentNode->left != nullptr) {
                nodeQueue.push(currentNode->left);
            }

            if (currentNode->right != nullptr) {
                nodeQueue.push(currentNode->right);
            }
        }

        std::cout << std::endl;
    }
}


