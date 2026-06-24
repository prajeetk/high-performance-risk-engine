#include "risk/FatFingerCheck.hpp"

#include <cmath>
#include <fmt/core.h>

namespace risk {

FatFingerCheck::FatFingerCheck(double max_deviation_pct) : max_deviation_pct_(max_deviation_pct) {}

RiskResult FatFingerCheck::check(const Order& order, const RiskContext& ctx) const {
    const double pct = ctx.fat_finger_pct > 0.0 ? ctx.fat_finger_pct : max_deviation_pct_;

    const auto ref_it = ctx.reference_prices.find(order.symbol);
    if (ref_it == ctx.reference_prices.end() || ref_it->second <= 0.0) {
        return RiskResult::reject(
            fmt::format("No reference price available for fat-finger check on {}", order.symbol));
    }

    const double reference = ref_it->second;
    const double deviation = std::abs(order.price - reference) / reference;

    if (deviation > pct) {
        return RiskResult::reject(fmt::format(
            "Fat-finger protection triggered for {}: order price {:.4f} deviates {:.2f}% "
            "from reference {:.4f} (max {:.2f}%)",
            order.symbol, order.price, deviation * 100.0, reference, pct * 100.0));
    }

    return RiskResult::pass();
}

}  // namespace risk
