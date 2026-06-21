#include "core/Order.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Order notional calculation", "[order]") {
    risk::Order order{
        .order_id = 1,
        .symbol = "AAPL",
        .side = risk::Side::Buy,
        .quantity = 100,
        .price = 150.25,
    };

    REQUIRE(order.notional() == Catch::Approx(15025.0));
}

TEST_CASE("Signed quantity helper", "[order]") {
    REQUIRE(risk::signed_quantity(risk::Side::Buy, 50) == 50);
    REQUIRE(risk::signed_quantity(risk::Side::Sell, 50) == -50);
}
