#include "dataupdatesacceleration.h"
#include "transformvalues.h"
#include <QLabel>
#include <QString>
#include <QVector>


DataUpdatesAcceleration::DataUpdatesAcceleration(QVector<QLabel*>& labels)
    : labels__(labels)
{
    for (int i = 0; i < labels.size(); ++i) {
        labels[i]->setText("*");
    }
}

void DataUpdatesAcceleration::update(const QVector<QString>& values, bool use_blank, bool use_g, bool use_ms2)
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
        if (use_g) {
            for (int i = 0; i < size; ++i) {
                raws[i] *= 9.81;
            }

            QVector<QString> rawsStr = TransformValues::convertFloatArrayToStringArray(raws);

            for (int i = 0; i < size; ++i) {
                labels__[i]->setText(rawsStr[i]);
            }
        }
        else {
            if (use_ms2) {
                for (int i = 0; i < size; ++i) {
                    raws[i] /= 9.81;
                }
                QVector<QString> rawsStr = TransformValues::convertFloatArrayToStringArray(raws);

                for (int i = 0; i < size; ++i) {
                    labels__[i]->setText(rawsStr[i]);
                }
            }
        }
    }
}


