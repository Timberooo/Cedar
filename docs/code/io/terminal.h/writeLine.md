# Cedar::Terminal::write

Writes a UTF-8 encoded string and newline character to the terminal associated with the Cedar engine.

## Syntax

``` C++
void writeLine(std::string_view str);
```

Writes a string and a newline character to the terminal using the terminal's default colors.

``` C++
void writeLine(std::string_view str, Color foregroundColor, Color backgroundColor);
```

Writes a string and a newline character to the terminal using the given foreground and background colors.

``` C++
void writeLine(char character);
```

Writes a character and a newline character to the terminal using the terminal's default colors.

``` C++
void writeLine(char character, Color foregroundColor, Color backgroundColor);
```

Writes a character and a newline character to the terminal using the given foreground and background colors.

## Parameters

**str** - The string to write to the terminal.

**character** - The character to write to the terminal.

**foregroundColor** - The foreground (text) color to use.

**backgroundColor** - The background color to use.

## Header

[io/terminal.h](/docs/code/io/terminal.h.md)

## See also

[write](/docs/code/io/terminal.h/write.md)
