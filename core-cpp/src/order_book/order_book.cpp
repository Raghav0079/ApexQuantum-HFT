#include "order_book/order_book.hpp"

#include <algorithm>
#include <atomic>
#include <limits>

namespace apexquantum {

namespace {

std::atomic<std::uint32_t> next_free_index{0};

}  // namespace

std::size_t OrderBook::price_to_index(Price price) const noexcept {
    const auto normalized = static_cast<std::uint64_t>(price >= 0 ? price : -price);
    return static_cast<std::size_t>(normalized % MaxLevels);
}

std::size_t OrderBook::find_or_create_level(std::array<PriceLevel, MaxLevels>& levels, Price price) const noexcept {
    auto start = price_to_index(price);
    for (std::size_t probe = 0; probe < MaxLevels; ++probe) {
        const auto index = (start + probe) % MaxLevels;
        auto& level = levels[index];
        if (!level.active) {
            level.price = price;
            level.volume = 0;
            level.head = InvalidIndex;
            level.tail = InvalidIndex;
            level.active = true;
            return index;
        }
        if (level.price == price) {
            return index;
        }
    }

    return MaxLevels;
}

std::uint32_t OrderBook::acquire_slot() noexcept {
    for (std::uint32_t i = 0; i < MaxOrders; ++i) {
        const auto index = (next_free_index.load(std::memory_order_relaxed) + i) % MaxOrders;
        if (!orders_[index].active) {
            std::uint32_t expected = next_free_index.load(std::memory_order_relaxed);
            while (!next_free_index.compare_exchange_weak(
                       expected,
                       (index + 1u) % MaxOrders,
                       std::memory_order_acq_rel,
                       std::memory_order_relaxed)) {
                if (!orders_[index].active) {
                    break;
                }
            }
            if (!orders_[index].active) {
                return index;
            }
        }
    }
    return InvalidIndex;
}

void OrderBook::add(const Order& order) {
    auto& levels = order.side == Side::Buy ? bids_ : asks_;
    const auto level_index = find_or_create_level(levels, order.price);
    if (level_index == MaxLevels) {
        return;
    }

    const auto slot_index = acquire_slot();
    if (slot_index == InvalidIndex) {
        return;
    }

    auto& level = levels[level_index];
    auto& node = orders_[slot_index];
    node.order = order;
    node.prev = level.tail;
    node.next = InvalidIndex;
    node.active = true;

    if (level.head == InvalidIndex) {
        level.head = slot_index;
        level.tail = slot_index;
    } else {
        orders_[level.tail].next = slot_index;
        level.tail = slot_index;
    }

    level.volume += order.quantity;
}

std::optional<Price> OrderBook::best_bid() const {
    Price best = std::numeric_limits<Price>::lowest();
    bool found = false;

    for (const auto& level : bids_) {
        if (level.active && level.price > best) {
            best = level.price;
            found = true;
        }
    }

    return found ? std::optional<Price>{best} : std::nullopt;
}

std::optional<Price> OrderBook::best_ask() const {
    Price best = std::numeric_limits<Price>::max();
    bool found = false;

    for (const auto& level : asks_) {
        if (level.active && level.price < best) {
            best = level.price;
            found = true;
        }
    }

    return found ? std::optional<Price>{best} : std::nullopt;
}

}  // namespace apexquantum
