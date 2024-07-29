#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>
#include <Eigen/Dense>

class EulerAngles;

class Quaternion {
public:
    double w, x, y, z;

    Quaternion();
    Quaternion(double w, double x, double y, double z);
    Quaternion(const Quaternion& q);

    Quaternion& operator=(const Quaternion& q);
    Quaternion operator+(const Quaternion& q) const;
    Quaternion operator-(const Quaternion& q) const;
    Quaternion operator*(const Quaternion& q) const;
    Quaternion operator*(double scalar) const;
    Quaternion operator/(double scalar) const;

    double norm() const;

    Quaternion conjugate() const;

    Quaternion inverse() const;

    Quaternion normalize() const;

    EulerAngles toEulerAngles() const;

    Eigen::Matrix3f toRotationMatrix() const;

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
};

#endif // QUATERNION_H
