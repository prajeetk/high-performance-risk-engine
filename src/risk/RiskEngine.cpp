#include "risk/RiskEngine.hpp"

#include <spdlog/spdlog.h>

namespace risk {

RiskEngine::RiskEngine(PositionBook& positions, std::span<std::unique_ptr<IRiskCheck>> checks)
    : positions_(positions), pnl_(positions) {
    checks_.reserve(checks.size());
    for (auto& check : checks) {
        checks_.push_back(std::move(check));
    }
}

void RiskEngine::set_reference_price(std::string symbol, double price) {
    reference_prices_[std::move(symbol)] = price;
}

void RiskEngine::set_limits(std::int64_t max_position_size, double max_notional_exposure,
                            double fat_finger_pct) {
    max_position_size_ = max_position_size;
    max_notional_exposure_ = max_notional_exposure;
    fat_finger_pct_ = fat_finger_pct;
}

RiskContext RiskEngine::build_context() const {
    return RiskContext{
        .positions = positions_,
        .reference_prices = reference_prices_,
        .max_notional_exposure = max_notional_exposure_,
        .max_position_size = max_position_size_,
        .fat_finger_pct = fat_finger_pct_,
    };
}

ValidationOutcome RiskEngine::validate(const Order& order) const {
    const RiskContext ctx = build_context();

    for (const auto& check : checks_) {
        const RiskResult result = check->check(order, ctx);
        if (!result.approved) {
            spdlog::warn("[{}] rejected order {} on {}: {}", check->name(), order.order_id,
                         order.symbol, result.reason);
            return ValidationOutcome{.approved = false,
                                     .reason = result.reason,
                                     .order = order};
        }
    }

    spdlog::info("[RiskEngine] approved order {} {} {} @ {:.4f}", order.order_id,
                 to_string(order.side), order.quantity, order.price);
    return ValidationOutcome{.approved = true, .reason = {}, .order = order};
}

ValidationOutcome RiskEngine::submit_order(Order order) {
    ValidationOutcome outcome = validate(order);
    if (outcome.approved) {
        order.status = OrderStatus::Approved;
    } else {
        order.status = OrderStatus::Rejected;
    }
    outcome.order = order;
    return outcome;
}

void RiskEngine::on_fill(const Trade& trade) {
    pnl_.on_trade(trade.symbol, trade.side, trade.quantity, trade.price);
    spdlog::info("[RiskEngine] fill {} {} {} @ {:.4f}", trade.symbol, to_string(trade.side),
                 trade.quantity, trade.price);
}

void RiskEngine::mark_to_market(const std::unordered_map<std::string, double>& marks) {
    pnl_.update_marks(marks);
}

}  // namespace risk
