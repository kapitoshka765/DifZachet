#ifndef SOSAKC_H
#define SOSAKC_H

#include <QDialog>

namespace Ui {
class sosakc;
}

class sosakc : public QDialog
{
    Q_OBJECT

public:
    explicit sosakc(QWidget *parent = nullptr);
    ~sosakc();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_11_clicked();

private:
    Ui::sosakc *ui;
    double fetchStockPrice(const QString& fromStockCode);
};

#endif // SOSAKC_H
