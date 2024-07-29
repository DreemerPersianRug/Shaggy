#include "labelupdate.h"

#include <QLabel>
#include <QVector>
#include <QLabel>

LabelUpdate::LabelUpdate(QVector<QLabel*>& labels): labels__(labels)
{
    for (int i = 0; i < labels.size(); ++i) {
        labels[i]->setText("*");
    }
}

void LabelUpdate::update(const QVector<QString> &values)
{
    int size = values.size();

    if (size != labels__.size()) {
        throw std::invalid_argument("Vectors must be of the same size");
    }

    for (int i = 0; i < size; ++i) {
        labels__[i]->setText(values[i]);
    }
}
