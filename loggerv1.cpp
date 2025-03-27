#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <string>

class SystemLogger {
public:
    // open the log file and start the async writer thread.

    SystemLogger(const std::string& filename, size_t bufferCapacity)
        : buffer(bufferCapacity), capacity(bufferCapacity), head(0), tail(0), exitFlag(false)
    {
        logFile.open(filename, std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            throw std::runtime_error("Failed to open log file.");
        }
        writerThread = std::thread(&SystemLogger::writerTask, this);
    }

    // destructor signal writer thread and then exit, flush the log data.
    ~SystemLogger() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            exitFlag = true;
            cv.notify_all();
        }
        if (writerThread.joinable()) {
            writerThread.join();
        }
        logFile.close();
    }

    // called from the sensor thread to log messages.
    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mtx);
        // ring buffer is full: next position equals tail
        if ((head + 1) % capacity == tail) {
            // buffer full: message can be dropped or overwrite older entries.
            return;
        }
        buffer[head] = message;
        head = (head + 1) % capacity;
        cv.notify_all();
    }

private:
    std::vector<std::string> buffer;
    const size_t capacity;
    size_t head; // next write
    size_t tail; // next read
    bool exitFlag;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread writerThread;
    std::ofstream logFile;

    // asynch writer function flushes logs to file
    void writerTask() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            // wait until data is available or exit is requested
            cv.wait(lock, [this] { return head != tail || exitFlag; });

            while (head != tail) {
                // write log entry to file
                logFile << buffer[tail] << std::endl;
                tail = (tail + 1) % capacity;
            }

            if (exitFlag) break;
            
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        // flush the rest
        logFile.flush();
    }
};

//
// EXAMPLE FOR FAKE SENSOR DATA
int main() {
    try {
        SystemLogger logger("system_log.txt", 1024);

        // same deal here, we simulate the sensor data production from different thread. 
            std::thread sensorThread([&logger]() {
            for (int i = 0; i < 10000; ++i) {
                std::string msg = "Sensor reading " + std::to_string(i) +
                                  " at " +
                                  std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(
                                                     std::chrono::steady_clock::now().time_since_epoch()).count()) +
                                  " µs";
                logger.log(msg);
                // here we simulate the high frequency of sensor data
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        });

        sensorThread.join();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}