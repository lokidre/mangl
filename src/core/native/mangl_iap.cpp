/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_iap.h"

#include "../mangl_dispatch.h"

namespace mangl {

SlotVoid InAppPurchase::onTransactionsRestored{};
SlotStrView InAppPurchase::onPurchase{};
std::function<void (ErrorInfoA)> InAppPurchase::onError{};
std::function<void (InAppPurchase::ProductListA)> InAppPurchase::onProducts{};


#if MANGL_IAP && APEAL_OS_MAC
#else
bool InAppPurchase::enabled() { return false; }
bool InAppPurchase::canMakePurchase() { return false; }
void InAppPurchase::restoreTransactions(){}
void InAppPurchase::purchase(StringA){}
void InAppPurchase::requestProducts(ProductIdListA productIds) {
    if (!onProducts || productIds.empty()) return;

    Dispatch::asyncMainDelayed([productIds] {

        ProductList products;
        products.push_back({
            .productId = productIds.front(),
            .price = 0.99,
            .priceLocalized = "$0.99",
        });

        if (onProducts) onProducts(products);

    }, 10);
}
#endif

}  // namespace mangl

