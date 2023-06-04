#ifndef DRAGMETAL_H
#define DRAGMETAL_H

#include <QDialog>

namespace Ui {
class DragMetal;
}

class DragMetal : public QDialog
{
    Q_OBJECT

public:
    explicit DragMetal(QWidget *parent = nullptr);
    ~DragMetal();

private slots:
    void on_pushButton_4_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_pushButton_11_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::DragMetal *ui;
    QString fromCurrency;
    QString toCurrency;
    double convertCurrency(const QString& fromCurrency, const QString& toCurrency);
    void populateMetalComboBox();
    void populateCurrencyComboBox();
    double getCurrencyMultiplier();
};

#endif // DRAGMETAL_H
