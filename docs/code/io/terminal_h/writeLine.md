# Cedar::Terminal::writeLine

**Type:** Function

Writes a UTF-8 encoded string to the terminal associated with the Cedar engine and moves the cursor to the beginning of the next line.

## Overloads

| Overload | Description |
| --- | --- |
| [void writeLine(std::string_view str, Color foregroundColor, Color backgroundColor)](#void-writelinestdstring_view-str-color-foregroundcolor-color-backgroundcolor) | Writes a string to the terminal using the given foreground and background colors and moves the cursor to the beginning of the next line. |
| [void writeLine(char character, Color foregroundColor, Color backgroundColor)](#void-writelinechar-character-color-foregroundcolor-color-backgroundcolor) | Writes a character to the terminal using the given foreground and background colors and moves the cursor to the beginning of the next line. |

### void writeLine(std::string_view str, Color foregroundColor, Color backgroundColor)

``` c++
void writeLine(std::string_view str, Color foregroundColor = Color::Use_Default, Color backgroundColor = Color::Use_Default);
```

Writes a string to the terminal using the given foreground and background colors and moves the cursor to the beginning of the next line.

#### Parameters

`str` - The string to write to the terminal.

`foregroundColor` - The foreground (text) color to use. Default value is Color::Use_Default.

`backgroundColor` - The background color to use. Default value is Color::Use_Default.

### void writeLine(char character, Color foregroundColor, Color backgroundColor)

``` c++
void writeLine(char character, Color foregroundColor = Color::Use_Default, Color backgroundColor = Color::Use_Default);
```

Writes a character to the terminal using the given foreground and background colors and moves the cursor to the beginning of the next line.

#### Parameters

`character` - The character to write to the terminal.

`foregroundColor` - The foreground (text) color to use. Default value is Color::Use_Default.

`backgroundColor` - The background color to use. Default value is Color::Use_Default.

## Header

[io/terminal.h](../terminal_h.md)
