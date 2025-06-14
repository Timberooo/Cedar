# io/terminal.h

Contains functions for writing to and controlling the terminal associated with the Cedar engine.

These functions should be preferred over C++ STL standard output streams, C standard output functions, and platform-specific output functions. All strings used in the terminal are assumed to be in the UTF-8 character encoding.

## Enums

| Enum | Description |
| --- | --- |
| [Color](/docs/code/io/terminal.h/Color.md) | Terminal output colors. |

## Functions

| Function | Description |
| --- | --- |
| [enable](/docs/code/io/terminal.h/enable.md) | Creates or terminates a terminal. |
| [enabled](/docs/code/io/terminal.h/enabled.md) | Checks if a terminal is present to output to. |
| [write](/docs/code/io/terminal.h/write.md) | Writes an optionally colored string to the terminal. |
| [writeLine](/docs/code/io/terminal.h/writeLine.md) | Writes an optionally colored string to the terminal followed by a newline character. |
