#include "dataupdatesmagneticfield.h"
#include "transformvalues.h"
#include <QLabel>
#include <QString>
#include <QVector>
#include <cmath>

#include <iostream>

DataUpdatesMagneticField::DataUpdatesMagneticField(QVector<QLabel*>& labels)
    : labels__(labels)
{
    for (int i = 0; i < labels.size(); ++i) {
        labels[i]->setText("*");
    }
}

void DataUpdatesMagneticField::update(const QVector<QString>& values, bool use_blank, bool use_t, bool use_nt)
{
    int size = values.size();

    if (size != labels__.size()) {
        throw std::invalid_argument("Vectors must be of the same size");
    }

    QVector<float> raws = TransformValues::convertStringArrayToFloatArray(values);

    if (use_blank) {
        for (int i = 0; i < size; ++i) {
            labels__[i]->setText(values[i]);
        }
    }
    else {
        if (use_t) {
            for (int i = 0; i < size; ++i) {
                raws[i] *= 1e-9;
            }

            QVector<QString> rawsStr = TransformValues::convertFloatArrayToStringArray(raws);

            for (int i = 0; i < size; ++i) {
                labels__[i]->setText(rawsStr[i]);
            }
        }
        else if (use_nt) {
            for (int i = 0; i < size; ++i) {
                raws[i] *= 1e9;
            }
            QVector<QString> rawsStr = TransformValues::convertFloatArrayToStringArray(raws);

            for (int i = 0; i < size; ++i) {
                labels__[i]->setText(rawsStr[i]);
            }
        }
    }
}
