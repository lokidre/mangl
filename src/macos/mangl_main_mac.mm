#include <Cocoa/Cocoa.h>

#include "../core/mangl_setup.h"
#include "../core/mangl_debug.h"
#include "../apple/mangl_imacos_util.h"


#ifndef QT_GUI_LIB

#ifdef MANGL_MAC_DELEGATE_HEADER
#include MANGL_MAC_DELEGATE_HEADER
#else
#include "mangl_app_delegate.h"
#endif


#include <stdexcept>

#include <apeal/debug.hpp>


int main(int argc, char *argv[])
{
    using namespace mangl;
    
	//debug_init(argv[0]) ;
    int retCode = 1;
   
	
#if DEBUG
    try { @try {
        @autoreleasepool {
#endif
            retCode = NSApplicationMain(argc, (const char **)argv);
#if DEBUG
        }
    }
    @catch(NSException *e) {
        MDebugPrint("Exception:%s:%s", toString(e.name), toString(e.reason));
    }}
    catch(const std::exception& e) {
        MDebugPrint("Exception:%s",e.what()) ;
    }
    catch(...) {
        MDebugPrint("Unhandled Exception!") ;
    }
#endif
	
    return retCode;
}

#endif // if Qt
