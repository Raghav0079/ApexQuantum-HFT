#pragma once

#include <cstdint>

namespace apexquantum {

using OrderId = std::uint64_t;
using Price = std::int64_t;
using Quantity = std::uint64_t;

enum class Side : std::uint8_t { Buy, Sell };

enum class OrderType : std::uint8_t { Limit, Market };

struct Order {
    OrderId id{};
    Price price{};
    Quantity quantity{};
    Side side{Side::Buy};
    OrderType type{OrderType::Limit};
};

}  // namespace apexquantum
