#include "transformvalues.h"
#include <QVector>
#include <QString>
#include <QDebug>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <stdexcept>

TransformValues::TransformValues()
{
}

QVector<float> TransformValues::convertStringArrayToFloatArray(const QVector<QString>& values) {
    QVector<float> floatArray;
    floatArray.reserve(values.size());

    std::transform(values.begin(), values.end(), std::back_inserter(floatArray), [](const QString& str) {
        bool ok;
        float value = str.toFloat(&ok);
        if (!ok) {
            qWarning() << "Conversion failed for string:" << str;
        }
        return value;
    });

    return floatArray;
}

QVector<QString> TransformValues::convertFloatArrayToStringArray(const QVector<float>& values) {
    QVector<QString> stringArray;
    stringArray.reserve(values.size());

    std::transform(values.begin(), values.end(), std::back_inserter(stringArray), [](float value) {
        return QString::number(value);
    });

    return stringArray;
}

void TransformValues::roundVector(QVector<float>& vec, int n) {
    float factor = std::pow(10, n);
    for (float& value : vec) {
        value = std::round(value * factor) / factor;
    }
}

void TransformValues::Ms2ToG(QVector<float> &vec, bool marker)
{
    if(marker) {
        for (float& value : vec) {
            value *= 9.81;
        }
    }
}

void TransformValues::GToMS2(QVector<float> &vec, bool marker)
{
    if(marker) {
        for (float& value : vec) {
            value /= 9.81;
        }
    }
}

void TransformValues::NTToT(QVector<float> &vec, bool marker)
{
    if(marker) {
        for (float& value : vec) {
            value *= 1e-9;
        }
    }
}

void TransformValues::TToNT(QVector<float> &vec, bool marker)
{
    if(marker) {
        for (float& value : vec) {
            value *= 1e9;
        }
    }
}

void TransformValues::DegToRad(QVector<float> &vec, bool marker)
{
    if(marker) {
        for (float& value : vec) {
            value = (value * M_PI) / 180.0f;
        }
    }
}

void TransformValues::RadToDeg(QVector<float> &vec, bool marker)
{
    if(marker) {
        for (float& value : vec) {
            value = (value * 180.0f) / M_PI;
        }
    }
}

float TransformValues::norm(QVector<float>& vec) {
    if (vec.isEmpty()) {
        throw std::invalid_argument("Vector is empty");
    }
    float norm = 0;
    for (float value : vec) {
        norm += value * value;
    }
    return float(std::sqrt(norm));
}
