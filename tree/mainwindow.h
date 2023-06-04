#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "treewidget.h"
#include "tree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TreeWidget *treeWidget;
    Tree tree;

private slots:
    void insertValue();
    void findMinValue();
    void traversePreOrder();
    void preOrderTraversal(const Tree::Node* node, QString& result);
    void traverseInOrder();
    void inOrderTraversal(const Tree::Node* node, QString& result);
    void traversePostOrder();
    void postOrderTraversal(const Tree::Node* node, QString& result);
    void balanceTree();
    void addTreeNodes(const Tree::Node* node, QTreeWidgetItem* parentItem);
    void printVerticalTree();
    void printVerticalTreeHelper(const Tree::Node* node, int level, std::map<int, std::vector<double>>& levelMap);
    void printHorizontalTree();
};

#endif // MAINWINDOW_H
