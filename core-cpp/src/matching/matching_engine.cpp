#include "matching/matching_engine.hpp"

namespace apexquantum {

MatchingEngine::MatchingEngine(OrderBook& order_book) : order_book_(order_book) {}

void MatchingEngine::submit(const Order& order) {
    order_book_.add(order);
}

}  // namespace apexquantum
