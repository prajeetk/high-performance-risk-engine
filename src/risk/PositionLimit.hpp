#pragma once

#include "risk/RiskCheck.hpp"

namespace risk {

class PositionLimitCheck final : public IRiskCheck {
public:
    explicit PositionLimitCheck(std::int64_t max_position_size);

    [[nodiscard]] RiskResult check(const Order& order, const RiskContext& ctx) const override;
    [[nodiscard]] std::string_view name() const noexcept override { return "PositionLimit"; }

private:
    std::int64_t max_position_size_;
};

}  // namespace risk
