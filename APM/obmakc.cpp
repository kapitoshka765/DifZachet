#include "obmakc.h"
#include "ui_obmakc.h"
#include <dragmetal.h>
#include <mainwindow.h>
#include <sosakc.h>
#include "QString"
#include "qeventloop.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QTextEdit>
#include <QDomDocument>

obmakc::obmakc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::obmakc)
{
    ui->setupUi(this);
    QString fromStockCode = ui->comboBox->currentText();
    QString toStockCode = ui->comboBox_2->currentText();
}

obmakc::~obmakc()
{
    delete ui;
}

void obmakc::on_pushButton_4_clicked()
{
    close();
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
}


void obmakc::on_pushButton_7_clicked()
{
    close();
    DragMetal dialog;
    dialog.setModal(true);
    dialog.exec();
}


void obmakc::on_pushButton_10_clicked()
{
    close();
    sosakc dialog;
    dialog.setModal(true);
    dialog.exec();
}


void obmakc::on_comboBox_currentIndexChanged(int /*index*/)
{
    QString fromStockCode = ui->comboBox->currentText();
    QString toStockCode = ui->comboBox_2->currentText();
}

void obmakc::on_comboBox_2_currentIndexChanged(int /*index*/)
{
    QString fromStockCode = ui->comboBox->currentText();
    QString toStockCode = ui->comboBox_2->currentText();
}

double obmakc::fetchStockPrice(const QString& stockCode, const QString& apiKey)
{
    QNetworkAccessManager manager;
    QUrl url(QString("https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=%1&apikey=%2").arg(stockCode).arg(apiKey));

    QNetworkRequest request(url);
    QNetworkReply* reply = manager.get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        // Handle request error
        qDebug() << "Error fetching stock price: " << reply->errorString();
        return 0.0;
    }

    QByteArray responseData = reply->readAll();
    qDebug() << "Stock Response Data:" << responseData;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);

    if (jsonDocument.isNull()) {
        // Handle JSON parsing error
        qDebug() << "Error: Failed to parse JSON response";
        return 0.0;
    }

    QJsonObject jsonObject = jsonDocument.object();
    QJsonObject globalQuoteObject = jsonObject["Global Quote"].toObject();
    double stockPrice = globalQuoteObject["05. price"].toString().toDouble();
    qDebug() << "Stock Price:" << stockPrice;
    return stockPrice;
}

void obmakc::fetchStockPrices(const QString& fromStockCode, const QString& toStockCode)
{
    QString fromApiKey = "9EAQNXHZ4IJ46YGI";
    QString toApiKey = "28N1I3P2Z1DBTM4P";

    double fromStockPrice = fetchStockPrice(fromStockCode, fromApiKey);
    double toStockPrice = fetchStockPrice(toStockCode, toApiKey);

    calculateTargetStockQuantity(fromStockPrice, toStockPrice);
}
void obmakc::calculateTargetStockQuantity(double fromStockPrice, double toStockPrice)
{
    double inputValue = ui->textEdit->toPlainText().toDouble();

    qDebug() << "Input Value:" << inputValue;

    // Check for division by zero
    if (toStockPrice == 0.0) {
        qDebug() << "Error: To stock price is zero";
        return;
    }

    // Calculate the number of target stocks that can be bought
    double targetStockQuantity = (inputValue * fromStockPrice) / toStockPrice;
    qDebug() << "Target Stock Quantity:" << targetStockQuantity;

    // Display the result in textEdit_2
    ui->textEdit_2->setText(QString::number(targetStockQuantity, 'f', 2));
}

void obmakc::on_pushButton_11_clicked()
{
    QString fromStockCode = ui->comboBox->currentText();
    QString toStockCode = ui->comboBox_2->currentText();
    qDebug() << "From Stock Code:" << fromStockCode;
    qDebug() << "To Stock Code:" << toStockCode;

    fetchStockPrices(fromStockCode, toStockCode);
}
