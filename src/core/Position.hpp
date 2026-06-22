#pragma once

#include "utils/Types.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace risk {

struct Position {
    std::string symbol;
    std::int64_t quantity{};       // signed: positive = long, negative = short
    double average_cost{};         // average entry price for open quantity
    double realized_pnl{};
    double unrealized_pnl{};

    [[nodiscard]] bool is_flat() const noexcept { return quantity == 0; }
    [[nodiscard]] double notional(double mark_price) const noexcept {
        return static_cast<double>(quantity) * mark_price;
    }
};

class PositionBook {
public:
    [[nodiscard]] const Position* get(std::string_view symbol) const;
    [[nodiscard]] Position* get(std::string_view symbol);

    void apply_trade(const std::string& symbol, Side side, std::int64_t quantity, double price);
    void mark_to_market(std::string_view symbol, double mark_price);
    void mark_all_to_market(const std::unordered_map<std::string, double>& marks);

    [[nodiscard]] const std::unordered_map<std::string, Position>& positions() const noexcept {
        return positions_;
    }

    [[nodiscard]] double total_realized_pnl() const noexcept;
    [[nodiscard]] double total_unrealized_pnl() const noexcept;

private:
    std::unordered_map<std::string, Position> positions_;
};

}  // namespace risk
