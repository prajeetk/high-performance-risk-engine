#pragma once

#include "core/Position.hpp"

#include <string>
#include <unordered_map>

namespace risk {

class PnLTracker {
public:
    explicit PnLTracker(PositionBook& book) : book_(book) {}

    void on_trade(const std::string& symbol, Side side, std::int64_t quantity, double price);
    void update_mark(std::string_view symbol, double mark_price);
    void update_marks(const std::unordered_map<std::string, double>& marks);

    [[nodiscard]] double total_realized() const noexcept { return book_.total_realized_pnl(); }
    [[nodiscard]] double total_unrealized() const noexcept { return book_.total_unrealized_pnl(); }
    [[nodiscard]] double total_pnl() const noexcept { return total_realized() + total_unrealized(); }
    [[nodiscard]] const PositionBook& book() const noexcept { return book_; }

private:
    PositionBook& book_;
};

}  // namespace risk
