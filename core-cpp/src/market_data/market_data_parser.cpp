#include "market_data/market_data_parser.hpp"

namespace apexquantum {

void MarketDataParser::parse(std::span<const std::byte> payload) {
    MarketDataHeader header{};
    if (!parse_header(payload, header)) {
        return;
    }

    (void)header;
}

}  // namespace apexquantum
