# Brainfuck by FERcsI

A simple,  minimal brainfuck  interpreter with  debugging functionality.
The  brainfuck language  was  not perfectly  specified  by the  original
author, so different  versions are circulating on the  net. This version
is  compatible  with the  first  implementation.  It has  the  following
variable properties:

- cell type: 8 bit unsigned integer initialized to 0
- data pointer: no negative values allowed, initialized to 0
- cell array size: 30.000
- input stream: stdin or file
- output stream: stdout

## Requirements

- No  special prescription  on OS, but  I tested it  only in  Debian and
  Ubuntu Linux.
- For compilation,  you  will  need  g++ with  c++17  support. For  more
  information, se [configuration](#configuration) below.

## Installation

Installation requires the following two simple instructions.

```
make
sudo make install
```

To run tests:

```
make test
```

Note   that   the   interpreter   is  installed   into   the   directory
`/usr/local/bin/`, so  if you  would like  to use  shebang, you  have to
start your brainfuck script as follows:

```brainfuck
#!/usr/local/bin/brainfuck

++++++[->++++++<].
```

## Debugging

For debugging there are some features:

- Commenting unnecessary code sniplets
- Dumping a part of the cell array
- Immediate exit

To  turn  on  debugging  mode,  call  brainfuck  with  option  `-d`  (or
`--debug`). You can add that to the shebang, too.

```brainfuck
#!/usr/local/bin/brainfuck -d

++++++[->++++++<]@.
```

### Comments

In debug mode,  comments start with `#`  and end with the  new line (LF,
0x0a) character.  brainfuck instructions in comments  are eliminated. So
the following code prints a `$` in debug mode and a `#` if debug mode is
off.

```brainfuck
++++++[->++++++<] #-
.
```

### Memory Dump

With the additional  instruction `@` the content of the  first _N_ cells
can be printed (to stderr):

```
[ 00 23 35 2f c1 00 01... ]
```

The number  of cells  to print  can be  given with  the option  `-s` (or
`--dump-size`). The default is 32.

Also here the current cell is highlighted.

### Immediate exit

With the additional instruction `$` the interpreter stops processing the
commands and exits with message to stderr:

```
[exit]
```

## Configuration

Currently, I do not plan to  change this project to be configurable, but
later, who knows.

However, if you would like to change some parameters:

- Cell   array   size:  In   `brainfuck.cpp`,   change   the  value   of
  `CELL_ARRAY_SIZE`.
- Negative direction: Double the array  size and set `START_CELL_POS` in
  the middle of the array.
- Cell  type: Change  the type  of  `CellType`. Note  that using  signed
  integers instead of unsigned, doesn't make any difference.
- Default dump size: Change `DEFAULT_DUMP_SIZE`

And the installation option:

- Compiler: If you would like to use a different compiler, simply use it
  instead of running  `make`. This is a single-file project  and it does
  not need any special sources
- Installation  directory:  Simply  copy   `brainfuck`  to  the  desired
  directory after running `make`.

## Using vim syntax file

If you are using vim, you can  Copy the files in vim directory into your
vim config environment (`$HOME/.vim/`, `/usr/share/vim/addons/`...).
