# io/terminal.h

Functions for writing to the terminal associated with the Cedar engine.

## Incldues

| File | Notes |
| --- | --- |
| \<cstddef> |  |
| \<string_view> |  |

## Enums

| Enum | Description |
| --- | --- |
| [Color](terminal_h/Color.md) | Terminal text and background colors. |

## Functions

| Function | Description |
| --- | --- |
| [void write(std::string_view str, Color foregroundColor, Color backgroundColor)](terminal_h/write.md#void-writestdstring_view-str-color-foregroundcolor-color-backgroundcolor) | Writes a string to the terminal using the given foreground and background colors. |
| [void write(char character, Color foregroundColor, Color backgroundColor)](terminal_h/write.md#void-writechar-character-color-foregroundcolor-color-backgroundcolor) | Writes a character to the terminal using the given foreground and background colors. |
| [void writeLine(std::string_view str, Color foregroundColor, Color backgroundColor)](terminal_h/writeLine.md#void-writelinestdstring_view-str-color-foregroundcolor-color-backgroundcolor) | Writes a string to the terminal using the given foreground and background colors and moves the cursor to the beginning of the next line. |
| [void writeLine(char character, Color foregroundColor, Color backgroundColor)](terminal_h/writeLine.md#void-writelinechar-character-color-foregroundcolor-color-backgroundcolor) | Writes a character to the terminal using the given foreground and background colors and moves the cursor to the beginning of the next line. |
