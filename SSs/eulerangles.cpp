#include "eulerangles.h"
#include "quaternion.h"

#include <stdexcept>
#include <cmath>

EulerAngles::EulerAngles() : roll(0.0), pitch(0.0), yaw(0.0) {}

EulerAngles::EulerAngles(double roll, double pitch, double yaw) : roll(roll), pitch(pitch), yaw(yaw) {}

EulerAngles::EulerAngles(const EulerAngles& e) : roll(e.roll), pitch(e.pitch), yaw(e.yaw) {}

EulerAngles& EulerAngles::operator=(const EulerAngles& e)
{
    if (this != &e) {
        roll = e.roll;
        pitch = e.pitch;
        yaw = e.yaw;
    }
    return *this;
}

EulerAngles EulerAngles::operator+(const EulerAngles& e) const
{
    return EulerAngles(roll + e.roll, pitch + e.pitch, yaw + e.yaw);
}

EulerAngles EulerAngles::operator-(const EulerAngles& e) const
{
    return EulerAngles(roll - e.roll, pitch - e.pitch, yaw - e.yaw);
}

EulerAngles EulerAngles::operator*(double scalar) const
{
    return EulerAngles(roll * scalar, pitch * scalar, yaw * scalar);
}

EulerAngles EulerAngles::operator/(double scalar) const
{
    if (scalar == 0.0) {
        throw std::runtime_error("Division by zero");
    }
    return EulerAngles(roll / scalar, pitch / scalar, yaw / scalar);
}

Quaternion EulerAngles::toQuaternion() const
{
    double cr = std::cos(roll * 0.5);
    double sr = std::sin(roll * 0.5);
    double cp = std::cos(pitch * 0.5);
    double sp = std::sin(pitch * 0.5);
    double cy = std::cos(yaw * 0.5);
    double sy = std::sin(yaw * 0.5);

    double w = cr * cp * cy + sr * sp * sy;
    double x = sr * cp * cy - cr * sp * sy;
    double y = cr * sp * cy + sr * cp * sy;
    double z = cr * cp * sy - sr * sp * cy;

    return Quaternion(w, x, y, z);
}

std::ostream& operator<<(std::ostream& os, const EulerAngles& e)
{
    os << "(" << e.roll << ", " << e.pitch << ", " << e.yaw << ")";
    return os;
}
