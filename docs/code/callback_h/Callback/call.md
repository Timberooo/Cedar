# Cedar::Callback<TReturn (*)(TArgs...)>::call

**Type:** Function\
**Access:** Public

``` c++
TReturn call(TArgs... args);
```

Calls the callback function.

#### Parameters

`args` - The arguments to pass to the callback function.

#### Returns

The value returned by the callback function.

#### Exceptions

If the callback function pointer is set to `nullptr` ([canCall()](canCall.md) returns `false`), throws an `std::logic_error`.

## See also

[bool tryCall(TArgs... args)](tryCall.md)

[bool tryCall(T& returnVal, TArgs... args)](tryCall.md)

## Class

[Cedar::Callback](../Callback.md)
