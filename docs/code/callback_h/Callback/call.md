# Cedar::Callback<TReturn (*)(TArgs...)>::call

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

## Class

[Cedar::Callback](../Callback.md)
