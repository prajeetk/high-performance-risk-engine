#include "core/Position.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Opening a long position updates average cost", "[position]") {
    risk::PositionBook book;
    book.apply_trade("AAPL", risk::Side::Buy, 100, 150.0);

    const auto* position = book.get("AAPL");
    REQUIRE(position != nullptr);
    REQUIRE(position->quantity == 100);
    REQUIRE(position->average_cost == Catch::Approx(150.0));
    REQUIRE(position->realized_pnl == Catch::Approx(0.0));
}

TEST_CASE("Adding to a long position computes weighted average cost", "[position]") {
    risk::PositionBook book;
    book.apply_trade("AAPL", risk::Side::Buy, 100, 100.0);
    book.apply_trade("AAPL", risk::Side::Buy, 100, 120.0);

    const auto* position = book.get("AAPL");
    REQUIRE(position->quantity == 200);
    REQUIRE(position->average_cost == Catch::Approx(110.0));
}

TEST_CASE("Partial close realizes PnL", "[position]") {
    risk::PositionBook book;
    book.apply_trade("AAPL", risk::Side::Buy, 100, 100.0);
    book.apply_trade("AAPL", risk::Side::Sell, 40, 110.0);

    const auto* position = book.get("AAPL");
    REQUIRE(position->quantity == 60);
    REQUIRE(position->realized_pnl == Catch::Approx(400.0));
}

TEST_CASE("Closing a position resets average cost", "[position]") {
    risk::PositionBook book;
    book.apply_trade("AAPL", risk::Side::Buy, 100, 100.0);
    book.apply_trade("AAPL", risk::Side::Sell, 100, 105.0);

    const auto* position = book.get("AAPL");
    REQUIRE(position->is_flat());
    REQUIRE(position->realized_pnl == Catch::Approx(500.0));
    REQUIRE(position->average_cost == Catch::Approx(0.0));
}

TEST_CASE("Position flip opens at trade price", "[position]") {
    risk::PositionBook book;
    book.apply_trade("AAPL", risk::Side::Buy, 100, 100.0);
    book.apply_trade("AAPL", risk::Side::Sell, 150, 95.0);

    const auto* position = book.get("AAPL");
    REQUIRE(position->quantity == -50);
    REQUIRE(position->average_cost == Catch::Approx(95.0));
    REQUIRE(position->realized_pnl == Catch::Approx(-500.0));
}

TEST_CASE("Position flip from long to short records realized PnL", "[position]") {
    risk::PositionBook book;
    book.apply_trade("AAPL", risk::Side::Buy, 100, 100.0);
    book.apply_trade("AAPL", risk::Side::Sell, 150, 105.0);

    const auto* pos = book.get("AAPL");
    REQUIRE(pos != nullptr);
    REQUIRE(pos->quantity == -50);
    REQUIRE(pos->average_cost == Catch::Approx(105.0));
    REQUIRE(pos->realized_pnl == Catch::Approx(500.0));
}

TEST_CASE("Mark-to-market updates unrealized PnL", "[position]") {
    risk::PositionBook book;
    book.apply_trade("AAPL", risk::Side::Buy, 100, 100.0);
    book.mark_to_market("AAPL", 105.0);

    const auto* position = book.get("AAPL");
    REQUIRE(position->unrealized_pnl == Catch::Approx(500.0));
}
