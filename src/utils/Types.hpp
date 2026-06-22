#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace risk {

enum class Side : std::uint8_t { Buy, Sell };

enum class OrderStatus : std::uint8_t { Pending, Approved, Rejected, Filled, PartiallyFilled };

inline constexpr std::int64_t signed_quantity(Side side, std::int64_t quantity) noexcept {
    return side == Side::Buy ? quantity : -quantity;
}

inline std::string_view to_string(Side side) noexcept {
    return side == Side::Buy ? "BUY" : "SELL";
}

}  // namespace risk
