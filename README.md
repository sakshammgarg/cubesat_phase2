## **CubeSat ADCS OBC Challenge** 🛰️

### **Overview**
This repository contains the software for an Attitude Determination and Control System (**ADCS**) for a CubeSat. The software is integrated with **cFS** (Core Flight System) and includes a telemetry bridge for data visualization. This project focuses on demonstrating robust fault handling and system stability.

***

### **Structure**
* `src/`: Contains the source files for the ADCS system and microcontroller emulation.
* `inc/`: Contains the corresponding header files.
* `adcs_test_harness.cpp`: A test harness for simulating the ADCS behavior and injecting controlled faults.
* `DOC.md`: Detailed documentation of the software's approach, integration, and testing methodology.
* `telemetry_bridge.py`: A Python script to process and handle telemetry output from the simulation.

***

### **Build & Run**
To build and run the simulation, navigate to the `fsw` directory and execute the following commands. The build command compiles the C++ source files into a single executable, `adcs_test_harness`.

```bash
cd fsw
g++ -std=c++17 -I./inc -I./src \
    src/adcs_controller.cpp \
    src/microcontroller.cpp \
    src/microcontroller_wrapper_c.cpp \
    src/pid_controller.cpp \
    adcs_test_harness.cpp \
    -o adcs_test_harness

./adcs_test_harness | python3 ../telemetry_bridge.py
