#include "mahonyfilter.h"
#include "quaternion.h"
#include <cmath>

MahonyFilter::MahonyFilter(float sampleFrequency, float kp, float ki)
    : sample_freq(sampleFrequency), kp(kp), ki(ki), integral_error__{0.0f, 0.0f, 0.0f}, quaternion__(1.0f, 0.0f, 0.0f, 0.0f) {}

void MahonyFilter::update(const QVector<float>& gyro, const QVector<float>& accel, const QVector<float>& mag)
{
    float q1 = quaternion__.w, q2 = quaternion__.x, q3 = quaternion__.y, q4 = quaternion__.z;
    float gx = gyro[0], gy = gyro[1], gz = gyro[2];
    float ax = accel[0], ay = accel[1], az = accel[2];
    float mx = mag[0], my = mag[1], mz = mag[2];

    // Normalize accelerometer and magnetometer data
    float norm_accel = std::sqrt(ax * ax + ay * ay + az * az);
    ax /= norm_accel; ay /= norm_accel; az /= norm_accel;

    float norm_mag = std::sqrt(mx * mx + my * my + mz * mz);
    mx /= norm_mag; my /= norm_mag; mz /= norm_mag;

    // Reference direction of Earth's magnetic field
    float hx = 2.0f * (mx * (0.5f - q3 * q3 - q4 * q4) + my * (q2 * q3 - q1 * q4) + mz * (q2 * q4 + q1 * q3));
    float hy = 2.0f * (mx * (q2 * q3 + q1 * q4) + my * (0.5f - q2 * q2 - q4 * q4) + mz * (q3 * q4 - q1 * q2));
    float bx = std::sqrt((hx * hx) + (hy * hy));
    float bz = 2.0f * (mx * (q2 * q4 - q1 * q3) + my * (q3 * q4 + q1 * q2) + mz * (0.5f - q2 * q2 - q3 * q3));

    // Estimated direction of gravity and magnetic field
    float vx = 2.0f * (q2 * q4 - q1 * q3);
    float vy = 2.0f * (q1 * q2 + q3 * q4);
    float vz = q1 * q1 - q2 * q2 - q3 * q3 + q4 * q4;

    float wx = 2.0f * bx * (0.5f - q3 * q3 - q4 * q4) + 2.0f * bz * (q2 * q4 - q1 * q3);
    float wy = 2.0f * bx * (q2 * q3 - q1 * q4) + 2.0f * bz * (q1 * q2 + q3 * q4);
    float wz = 2.0f * bx * (q1 * q3 + q2 * q4) + 2.0f * bz * (0.5f - q2 * q2 - q3 * q3);

    // Error is sum of cross product between estimated direction and measured direction of fields
    float ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    float ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    float ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

    // Compute and apply integral feedback if enabled
    if (ki > 0.0f) {
        integral_error__[0] += ex;
        integral_error__[1] += ey;
        integral_error__[2] += ez;
        gx += ki * integral_error__[0];
        gy += ki * integral_error__[1];
        gz += ki * integral_error__[2];
    }

    // Apply proportional feedback
    gx += kp * ex;
    gy += kp * ey;
    gz += kp * ez;

    // Integrate rate of change of quaternion
    gx *= (0.5f * (1.0f / sample_freq));
    gy *= (0.5f * (1.0f / sample_freq));
    gz *= (0.5f * (1.0f / sample_freq));
    float qa = q1, qb = q2, qc = q3;
    q1 += (-qb * gx - qc * gy - q4 * gz);
    q2 += (qa * gx + qc * gz - q4 * gy);
    q3 += (qa * gy - qb * gz + q4 * gx);
    q4 += (qa * gz + qb * gy - qc * gx);

    // Normalize quaternion
    quaternion__ = Quaternion(q1, q2, q3, q4).normalize();
}

void MahonyFilter::updateIMU(const QVector<float>& gyro, const QVector<float>& accel)
{
    float q1 = quaternion__.w, q2 = quaternion__.x, q3 = quaternion__.y, q4 = quaternion__.z;
    float gx = gyro[0], gy = gyro[1], gz = gyro[2];
    float ax = accel[0], ay = accel[1], az = accel[2];

    // Normalize accelerometer data
    float normAccel = std::sqrt(ax * ax + ay * ay + az * az);
    ax /= normAccel; ay /= normAccel; az /= normAccel;

    // Estimated direction of gravity
    float vx = 2.0f * (q2 * q4 - q1 * q3);
    float vy = 2.0f * (q1 * q2 + q3 * q4);
    float vz = q1 * q1 - q2 * q2 - q3 * q3 + q4 * q4;

    // Error is sum of cross product between estimated direction and measured direction of gravity
    float ex = (ay * vz - az * vy);
    float ey = (az * vx - ax * vz);
    float ez = (ax * vy - ay * vx);

    // Compute and apply integral feedback if enabled
    if (ki > 0.0f) {
        integral_error__[0] += ex;
        integral_error__[1] += ey;
        integral_error__[2] += ez;
        gx += ki * integral_error__[0];
        gy += ki * integral_error__[1];
        gz += ki * integral_error__[2];
    }

    // Apply proportional feedback
    gx += kp * ex;
    gy += kp* ey;
    gz += kp * ez;

    // Integrate rate of change of quaternion
    gx *= (0.5f * (1.0f / sample_freq));
    gy *= (0.5f * (1.0f / sample_freq));
    gz *= (0.5f * (1.0f / sample_freq));
    float qa = q1, qb = q2, qc = q3;
    q1 += (-qb * gx - qc * gy - q4 * gz);
    q2 += (qa * gx + qc * gz - q4 * gy);
    q3 += (qa * gy - qb * gz + q4 * gx);
    q4 += (qa * gz + qb * gy - qc * gx);

    quaternion__ = Quaternion(q1, q2, q3, q4).normalize();
}

Quaternion MahonyFilter::getQuaternion() const {
    return quaternion__;
}

void MahonyFilter::setParam(float freq, float p, float i)
{
    sample_freq = freq;
    kp = p;
    ki = i;
}
