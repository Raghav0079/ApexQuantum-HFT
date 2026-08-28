#pragma once

#include <array>
#include <atomic>
#include <cstddef>

namespace apexquantum {

template <typename T, std::size_t Capacity>
class SpscQueue {
public:
    bool push(const T& value) {
        const auto current = head_.load(std::memory_order_relaxed);
        const auto next = (current + 1) % Capacity;
        if (next == tail_.load(std::memory_order_acquire)) {
            return false;
        }
        buffer_[current] = value;
        head_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& value) {
        const auto current = tail_.load(std::memory_order_relaxed);
        if (current == head_.load(std::memory_order_acquire)) {
            return false;
        }
        value = buffer_[current];
        tail_.store((current + 1) % Capacity, std::memory_order_release);
        return true;
    }

private:
    std::array<T, Capacity> buffer_{};
    std::atomic<std::size_t> head_{0};
    std::atomic<std::size_t> tail_{0};
};

}  // namespace apexquantum
