# Cedar::Callback<TReturn (*)(TArgs...)>::Callback

Initializes the callback function.

## Overloads

| Overload | Description |
| --- | --- |
| [Callback()](#callback) | Initializes the callback function to `nullptr`. |
| [Callback(Function function)](#callbackfunction-function) | Initializes the callback function using the given function pointer. |

### Callback()

``` c++
Callback();
```

Initializes the callback function to `nullptr`.

### Callback(Function function)

``` c++
Callback(Function function);
```

Initializes the callback function using the given function pointer. The function pointer type must match the [Function](Function.md) type.

## Class

[Cedar::Callback](../Callback.md)
