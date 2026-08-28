#pragma once

#include "common/types.hpp"
#include "order_book/order_book.hpp"

namespace apexquantum {

class MatchingEngine {
public:
    explicit MatchingEngine(OrderBook& order_book);
    void submit(const Order& order);

private:
    OrderBook& order_book_;
};

}  // namespace apexquantum
