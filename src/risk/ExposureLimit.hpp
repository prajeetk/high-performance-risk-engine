#pragma once

#include "risk/RiskCheck.hpp"

namespace risk {

class ExposureLimitCheck final : public IRiskCheck {
public:
    explicit ExposureLimitCheck(double max_notional_exposure);

    [[nodiscard]] RiskResult check(const Order& order, const RiskContext& ctx) const override;
    [[nodiscard]] std::string_view name() const noexcept override { return "ExposureLimit"; }

private:
    double max_notional_exposure_;
};

}  // namespace risk
