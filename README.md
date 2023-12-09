# Matrix Multiplication Program

## Overview
This program performs matrix multiplication using two different parallelization strategies:

1. **Element-wise Thread Computation:** This strategy assigns a thread to compute each element of the output matrix individually.
2. **Row-wise Thread Computation:** This strategy assigns a thread to compute each row of the output matrix.

## Requirements
- **Programming Language:** C/C++ (This program is written in C++)
- **Compiler:** Any C/C++ compiler that supports multi-threading (e.g., GCC, Clang)
- **Operating System:** Compatible with most major operating systems (Windows, Linux, macOS)

## Usage
1. **Compilation:** Compile the program using your preferred C/C++ compiler. For example:
Note: The `-pthread` flag is used for enabling multi-threading in GCC.

2. **Execution:** Run the compiled program with the desired input file that has matrix dimensions and its entries
