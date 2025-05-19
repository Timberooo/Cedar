#ifndef CEDAR_NIFTY_COUNTER_H
#define CEDAR_NIFTY_COUNTER_H

#include <cstddef>
#include <new>
#include <type_traits>



#define CEDAR_NIFTY_COUNTER_HEADER(counterName) \
class counterName                               \
{                                               \
public:                                         \
    counterName();                              \
    ~counterName();                             \
private:                                        \
    static std::size_t s_counter;               \
};                                              \
                                                \
static counterName g_##counterName##;



#define CEDAR_NIFTY_COUNTER_INTERNAL(internalStructName, counterName, counterNamespace)                                   \
namespace                                                                                                                 \
{                                                                                                                         \
    static typename std::aligned_storage<sizeof(counterName), alignof(counterName)>::type g_##internalStructName##Buffer; \
    internalStructName& g_##internalStructName## = reinterpret_cast<internalStructName&>(g_##internalStructName##Buffer); \
}                                                                                                                         \
                                                                                                                          \
namespace counterNamespace                                                                                                \
{                                                                                                                         \
    std::size_t counterName::s_counter = 0;                                                                               \
                                                                                                                          \
    counterName::counterName()                                                                                            \
    {                                                                                                                     \
        if (s_counter == 0)                                                                                               \
            new (&g_##internalStructName##)internalStructName();                                                          \
                                                                                                                          \
        s_counter++;                                                                                                      \
    }                                                                                                                     \
                                                                                                                          \
    counterName::~counterName()                                                                                           \
    {                                                                                                                     \
        s_counter--;                                                                                                      \
                                                                                                                          \
        if (s_counter == 0)                                                                                               \
            g_##internalStructName##.~internalStructName();                                                               \
    }                                                                                                                     \
}



//#define CEDAR_NIFTY_COUNTER_INTERNAL(internalStructName, counterName) \
//namespace \
//{ \
//    static typename std::aligned_storage<sizeof(counterName), alignof(counterName)>::type g_##internalStructName##Buffer; \
//    internalStructName& g_##internalStructName## = reinterpret_cast<internalStructName&>(g_##internalStructName##Buffer); \
//} \
//\
//std::size_t counterName
//
//
//
//#define CEDAR_NIFTY_COUNTER_INTERNAL_WITH_NAMESPACE(internalStructName, counterName, counterNamespace) CEDAR_NIFTY_COUNTER_INTERNAL(internalStructName, counterNamespace::counterName)

#endif // CEDAR_NIFTY_COUNTER_H





//#ifndef CEDAR_NIFTY_COUNTER_H
//#define CEDAR_NIFTY_COUNTER_H
//
//#include <cstddef>
//#include <new>
//#include <type_traits>
//
//
//
//#define CEDAR_NIFTY_COUNTER_HEADER(initializerName) \
//class initializerName                               \
//{                                                   \
//public:                                             \
//    initializerName();                              \
//    ~initializerName();                             \
//private:                                            \
//    static std::size_t s_counter;                   \
//};                                                  \
//                                                    \
//static initializerName g_initializerName;
//
//
//
//#define CEDAR_NIFTY_COUNTER_INTERNAL(initializerName, internalStructName) \
//namespace \
//{ \
//    static typename std::aligned_storage<sizeof(initializerName), alignof(initializerName)>::type g_##internalStructName##Buffer; \
//    internalStructName& g_internalStructName = reinterpret_cast<internalStructName&>(g_##internalStructName##Buffer); \
//}
//
//
//
//#define CEDAR_NIFTY_COUNTER_INTERNAL_WITH_NAMESPACE(initializerName, internalStructName, initializerNamespace) CEDAR_NIFTY_COUNTER_INTERNAL(initializerNamespace::initializerName, internalStructName)
//
//#endif // CEDAR_NIFTY_COUNTER_H





//#ifndef CEDAR_NIFTY_COUNTER_H
//#define CEDAR_NIFTY_COUNTER_H
//
//#include <cstddef>
//#include <new>
//#include <type_traits>
//
//
//
//#define CEDAR_NIFTY_COUNTER_HEADER(initializerName) \
//class initializerName                               \
//{                                                   \
//public:                                             \
//    initializerName();                              \
//    ~initializerName();                             \
//private:                                            \
//    static std::size_t s_counter;                   \
//};                                                  \
//static initializerName g_initializerName;
//
//
//
//#define CEDAR_NIFTY_COUNTER_INTERNAL(initializerName, initializerNamespace, internalDataStructName)                                                                           \
//namespace                                                                                                                                                                     \
//{                                                                                                                                                                             \
//    static typename std::aligned_storage<sizeof(initializerNamespace::initializerName), alignof(initializerNamespace::initializerName)>::type g_internalDataStructNameBuffer; \
//    internalDataStructName& g_internalDataStructName = reinterpret_cast<internalDataStructName&>(g_internalDataStructNameBuffer);                                             \
//}                                                                                                                                                                             \
//                                                                                                                                                                              \
//namespace initializerNamespace                                                                                                                                                \
//{                                                                                                                                                                             \
//    std::size_t initializerName::s_counter = 0;                                                                                                                               \
//                                                                                                                                                                              \
//    initializerName::initializerName()                                                                                                                                        \
//    {                                                                                                                                                                         \
//        if (s_counter == 0)                                                                                                                                                   \
//            new (&g_internalDataStructName)internalDataStructName();                                                                                                          \
//                                                                                                                                                                              \
//        s_counter++;                                                                                                                                                          \
//    }                                                                                                                                                                         \
//                                                                                                                                                                              \
//    initializerName::~initializerName()                                                                                                                                       \
//    {                                                                                                                                                                         \
//        s_counter--;                                                                                                                                                          \
//                                                                                                                                                                              \
//        if (s_counter == 0)                                                                                                                                                   \
//            g_internalDataStructName.~internalDataStructName();                                                                                                               \
//    }                                                                                                                                                                         \
//}
//
//#endif // CEDAR_NIFTY_COUNTER_H