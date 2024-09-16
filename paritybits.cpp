#include "paritybits.h"
#include "ui_paritybits.h"

#include <QMessageBox>
#include <QRegularExpression>

ParityBits::ParityBits(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::parityBits)
{
    ui->setupUi(this);

}

ParityBits::~ParityBits()
{
    delete ui;
}

void ParityBits::on_checkButton_clicked()
{
    QString str = ui->lineEditBinary->text();
    QMessageBox msgBox;

    if (str.isEmpty()) {
        msgBox.setText("Вы не ввели бинарное сообщение!");
        msgBox.exec();
        return;
    }

    if (ui->parityRadBtn->isChecked()) {
        ui->resOfCalcParityLabel->setText(calculateParityBits(str));

        if (checkDataParity(str, ui->resOfCalcParityLabel->text()))
            msgBox.setText("Ошибок нет");
        else
            msgBox.setText("Ошибка обнаружена!");

         msgBox.exec();
    }

    if (ui->VerHorParityRadBtn->isChecked()) {
        if (str.length() % 64 != 0) {
            msgBox.setText("Введите сообщение длина которого кратна 64!");
            msgBox.exec();
            return;
        }

        QVector<QString> msg = splitBinaryString(str);
        ui->resOfCalcParityLabel->setText(verHorCalculateParity(msg));

        if (checkDataVerHorParity(msg, calculateHorizontalParity(msg), calculateVerticalParity(msg, calculateHorizontalParity(msg))))
            msgBox.setText("Ошибок нет");
        else
            msgBox.setText("Ошибка обнаружена!");

        msgBox.exec();
    }

    if (!ui->parityRadBtn->isChecked() && !ui->VerHorParityRadBtn->isChecked()) {
        msgBox.setText("Выберете метод!");
        msgBox.exec();
    }
}

QString ParityBits::calculateParityBits(const QString &message)
{
    QString parityBits;

    for (unsigned int i = 0; i < message.length(); i += 8) {
        int count = 0;
        // Обрабатываем блок длиной 8 бит
        for (unsigned int j = i; j < i + 8 && j < message.length(); ++j) {
            if (message[j] == '1') {
                count++;
            }
        }
        // Добавляем бит контроля
        parityBits += (count % 2 == 0) ? '0' : '1';
    }
    return parityBits;
}

QString ParityBits::verHorCalculateParity(const QVector<QString> &message)
{
    QString horizontalParity = calculateHorizontalParity(message);

    // Вычисляем вертикальный паритет
    QString verticalParity = calculateVerticalParity(message, horizontalParity);

    return verticalParity;
}

QString ParityBits::calculateHorizontalParity(const QVector<QString> &data)
{
    QString parityBits;
    for (const auto& row : data) {
        int count = 0;
        for (QChar bit : row) {
            if (bit == '1') count++;
        }
        parityBits += (count % 2 == 0) ? '0' : '1'; // Четный паритет
    }
    return parityBits;
}

QString ParityBits::calculateVerticalParity(const QVector<QString> &data, const QString &horizontalParity)
{
    QString verticalParity;
    int cols = data[0].size();

    for (int j = 0; j < cols; ++j) {
        int count = 0;
        for (int i = 0; i < data.size(); ++i) {
            if (data[i][j] == '1') count++;
        }
        // Учитываем горизонтальный паритет
        //if (horizontalParity[j / (cols / data.size())] == '1') count++;
        verticalParity += (count % 2 == 0) ? '0' : '1'; // Четный паритет
    }
    return verticalParity;
}

QVector<QString> ParityBits::splitBinaryString(const QString &binaryString)
{
    QVector<QString> result;

    // Убедимся, что строка состоит только из '0' и '1'
    if (!binaryString.contains(QRegularExpression("[^01]"))) {
        int length = binaryString.length();

        // Разделяем строку на блоки по 16 бит
        for (int i = 0; i < length; i += 16) {
            QString byte = binaryString.mid(i, 16);
            result.append(byte);
        }
    } else {
        qDebug() << "Ошибка: строка содержит недопустимые символы.";
    }

    return result;
}

bool ParityBits::checkDataVerHorParity(const QVector<QString> &data, const QString &horizontalParity, const QString &verticalParity)
{
    for (unsigned int i = 0; i < data.size(); ++i) {
        int count = 0;
        for (auto bit : data[i]) {
            if (bit == '1') count++;
        }
        if ((count % 2) != (horizontalParity[i].digitValue() - 0)) {
            return false; // Ошибка в строке
        }
    }

    // Проверка вертикального паритета
    int cols = data[0].size();
    for (int j = 0; j < cols; ++j) {
        int count = 0;
        for (unsigned int i = 0; i < data.size(); ++i) {
            if (data[i][j] == '1') count++;
        }
        if ((count % 2) != (verticalParity[j].digitValue() - 0)) {
            return false; // Ошибка в столбце
        }
    }

    return true; // Ошибок не найдено
}

bool ParityBits::checkDataParity(const QString &receivedMessage, const QString &receivedParity)
{
    QString calculatedParity = calculateParityBits(receivedMessage);
    return calculatedParity == receivedParity; // true, если ошибок нет
}
