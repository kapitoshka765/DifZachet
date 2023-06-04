#include "sosakc.h"
#include "ui_sosakc.h"
#include <mainwindow.h>
#include <obmakc.h>
#include <dragmetal.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QTextEdit>
#include <QDomDocument>
#include <QWebEngineView>

sosakc::sosakc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sosakc)
{
    ui->setupUi(this);
    QString fromStockCode = ui->comboBox->currentText();
    ui->textEdit->setText(QString::number(fetchStockPrice(fromStockCode), 'f', 2).append("$"));
}

sosakc::~sosakc()
{
    delete ui;
}

void sosakc::on_pushButton_4_clicked()
{
    close();
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
}

void sosakc::on_pushButton_7_clicked()
{
    close();
    DragMetal dialog;
    dialog.setModal(true);
    dialog.exec();
}


void sosakc::on_pushButton_9_clicked()
{
    close();
    obmakc dialog;
    dialog.setModal(true);
    dialog.exec();
}


void sosakc::on_comboBox_currentIndexChanged(int /*index*/)
{
    QString fromStockCode = ui->comboBox->currentText();
    ui->textEdit->setText(QString::number(fetchStockPrice(fromStockCode), 'f', 2).append("$"));
}

double sosakc::fetchStockPrice(const QString& fromStockCode)
{
    QNetworkAccessManager manager;
    QUrl url(QString("https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=%1&apikey=9EAQNXHZ4IJ46YGI").arg(fromStockCode));

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

void sosakc::on_pushButton_11_clicked()
{
    QWebEngineView* webView = new QWebEngineView(this);
    QString stockCode = ui->comboBox->currentText();
    webView->load(QUrl("https://finance.yahoo.com/chart/" + stockCode));
Ё
    QMainWindow* mainWindow = new QMainWindow(this);
    mainWindow->setCentralWidget(webView);
    mainWindow->resize(900, 900); // Установка размеров окна
    mainWindow->setWindowTitle("LSD ARM");
    mainWindow->show();
}

