#pragma once

#include "core/Order.hpp"
#include "core/Position.hpp"

#include <string>
#include <unordered_map>

namespace risk {

struct RiskResult {
    bool approved{true};
    std::string reason;

    static RiskResult pass() { return {}; }

    static RiskResult reject(std::string reason) {
        RiskResult result;
        result.approved = false;
        result.reason = std::move(reason);
        return result;
    }
};

struct RiskContext {
    const PositionBook& positions;
    std::unordered_map<std::string, double> reference_prices;
    double max_notional_exposure{0.0};
    std::int64_t max_position_size{0};
    double fat_finger_pct{0.0};
};

class IRiskCheck {
public:
    virtual ~IRiskCheck() = default;
    [[nodiscard]] virtual RiskResult check(const Order& order, const RiskContext& ctx) const = 0;
    [[nodiscard]] virtual std::string_view name() const noexcept = 0;
};

}  // namespace risk
