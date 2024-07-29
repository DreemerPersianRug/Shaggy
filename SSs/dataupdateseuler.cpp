#include "dataupdateseuler.h"
#include "transformvalues.h"
#include <QLabel>
#include <QString>
#include <QVector>
#include <cmath>

DataUpdatesEuler::DataUpdatesEuler(QVector<QLabel*>& labels)
    : labels__(labels)
{
    for (int i = 0; i < labels.size(); ++i) {
        labels[i]->setText("*");
    }
}

void DataUpdatesEuler::update(const QVector<QString>& values, bool use_blank, bool use_rad, bool use_deg)
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
        if (use_rad) {
            for (int i = 0; i < size; ++i) {
                raws[i] = (raws[i] * 180.0f) / M_PI;
            }

            QVector<QString> rawsStr = TransformValues::convertFloatArrayToStringArray(raws);

            for (int i = 0; i < size; ++i) {
                labels__[i]->setText(rawsStr[i]);
            }
        }
        else if (use_deg) {
            for (int i = 0; i < size; ++i) {
                raws[i] = (raws[i] * M_PI) / 180.0f;
            }
            QVector<QString> rawsStr = TransformValues::convertFloatArrayToStringArray(raws);

            for (int i = 0; i < size; ++i) {
                labels__[i]->setText(rawsStr[i]);
            }
        }
    }
}
