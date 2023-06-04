#ifndef TREE_H
#define TREE_H

#include <QTreeWidget>

class Tree {
public:
    struct Node {
        double data;
        Node* left;
        Node* right;

        Node(double value) : data(value), left(nullptr), right(nullptr) {}
    };

    Tree();
    ~Tree();
    void insert(double value);
    double findMin() const;
    const Node* getRoot() const;
    Node* balanceTree();
    Node* balanceTreeHelper(std::vector<Node*>& nodes, int start, int end);

    QTreeWidgetItem* createTreeItem(const Node* node) const;
    void visualize(QTreeWidget* treeWidget) const;

private:
    Node* root;

    void destroyTree(Node* node);
    Node* insertNode(Node* node, double value) const;
    Node* findMinNode(Node* node) const;
    int getHeight(const Node* node) const;
    int getBalanceFactor(const Node* node) const;
    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);
    Node* balanceNode(Node* node);
};

#endif  // TREE_H
