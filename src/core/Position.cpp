#include "core/Position.hpp"

#include <algorithm>
#include <cmath>

namespace risk {

namespace {

void update_unrealized(Position& position, double mark_price) {
    if (position.quantity == 0) {
        position.unrealized_pnl = 0.0;
        return;
    }
    position.unrealized_pnl =
        static_cast<double>(position.quantity) * (mark_price - position.average_cost);
}

}  // namespace

const Position* PositionBook::get(std::string_view symbol) const {
    const auto it = positions_.find(std::string(symbol));
    return it == positions_.end() ? nullptr : &it->second;
}

Position* PositionBook::get(std::string_view symbol) {
    const auto it = positions_.find(std::string(symbol));
    return it == positions_.end() ? nullptr : &it->second;
}

void PositionBook::apply_trade(const std::string& symbol, Side side, std::int64_t quantity,
                               double price) {
    auto& position = positions_[symbol];
    position.symbol = symbol;

    const std::int64_t signed_qty = signed_quantity(side, quantity);
    const std::int64_t new_qty = position.quantity + signed_qty;

    if (position.quantity == 0) {
        position.quantity = signed_qty;
        position.average_cost = price;
        position.unrealized_pnl = 0.0;
        return;
    }

    // Same direction: update average cost.
    if ((position.quantity > 0 && signed_qty > 0) || (position.quantity < 0 && signed_qty < 0)) {
        const double existing_notional =
            static_cast<double>(std::llabs(position.quantity)) * position.average_cost;
        const double trade_notional = static_cast<double>(quantity) * price;
        const std::int64_t total_abs_qty = std::llabs(position.quantity) + quantity;
        position.average_cost = (existing_notional + trade_notional) / static_cast<double>(total_abs_qty);
        position.quantity = new_qty;
        return;
    }

    // Opposite direction: realize PnL on the closed portion.
    const std::int64_t closing_qty = std::min<std::int64_t>(
        static_cast<std::int64_t>(std::llabs(position.quantity)), quantity);
    const double pnl_per_unit =
        position.quantity > 0 ? (price - position.average_cost) : (position.average_cost - price);
    position.realized_pnl += static_cast<double>(closing_qty) * pnl_per_unit;

    if (new_qty == 0) {
        position.quantity = 0;
        position.average_cost = 0.0;
        position.unrealized_pnl = 0.0;
        return;
    }

    if ((position.quantity > 0 && new_qty > 0) || (position.quantity < 0 && new_qty < 0)) {
        position.quantity = new_qty;
        return;
    }

    // Position flipped: remaining quantity opens at trade price.
    position.quantity = new_qty;
    position.average_cost = price;
    position.unrealized_pnl = 0.0;
}

void PositionBook::mark_to_market(std::string_view symbol, double mark_price) {
    if (auto* position = get(symbol)) {
        update_unrealized(*position, mark_price);
    }
}

void PositionBook::mark_all_to_market(const std::unordered_map<std::string, double>& marks) {
    for (auto& [symbol, position] : positions_) {
        const auto it = marks.find(symbol);
        if (it != marks.end()) {
            update_unrealized(position, it->second);
        }
    }
}

double PositionBook::total_realized_pnl() const noexcept {
    double total = 0.0;
    for (const auto& [_, position] : positions_) {
        total += position.realized_pnl;
    }
    return total;
}

double PositionBook::total_unrealized_pnl() const noexcept {
    double total = 0.0;
    for (const auto& [_, position] : positions_) {
        total += position.unrealized_pnl;
    }
    return total;
}

}  // namespace risk
