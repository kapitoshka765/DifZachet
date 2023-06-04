#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeWidget>
#include "tree.h"

class TreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    TreeWidget(QWidget *parent = nullptr);
    void setTree(const Tree::Node* root);

private:
    void clearTree();
    void createTreeItem(const Tree::Node* node, QTreeWidgetItem* parentItem = nullptr);

};

#endif // TREEWIDGET_H
