# Wurd - A CLI Text Editor

## Introduction

Wurd is a command-line based text editor that allows to edit text documents of virtually any size. 

## Features

- Wurd supports all basic capabilities of a text editor, such as insertion, deletion, file I/O, undo (supports batched undo).
- Additionally, Wurd comes with real-time spell-checking with spelling suggestions.
  - Misspelled words are highlighted in red by default, and a list of suggested words show up when the cursor hovers over a misspelled word.
- The built-in dictionary contains about 110,000 words. Users could also load dictionaries of their choice.

## Configuration

Default dictionary path, foreground/background color, and highlight color could be configured in lines 9-12 of `main.cpp`:

```c++
const char* DICTIONARYPATH = "dictionary.txt";
const int FOREGROUND_COLOR = COLOR_WHITE;
const int BACKGROUND_COLOR = COLOR_BLACK;
const int HIGHLIGHT_COLOR  = COLOR_GREEN;
// Choices are COLOR_x, where x is WHITE, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN
```

## How to Run

You can run the command

```sh
wurd
```

to start editing with an empty file, or

```sh
wurd someFileName
```

to start editing the named file.

In either case, if in the same directory the wurd program there is a file named dictionary.txt, it will be used as the dictionary.

Some text files to edit of various sizes are provided as samples: `civildis.txt` (1KB), `threemen.txt` (382KB), and `warandpeace.txt` (3.3MB). If you run wurd from the command line, you can give the file to edit as an argument. Otherwise, you can load the file with the Ctrl-L command in the editor.

## Key bindings

- Arrow keys - move the cursor
- Home - move to front of line
- End - move to end of line
- Page up - previous page
- Page down - next page
- Delete/backspace - delete/backspace
- Enter - line break
- CTRL-S - save the current changes
- CTRL-L - load a new file
- CTRL-Z - undo last change
- CTRL-D - load a new dictionary
- CTRL-X - quit
