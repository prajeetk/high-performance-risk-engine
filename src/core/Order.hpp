#pragma once

#include "utils/Types.hpp"

#include <cstdint>
#include <string>

namespace risk {

struct Order {
    std::uint64_t order_id{};
    std::string symbol;
    Side side{Side::Buy};
    std::int64_t quantity{};
    double price{};
    OrderStatus status{OrderStatus::Pending};

    [[nodiscard]] double notional() const noexcept { return static_cast<double>(quantity) * price; }
};

}  // namespace risk
