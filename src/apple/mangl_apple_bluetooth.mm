#include "mangl_apple_bluetooth.h"

@import CoreBluetooth;

using namespace mangl;
using namespace mangl::net;

// MARK: AppleBluetoothCore

@interface AppleBluetoothCore : NSObject<CBCentralManagerDelegate> {
    
    BluetoothServerApple* server_;
    BluetoothClientApple* client_;
}

@end

// MARK: AppleBluetoothCore Implementation

@implementation AppleBluetoothCore

+ (instancetype)instance {
    static AppleBluetoothCore *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[AppleBluetoothCore alloc] init];
    });

    return sharedInstance;
}

- (instancetype)init {
    self = [super init];

    server_ = nullptr;
    client_ = nullptr;
    
    return self;
}

- (void)startServer: (BluetoothServerApple *)server {
    server_ = server;
}


// MARK: CBCentralManagerDelegate

- (void) centralManager:(CBCentralManager *) central didConnectPeripheral:(CBPeripheral *) peripheral {
    MDebugFunc(": %s", peripheral.name);
    
}

- (void) centralManager:(CBCentralManager *) central didDisconnectPeripheral:(CBPeripheral *) peripheral error:(NSError *) error {
    MDebugFunc(": %s");
    if (error) {
        MDebugPrint("    Error: %s", error.description);
    }
}

- (void) centralManager:(CBCentralManager *) central didFailToConnectPeripheral:(CBPeripheral *) peripheral error:(NSError *) error {
    MDebugFunc(": %s");
    if (error) {
        MDebugPrint("    Error: %s", error.description);
    }
}

- (void) centralManager:(CBCentralManager *) central
connectionEventDidOccur:(CBConnectionEvent) event
          forPeripheral:(CBPeripheral *) peripheral {
    MDebugFunc(": %s - %s", event == CBConnectionEventPeerConnected ? "Conntected" : "Disconnected");
}

- (void) centralManager:(CBCentralManager *) central
  didDiscoverPeripheral:(CBPeripheral *) peripheral
      advertisementData:(NSDictionary<NSString *,id> *) advertisementData
                   RSSI:(NSNumber *) RSSI {
    
    MDebugFunc(": %s", peripheral.name);

    
}

- (void) centralManagerDidUpdateState:(CBCentralManager *) central {
    MDebugFunc();
}

- (void) centralManager:(CBCentralManager *) central willRestoreState:(NSDictionary<NSString *,id> *) dict {
    MDebugFunc();
}

- (void) centralManager:(CBCentralManager *) central didUpdateANCSAuthorizationForPeripheral:(CBPeripheral *) peripheral {
    MDebugFunc(": %s", peripheral.name);
}

- (void) centralManager:(CBCentralManager *) central
didDisconnectPeripheral:(CBPeripheral *) peripheral
              timestamp:(CFAbsoluteTime) timestamp
         isReconnecting:(BOOL) isReconnecting
                  error:(NSError *) error {
    
    MDebugFunc(": %s", peripheral.name);
    if (error) {
        MDebugPrint("    Error: %s", error.description);
    }
    
    
}

@end


namespace mangl::net {

// MARK: BluetoothServerApple

void BluetoothServerApple::doStart() {
    [AppleBluetoothCore.instance startServer:this];
}


// MARK: BluetoothClientApple



}  // namespace mangl

