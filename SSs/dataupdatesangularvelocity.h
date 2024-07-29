#ifndef DATAUPDATESANGULARVELOCITY_H
#define DATAUPDATESANGULARVELOCITY_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QLabel>

class DataUpdatesAngularVelocity:public QObject {
    Q_OBJECT
public:
    DataUpdatesAngularVelocity(QVector<QLabel*>& labels);
    void update(const QVector<QString>& values, bool use_blank, bool use_rad, bool use_deg);

private:
    QVector<QLabel*>& labels__;
};

#endif // DATAUPDATESANGULARVELOCITY_H
