# Brainiac
_A Brainfuck interpreter written in C._  
__Copyright &copy; 2023 Chris Roberts__ (Krobbizoid).

# Contents
1. [About](#about)
2. [License](#license)

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

# License
Brainiac is released under the MIT License:  
https://krobbi.github.io/license/2023/mit.txt

See [license.txt](./license.txt) for a full copy of the license text.
