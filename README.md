# LLVM-based Circuit Compilation for Practical Secure Computation

This project is a proof-of-concept implementation to generate Boolean circuits optimized for MPC using LLVM. It was developed as part of our [ACNS'21](https://sulab-sever.u-aizu.ac.jp/ACNS2021/) paper [LLVM-based Circuit Compilation for Practical Secure Computation](https://encrypto.de/papers/HSTWY21.pdf).

In the following, we briefly describe how to set up the toolchain and how to reproduce our evaluation results. Note that due to an update from LLVM 8 to LLVM 12 after publication, circuit sizes slightly differ from the results reported in the paper.

## Requirements

Everything was tested under Debian 10 "Buster". The following packages must be installed via `apt install`:
- `bison`
- `cmake`
- `clang`
- `flex`
- `gcc`
- `g++`
- `git`
- `libwww-perl`
- `openjdk-11-jdk`
- `python3`
- `yosys`

## Usage

Everything is controlled via the central `Makefile`.

1. `make init` initializes the git submodules and adds our custom library to LLVM 12.
2. `make compile` compiles LLVM (including our CircuitCompilation library) and HyCC.
3. `make benchmark` runs circuit compilation for all our benchmarks using LLVM and HyCC, including post-processing for circuits generated with LLVM. The results can be found in the `Benchmark` folder.
