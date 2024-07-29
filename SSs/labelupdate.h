#ifndef LABELUPDATE_H
#define LABELUPDATE_H

#include <QVector>
#include <QString>
#include <QLabel>

class LabelUpdate
{
public:
    LabelUpdate(QVector<QLabel*>& labels);
    void update(const QVector<QString>& values);

private:
    QVector<QLabel*>& labels__;
};

#endif // LABELUPDATE_H
