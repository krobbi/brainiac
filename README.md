# Brainiac
Brainiac is an optimizing [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck)
interpreter written in C. It was initially written as a refresher for C and
low-ish level concepts, but soon grew into a more sophisticated implementation.

See the [`rust` branch](https://github.com/krobbi/brainiac/tree/rust) for a
newer implementation written in Rust.

Brainiac aims to implement some
[optimizations](https://nayuki.io/page/optimizing-brainfuck-compiler) by
parsing and optimizing an
[abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) and
generating executable bytecode. This approach closely mirrors a typical
compiler, but interpreting the source code directly is also feasible for such a
simple language.

## Usage
Brainiac is run from the command line. Include a path to a source file to
interpret it:
```shell
brainiac hello.bf
```
> Hello, world!

If no source file path is provided, then Brainiac will run in REPL mode:
```shell
brainiac
```
> Brainiac REPL - Enter Brainfuck code or 'exit' to exit:
>
> bf: (prompt for input)

Any input starting with `exit` will exit the REPL. Entering `Ctrl+D`
(`Ctrl+Z` on Windows) can also be used to exit.

Pressing `Ctrl+C` will exit the program in interpreter mode or REPL mode. This
is useful if a program enters an infinite loop.

## Building
Brainiac is built using [GCC](https://gnu.org/software/gcc/) and
[Make](https://gnu.org/software/make/):
```shell
make
```

The source files in `src/` will produce the binary file `bin/brainiac`
(`bin/brainiac.exe` on Windows) and intermediate object files. These object
files are not necessary to run the binary.

Make can be used to build Brainiac in debug mode:
```shell
make debug
```

In debug mode the `BRAINIAC_DEBUG` macro is defined, which causes a program's
AST and bytecode to be printed after successful parsing. The binary is also
optimized for use with debugging software.

Make can also be used to remove the `bin/` directory:
```shell
make clean
```

This subcommand requires the POSIX `rm` command, which is not available by
default on Windows.
