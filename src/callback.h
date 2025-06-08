#ifndef CEDAR_CALLBACK_H
#define CEDAR_CALLBACK_H

#include <exception>



namespace Cedar
{
    template <typename TFunction>
    class Callback;



    template <typename TReturn, typename... TArgs>
    class Callback<TReturn (*)(TArgs...)>
    {
    public:

        typedef TReturn (*Function)(TArgs...);


        TReturn call(TArgs... args);

        bool tryCall(TReturn& returnVal, TArgs... args);


        inline bool canCall() const;


        inline Function get() const;

        inline void set(Function function);

    private:

        Function m_function = nullptr;
    };



    template <typename TReturn, typename... TArgs>
    TReturn Callback<TReturn (*)(TArgs...)>::call(TArgs... args)
    {
        if (canCall())
            return m_function(args...);
        else
            throw std::exception();
    }



    template <typename TReturn, typename... TArgs>
    bool Callback<TReturn (*)(TArgs...)>::tryCall(TReturn& returnVal, TArgs... args)
    {
        bool success = canCall();

        if (success)
            returnVal = m_function(args...);
        
        return success;
    }



    template <typename TReturn, typename... TArgs>
    inline bool Callback<TReturn (*)(TArgs...)>::canCall() const {
        return m_function != nullptr;
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