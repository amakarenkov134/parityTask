#ifndef PARITYBITS_H
#define PARITYBITS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class parityBits;
}
QT_END_NAMESPACE

class ParityBits : public QWidget
{
    Q_OBJECT

public:
    ParityBits(QWidget *parent = nullptr);
    ~ParityBits();

signals:

private slots:
    void on_checkButton_clicked();

private:
    Ui::parityBits *ui;

    QString calculateParityBits(const QString &message);

    QString verHorCalculateParity(const QVector<QString> &message);

    QString calculateHorizontalParity(const QVector<QString> &data);
    QString calculateVerticalParity(const QVector<QString> &data);

    QVector<QString> splitBinaryString(const QString& binaryString);

    bool checkDataVerHorParity(const QVector<QString> &data, const QString &horizontalParity, const QString &verticalParity);

    bool checkDataParity(const QString &receivedMessage, const QString &receivedParity);

    void imitateErrors(QVector<QString> &message);

    void imitateErrors(QString &message);
};
#endif // PARITYBITS_H
