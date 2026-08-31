#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

namespace apexquantum {

struct alignas(8) MarketDataHeader {
    std::uint8_t msg_type{};
    std::uint8_t channel_id{};
    std::uint16_t seq_no{};
    std::uint32_t nano_ts{};
    std::uint32_t payload_len{};
};

class MarketDataParser {
public:
    static bool parse_header(std::span<const std::byte> frame, MarketDataHeader& header) noexcept {
        if (frame.size() < sizeof(MarketDataHeader)) {
            return false;
        }

        std::memcpy(&header, frame.data(), sizeof(MarketDataHeader));
        return true;
    }

    static std::span<const std::byte> payload(std::span<const std::byte> frame) noexcept {
        const auto header_size = sizeof(MarketDataHeader);
        return frame.subspan(header_size, frame.size() - header_size);
    }

    void parse(std::span<const std::byte> payload);
};

}  // namespace apexquantum
