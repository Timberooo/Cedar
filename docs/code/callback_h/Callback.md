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

| Type | Access | Description |
| --- | --- | --- |
| [Function](Callback/Function.md) | Public | The function pointer type. |

### Constructors

| Constructor | Access | Description |
| --- | --- | --- |
| [Callback()](Callback/constructor.md#callback) | Public | Initializes the callback function to `nullptr`. |
| [Callback(Function function)](Callback/constructor.md#callbackfunction-function) | Public | Initializes the callback function using the given function pointer. |

### Operators

| Operator | Access | Description |
| --- | --- | --- |
| [operator Function() const](Callback/Function.md) | Public | Returns the value of the callback pointer. |

### Functions

| Function | Access | Description |
| --- | --- | --- |
| [TReturn call(TArgs... args)](Callback/call.md) | Public | Calls the callback function. |
| [bool canCall() const](Callback/canCall.md) | Public | Checks if the callback function can be called. |
| [bool tryCall(TArgs... args)](Callback/tryCall.md#bool-trycalltargs-args) | Public | Attempts to call the callback function. Only available when `TReturn` is `void`. |
| [bool tryCall(T& returnVal, TArgs... args)](Callback/tryCall.md#bool-trycallt-returnval-targs-args) | Public | Attempts to call the callback function. Only available when `TReturn` is not `void`. |

## Static Members

### Variables

| Variable | Access | Description |
| --- | --- | --- |
| [constexpr bool returnsVoid](Callback/returnsVoid.md) | Public | Indicates whether or not `TReturn` is `void`. |

## Header

[callback.h](../callback_h.md)
