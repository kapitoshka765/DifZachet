#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <iostream>
#include <queue>
#include "tree.h"
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    treeWidget = new TreeWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(ui->centralwidget);
    layout->addWidget(treeWidget);

    QPushButton *insertButton = new QPushButton("Ввод", this);
    QPushButton *findMinButton = new QPushButton("Нахождение минимального", this);
    QPushButton *preOrderButton = new QPushButton("Прямой обход", this);
    QPushButton *traverseInOrderButton = new QPushButton("Симметричный обход", this);
    QPushButton *traversePostOrderButton = new QPushButton("Обратный обход", this);
    QPushButton *balanceButton = new QPushButton("Балансировка", this);
    QPushButton *printVerticalTreeButton = new QPushButton("Вертикальная печать", this);
    QPushButton *printHorizontalTreeButton = new QPushButton("Горизонтальная печать", this);


    QHBoxLayout *buttonLayout = new QHBoxLayout;
    layout->addWidget(insertButton);
    layout->addWidget(findMinButton);
    layout->addWidget(preOrderButton);
    layout->addWidget(traverseInOrderButton);
    layout->addWidget(traversePostOrderButton);
    layout->addWidget(balanceButton);
    layout->addWidget(printVerticalTreeButton);
    layout->addWidget(printHorizontalTreeButton);

    layout->addLayout(buttonLayout);

    connect(insertButton, &QPushButton::clicked, this, &MainWindow::insertValue);
    connect(findMinButton, &QPushButton::clicked, this, &MainWindow::findMinValue);
    connect(preOrderButton, &QPushButton::clicked, this, &MainWindow::traversePreOrder);
    connect(traverseInOrderButton, &QPushButton::clicked, this, &MainWindow::traverseInOrder);
    connect(traversePostOrderButton, &QPushButton::clicked, this, &MainWindow::traversePostOrder);
    connect(balanceButton, &QPushButton::clicked, this, &MainWindow::balanceTree);
    connect(printVerticalTreeButton, &QPushButton::clicked, this, &MainWindow::printVerticalTree);
    connect(printHorizontalTreeButton, &QPushButton::clicked, this, &MainWindow::printHorizontalTree);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::insertValue()
{
    bool ok;
    double value = QInputDialog::getDouble(this, "Введите значение", "Введите значение:", 0, -2147483647, 2147483647, 1, &ok);
    if (ok) {
        tree.insert(value);
        treeWidget->setTree(tree.getRoot());
    }
}

void MainWindow::findMinValue()
{
    try {
        double minValue = tree.findMin();
        QMessageBox::information(this, "Минимальное значение", "Минимальное значение в дереве: " + QString::number(minValue));
    } catch (const std::runtime_error& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::traversePreOrder()
{
    QString result = "Прямой обход: ";
    preOrderTraversal(tree.getRoot(), result);

    QMessageBox::information(this, "Прямой обход", result);
}

void MainWindow::preOrderTraversal(const Tree::Node* node, QString& result)
{
    if (node == nullptr)
        return;

    result += QString::number(node->data) + " ";

    preOrderTraversal(node->left, result);
    preOrderTraversal(node->right, result);
}

void MainWindow::traverseInOrder()
{
    QString result = "Симметричный обход: ";
    inOrderTraversal(tree.getRoot(), result);

    QMessageBox::information(this, "Симметричный обход", result);
}

void MainWindow::inOrderTraversal(const Tree::Node* node, QString& result)
{
    if (node == nullptr)
        return;

    inOrderTraversal(node->left, result);
    result += QString::number(node->data) + " ";
    inOrderTraversal(node->right, result);
}

void MainWindow::traversePostOrder()
{
    QString result = "Обратный обход: ";
    postOrderTraversal(tree.getRoot(), result);

    QMessageBox::information(this, "Обратный обход", result);
}

void MainWindow::postOrderTraversal(const Tree::Node* node, QString& result)
{
    if (node == nullptr)
        return;

    postOrderTraversal(node->left, result);
    postOrderTraversal(node->right, result);
    result += QString::number(node->data) + " ";
}

void MainWindow::balanceTree()
{
    tree.balanceTree();

    const Tree::Node* root = tree.getRoot();

    treeWidget->clear();

    addTreeNodes(root, nullptr);

    treeWidget->expandAll();
}

void MainWindow::addTreeNodes(const Tree::Node* node, QTreeWidgetItem* parentItem)
{
    if (node == nullptr)
        return;

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, QString::number(node->data));

    if (parentItem != nullptr)
        parentItem->addChild(item);
    else
        treeWidget->addTopLevelItem(item);

    addTreeNodes(node->left, item);
    addTreeNodes(node->right, item);
}

void MainWindow::printVerticalTreeHelper(const Tree::Node* node, int level, std::map<int, std::vector<double>>& levelMap)
{
    if (node == nullptr) {
        return;
    }

    levelMap[level].push_back(node->data);

    printVerticalTreeHelper(node->left, level - 1, levelMap);
    printVerticalTreeHelper(node->right, level + 1, levelMap);
}

void MainWindow::printVerticalTree()
{
    std::map<int, std::vector<double>> levelMap;
    printVerticalTreeHelper(tree.getRoot(), 0, levelMap);

    QString messageText;
    for (const auto& levelPair : levelMap) {
        messageText += "Уровень " + QString::number(levelPair.first) + ": ";
        for (const auto& value : levelPair.second) {
            messageText += QString::number(value) + " ";
        }
        messageText += "\n";
    }

    QMessageBox::information(this, "Вертикальная печать дерева", messageText);
}

void MainWindow::printHorizontalTree()
{
    QString messageText;
    std::stringstream ss;

    std::queue<const Tree::Node*> nodeQueue;
    nodeQueue.push(tree.getRoot());

    while (!nodeQueue.empty()) {
        int levelSize = nodeQueue.size();

        for (int i = 0; i < levelSize; i++) {
            const Tree::Node* currentNode = nodeQueue.front();
            nodeQueue.pop();

            ss << currentNode->data << " ";

            if (currentNode->left != nullptr) {
                nodeQueue.push(currentNode->left);
            }

            if (currentNode->right != nullptr) {
                nodeQueue.push(currentNode->right);
            }
        }

        ss << std::endl;
    }

    messageText = QString::fromStdString(ss.str());

    QMessageBox::information(this, "Горизонтальная печать дерева", messageText);
}
