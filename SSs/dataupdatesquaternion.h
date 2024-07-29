#ifndef DATAUPDATESQUATERNION_H
#define DATAUPDATESQUATERNION_H

#include <QVector>
#include <QString>
#include <QLabel>

class DataUpdatesQuaternion
{
public:
    DataUpdatesQuaternion(QVector<QLabel*>& labels);
    void update(const QVector<QString>& values);

private:
    QVector<QLabel*>& labels__;
};

#endif // DATAUPDATESQUATERNION_H
