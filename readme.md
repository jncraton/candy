Candy
=====

Candy is a simple programming language that adds some syntactic sugar
to the C language. Programs written in candy are translated to C and 
then can be compiled using a traditional C compiler such as GCC. The
candy translator is written in candy.

Features
-------

The features of the language revolve around creating a more Pythonic
syntax while keeping a tight one-to-one mapping to the C language. It
should be obvious what C code will be generated when writing a
program using candy. It just saves some braces and semicolons and 
looks a little more like Python.

- Pythonic indentation-based blocks
- Optional parens around if statements
- Optional parens for a function passed a single contstant string (e.g. `printf "hello, world"` is valid)
- Semicolons are optional at the end of a line
- Supports "and", "or", and "not" as operators
- `import` rather than `#include`

Usage
-----

The translator is so simple that it doesn't require reading and
writing to files. It reads a program from stdio and writes C to
stdout. The following will translate a program from candy to C:

`candy < in.cn > out.c`

There are currently no option to pass to the translator.