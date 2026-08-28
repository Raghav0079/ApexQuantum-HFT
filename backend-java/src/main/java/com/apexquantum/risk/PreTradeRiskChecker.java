package com.apexquantum.risk;

public final class PreTradeRiskChecker {
    public boolean accepts(long quantity, long maxOrderQuantity) {
        return quantity > 0 && quantity <= maxOrderQuantity;
    }
}
