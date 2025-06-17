# Cedar::Callback

``` c++
template <typename TFunction>
class Callback;

template <typename TReturn, typename... TArgs>
class Callback<TReturn (*)(TArgs...)>;
```

Contains a function pointer to use as a callback function.

#### Template Parameters

`TFunction` - The function pointer type.

`TReturn` - The function pointer's return type.

`TArgs...` - The function's argument types.

> [!NOTE]
> The first template declaration is a primary template and the second declaration is a partial template specialization for function pointers. The primary template does not have a definition, but both are necessary to enable the use of function pointers, their argument types, and their return types.

## Members

### Types

| Type | Description |
| --- | --- |
| [Function](Callback/Function.md) | The function pointer type. |

### Variables

| Variable | Description |
| --- | --- |
| [static constexpr bool returnsVoid](Callback/returnsVoid.md) | Indicates whether or not `TReturn` is `void`. |

### Constructors

| Constructor | Description |
| --- | --- |
| [Callback()](Callback/constructor.md#callback) | Initializes the callback function to `nullptr`. |
| [Callback(Function function)](Callback/constructor.md#callbackfunction-function) | Initializes the callback function using the given function pointer. |

### Operators

| Operator | Description |
| --- | --- |
| [operator Function() const](Callback/Function.md) | Returns the value of the callback pointer. |

### Functions

| Function | Description |
| --- | --- |
| [TReturn call(TArgs... args)](Callback/call.md) | Calls the callback function. |
| [bool canCall() const](Callback/canCall.md) | Checks if the callback function can be called. |
| [bool tryCall(TArgs... args)](Callback/tryCall.md#bool-trycalltargs-args) | Attempts to call the callback function. Only available when `TReturn` is `void`. |
| [bool tryCall(T& returnVal, TArgs... args)](Callback/tryCall.md#bool-trycallt-returnval-targs-args) | Attempts to call the callback function. Only available when `TReturn` is not `void`. |

## Header

[callback.h](../callback_h.md)
