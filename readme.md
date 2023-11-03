# Brainiac
_A Brainfuck interpreter written in C._  
__Copyright &copy; 2023 Chris Roberts__ (Krobbizoid).

# Contents
1. [About](#about)
2. [Usage](#usage)
3. [Building](#building)
4. [Credits and License](#credits-and-license)

# About
Brainiac is a [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) interpreter
written in C. This is mostly a refresher for C and low level concepts.

I chose Brainfuck because it is simple, but not trivial. It can produce some
interesting output, and leaves some room for expansion and optimization.
Interpreting rather than compiling is simpler and safer, and gives me a VM to
try out a computed goto dispatch table with.

Brainfuck implementations have already been created many times before. Many of
which are probably faster than Brainiac due to some clever
[optimizations](https://www.nayuki.io/page/optimizing-brainfuck-compiler).

# Usage
Run Brainiac from the command line with a path to a Brainfuck source file:
```bash
brainiac <path>
```

# Building
Brainiac is built using [GCC](https://gnu.org/software/gcc) and
[Make](https://gnu.org/software/make):
```bash
make
```

The source files in `src/` will produce the binary file `bin/brainiac` or
`bin/brainiac.exe` on Windows, along with intermediate object files. These
object files are not necessary for the binary to run.

Make can be used to remove the `bin/` directory:
```bash
make clean
```

This subcommand requires `rm` from
[Coreutils](https://gnu.org/software/coreutils).

Make can also be used to run `test/mandelbrot.bf` after building:
```bash
make test
```

# Credits and License
Mandelbrot set fractal viewer at `test/mandelbrot.bf` by Erik Bosman:  
https://esoteric.sange.fi/brainfuck/utils/mandelbrot

Brainiac is released under the MIT License:  
https://krobbi.github.io/license/2023/mit.txt

See [license.txt](./license.txt) for a full copy of the license text.
