#include "order_book/order_book.hpp"

namespace apexquantum {

void OrderBook::add(const Order& order) {
    auto& levels = order.side == Side::Buy ? bids_ : asks_;
    levels[order.price] += order.quantity;
}

std::optional<Price> OrderBook::best_bid() const {
    return bids_.empty() ? std::nullopt : std::optional{bids_.begin()->first};
}

std::optional<Price> OrderBook::best_ask() const {
    return asks_.empty() ? std::nullopt : std::optional{asks_.begin()->first};
}

}  // namespace apexquantum
