# Brainiac
_A heavy-duty optimizing Brainfuck interpreter written in C._  
__Copyright &copy; 2023 Chris Roberts__ (Krobbizoid).

# Contents
1. [About](#about)
2. [Usage](#usage)
3. [Building](#building)
4. [Credits and License](#credits-and-license)

# About
Brainiac is a [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) interpreter
written in C. It was initially written as a refresher for C and low-ish level
concepts, but soon grew into a more sophisticated implementation.

Brainiac aims to implement some
[optimizations](https://nayuki.io/page/optimizing-brainfuck-compiler) by
parsing and optimizing an
[abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) and
generating executable bytecode. This approach closely mirrors a typical
compiler, but interpreting the source code directly is also feasible for such a
simple language.

# Usage
Run Brainiac from the command line with a path to a Brainfuck source file to
interpret it:
```bash
brainiac test/hello.bf
```
> Hello, world!

Run Brainiac from the command line without any arguments to enter REPL mode:
```bash
brainiac
```
> Brainiac REPL - Enter Brainfuck code or 'exit' to exit:
>
> bf: (prompt for input)

Any input starting with `exit` will exit the REPL. Entering `Ctrl+D` or
`Ctrl+Z` on Windows can also be used to exit.

Pressing `Ctrl+C` will exit the program in interpreter mode or REPL mode. This
is useful if a program enters an infinite loop.

# Building
Brainiac is built using [GCC](https://gnu.org/software/gcc) and
[Make](https://gnu.org/software/make):
```bash
make
```

The source files in `src/` will produce the binary file `bin/brainiac` or
`bin/brainiac.exe` on Windows, along with intermediate object files. These
object files are not necessary for the binary to run.

Make can be used to build Brainiac in debug mode:
```bash
make debug
```

In debug mode the `BRAINIAC_DEBUG` macro is defined, which causes a program's
AST and bytecode to be printed after successful parsing. The binary is also
optimized for use with debugging software.

Make can also be used to remove the `bin/` directory:
```bash
make clean
```

This subcommand requires `rm` from
[Coreutils](https://gnu.org/software/coreutils).

# Credits and License
Hello world program at `test/hello.bf` by Khalid Sabry:  
https://codegolf.stackexchange.com/questions/55422/hello-world/163590#163590

Mandelbrot set fractal viewer at `test/mandelbrot.bf` by Erik Bosman:  
https://esoteric.sange.fi/brainfuck/utils/mandelbrot

Brainiac is released under the MIT License:  
https://krobbi.github.io/license/2023/mit.txt

See [license.txt](./license.txt) for a full copy of the license text.
