#include "risk/ExposureLimit.hpp"

#include <cmath>
#include <fmt/core.h>

namespace risk {

ExposureLimitCheck::ExposureLimitCheck(double max_notional_exposure)
    : max_notional_exposure_(max_notional_exposure) {}

RiskResult ExposureLimitCheck::check(const Order& order, const RiskContext& ctx) const {
    const double limit =
        ctx.max_notional_exposure > 0.0 ? ctx.max_notional_exposure : max_notional_exposure_;

    double total_exposure = 0.0;
    for (const auto& [symbol, position] : ctx.positions.positions()) {
        const auto ref_it = ctx.reference_prices.find(symbol);
        const double mark = ref_it != ctx.reference_prices.end() ? ref_it->second
                                                                   : position.average_cost;
        total_exposure += std::abs(position.notional(mark));
    }

    const Position* position = ctx.positions.get(order.symbol);
    const std::int64_t current_qty = position ? position->quantity : 0;
    const std::int64_t projected_qty =
        current_qty + signed_quantity(order.side, order.quantity);

    const auto ref_it = ctx.reference_prices.find(order.symbol);
    const double mark =
        ref_it != ctx.reference_prices.end() ? ref_it->second : order.price;

    if (position) {
        total_exposure -= std::abs(position->notional(mark));
    }
    total_exposure += std::abs(static_cast<double>(projected_qty) * mark);

    if (total_exposure > limit) {
        return RiskResult::reject(fmt::format(
            "Notional exposure limit exceeded: projected {:.2f} exceeds max {:.2f}",
            total_exposure, limit));
    }

    return RiskResult::pass();
}

}  // namespace risk
