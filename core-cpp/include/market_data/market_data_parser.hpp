#pragma once

#include <cstddef>
#include <span>

namespace apexquantum {

class MarketDataParser {
public:
    void parse(std::span<const std::byte> payload);
};

}  // namespace apexquantum
