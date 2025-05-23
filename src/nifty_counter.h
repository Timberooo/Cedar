//
// Macros for implementing the nifty counter idiom.
// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Nifty_Counter
//

#ifndef CEDAR_NIFTY_COUNTER_H
#define CEDAR_NIFTY_COUNTER_H

#include <cstddef>
#include <new>
#include <type_traits>



#define CEDAR_NIFTY_COUNTER_PUBLIC(counterName) \
class counterName                               \
{                                               \
public:                                         \
    counterName();                              \
    ~counterName();                             \
private:                                        \
    static std::size_t s_counter;               \
};                                              \
                                                \
static counterName g_##counterName;



#define CEDAR_NIFTY_COUNTER_INTERNAL(internalStructName, counterName, counterNamespace)                                               \
namespace                                                                                                                             \
{                                                                                                                                     \
    static typename std::aligned_storage<sizeof(internalStructName), alignof(internalStructName)>::type g_##internalStructNameBuffer; \
    internalStructName& g_##internalStructName = reinterpret_cast<internalStructName&>(g_##internalStructNameBuffer);                 \
}                                                                                                                                     \
                                                                                                                                      \
namespace counterNamespace                                                                                                            \
{                                                                                                                                     \
    std::size_t counterName::s_counter = 0;                                                                                           \
                                                                                                                                      \
    counterName::counterName()                                                                                                        \
    {                                                                                                                                 \
        if (s_counter == 0)                                                                                                           \
            new (&g_##internalStructName)internalStructName();                                                                        \
                                                                                                                                      \
        s_counter++;                                                                                                                  \
    }                                                                                                                                 \
                                                                                                                                      \
    counterName::~counterName()                                                                                                       \
    {                                                                                                                                 \
        s_counter--;                                                                                                                  \
                                                                                                                                      \
        if (s_counter == 0)                                                                                                           \
            g_##internalStructName.~internalStructName();                                                                             \
    }                                                                                                                                 \
}

#endif // CEDAR_NIFTY_COUNTER_H