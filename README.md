# Virtual Machine

This project implements a stack-based virtual machine in C. It executes a
bytecode instruction set for integer and floating-point arithmetic, comparison,
control flow, functions, local and global variables, strings, arrays, and
events. The included test runner exercises the VM through unit-style
instruction tests.

## Related projects

- [VM-CLI](https://github.com/littleEndianOne/VM-CLI) provides a command-line
  interface for running this virtual machine.
- [Assembler](https://github.com/littleEndianOne/Assembler) is a TypeScript
  assembler that produces programs for this virtual machine.

## Project layout

| Path | Contents |
| --- | --- |
| `VM/` | Core VM types, CPU loop, opcode definitions, and memory, stack, string, array, and event operations. |
| `VM Utility/` | VM state and stack-printing helpers. |
| `src/` | Test-runner entry point, inline-function examples, and the MinUnit-based test macros. |
| `Test Utility/` | Helpers for creating VM instances and providing test program memory. |
| `Tests/` | Instruction and error-handling test sets executed by the test runner. |
| `include/` | Compatibility headers used by the GCC build. |

## Build and test

Requirements:

- GCC with C11 support
- GNU Make
- POSIX threads support

Build the test runner:

```sh
make
```

This produces `build/vm-tests`. Run the full test suite with:

```sh
make test
```

The test runner reports individual results and summary metrics. It exits with a
nonzero status when one or more tests fail, so the target can be used in CI.

Remove generated files with:

```sh
make clean
```
