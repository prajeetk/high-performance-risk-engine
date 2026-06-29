#include "risk/PositionLimit.hpp"

#include <cmath>
#include <fmt/core.h>

namespace risk {

PositionLimitCheck::PositionLimitCheck(std::int64_t max_position_size)
    : max_position_size_(max_position_size) {}

RiskResult PositionLimitCheck::check(const Order& order, const RiskContext& ctx) const {
    const std::int64_t limit =
        ctx.max_position_size > 0 ? ctx.max_position_size : max_position_size_;

    const Position* position = ctx.positions.get(order.symbol);
    const std::int64_t current_qty = position ? position->quantity : 0;
    const std::int64_t delta = signed_quantity(order.side, order.quantity);
    const std::int64_t projected_qty = current_qty + delta;

    if (std::llabs(projected_qty) > limit) {
        return RiskResult::reject(fmt::format(
            "Position limit exceeded for {}: projected {} exceeds max {}",
            order.symbol, projected_qty, limit));
    }

    return RiskResult::pass();
}

}  // namespace risk
