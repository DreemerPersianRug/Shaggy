#ifndef DATAUPDATESMAGNETICFIELD_H
#define DATAUPDATESMAGNETICFIELD_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QLabel>

class DataUpdatesMagneticField:public QObject {
    Q_OBJECT
public:
    DataUpdatesMagneticField(QVector<QLabel*>& labels);
    void update(const QVector<QString>& values, bool use_blank, bool use_t, bool use_nt);

private:
    QVector<QLabel*>& labels__;
};

#endif // DATAUPDATESMAGNETICFIELD_H
