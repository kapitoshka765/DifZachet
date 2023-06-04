#ifndef OBMAKC_H
#define OBMAKC_H

#include "ui_obmakc.h"
#include <QDialog>

namespace Ui {
class obmakc;
}

class obmakc : public QDialog
{
    Q_OBJECT

public:
    explicit obmakc(QWidget *parent = nullptr);
    ~obmakc();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_pushButton_11_clicked();

private:
    Ui::obmakc *ui;
    QString fromStockCode;
    QString toStockCode;
    double fetchStockPrice(const QString& fromStockCode, const QString& toStockCode);
    void calculateTargetStockQuantity(double fromStockPrice, double toStockPrice);
    void fetchStockPrices(const QString& fromStockCode, const QString& toStockCode);
};

#endif // OBMAKC_H
