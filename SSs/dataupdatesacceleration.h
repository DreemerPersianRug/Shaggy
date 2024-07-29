#ifndef DATAUPDATESACCELERATION_H
#define DATAUPDATESACCELERATION_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QLabel>

class DataUpdatesAcceleration: public QObject {
    Q_OBJECT
public:
    DataUpdatesAcceleration(QVector<QLabel*>& labels);
    void update(const QVector<QString>& values, bool use_blank, bool use_g, bool use_ms2);

private:
    QVector<QLabel*>& labels__;
};

#endif // DATAUPDATESACCELERATION_H
