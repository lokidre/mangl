#include "../core/mangl_setup.h"

#if MANGL_IAP

@import StoreKit;


#include "mangl_imacos_util.h"
#include "../ios/mangl_hooks_ios.h"

#include "../core/mangl_debug.h"
#include "../core/mangl_dispatch.h"

#include "../core/native/mangl_iap.h"

#if APEAL_OS_IPHONE
#import "../ios/mangl_main_vc.h"
#else
#import "../mac/mangl_main_view.h"
#endif


using namespace mangl;


@interface ManglStoreKitIAPHelper: NSObject <SKPaymentTransactionObserver, SKProductsRequestDelegate> {
    bool initialized_;
}

+ (instancetype)instance;

- (BOOL)onLaunch;
- (void)onTerminate;
- (void)restoreTransactions;
- (void)requestProducts:(const InAppPurchase::ProductIdList&)productIds;
- (void)purchase:(StringA)productId;

@property (nonatomic) NSMutableDictionary<NSString*, SKProduct*> * products;
//@property (nonatomic) NSArray<SKProduct *>* products;
@property (nonatomic) SKProductsRequest* productsRequest;
@property (nonatomic) NSString* pendingPurchaseId;


@end



// MARK: Launch hook
@interface ManglStoreKitLaunchHook: NSObject<ManglIosMainHook>
@end

@implementation ManglStoreKitLaunchHook

- (BOOL)didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    return [ManglStoreKitIAPHelper.instance onLaunch];
}

- (void)applicationWillTerminate {
    [ManglStoreKitIAPHelper.instance onTerminate];
}
@end



[[maybe_unused]]
static bool launchHook = []{
    [ManglIosHooks.instance registerHook: [[ManglStoreKitLaunchHook alloc] init]] ;
    return true;
}();



//@interface ManglStoreKitIAPHelper() {
//    bool initialized_;
//}
//
//@property (nonatomic) NSMutableDictionary* products;
//@property (nonatomic) SKProductsRequest* productsRequest;
//@property (nonatomic) NSString* pendingPurchaseId;
//
//@end


@implementation ManglStoreKitIAPHelper

+ (instancetype)instance {
    static ManglStoreKitIAPHelper *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ManglStoreKitIAPHelper alloc] init];
    });

    return sharedInstance;
}

- (instancetype)init {
    self = [super init];
    initialized_ = false;
    _productsRequest = nil;
    _pendingPurchaseId = nil;
    return self;
}

-(BOOL)onLaunch {
    [self checkInit];
    return true;
}

-(void)onTerminate {
    [SKPaymentQueue.defaultQueue removeTransactionObserver:self];
}


- (void)checkInit {
    if (initialized_)
        return;
    
    self.products = [[NSMutableDictionary<NSString*, SKProduct*> alloc] init];
    
    [SKPaymentQueue.defaultQueue addTransactionObserver:self];
    initialized_ = true;
}


- (void)restoreTransactions {
    [self checkInit];
    [SKPaymentQueue.defaultQueue restoreCompletedTransactions];
}

- (void)requestProducts:(const InAppPurchase::ProductIdList&)productIds {
    NSMutableSet* idSet = [NSMutableSet setWithCapacity:productIds.size()];
    for (auto& productId: productIds) {
        [idSet addObject:toNSString(productId)];
    }
    
    self.productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:idSet];
    self.productsRequest.delegate = self;
    [self.productsRequest start];
}


- (void)purchaseProductByPtr:(SKProduct *)product {
    //SKPayment* payment = [SKPayment paymentWithProduct:product];
    SKMutablePayment *payment = [SKMutablePayment paymentWithProduct:product];
    [SKPaymentQueue.defaultQueue addPayment:payment];
}

- (void)purchaseProductById:(NSString *)productId {
    SKProduct* product = [_products objectForKey:productId];
    
    if (!product) {
        if (InAppPurchase::onError)
            InAppPurchase::onError(ErrorInfo("Invalid product id"));
        return;
    }
    
    [self purchaseProductByPtr:product];
}



- (void)purchase:(StringA)productId {
    [self checkInit];
    
//    if (_products.count == 0) {
        _pendingPurchaseId = toNSString(productId);
//        if (!_productsRequest) {
    InAppPurchase::ProductIdList ids{productId};
            [self requestProducts:ids];
//        }
//        return;
//    }
//    [self purchaseProductById:toNSString(productId)];
}



