#ifndef TRANSFORMVALUES_H
#define TRANSFORMVALUES_H

#include <QVector>
#include <QString>

class TransformValues
{
public:
    TransformValues();

    static QVector<float> convertStringArrayToFloatArray(const QVector<QString>& values);
    static QVector<QString> convertFloatArrayToStringArray(const QVector<float>& values);

    static void roundVector(QVector<float>& vec, int n);

    static void Ms2ToG(QVector<float>& vec, bool marker);
    static void GToMS2(QVector<float>& vec, bool marker);
    static void NTToT(QVector<float>& vec, bool marker);
    static void TToNT(QVector<float>& vec, bool marker);
    static void DegToRad(QVector<float>& vec, bool marker);
    static void RadToDeg(QVector<float>& vec, bool marker);
    static float norm(QVector<float>& vec);
};

#endif // TRANSFORMVALUES_H
