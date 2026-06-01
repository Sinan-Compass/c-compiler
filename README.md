# C Compiler Course Project

这是一个编译原理课程设计项目，实现了一个面向 C-like 语言子集的简单编译器流程。项目包含词法分析、语法分析、AST 可视化、语义分析、四元式生成以及简易汇编代码生成。

## 功能概览

- 词法分析：读取 `samples/fbnq.txt`，生成 token 序列并维护关键字、界符、常量、标识符等表。
- 语法分析：基于递归下降分析构建抽象语法树 AST。
- AST 可视化：输出 Graphviz DOT 文件到 `outputs/ast7.dot`。
- 语义分析：遍历 AST 并生成四元式，输出到 `outputs/quadruple.txt`。
- 目标代码生成：根据四元式生成简易汇编，输出到 `outputs/assmbly.asm`。

## 目录结构

```text
.
├── src/                         # 编译器源码
│   ├── main.cpp                 # 编译流程入口
│   ├── Lexical.*                # 词法分析与符号表
│   ├── Syntactic.*              # 语法分析器
│   ├── ast.h                    # AST 节点定义
│   ├── semantic.*               # 语义分析与四元式生成
│   ├── toAssembly.*             # 汇编代码生成
│   └── TuXing.h                 # AST DOT 可视化
├── samples/                     # 输入样例程序
├── outputs/                     # 示例输出与运行生成结果
├── docs/theory/                 # 课程设计理论笔记
├── SemanticAnalysis/            # 早期语义分析实验代码
└── 编译原理课设_语法分析器.sln      # Visual Studio 解决方案
```

## 开发环境

- Windows
- Visual Studio 2022 / MSVC v143
- C++20
- EasyX/graphics.h 环境：源码中包含 `graphics.h`、`conio.h`、`windows.h`，如果本机缺少 EasyX，需要先安装相应图形库或移除界面相关代码后再编译。

## 构建与运行

1. 使用 Visual Studio 打开 `编译原理课设_语法分析器.sln`。
2. 选择 `Debug|x64` 或 `Release|x64`。
3. 构建并运行项目。
4. 默认输入文件为 `samples/fbnq.txt`。
5. 运行后会生成或更新：
   - `outputs/ast7.dot`
   - `outputs/quadruple.txt`
   - `outputs/assmbly.asm`

如果想测试其他样例，可以将 `samples/` 下的其他文件内容复制到 `samples/fbnq.txt`，或修改 `src/Lexical.h` 中的输入文件路径。

## 支持的语言特性

当前实现主要覆盖课程设计所需的 C-like 子集，包括：

- 函数定义与函数调用
- `int`、`float`、`void` 等基础类型相关语法
- 变量声明、数组声明与数组访问
- 赋值表达式、算术表达式、关系表达式、逻辑表达式
- `if/else`、`while`、`return`
- 四元式中间表示
- 简易汇编输出

## 说明

仓库从 Gitee 迁移而来，历史提交会随 Git 推送保留。此次整理主要调整目录结构、补充 README、更新工程文件路径，并将 Visual Studio 缓存和编译产物加入 `.gitignore`。
