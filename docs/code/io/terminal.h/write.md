# Cedar::Terminal::write

Writes a UTF-8 encoded string to the terminal associated with the Cedar engine.

## Syntax

``` C++
void write(std::string_view str);
```

Writes a string to the terminal using the terminal's default colors.

``` C++
void write(std::string_view str, Color foregroundColor, Color backgroundColor);
```

Writes a string to the terminal using the given foreground and background colors.

``` C++
void write(char character);
```

Writes a character to the terminal using the terminal's default colors.

``` C++
void write(char character, Color foregroundColor, Color backgroundColor);
```

Writes a character to the terminal using the given foreground and background colors.

## Parameters

**str** - The string to write to the terminal.

**character** - The character to write to the terminal.

**foregroundColor** - The foreground (text) color to use.

**backgroundColor** - The background color to use.

## Header

[io/terminal.h](/docs/code/io/terminal.h.md)

## See also

[writeLine](/docs/code/io/terminal.h/writeLine.md)
