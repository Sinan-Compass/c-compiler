# C Compiler Course Project

A compiler theory course project that implements a small compiler pipeline for a C-like language subset. The project includes lexical analysis, syntax analysis, AST visualization, semantic analysis, quadruple generation, and simple assembly generation.

## Features

- Lexical analysis from `samples/fbnq.txt`.
- Recursive-descent parsing and AST construction.
- AST visualization as Graphviz DOT output in `outputs/ast7.dot`.
- Semantic analysis and quadruple output in `outputs/quadruple.txt`.
- Simple assembly output in `outputs/assmbly.asm`.

## Structure

```text
.
├── src/                         # Compiler source code
├── samples/                     # Input sample programs
├── outputs/                     # Generated/sample outputs
├── docs/theory/                 # Course notes
├── SemanticAnalysis/            # Earlier semantic-analysis experiment
└── 编译原理课设_语法分析器.sln      # Visual Studio solution
```

## Environment

- Windows
- Visual Studio 2022 / MSVC v143
- C++20
- EasyX/graphics.h if the UI-related code is built

## Build And Run

1. Open `编译原理课设_语法分析器.sln` with Visual Studio.
2. Select `Debug|x64` or `Release|x64`.
3. Build and run the project.
4. The default input is `samples/fbnq.txt`.
5. Generated files are written to `outputs/`.

This repository was migrated from Gitee. Commit history is preserved when the Git repository is pushed to GitHub.
