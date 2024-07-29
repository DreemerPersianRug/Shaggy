#ifndef EULERANGLES_H
#define EULERANGLES_H

#include <iostream>

class Quaternion;

class EulerAngles {
public:
    double roll, pitch, yaw;

    EulerAngles();
    EulerAngles(double roll, double pitch, double yaw);
    EulerAngles(const EulerAngles& e);
    EulerAngles& operator=(const EulerAngles& e);

    EulerAngles operator+(const EulerAngles& e) const;
    EulerAngles operator-(const EulerAngles& e) const;
    EulerAngles operator*(double scalar) const;
    EulerAngles operator/(double scalar) const;

    Quaternion toQuaternion() const;

    friend std::ostream& operator<<(std::ostream& os, const EulerAngles& e);
};

#endif // EULERANGLES_H
