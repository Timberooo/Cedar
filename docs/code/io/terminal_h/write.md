# Cedar::Terminal::write

**Type:** Function

Writes a UTF-8 encoded string to the terminal associated with the Cedar engine.

## Overloads

| Overload | Description |
| --- | --- |
| [void write(std::string_view str, Color, foregroundColor, Color backgroundColor)](#void-writestdstring_view-str-color-foregroundcolor-color-backgroundcolor) | Writes a string to the terminal using the given foreground and background colors. |
| [void write(char character, Color foregroundColor, Color backgroundColor)](#void-writechar-character-color-foregroundcolor-color-backgroundcolor) | Writes a character to the terminal using the given foreground and background colors. |

### void write(std::string_view str, Color foregroundColor, Color backgroundColor)

``` c++
void write(std::string_view str, Color foregroundColor = Color::Use_Default, Color backgroundColor = Color::Use_Default);
```

Writes a string to the terminal using the given foreground and background colors.

#### Parameters

`str` - The string to write to the terminal.

`foregroundColor` - The foreground (text) color to use. Default value is Color::Use_Default.

`background` - The background color to use. Default value is Color::Use_Default.

### void write(char character, Color foregroundColor, Color backgroundColor)

``` c++
void write(std::string_view str, Color foregroundColor = Color::Use_Default, Color backgroundColor = Color::Use_Default);
```

Writes a character to the terminal using the given foreground and background colors.

#### Parameters

`character` - The character to write to the terminal.

`foregroundColor` - The foreground (text) color to use. Default value is Color::Use_Default.

`background` - The background color to use. Default value is Color::Use_Default.

## Header

[io/terminal.h](../terminal_h.md)
