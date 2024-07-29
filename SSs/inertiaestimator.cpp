#include "inertiaestimator.h"

InertiaEstimator::InertiaEstimator() {
    // Initialize the inertia tensor
    inertiaTensor = Eigen::Matrix3f::Zero();
}

void InertiaEstimator::update(const QVector<float>& gyroData, const QVector<float>& accelData, const Quaternion& quaternion) {
    // Convert data to Eigen vectors
    Eigen::Vector3f omega(gyroData[0], gyroData[1], gyroData[2]);
    Eigen::Vector3f acceleration(accelData[0], accelData[1], accelData[2]);

    // Convert quaternion to rotation matrix
    Eigen::Matrix3f rotationMatrix = quaternion.toRotationMatrix();

    // Estimate the torque
    Eigen::Vector3f torque = estimateTorque(omega, acceleration, rotationMatrix);

    // Update the inertia tensor
    updateInertiaTensor(omega, torque);
}

Eigen::Matrix3f InertiaEstimator::getInertiaTensor() const {
    return inertiaTensor;
}

Eigen::Vector3f InertiaEstimator::estimateTorque(const Eigen::Vector3f& omega, const Eigen::Vector3f& acceleration, const Eigen::Matrix3f& rotationMatrix) {
    // Use Euler's equations to estimate the torque
    Eigen::Vector3f torque = inertiaTensor * omega.cross(inertiaTensor * omega) + rotationMatrix * acceleration;
    return torque;
}

void InertiaEstimator::updateInertiaTensor(const Eigen::Vector3f& omega, const Eigen::Vector3f& torque) {
    // Update the inertia tensor using the least squares method
    Eigen::Matrix3f omegaCross = Eigen::Matrix3f::Zero();
    omegaCross << 0, -omega.z(), omega.y(),
                  omega.z(), 0, -omega.x(),
                  -omega.y(), omega.x(), 0;

    Eigen::Matrix3f A = omegaCross * inertiaTensor - inertiaTensor * omegaCross;
    Eigen::Vector3f b = torque - inertiaTensor * omega.cross(inertiaTensor * omega);

    // Solve the system of linear equations A * deltaI = b
    Eigen::Vector3f deltaI = A.colPivHouseholderQr().solve(b);

    // Update the inertia tensor
    inertiaTensor += Eigen::Map<Eigen::Matrix3f>(deltaI.data());
}
