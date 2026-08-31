#include "execution_engine.hpp"

#include <chrono>
#include <thread>

namespace apexquantum {

void ExecutionEngine::run() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

}  // namespace apexquantum
