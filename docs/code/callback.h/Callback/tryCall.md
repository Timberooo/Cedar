# Cedar::Callback<TReturn (*)(TArgs...)>::tryCall

Attempts to call the function associated with the callback.

## Overloads

| Overloads | Description |
| --- | --- |
| [bool tryCall(TArgs... args)](#bool-trycalltargs-args) | Attempts to call the callback function. Only exists for functions that return `void`. |
| [bool tryCall(T& returnVal, TArgs... args)](#bool-trycallt-returnval-targs-args) | Attempts to call the callback function. Only exists for functions that do not return `void`. |

### bool tryCall(TArgs... args)

``` c++
template <typename T = TReturn, typename = typename std::enable_if<returnsVoid, T>::type>
bool tryCall(TArgs... args);
```

Attempts to call the callback function. Only exists when `TReturn` is `void`.

#### Parameters

`args` - The callback function's arguments.

#### Returns

A `bool` indicating if the function was called successfully or not.

### bool tryCall(T& returnVal, TArgs... args)

``` c++
template <typename T = TReturn, typename = typename std::enable_if<!returnsVoid, T>::type>
bool tryCall(T& returnVal, TArgs... args);
```

Attempts to call the callback function. Only exists when `TReturn` is not `void`.

#### Template Parameters

`T` - The same type as `TReturn`.

#### Parameters

`returnVal` - The value returned by the callback function. This value is unchanged if the callback fails.

`args` - The callback function's arguments.

#### Returns

A `bool` indicating if the function was called successfully or not.

## Class

[Cedar::Callback](/docs/code/callback.h/Callback.md)

## Header

[callback.h](/docs/code/callback.h.md)
