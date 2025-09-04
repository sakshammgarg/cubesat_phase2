# CubeSat ADCS OBC Challenge

## Overview
This repository contains the corrected ADCS software integrated with cFS and linked to a telemetry bridge.

## Structure
- `src/` : Source files for ADCS and microcontroller.
- `inc/` : Header files.
- `adcs_test_harness.cpp` : Test harness for simulation and fault injection.
- `DOC.md` : Documentation of approach, integration, and testing.
- `telemetry_bridge.py` : Python script for handling telemetry output.

## Build & Run
```bash
cd fsw
g++ -std=c++17 -I./inc -I./src \
    src/adcs_controller.cpp \
    src/microcontroller.cpp \
    src/microcontroller_wrapper_c.cpp \
    src/pid_controller.cpp \
    adcs_test_harness.cpp \
    -o adcs_test_harness

./adcs_test_harness | python3 /home/osk/telemetry_bridge.py
•	Controlled faults are injected at cycle 10 for demonstration.

---

### **2. Docker Image**

**Steps to create a Docker image:**

1. Create a `Dockerfile` in your repo root:

```dockerfile
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Copy repository files
COPY . /workspace
WORKDIR /workspace/fsw

# Build ADCS test harness
RUN g++ -std=c++17 -I./inc -I./src \
    src/adcs_controller.cpp \
    src/microcontroller.cpp \
    src/microcontroller_wrapper_c.cpp \
    src/pid_controller.cpp \
    adcs_test_harness.cpp \
    -o adcs_test_harness

CMD ["bash"]

2.	Build Docker image:
docker build -t adcs_cfs_sim:latest .

3.	Run container:
docker run -it adcs_cfs_sim:latest
cd fsw
./adcs_test_harness | python3 /workspace/telemetry_bridge.py
