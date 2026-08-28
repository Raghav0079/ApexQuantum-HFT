#pragma once

#include "common/types.hpp"

#include <functional>
#include <map>
#include <optional>

namespace apexquantum {

class OrderBook {
public:
    void add(const Order& order);
    std::optional<Price> best_bid() const;
    std::optional<Price> best_ask() const;

private:
    std::map<Price, Quantity, std::greater<>> bids_;
    std::map<Price, Quantity> asks_;
};

}  // namespace apexquantum
