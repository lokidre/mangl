/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"
#include "../mangl_error.h"

namespace mangl {

class InAppPurchase {
public:
    struct Product {
        String productId;
        Real price{};
        String priceLocalized{};
    };
    
    
    using ProductIdList = std::vector<String>;
    using ProductIdListA = const ProductIdList&;

    using ProductList = std::vector<Product>;
    using ProductListA = const ProductList&;
    
    static SlotVoid onTransactionsRestored;
    static SlotStrView onPurchase;
    static std::function<void (ErrorInfoA)> onError;
    static std::function<void (ProductListA)> onProducts;
    
    static void clearCallbacks() {
        onTransactionsRestored = nullptr;
        onPurchase = nullptr;
        onError = nullptr;
        onProducts = nullptr;
    }

    [[nodiscard]] static bool enabled();
    [[nodiscard]] static bool canMakePurchase();
    static void restoreTransactions();
    static void purchase(StringA productId);
    static void requestProducts(ProductIdListA productIds);
};

}  // namespace mangl
