#ifndef DATAUPDATESEULER_H
#define DATAUPDATESEULER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QLabel>

class DataUpdatesEuler:public QObject {
    Q_OBJECT
public:
    DataUpdatesEuler(QVector<QLabel*>& labels);
    void update(const QVector<QString>& values, bool use_blank, bool use_rad, bool use_deg);

private:
    QVector<QLabel*>& labels__;
};

#endif // DATAUPDATESEULER_H
