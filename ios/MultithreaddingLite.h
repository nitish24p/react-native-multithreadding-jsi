#ifdef __cplusplus
#import "react-native-multithreadding-lite.h"
#endif

#ifdef RCT_NEW_ARCH_ENABLED
#import "RNMultithreaddingLiteSpec.h"

@interface MultithreaddingLite : NSObject <NativeMultithreaddingLiteSpec>
#else
#import <React/RCTBridgeModule.h>

@interface MultithreaddingLite : NSObject <RCTBridgeModule>
#endif

@end
