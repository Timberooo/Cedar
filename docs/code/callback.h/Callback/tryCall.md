# Cedar::Callback::tryCall

Attempts to call the function associated with the callback.

## Overloads

| Overloads | Description |
| --- | --- |
| [bool tryCall(TArgs... args)](#bool-trycalltargs-args) | Attempts to call the callback function. Only exists for functions that return `void`. |
| [bool tryCall(T& returnVal, TArgs... args)](#bool-trycallt-returnval-targs-args) | Attempts to call the callback function. Only exists for functions that do not return `void`. |

### bool tryCall(TArgs... args)

``` c++
bool tryCall(TArgs... args);
```

Attempts to call the callback function. Only exists for functions that return `void`.

#### Parameters

`args` - The callback function's arguments.

#### Returns

A `bool` indicating if the function was called successfully (true) or not (false).

### bool tryCall(T& returnVal, TArgs... args)

``` c++
bool tryCall(T& returnVal, TArgs... args);
```

Attempts to call the callback function. Only exists for functions that do not return `void`.

#### Parameters

`returnVal` - The value returned by the callback function. This value is unchanged if no callback function is set.

`args` - The callback function's arguments.

#### Returns

A `bool` indicating if the function was called successfully (true) or not (false).

## Class

[Cedar::Callback](/docs/code/callback.h/Callback.md)

## Header

[callback.h](/docs/code/callback.h.md)
