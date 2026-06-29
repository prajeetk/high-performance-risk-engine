#include "risk/PnL.hpp"

namespace risk {

void PnLTracker::on_trade(const std::string& symbol, Side side, std::int64_t quantity,
                          double price) {
    book_.apply_trade(symbol, side, quantity, price);
}

void PnLTracker::update_mark(std::string_view symbol, double mark_price) {
    book_.mark_to_market(symbol, mark_price);
}

void PnLTracker::update_marks(const std::unordered_map<std::string, double>& marks) {
    book_.mark_all_to_market(marks);
}

}  // namespace risk
