#include "dragmetal.h"
#include "qevent.h"
#include "ui_dragmetal.h"
#include <mainwindow.h>
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
#include <obmakc.h>
#include <sosakc.h>

DragMetal::DragMetal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DragMetal)
{
    ui->setupUi(this);
    populateMetalComboBox();
    fromCurrency = ui->comboBox->currentText();
    toCurrency = ui->comboBox_2->currentData().toString();
}

DragMetal::~DragMetal()
{
    delete ui;
}

void DragMetal::on_pushButton_4_clicked()
{
    close();
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
}


void DragMetal::populateCurrencyComboBox()
{
    // Assuming you have a list of currency names and codes
    QStringList currencyNames = { "RUB", "EUR", "USD" };

    // Clear the combo box
    ui->comboBox->clear();

    // Populate the combo box with currency names
    ui->comboBox->addItems(currencyNames);
}

void DragMetal::populateMetalComboBox()
{
    // Assuming you have a list of metal names and codes
    QStringList metalNames = { "Gold", "Silver", "Platinum", "Palladium" };
    QStringList metalCodes = { "1", "2", "3", "4" };

    // Clear the combo box
    ui->comboBox_2->clear();

    // Populate the combo box with metal names
    for (int i = 0; i < metalNames.size(); ++i) {
        QString metalName = metalNames.at(i);
        QString metalCode = metalCodes.at(i);
        ui->comboBox_2->addItem(metalName, metalCode); // Set the metal code as the data for each item
    }
}

void DragMetal::on_comboBox_currentIndexChanged(int /*text*/)
{
    fromCurrency = ui->comboBox->currentText();

    qDebug() << "Selected From Currency:" << fromCurrency;

}

void DragMetal::on_comboBox_2_currentIndexChanged(int /*index*/)
{
    // Update the toCurrency value when the metal selection changes
    toCurrency = ui->comboBox_2->currentData().toString();
}

double getCurrencyMultiplier(const QString& currencyCode)
{
    QNetworkAccessManager manager;
    QUrl url("https://www.cbr-xml-daily.ru/daily_json.js"); // URL API Центробанка

    QNetworkRequest request(url);

    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    DragMetal::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        // Обработка ошибки запроса
        qDebug() << "Error: " << reply->errorString();
        return 1.0; // Возвращаем 1.0 в случае ошибки
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);

    if (jsonDocument.isNull()) {
        // Обработка ошибки парсинга JSON
        qDebug() << "Error: Failed to parse JSON response";
        return 1.0; // Возвращаем 1.0 в случае ошибки
    }

    QJsonObject jsonObject = jsonDocument.object();
    QJsonObject valuteObject = jsonObject["Valute"].toObject();

    double currencyMultiplier = 1.0;

    if (valuteObject.contains(currencyCode)) {
        QJsonObject currencyObject = valuteObject[currencyCode].toObject();
        currencyMultiplier = currencyObject["Value"].toDouble();
    }
    else {
        qDebug() << "Invalid currency code:" << currencyCode;
    }

    return currencyMultiplier;
}

double DragMetal::convertCurrency(const QString& fromCurrency, const QString& toCurrency)
{
    bool isMetal = false;

    // Проверка, является ли toCurrency кодом металла
    if (toCurrency.toInt(&isMetal) && isMetal) {
        QNetworkAccessManager manager;
        QUrl url("https://www.cbr.ru/scripts/xml_metall.asp?date_req1=27/05/2023&date_req2=27/05/2023"); // URL API для получения курса металлов
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
        QString responseDataString = QString::fromUtf8(responseData);

        // Обработка XML-ответа
        QDomDocument doc;
        if (!doc.setContent(responseDataString)) {
            // Обработка ошибки парсинга XML
            qDebug() << "Error: Failed to parse XML response" << responseData;
            return 0.0;
        }

        QDomElement root = doc.documentElement();
        QDomNodeList metalNodes = root.elementsByTagName("Record");

        double metalPrice = 0.0;

        // Поиск цены металла по его коду
        for (int i = 0; i < metalNodes.size(); ++i) {
            QDomElement metalElement = metalNodes.at(i).toElement();
            QString metalCode = metalElement.attribute("Code");

            if (metalCode == toCurrency) {
                QDomElement buyElement = metalElement.firstChildElement("Buy");
                QString metalPriceString = buyElement.text().replace(",", ".");
                metalPrice = metalPriceString.toDouble();
                break;
            }
        }

        if (metalPrice == 0.0) {
            qDebug() << "Invalid metal code:" << toCurrency;
            return 0.0;
        }

        double inputValue = ui->textEdit->toPlainText().toDouble();// Получение значения из первого текстового поля
        qDebug() << inputValue;
        qDebug() << fromCurrency;
        if (fromCurrency == "RUB"){
            double convertedValue = inputValue / metalPrice;
            return convertedValue;
        }
        if (fromCurrency == "EUR"){
            double eurMultiplier = ::getCurrencyMultiplier("EUR");
            double convertedValue = (inputValue * eurMultiplier)/ metalPrice;
            return convertedValue;
        }
        if (fromCurrency == "USD"){
            double usdMultiplier = ::getCurrencyMultiplier("USD");
            double convertedValue = (inputValue * usdMultiplier) / metalPrice;
            return convertedValue;
        }
    } else {

    }
}

void DragMetal::on_pushButton_11_clicked()
{
    double convertedValue = convertCurrency(fromCurrency, toCurrency);
    QString convertedValueString = QString::number(convertedValue, 'f', 2);
    convertedValueString.append("г"); // Добавление символа "г" к строке
    ui->textEdit_2->setText(convertedValueString);

    // Check if the metal code matches the selected value in comboBox_2
    QString selectedMetalCode = ui->comboBox_2->currentData().toString(); // Assuming you set the metal code as the data for each combo box item
    QString selectedMetalName = ui->comboBox_2->currentText();

    qDebug() << "Selected Metal Code:" << selectedMetalCode;
    qDebug() << "To Currency:" << toCurrency;

    if (QString::compare(selectedMetalCode, toCurrency, Qt::CaseInsensitive) == 0) {
        qDebug() << "Metal code matches the selected value in comboBox_2.";
    } else {
        qDebug() << "Metal code does not match the selected value in comboBox_2.";
        qDebug() << "Selected Metal Name:" << selectedMetalName;
    }
}

void DragMetal::on_pushButton_9_clicked()
{
    close();
    obmakc dialog;
    dialog.setModal(true);
    dialog.exec();
}


void DragMetal::on_pushButton_10_clicked()
{
    close();
    sosakc dialog;
    dialog.setModal(true);
    dialog.exec();
}