//
// MARK: SKProductsRequestDelegate
//
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    MDebugFunc();
    
    [_products removeAllObjects];

    for (SKProduct* prod in response.products) {
        MDebugPrint("  Product title: %s", prod.localizedTitle);
        MDebugPrint("  Product description: %s", prod.localizedDescription);
        MDebugPrint("  Product price: %f", prod.price);
        MDebugPrint("  Product id: %s", prod.productIdentifier);
        
    
        [_products setObject:prod forKey:prod.productIdentifier];
    }
    
    std::vector<String> invalidProducts;

    for (NSString* invalidProductId in response.invalidProductIdentifiers) {
        MDebugPrint("  Invalid product id: %s", invalidProductId);
        invalidProducts.push_back(toString(invalidProductId));
    }
    if (!invalidProducts.empty()) {
        if (InAppPurchase::onError)
            InAppPurchase::onError(ErrorInfo{"Invalid Product Id: %s", invalidProducts.front()});
    }
    
    if (_pendingPurchaseId) {
        [self purchaseProductById:_pendingPurchaseId];
        _pendingPurchaseId = nil;
    }
    
    if (InAppPurchase::onProducts) {
        InAppPurchase::ProductList products;
        
        for (SKProduct* prod in response.products) {

            NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
            [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
            [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
            [numberFormatter setLocale:prod.priceLocale];
            
//#if _DEBUG
//            NSLocale* debugLocale = [NSLocale localeWithLocaleIdentifier: @"ru_RU"];
////            NSLocale* debugLocale = [NSLocale localeWithLocaleIdentifier: @"ko_KR"];
//            [numberFormatter setLocale:debugLocale];
//#endif
            
            NSString *formattedString = [numberFormatter stringFromNumber:prod.price];

            products.push_back({
                .productId = toString(prod.productIdentifier),
                .price = prod.price.doubleValue,
                .priceLocalized = toString(formattedString),
            });
        }
        
        Dispatch::asyncMain([products]{
            InAppPurchase::onProducts(products);
        });
    }

}



//
// MARK: SKPaymentTransactionObserver
//
- (String)transactionErrorCodeToString:(long)errorCode {
    String errorString;

    switch (errorCode) {
    case SKErrorUnknown:
        errorString = "Error Purchasing this Item.";
        break;
    case SKErrorClientInvalid:   // client is not allowed to issue the request, etc.
        errorString = "Error Purchasing this Item.";
        break;
    case SKErrorPaymentCancelled:   // user cancelled the request, etc.
        errorString = "Payment Cancelled";
        break;
    case SKErrorPaymentInvalid:   // purchase identifier was invalid, etc.
        errorString = "Invalid Item";
        break;
    case SKErrorPaymentNotAllowed:   // this device is not allowed to make the payment
        errorString = "Payments are Disabled from this Device";
        break;
    case SKErrorStoreProductNotAvailable:   // Product is not available in the current storefront
        errorString = "Product is not Available";
        break;
    default:
        errorString = "Error Purchasing this Item.";
        break;
    }

    return errorString;
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    MDebugFunc();

    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
        case SKPaymentTransactionStatePurchased:
        case SKPaymentTransactionStateRestored: {
            auto productId = toString(transaction.payment.productIdentifier);

            MDebugPrint("    purchased: %s", productId);

            if (InAppPurchase::onPurchase)
                InAppPurchase::onPurchase(productId);
            [queue finishTransaction:transaction];
            break;
        }

        case SKPaymentTransactionStateFailed: {
            auto errorString = toString(transaction.error.localizedDescription);

            MDebugPrint("    failed: %s", errorString);

            auto errorCode = transaction.error.code;
            if (errorString.empty()) {
                errorString = [self transactionErrorCodeToString:errorCode];
            }

            if (InAppPurchase::onError)
                InAppPurchase::onError(ErrorInfo(errorString));
            [queue finishTransaction:transaction];
            break;
        }

        default:
            break;
        }
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error  {
    auto errorString = toString(error.localizedDescription);

    if (errorString.empty())
        errorString = [self transactionErrorCodeToString:error.code];

    MDebugFunc("%s", errorString);

    if (InAppPurchase::onError)
        InAppPurchase::onError(ErrorInfo(errorString));
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue {
    MDebugFunc();

    for (SKPaymentTransaction *transaction in queue.transactions) {
        auto productId = toString(transaction.payment.productIdentifier);

        MDebugPrint("    product id: %s", productId);

        if (InAppPurchase::onPurchase)
            InAppPurchase::onPurchase(productId);
    }
    if (InAppPurchase::onTransactionsRestored)
        InAppPurchase::onTransactionsRestored();
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedDownloads:(NSArray *)downloads {
    MDebugFunc();
}

- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions {
    MDebugFunc();
}




@end

namespace mangl {

bool InAppPurchase::canMakePurchase() {
    return [SKPaymentQueue canMakePayments];
    //return true; //[ManglStoreKitIAPHelper.instance canMakePurchase];
}

void InAppPurchase::requestProducts(ProductIdListA productIds) {
    [ManglStoreKitIAPHelper.instance requestProducts:productIds];
}



void InAppPurchase::restoreTransactions() {
    [ManglStoreKitIAPHelper.instance restoreTransactions];
}

void InAppPurchase::purchase(StringA productId) {
    [ManglStoreKitIAPHelper.instance purchase:productId];
}

}  // namespace mangl

#endif
