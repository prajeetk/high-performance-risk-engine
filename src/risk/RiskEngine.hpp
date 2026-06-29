#pragma once

#include "core/Order.hpp"
#include "core/Position.hpp"
#include "core/Trade.hpp"
#include "risk/PnL.hpp"
#include "risk/RiskCheck.hpp"

#include <memory>
#include <span>
#include <unordered_map>
#include <vector>

namespace risk {

struct ValidationOutcome {
    bool approved{};
    std::string reason;
    Order order;
};

class RiskEngine {
public:
    RiskEngine(PositionBook& positions, std::span<std::unique_ptr<IRiskCheck>> checks);

    void set_reference_price(std::string symbol, double price);
    void set_limits(std::int64_t max_position_size, double max_notional_exposure,
                    double fat_finger_pct);

    [[nodiscard]] ValidationOutcome validate(const Order& order) const;
    [[nodiscard]] ValidationOutcome submit_order(Order order);

    void on_fill(const Trade& trade);
    void mark_to_market(const std::unordered_map<std::string, double>& marks);

    [[nodiscard]] const PositionBook& positions() const noexcept { return positions_; }
    [[nodiscard]] PnLTracker& pnl() noexcept { return pnl_; }
    [[nodiscard]] const PnLTracker& pnl() const noexcept { return pnl_; }

private:
    [[nodiscard]] RiskContext build_context() const;

    PositionBook& positions_;
    PnLTracker pnl_;
    std::vector<std::unique_ptr<IRiskCheck>> checks_;
    std::unordered_map<std::string, double> reference_prices_;
    std::int64_t max_position_size_{};
    double max_notional_exposure_{};
    double fat_finger_pct_{};
};

}  // namespace risk
