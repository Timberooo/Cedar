# Cedar::Callback<TReturn (*)(TArgs...)>::tryCall

**Type:** Function\
**Access:** Public

Attempts to call the callback function.

## Overloads

| Overload | Description |
| --- | --- |
| [bool tryCall(TArgs... args)](#bool-trycalltargs-args) | Attempts to call the callback function. Only available when `TReturn` is `void`. |
| [bool tryCall(T& returnVal, TArgs... args)](#bool-trycallt-returnval-targs-args) | Attempts to call the callback function. Only available when `TReturn` is not `void`. |

### bool tryCall(TArgs... args)

``` c++
template <typename T = TReturn, typename = typename std::enable_if<returnsVoid, T>::type>
bool tryCall(TArgs... args);
```

Attempts to call the callback function. Only available when `TReturn` is `void`.

#### Parameters

`args` - The arguments to pass to the callback function.

#### Returns

`true` if the callback function was successfully called; otherwise `false`.

### bool tryCall(T& returnVal, TArgs... args)

``` c++
template <typename T = TReturn, typename = typename std::enable_if<!returnsVoid, T>::type>
bool tryCall(T& returnVal, TArgs... args);
```

Attempts to call the callback function. Only available when `TReturn` is not `void`.

#### Template Parameters

`T` - The same type as `TReturn`.

#### Parameters

`returnVal` - The value returned by the callback function. If the callback function is not called this value is unchanged.

`args` - The callback function's arguments.

#### Returns

`true` if the callback function was successfully called; otherwise `false`.

## Class

[Cedar::Callback](../Callback.md)
