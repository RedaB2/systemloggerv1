# System Logger for Embedded Devices 🚀

I built this to learn about embedded devices. It logs sensor data with sub-millisecond latency using a dynamic ring buffer, multithreading, and asynchronous file writes. ✌️ 

I'm planning on continuously improving this project to learn more!

## Features ⚡
- **Dynamic Ring Buffer:** Handles high-frequency sensor data smoothly.
- **Multithreading:** Asynchronous writer thread for continuous logging.
- **Mutex Locking:** Ensures safe access in a real-time environment.
- **Embedded Linux Optimized:** Perfect for low-power autonomous devices.

## Getting Started 🛠️
1. **Clone the Repository:**
   ```bash
   git clone https://github.com/RedaB2/systemloggerv1.git
   cd system-logger
   ```

2. **Compile the code:**
   ```bash
   g++ -std=c++11 -pthread -O2 -o system_logger loggerv1.cpp
   ```

3. **Run the logger:**
   ```bash
   ./system_logger
   ```

