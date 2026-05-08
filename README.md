# Encoder

This encoder converts simple assembly code into machine code.  
Designed by [Amir](https://github.com/Amirht84) — collaborations are welcome.

## How to Use

- Clone the whole or download the whole zip file, containing configuration, lib, and src.
- Then **build** the excecutable file by excecuting `Makefile` written in the root.
- To assemble your code, pipe the output of cat to encoder.out and redirect the result to a file. (e.g., `cat Assembly.txt | ./encoder.out > MachineCode.txt`), better user interfaces will be developed soon...

## Features

- Tokens can be separated by `,`, `space`, or `tab` — one or more characters are supported.
- Registers can be referred to by name or number; numbers must be prefixed with `x` (e.g., `x0` for `zero`).
- Immediate values can be written in signed decimal or hexadecimal format.
- Syntax errors are detected — for example, immediate overflow or invalid token names.
- UTF-8 or CLRF formats are acceptable for configuration file.

> ## NOTICE
>
> - The `Makefile` is designed **only for Linux**. If you want to use this encoder on Windows, you will need to build it manually. :>
> - The encoder only supports instructions listed in `opcode_table.csv`. To extend functionality, update the CSV file.
> - For base+offset instructions like `sw s0, -6(s3)`, the offset+parentheses+base-register **must** be written **without any spaces, commas, or tabs** (e.g., `-6(s3)`). Otherwise, the encoder will fail to parse the input.
> - This tool is **not suitable** for complex assembly code with pseudo-instructions or labels. It only encodes instructions line by line.
