#pragma once

#include "common/types.hpp"

#include <array>
#include <cstdint>
#include <limits>
#include <optional>

namespace apexquantum {

struct alignas(64) OrderNode {
    Order order{};
    std::uint32_t prev{std::numeric_limits<std::uint32_t>::max()};
    std::uint32_t next{std::numeric_limits<std::uint32_t>::max()};
    bool active{false};
};

struct alignas(64) PriceLevel {
    Price price{};
    Quantity volume{};
    std::uint32_t head{std::numeric_limits<std::uint32_t>::max()};
    std::uint32_t tail{std::numeric_limits<std::uint32_t>::max()};
    bool active{false};
};

class OrderBook {
public:
    static constexpr std::size_t MaxLevels = 4096;
    static constexpr std::size_t MaxOrders = 1u << 20;

    void add(const Order& order);
    std::optional<Price> best_bid() const;
    std::optional<Price> best_ask() const;

private:
    static constexpr std::uint32_t InvalidIndex = std::numeric_limits<std::uint32_t>::max();

    std::size_t find_or_create_level(std::array<PriceLevel, MaxLevels>& levels, Price price) const noexcept;
    std::size_t price_to_index(Price price) const noexcept;
    std::uint32_t acquire_slot() noexcept;

    alignas(64) inline static std::array<PriceLevel, MaxLevels> bids_{};
    alignas(64) inline static std::array<PriceLevel, MaxLevels> asks_{};
    alignas(64) inline static std::array<OrderNode, MaxOrders> orders_{};
};

}  // namespace apexquantum
