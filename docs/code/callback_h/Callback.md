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

### Typedefs

| Typedef | Description |
| --- | --- |
| [TReturn (*Function)(TArgs...)](Callback/Function.md) | The function pointer type. |

### Variables

| Variable | Description |
| --- | --- |
| [static constexpr bool returnsVoid](Callback/returnsVoid.md) | Indicates whether or not `TReturn` is `void`. |

### Constructors

| Constructor | Description |
| --- | --- |
| [Callback()]() | Initializes the callback function to `nullptr`. |
| [Callback(Function function)]() | Initializes the callback function using the given function pointer. |

### Operators

| Operator | Description |
| --- | --- |
| [operator Function() const]() | Returns the value of the callback pointer. |

### Functions

| Function | Description |
| --- | --- |
| [TReturn call(TArgs... args)](Callback/call.md) | Calls the callback function. |
| [bool canCall() const](Callback/canCall.md) | Checks if the callback function can be called. |
| [bool tryCall(TArgs... args)]() | Attempts to call the callback function. Only available when `TReturn` is `void`. |
| [bool tryCall(T& returnVal, TArgs... args)]() | Attempts to call the callback function. Only available when `TReturn` is not `void`. |

## Header

[callback.h](../callback_h.md)
