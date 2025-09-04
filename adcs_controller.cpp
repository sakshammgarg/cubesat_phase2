#include "adcs_controller.h"
#include <array>
#include <algorithm>
#include <cstring>
#include <memory>

ADCSController::ADCSController(float kp_detumble, float kp_point, float kd_point)
    : kp_detumble_(kp_detumble), kp_point_(kp_point), kd_point_(kd_point),
      max_torque_(0.1f), first_mag_reading_(true) {

    for (auto& pid : pid_controllers_) {
        pid = std::make_unique<PIDController>(kp_point, 0.01f, kd_point);
        pid->setLimits(-max_torque_, max_torque_);
    }

    previous_mag_.fill(0.0f);
}

void ADCSController::computeControl(const float* gyro_rates, const float* magnetometer,
                                    float sun_angle, float* wheel_torques,
                                    float* magnetorquer, int control_mode) {

    std::fill_n(wheel_torques, 3, 0.0f);
    std::fill_n(magnetorquer, 3, 0.0f);

    switch (control_mode) {
        case 0: safeMode(wheel_torques, magnetorquer); break;
        case 1: detumbleControl(gyro_rates, magnetometer, magnetorquer); break;
        case 2: pointingControl(gyro_rates, wheel_torques); break;
        case 3: scienceControl(gyro_rates, wheel_torques); break;
        default: safeMode(wheel_torques, magnetorquer); break;
    }
}

void ADCSController::detumbleControl(const float* gyro_rates, const float* magnetometer,
                                     float* magnetorquer) {

    if (first_mag_reading_) {
        std::memcpy(previous_mag_.data(), magnetometer, 3 * sizeof(float));
        first_mag_reading_ = false;
        return;
    }

    float dt = 0.1f;

    for (size_t i = 0; i < 3; ++i) {
        float b_dot = (magnetometer[i] - previous_mag_[i]) / dt;
        magnetorquer[i] = std::clamp(-kp_detumble_ * b_dot, -1.0f, 1.0f);
        previous_mag_[i] = magnetometer[i];
    }
}

void ADCSController::pointingControl(const float* gyro_rates, float* wheel_torques) {
    float dt = 0.1f;
    for (size_t i = 0; i < 3; ++i) {
        wheel_torques[i] = pid_controllers_[i]->compute(0.0f, gyro_rates[i], dt);
    }
}

void ADCSController::scienceControl(const float* gyro_rates, float* wheel_torques) {
    float dt = 0.1f;
    for (size_t i = 0; i < 3; ++i) {
        float torque = pid_controllers_[i]->compute(0.0f, gyro_rates[i], dt) * 0.5f;
        wheel_torques[i] = std::clamp(torque, -0.05f, 0.05f);
    }
}

void ADCSController::safeMode(float* wheel_torques, float* magnetorquer) {
    std::fill_n(wheel_torques, 3, 0.0f);
    std::fill_n(magnetorquer, 3, 0.0f);
}

void ADCSController::setGains(float kp_detumble, float kp_point, float kd_point) {
    kp_detumble_ = kp_detumble;
    kp_point_ = kp_point;
    kd_point_ = kd_point;

    for (auto& pid : pid_controllers_) {
        pid = std::make_unique<PIDController>(kp_point, 0.01f, kd_point);
        pid->setLimits(-max_torque_, max_torque_);
    }
}

void ADCSController::setMaxTorque(float max_torque) {
    max_torque_ = max_torque;
    for (auto& pid : pid_controllers_) {
        pid->setLimits(-max_torque_, max_torque_);
    }
}
