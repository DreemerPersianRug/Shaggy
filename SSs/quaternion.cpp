#include "quaternion.h"
#include "eulerangles.h"

#include <cmath>
#include <stdexcept>

Quaternion::Quaternion() : w(1.0), x(0.0), y(0.0), z(0.0) {}

Quaternion::Quaternion(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}

Quaternion::Quaternion(const Quaternion& q) : w(q.w), x(q.x), y(q.y), z(q.z) {}

Quaternion& Quaternion::operator=(const Quaternion& q)
{
    if (this != &q) {
        w = q.w;
        x = q.x;
        y = q.y;
        z = q.z;
    }
    return *this;
}

Quaternion Quaternion::operator+(const Quaternion& q) const
{
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

Quaternion Quaternion::operator-(const Quaternion& q) const
{
    return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
    return Quaternion(
        w * q.w - x * q.x - y * q.y - z * q.z,
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w
    );
}

Quaternion Quaternion::operator*(double scalar) const
{
    return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
}

Quaternion Quaternion::operator/(double scalar) const
{
    if (scalar == 0.0) {
        throw std::runtime_error("Division by zero");
    }
    return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
}

double Quaternion::norm() const
{
    return std::sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::conjugate() const
{
    return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::inverse() const
{
    double n = norm();
    if (n == 0.0) {
        throw std::runtime_error("Inverse of zero quaternion");
    }
    return conjugate() / (n * n);
}

Quaternion Quaternion::normalize() const
{
    double n = norm();
    if (n == 0.0) {
        throw std::runtime_error("Inverse of zero quaternion");
    }
    return Quaternion(w, x, y, z) / n;
}

EulerAngles Quaternion::toEulerAngles() const
{
    double roll, pitch, yaw;

    double sinr_cosp = 2 * (w * x + y * z);
    double cosr_cosp = 1 - 2 * (x * x + y * y);
    roll = std::atan2(sinr_cosp, cosr_cosp);

    double sinp = 2 * (w * y - z * x);
    if (std::abs(sinp) >= 1) {
        pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    } else {
        pitch = std::asin(sinp);
    }

    double siny_cosp = 2 * (w * z + x * y);
    double cosy_cosp = 1 - 2 * (y * y + z * z);
    yaw = std::atan2(siny_cosp, cosy_cosp);

    return EulerAngles(roll, pitch, yaw);
}

Eigen::Matrix3f Quaternion::toRotationMatrix() const
{
    Eigen::Matrix3f rotationMatrix;
    rotationMatrix <<
        1 - 2*y*y - 2*z*z, 2*x*y - 2*w*z, 2*x*z + 2*w*y,
        2*x*y + 2*w*z, 1 - 2*x*x - 2*z*z, 2*y*z - 2*w*x,
        2*x*z - 2*w*y, 2*y*z + 2*w*x, 1 - 2*x*x - 2*y*y;
    return rotationMatrix;
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
    os << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
    return os;
}
