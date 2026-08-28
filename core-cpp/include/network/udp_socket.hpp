#pragma once

#include <cstdint>
#include <string_view>

namespace apexquantum {

class UdpSocket {
public:
    void bind(std::string_view address, std::uint16_t port);
};

}  // namespace apexquantum
