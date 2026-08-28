#include "common/types.hpp"
#include "order_book/order_book.hpp"

#include <cassert>

int main() {
    apexquantum::OrderBook order_book;
    order_book.add({1, 100, 10, apexquantum::Side::Buy, apexquantum::OrderType::Limit});
    order_book.add({2, 105, 5, apexquantum::Side::Sell, apexquantum::OrderType::Limit});

    assert(order_book.best_bid().value() == 100);
    assert(order_book.best_ask().value() == 105);
    return 0;
}
