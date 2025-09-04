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
