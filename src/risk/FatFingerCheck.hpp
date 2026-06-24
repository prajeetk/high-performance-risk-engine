#pragma once

#include "risk/RiskCheck.hpp"

namespace risk {

class FatFingerCheck final : public IRiskCheck {
public:
    explicit FatFingerCheck(double max_deviation_pct);

    [[nodiscard]] RiskResult check(const Order& order, const RiskContext& ctx) const override;
    [[nodiscard]] std::string_view name() const noexcept override { return "FatFinger"; }

private:
    double max_deviation_pct_;
};

}  // namespace risk
