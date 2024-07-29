#ifndef INERTIAESTIMATOR_H
#define INERTIAESTIMATOR_H

#include <QVector>
#include <Eigen/Dense>
#include "quaternion.h"

class InertiaEstimator {
public:
    InertiaEstimator();

    void update(const QVector<float>& gyroData, const QVector<float>& accelData, const Quaternion& quaternion);

    Eigen::Matrix3f getInertiaTensor() const;

private:
    Eigen::Matrix3f inertiaTensor;

    Eigen::Vector3f estimateTorque(const Eigen::Vector3f& omega, const Eigen::Vector3f& acceleration, const Eigen::Matrix3f& rotationMatrix);

    void updateInertiaTensor(const Eigen::Vector3f& omega, const Eigen::Vector3f& torque);
};

#endif // INERTIAESTIMATOR_H
