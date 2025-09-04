# Use an Ubuntu base
FROM ubuntu:22.04

# Set non-interactive mode
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && \
    apt-get install -y build-essential cmake g++ python3 python3-pip git && \
    apt-get clean

# Set working directory
WORKDIR /home/osk/cFS

# Copy the entire project into the container
COPY . /home/osk/cFS

# Build the ADCS test harness
WORKDIR /home/osk/cFS/apps/space_adcs/fsw
RUN mkdir -p build && cd build && cmake .. && make

# Compile the test harness separately (optional)
RUN g++ -std=c++17 -I./inc -I./src \
    src/adcs_controller.cpp \
    src/microcontroller.cpp \
    src/microcontroller_wrapper_c.cpp \
    src/pid_controller.cpp \
    ../adcs_test_harness.cpp \
    -o ../adcs_test_harness

# Set the entrypoint to run the test harness with telemetry
ENTRYPOINT ["bash", "-c", "./adcs_test_harness | python3 /home/osk/telemetry_bridge.py"]
