//#include "window.h"
//
//#include "core.h"
//#include "niftyCounter.h"
//
//
//
//namespace
//{
//    struct WindowInternals;
//}
//
//
//
//CEDAR_NIFTY_COUNTER_INTERNAL(WindowInternals, WindowInitializer, Cedar);
//
//
//
//// OS-specific implementation of WindowInternals
//#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv
//
//namespace
//{
//    struct WindowInternals
//    {
//        Cedar::WindowClosedFunc  windowClosedCallback  = nullptr;
//        Cedar::WindowClosingFunc windowClosingCallback = nullptr;
//        Cedar::WindowResizedFunc windowResizedCallback = nullptr;
//
//
//        WindowInternals();
//
//        ~WindowInternals();
//    };
//}
//
//#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^
//
//// URGENT: Implement Linux version.
//
//#endif // ^^^ Linux ^^^
//
//
//
//// OS-specific implementation of window.h
//#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv
//
//namespace Cedar
//{
//
//}
//
//#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^
//
//// TODO: Implement Linux version.
//
//#endif // ^^^ Linux ^^^