#include "microcontroller.h"
#include "adcs_controller.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <memory>
#include <array>

Microcontroller::Microcontroller()
    : control_mode_(0), fault_flags_(0), control_cycles_(0), uptime_seconds_(0),
      fault_threshold_(0.5f), max_torque_(0.1f), fault_count_(0), consecutive_faults_(0),
      gyro_rates_{}, magnetometer_{}, wheel_torques_{}, magnetorquer_{}, sun_angle_(0.0f) {

    adcs_controller_ = std::make_unique<ADCSController>(0.1f, 0.05f, 0.01f);

    std::cout << "Microcontroller initialized - Safe Mode" << std::endl;
}

Microcontroller::~Microcontroller() = default; // unique_ptr handles deletion

void Microcontroller::processSensorData(const SensorData& sensor_data) {
    if (!sensor_data.valid) {
        consecutive_faults_++;
        if (consecutive_faults_ > 5) {
            control_mode_ = 0; // Safe mode
        }
        return;
    }

    for (size_t i = 0; i < 3; ++i) {
        gyro_rates_[i] = sensor_data.gyro[i];
        magnetometer_[i] = sensor_data.magnetometer[i];
    }
    sun_angle_ = sensor_data.sun_angle;

    consecutive_faults_ = 0;

    performFaultDetection();

    // Compute control using ADCS
    adcs_controller_->computeControl(gyro_rates_.data(), magnetometer_.data(), sun_angle_,
                                     wheel_torques_.data(), magnetorquer_.data(), control_mode_);

    // Clamp outputs
    for (size_t i = 0; i < 3; ++i) {
        wheel_torques_[i] = std::clamp(wheel_torques_[i], -max_torque_, max_torque_);
        magnetorquer_[i] = std::clamp(magnetorquer_[i], -1.0f, 1.0f);
    }

    control_cycles_++;
    updateHealthMonitoring();
}

void Microcontroller::getActuatorCommands(ActuatorCommands& commands) {
    for (size_t i = 0; i < 3; ++i) {
        commands.wheel_torques[i] = wheel_torques_[i];
        commands.magnetorquer[i] = magnetorquer_[i];
    }
    commands.timestamp = control_cycles_;
}

void Microcontroller::setControlMode(int mode) {
    if (mode >= 0 && mode <= 3) {
        control_mode_ = mode;
        std::cout << "Control mode set to: " << mode << std::endl;
    }
}

void Microcontroller::setFaultThreshold(float threshold) {
    if (threshold > 0.0f) {
        fault_threshold_ = threshold;
    }
}

void Microcontroller::performFaultDetection() {
    fault_flags_ = 0;

    for (size_t i = 0; i < 3; ++i) {
        if (std::abs(gyro_rates_[i]) > fault_threshold_) {
            fault_flags_ |= (1 << i);
        }
    }

    bool sensor_fault = false;
    for (size_t i = 0; i < 3; ++i) {
        if (std::isnan(gyro_rates_[i]) || std::isinf(gyro_rates_[i]) ||
            std::isnan(magnetometer_[i]) || std::isinf(magnetometer_[i])) {
            sensor_fault = true;
            break;
        }
    }

    if (sensor_fault) {
        fault_flags_ |= 0x08;
    }

    if (fault_flags_ != 0) {
        fault_count_++;
        if (fault_count_ > 3 && control_mode_ != 0) {
            control_mode_ = 0;
            std::cout << "Multiple faults detected. Entering safe mode." << std::endl;
        }
    } else {
        fault_count_ = 0;
    }
}

void Microcontroller::updateHealthMonitoring() {
    if (control_cycles_ % 10 == 0) {
        uptime_seconds_++;
    }
}
