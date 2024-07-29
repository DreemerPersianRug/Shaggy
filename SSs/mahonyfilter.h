#ifndef MAHONYFILTER_H
#define MAHONYFILTER_H

#include <QVector>
#include "quaternion.h"

class MahonyFilter {
public:
    MahonyFilter(float sampleFrequency, float kp = 1.0f, float ki = 0.0f);

    void update(const QVector<float>& gyro, const QVector<float>& accel, const QVector<float>& mag);
    void updateIMU(const QVector<float>& gyro, const QVector<float>& accel);

    Quaternion getQuaternion() const;
    void setParam(float freq, float p, float i);

    float sample_freq;
    float kp, ki;

private:
    float integral_error__[3];
    Quaternion quaternion__;
};

#endif // MAHONYFILTER_H
