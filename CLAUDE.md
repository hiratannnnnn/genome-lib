# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

A C library implementing the **Labarre (2018) ApproximateSBPBI** — a 2-approximation algorithm for Sorting by Prefix Block Interchanges (SBPBI). The project is transitioning toward a general-purpose genome rearrangement analysis library with constraint-based algorithm selection.

## Build Commands

```bash
# Build the library → dist/libgenome.a
make

# Build SBPBI tools (check, all_check, make_input, benchmark)
make sbpbi

# Compile a custom program against the library
make compile SRC=file.c [OUT=output_name]
```

## Test Commands

```bash
cd tests && make run    # Compile and run all tests
cd tests && make clean  # Remove test artifacts
```

Tests use a custom C framework (`tests/test_framework.h`) with global counters `g_passed` / `g_failed`. All randomized tests use seed `42` for reproducibility.

## SBPBI Tools (after `make sbpbi`)

```bash
make run-check ARGS="5 3 2 4 1"   # Run algorithm on a single permutation
make run-all_check N=7             # Exhaustive check for all permutations of size N
make gen N=10 && make check-input  # Generate random permutation and run check
make run-benchmark N=100 T=1000    # Benchmark with statistics
```

## Architecture

```
genome-lib/
├── srcs/
│   ├── matrix/      # Matrix operations
│   ├── graph/       # Graph generation, analysis, and conversion
│   ├── permutation/ # Permutation operations and breakpoint graph
│   ├── io/          # File I/O (read/write/GNL)
│   └── utils/       # Utilities, memory, sorting, strings
├── objs/            # Compiled object files (mirrors srcs/ structure)
├── sbpbi/           # SBPBI algorithm tool implementations
├── includes/        # Public headers
│   ├── lib.h        # Umbrella header (include this to get everything)
│   ├── types.h      # Struct definitions (Edge, Vertex, Node, t_bp_graph)
│   ├── matrix.h     # Matrix operations
│   ├── graph.h      # Graph gen + analysis + conversion
│   ├── permutation.h# Permutation operations
│   ├── io.h         # I/O + strings
│   ├── sort.h       # Comparators and sorting
│   ├── utils.h      # Edge/vertex/node/array/print utilities
│   ├── memory.h     # xmalloc, memory_dump, global externs
│   ├── approx_sbpbi.h # SBPBI algorithm context and functions
│   └── get_next_line.h
├── tests/           # Test suite (12+ modules)
├── dist/            # Output: libgenome.a
└── docs/            # Design docs, roadmap, algorithm notes
```

The library compiles to `dist/libgenome.a`. Object files go to `objs/<subdir>/`.

| srcs subdir | Header | Role |
|-------------|--------|------|
| `matrix/` | `matrix.h` | Integer/double matrix gen and operations |
| `graph/` | `graph.h` | Random graph gen, structural analysis, format conversion |
| `permutation/` | `permutation.h` | Identity, composition, breakpoints, cycle detection, breakpoint graph |
| `io/` | `io.h` | Read/write matrix files, GNL, ft_split/ft_strtrim |
| `utils/` (sort) | `sort.h` | Comparators and list sorting |
| `utils/` (mem) | `memory.h` | xmalloc/xfree, memory_dump, global mem counters |
| `utils/` (rest) | `utils.h` | Edge/vertex/node ops, array/basic/char/free/math/print |

## Code Conventions

- **Memory**: Use `xmalloc`/`xfree` exclusively — never mix with raw `malloc`/`free`. Global `mem` tracks current usage; `mem_peak` tracks the peak.
- **Output**: Use `printf` only. Do not use `puts` or `write`.
- **Compiler flags**: `-Wall -Wextra -Werror -Iincludes` — all warnings are errors.
- **Math**: Link with `-lm` when using math functions.

## Known Issues (see `docs/roadmap.md`)

- `gen_rand_tree`: crashes on `n < 3`
- `gen_rand_bigraph`: heap-buffer-overflow when `a ≠ b`
- Mixed `malloc`/`xmalloc` usage in some files
- `cmp_double_*` functions return inconsistent values
- Naming inconsistency: `len_edge_list` vs `count_*` style

## Docs

- `docs/overview.md` — module inventory, global variables, design promises
- `docs/sbpbi_algorithm.md` — SBPBI algorithm details and cycle decomposition
- `docs/data_structures.md` — struct definitions and memory design
- `docs/patterns.md` — future constraint system for genome algorithm selection
- `docs/roadmap.md` — bug list and refactoring targets
