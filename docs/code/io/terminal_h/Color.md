# Cedar::Terminal::Color

Colors used for writing colored output to the terminal associated with the Cedar engine.

> [!NOTE]
> Not all terminals and terminal emulators display colors the same; for example some terminals display `Yellow` and `Bright_Yellow` as shades of orange.

## Syntax

``` C++
enum class Color;
```

## Values

| Value | Description |
| --- | --- |
| `Use_Default` | Default color for the terminal. |
| `Black` |  |
| `Red` |  |
| `Green` |  |
| `Yellow` |  |
| `Blue` |  |
| `Magenta` |  |
| `Cyan` |  |
| `White` | Also known as light gray. |
| `Bright_Black` | Also known as dark gray. |
| `Bright_Red` |  |
| `Bright_Green` |  |
| `Bright_Yellow` |  |
| `Bright_Blue` |  |
| `Bright_Magenta` |  |
| `Bright_Cyan` |  |
| `Bright_White` |  |

## Header

[io/terminal.h](/docs/code/io/terminal.h.md)

## See also

[write](/docs/code/io/terminal.h/write.md)

[writeLine](/docs/code/io/terminal.h/writeLine.md)
