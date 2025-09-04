# ADCS OBC Challenge Documentation

## 1. Bug Fixing
- Corrected the following source files:
  - `adcs_controller.cpp`
  - `microcontroller.cpp`
  - `microcontroller_wrapper_c.cpp`
  - `pid_controller.cpp`
  - `adcs_test_harness.cpp`
- Ensured the code compiles with **C++17**.
- Fixed variable, scope, and type issues.
- Added controlled fault injection for demonstration purposes.

## 2. Integration
- Integrated the corrected code into the **cFS** applications.
- Linked telemetry output to `telemetry_bridge.py`.
- Verified real-time telemetry output and system health monitoring.

## 3. Fault Injection
- Controlled faults are injected in `adcs_test_harness.cpp`:
  - Example: Gyro sensor spike at cycle 10.
- Observed **Safe Mode** activation in telemetry output when faults exceed thresholds.
- Verified correct reaction of ADCS to fault conditions.

## 4. Run Instructions
1. Navigate to the ADCS working directory:
   cd fsw

2. Build the ADCS test harness:
   g++ -std=c++17 -I./inc -I./src \
    src/adcs_controller.cpp \
    src/microcontroller.cpp \
    src/microcontroller_wrapper_c.cpp \
    src/pid_controller.cpp \
    adcs_test_harness.cpp \
    -o adcs_test_harness

3. Run with telemetry bridge:
   ./adcs_test_harness | python3 /home/osk/telemetry_bridge.py

•	Normal operation: Status: NORMAL
•	Fault injection: Status changes to FAULT when thresholds are breached.

## 5. Notes
- C++17 is required for std::clamp and std::unique_ptr.
- Telemetry bridge script is provided at /home/osk/telemetry_bridge.py.
- Fault injection logic is implemented in adcs_test_harness.cpp for demonstration.
- The system has been tested for integration with cFS and verified to run without errors.

## 6. References
- Core Flight System (cFS): https://github.com/nasa/cFS
- CubeSat ADCS Simulator: 42 Simulator and telemetry bridge script


This covers:

- **Bug fixes done**  
- **Integration steps**  
- **Fault injection description**  
- **Run instructions**  

You just need to create a file in your repo and paste this content:

```bash
cd /path/to/your/repo
touch DOC.md
vim DOC.md
# paste the content above
   
