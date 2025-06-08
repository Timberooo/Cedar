#ifndef CEDAR_CALLBACK_H
#define CEDAR_CALLBACK_H

#include <exception>
#include <type_traits>



namespace Cedar
{
    template <typename TFunction>
    class Callback;
    


    template <typename TReturn, typename... TArgs>
    class Callback<TReturn (*)(TArgs...)>
    {
    public:

        typedef TReturn (*Function)(TArgs...);

        static constexpr bool ReturnsVoid = std::is_same<TReturn, void>::value;


        inline bool canCall() const;


        TReturn call(TArgs... args);
        

        template <typename T = TReturn, typename = typename std::enable_if<ReturnsVoid, T>::type>
        bool tryCall(TArgs... args)
        {
            bool called = canCall();

            if (called)
                m_function(args...);

            return called;
        }

        template <typename T = TReturn, typename = typename std::enable_if<!ReturnsVoid, T>::type>
        bool tryCall(T& returnVal, TArgs... args)
        {
            bool called = canCall();

            if (called)
                returnVal = m_function(args...);

            return called;
        }


        inline Function get() const;

        inline void set(Function function);

    private:

        Function m_function = nullptr;
    };



    template <typename TReturn, typename... TArgs>
    inline bool Callback<TReturn (*)(TArgs...)>::canCall() const {
        return m_function != nullptr;
    }



    template <typename TReturn, typename... TArgs>
    TReturn Callback<TReturn (*)(TArgs...)>::call(TArgs... args)
    {
        if (canCall())
            return m_function(args...);
        else
            throw std::exception();
    }



    template <typename TReturn, typename... TArgs>
    inline Callback<TReturn (*)(TArgs...)>::Function Callback<TReturn (*)(TArgs...)>::get() const {
        return m_function;
    }



    template <typename TReturn, typename... TArgs>
    inline void Callback<TReturn (*)(TArgs...)>::set(Function function) {
        m_function = function;
    }
}

#endif // CEDAR_CALLBACK_H