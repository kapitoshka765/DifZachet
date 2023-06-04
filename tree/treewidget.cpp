#include "treewidget.h"
#include "qlabel.h"

TreeWidget::TreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setHeaderHidden(true);
}

void TreeWidget::setTree(const Tree::Node* root)
{
    clearTree();
    createTreeItem(root);
}

void TreeWidget::clearTree()
{
    clear();
}

void TreeWidget::createTreeItem(const Tree::Node* node, QTreeWidgetItem* parentItem)
{
    if (node == nullptr)
        return;

    QTreeWidgetItem* item;
    if (parentItem == nullptr)
    {
        item = new QTreeWidgetItem(this);
        setItemWidget(item, 0, new QLabel(QString::number(node->data)));
    }
    else
    {
        item = new QTreeWidgetItem(parentItem);
        parentItem->addChild(item);
        setItemWidget(item, 0, new QLabel(QString::number(node->data)));
    }

    createTreeItem(node->left, item);
    createTreeItem(node->right, item);
}
