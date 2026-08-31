#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <type_traits>

namespace apexquantum {

template <typename T, std::size_t Capacity>
class SpscQueue {
    static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be a power of two");
    static_assert(std::is_nothrow_copy_constructible_v<T> || std::is_nothrow_move_constructible_v<T>,
                  "T must be safely copyable/movable");

public:
    bool push(const T& value) noexcept {
        const std::size_t head = head_.load(std::memory_order_relaxed);
        const std::size_t next = (head + 1u) & (Capacity - 1u);
        if (next == tail_.load(std::memory_order_acquire)) {
            return false;
        }

        buffer_[head] = value;
        head_.store(next, std::memory_order_release);
        return true;
    }

    bool push(T&& value) noexcept {
        const std::size_t head = head_.load(std::memory_order_relaxed);
        const std::size_t next = (head + 1u) & (Capacity - 1u);
        if (next == tail_.load(std::memory_order_acquire)) {
            return false;
        }

        buffer_[head] = std::move(value);
        head_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& value) noexcept {
        const std::size_t tail = tail_.load(std::memory_order_relaxed);
        if (tail == head_.load(std::memory_order_acquire)) {
            return false;
        }

        value = buffer_[tail];
        tail_.store((tail + 1u) & (Capacity - 1u), std::memory_order_release);
        return true;
    }

private:
    alignas(64) std::atomic<std::size_t> head_{0};
    alignas(64) std::array<T, Capacity> buffer_{};
    alignas(64) std::atomic<std::size_t> tail_{0};
};

}  // namespace apexquantum
