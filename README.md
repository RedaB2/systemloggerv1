# System Logger for Embedded Devices 🤖

I built this to learn about embedded devices. It logs sensor data with sub-millisecond latency using a dynamic ring buffer, multithreading, and asynchronous file writes. ✌️ 

I'm planning on continuously improving this project to learn more!

Learned a lot from: https://github.com/toolbuilder/dynamic-ring-buffer

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/RedaB2/systemloggerv1.git
   cd systemloggerv1
   ```

2. **Compile the code:**
   ```bash
   g++ -std=c++11 -pthread -O2 -o system_logger loggerv1.cpp
   ```

3. **Run the logger:**
   ```bash
   ./system_logger
   ```

