#pragma once

#include "utils/Types.hpp"

#include <cstdint>
#include <string>

namespace risk {

struct Trade {
    std::uint64_t trade_id{};
    std::uint64_t order_id{};
    std::string symbol;
    Side side{Side::Buy};
    std::int64_t quantity{};
    double price{};

    [[nodiscard]] double notional() const noexcept { return static_cast<double>(quantity) * price; }
};

}  // namespace risk
