# Cedar::Callback

Contains a function pointer to use as a callback function.

``` c++
template <typename TFunction>
class Callback;

template <typename TReturn, typename... TArgs>
class Callback<TReturn (*)(TArgs...)>;
```

> [!NOTE]
> The first template declaration is a primary template and the second template is a partial template specialization for function pointers. The primary template does not have a definition, but both are necessary to enable the use of function pointers, their argument types, and their return types.

## Members

### Constructors

| Constructor | Description |
| --- | --- |
| [Callback()](/docs/code/callback.h/Callback/constructor.md) | Initializes the callback function to `nullptr`. |
| [Callback(Function function)](/docs/code/callback.h/Callback/constructor.md) | Initializes the callback function to the given function pointer. |

### Functions

| Function | Description |
| --- | --- |
| [TReturn call(TArgs... args)](/docs/code/callback.h/Callback/call.md) | Calls the callback function. |
| [bool canCall() const](/docs/code/callback.h/Callback/canCall.md) | Checks if the callback function can be called. |
| [bool tryCall(TArgs... args)](/docs/code/callback.h/Callback/tryCall.md#bool-trycalltargs-args) | Attempts to call the callback function. Only exists for callbacks that return `void`. |
| [bool tryCall(T& returnVal, TArgs... args)](/docs/code/callback.h/Callback/tryCall.md#bool-trycallt-returnval-targs-args) | Attempts to call the callback function. Only exists for callbacks that do not return `void`. |

## Header

[callback.h](/docs/code/callback.h.md)
