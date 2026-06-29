#include "core/Order.hpp"
#include "core/Trade.hpp"
#include "risk/ExposureLimit.hpp"
#include "risk/FatFingerCheck.hpp"
#include "risk/PositionLimit.hpp"
#include "risk/RiskEngine.hpp"

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <vector>

namespace {

risk::Order make_order(std::uint64_t id, std::string symbol, risk::Side side,
                       std::int64_t quantity, double price) {
    return risk::Order{
        .order_id = id,
        .symbol = std::move(symbol),
        .side = side,
        .quantity = quantity,
        .price = price,
    };
}

void print_outcome(const risk::ValidationOutcome& outcome) {
    if (outcome.approved) {
        fmt::print("  APPROVED  order {} {} {} @ {:.2f}\n", outcome.order.order_id,
                   risk::to_string(outcome.order.side), outcome.order.quantity,
                   outcome.order.price);
    } else {
        fmt::print("  REJECTED  order {} - {}\n", outcome.order.order_id, outcome.reason);
    }
}

}  // namespace

int main() {
    spdlog::set_level(spdlog::level::info);

    risk::PositionBook positions;
    std::vector<std::unique_ptr<risk::IRiskCheck>> checks;
    checks.push_back(std::make_unique<risk::FatFingerCheck>(0.05));
    checks.push_back(std::make_unique<risk::PositionLimitCheck>(1'000));
    checks.push_back(std::make_unique<risk::ExposureLimitCheck>(500'000.0));

    risk::RiskEngine engine(positions, checks);
    engine.set_limits(1'000, 500'000.0, 0.05);
    engine.set_reference_price("AAPL", 150.00);

    fmt::print("High-Performance Risk Engine\n");
    fmt::print("============================\n\n");

    fmt::print("Pre-trade validation:\n");
    print_outcome(engine.submit_order(make_order(1, "AAPL", risk::Side::Buy, 100, 150.25)));
    print_outcome(engine.submit_order(make_order(2, "AAPL", risk::Side::Buy, 100, 160.00)));
    print_outcome(engine.submit_order(make_order(3, "AAPL", risk::Side::Buy, 2'000, 150.10)));

    fmt::print("\nSimulating approved fills:\n");
    engine.on_fill(risk::Trade{.trade_id = 1, .order_id = 1, .symbol = "AAPL",
                               .side = risk::Side::Buy, .quantity = 100, .price = 150.25});

    engine.mark_to_market({{"AAPL", 151.00}});

    const auto* position = engine.positions().get("AAPL");
    if (position) {
        fmt::print("\nPosition AAPL: qty={}, avg_cost={:.2f}\n", position->quantity,
                   position->average_cost);
    }

    fmt::print("Realized PnL:   {:.2f}\n", engine.pnl().total_realized());
    fmt::print("Unrealized PnL: {:.2f}\n", engine.pnl().total_unrealized());
    fmt::print("Total PnL:      {:.2f}\n", engine.pnl().total_pnl());

    return 0;
}
