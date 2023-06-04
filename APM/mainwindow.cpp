#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QString"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QTextEdit>
#include <dragmetal.h>
#include <obmakc.h>
#include <sosakc.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_comboBox_currentIndexChanged(int /*index*/)
{
    QString selectedFromCurrency = ui->comboBox->currentText();
    QString selectedToCurrency = ui->comboBox_2->currentText();

    if (!selectedFromCurrency.isEmpty()) {
        fromCurrency = selectedFromCurrency;
    } else {
        qDebug() << "Invalid fromCurrency";
    }

    if (!selectedToCurrency.isEmpty()) {
        toCurrency = selectedToCurrency;
    } else {
        qDebug() << "Invalid toCurrency";
    }
}

void MainWindow::on_comboBox_2_currentIndexChanged(int /*index*/)
{
    QString selectedFromCurrency = ui->comboBox->currentText();
    QString selectedToCurrency = ui->comboBox_2->currentText();

    if (!selectedFromCurrency.isEmpty()) {
        fromCurrency = selectedFromCurrency;
    } else {
        qDebug() << "Invalid fromCurrency";
    }

    if (!selectedToCurrency.isEmpty()) {
        toCurrency = selectedToCurrency;
    } else {
        qDebug() << "Invalid toCurrency";
    }
}


double MainWindow::convertCurrency(const QString& fromCurrency, const QString& toCurrency)
{
    QNetworkAccessManager manager;
    QUrl url("https://www.cbr-xml-daily.ru/daily_json.js"); // URL API Центробанка

    QNetworkRequest request(url);

    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        // Обработка ошибки запроса
        qDebug() << "Error: " << reply->errorString();
        return 0.0;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);

    if (jsonDocument.isNull()) {
        // Обработка ошибки парсинга JSON
        qDebug() << "Error: Failed to parse JSON response";
        return 0.0;
    }

    QJsonObject jsonObject = jsonDocument.object();
    QJsonObject valuteObject = jsonObject["Valute"].toObject();

    double fromRate = 0.0;
    double toRate = 0.0;

    if (fromCurrency == "RUB") {
        // Конвертация из рубля
        fromRate = 1.0;
    } else {
        if (valuteObject.contains(fromCurrency)) {
            QJsonObject fromCurrencyObject = valuteObject[fromCurrency].toObject();
            fromRate = fromCurrencyObject["Value"].toDouble();
        } else {
            qDebug() << "Invalid fromCurrency:" << fromCurrency;
        }
    }

    if (toCurrency == "RUB") {
        toRate = 1.0;
    }

    if (valuteObject.contains(toCurrency)) {
        QJsonObject toCurrencyObject = valuteObject[toCurrency].toObject();
        toRate = toCurrencyObject["Value"].toDouble();
    } else {
        qDebug() << "Invalid toCurrency:" << toCurrency;
    }

    if (fromRate == 0.0 || toRate == 0.0) {
        // Обработка некорректных значений курсов валют
        qDebug() << "Error: Invalid currency code";
        return 0.0;
    }

    double inputValue = ui->textEdit->toPlainText().toDouble(); // Получение значения из первого текстового поля
    double convertedValue = inputValue * (fromRate / toRate);

    return convertedValue;
}


void MainWindow::on_pushButton_4_clicked()
{
    if (fromCurrency == toCurrency) {
        QString inputValue = ui->textEdit->toPlainText();
        ui->textEdit_2->setText(inputValue);
        return;
    }
    // Выполнение операции конвертации и обновление значения валюты
    double convertedValue = convertCurrency(fromCurrency, toCurrency);
    QString roundedValue = QString::number(convertedValue, 'f', 2);  // Округление до сотых
    ui->textEdit_2->setText(roundedValue);
}



void MainWindow::on_pushButton_6_clicked()
{
    close();
    DragMetal dialog; // Create an instance of the Drag_Metal dialog
    dialog.setModal(true); // Set the dialog as modal (blocks input to other windows)
    dialog.exec(); // Execute the dialog, which will open it
}


void MainWindow::on_pushButton_3_clicked()
{
    close();
    obmakc dialog;
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::on_pushButton_5_clicked()
{
    close();
    sosakc dialog;
    dialog.setModal(true);
    dialog.exec();
}

