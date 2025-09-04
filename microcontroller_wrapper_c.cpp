#include "microcontroller.h"
#include <array>
#include <iostream>

static Microcontroller* g_microcontroller = nullptr;

extern "C" {

// Initialize the microcontroller instance
void Microcontroller_Init() {
    if (!g_microcontroller) {
        g_microcontroller = new Microcontroller();
    }
}

// Sensor data struct matching cFS telemetry payload
typedef struct {
    float gyro[3];
    float magnetometer[3];
    float sun_angle;
    unsigned char valid;
} SensorData_t;

// Actuator command struct for output
typedef struct {
    float wheel_torques[3];
    float magnetorquer[3];
    unsigned int timestamp;
} ActuatorCommands_t;

// Process incoming sensor data (called from cFS app)
void Microcontroller_ProcessSensor(const SensorData_t* sensor) {
    if (!g_microcontroller || !sensor) return;

    // Convert to Microcontroller::SensorData safely
    Microcontroller::SensorData sensorData;
    for (size_t i = 0; i < 3; ++i) {
        sensorData.gyro[i] = sensor->gyro[i];
        sensorData.magnetometer[i] = sensor->magnetometer[i];
    }
    sensorData.sun_angle = sensor->sun_angle;
    sensorData.valid = sensor->valid != 0;

    g_microcontroller->processSensorData(sensorData);
}

// Get actuator commands from controller
void Microcontroller_GetActuatorCommands(ActuatorCommands_t* commands) {
    if (!g_microcontroller || !commands) return;

    Microcontroller::ActuatorCommands actuators;
    g_microcontroller->getActuatorCommands(actuators);

    for (size_t i = 0; i < 3; ++i) {
        commands->wheel_torques[i] = actuators.wheel_torques[i];
        commands->magnetorquer[i] = actuators.magnetorquer[i];
    }
    commands->timestamp = actuators.timestamp;
}

// Cleanup microcontroller instance
void Microcontroller_Cleanup() {
    delete g_microcontroller;
    g_microcontroller = nullptr;
}

}
