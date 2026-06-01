# 1.8086 汇编语言的**指令格式和语法结构**

------

###  1.1数据传送类（Data Transfer Instructions）

```
MOV     目的, 源        ; 数据传送
PUSH    操作数           ; 入栈
POP     操作数           ; 出栈
XCHG    操作数1, 操作数2 ; 交换
IN      AL/AX, 端口号     ; 从端口读取
OUT     端口号, AL/AX     ; 向端口写数据
LEA     寄存器, 地址      ; 装载有效地址
LDS     寄存器, 内存地址  ; 装载段寄存器
LES     寄存器, 内存地址
```

------

###  1.2算术运算类（Arithmetic Instructions）

```
ADD     操作数1, 操作数2 ; 加
ADC     操作数1, 操作数2 ; 加上进位
SUB     操作数1, 操作数2 ; 减
SBB     操作数1, 操作数2 ; 减去带借位
INC     操作数           ; 加1
DEC     操作数           ; 减1
NEG     操作数           ; 取负
CMP     操作数1, 操作数2 ; 比较
MUL     操作数           ; 无符号乘法
IMUL    操作数           ; 有符号乘法
DIV     操作数           ; 无符号除法
IDIV    操作数           ; 有符号除法
```

------

###  1.3逻辑运算类（Logic Instructions）

```
AND     操作数1, 操作数2 ; 与
OR      操作数1, 操作数2 ; 或
XOR     操作数1, 操作数2 ; 异或
NOT     操作数           ; 非（取反）
TEST    操作数1, 操作数2 ; 位测试（不影响操作数）
```

------

###  1.4移位与旋转（Shift and Rotate）

```
SHL/SAL 操作数, 位数     ; 左移
SHR     操作数, 位数     ; 逻辑右移
SAR     操作数, 位数     ; 算术右移
ROL     操作数, 位数     ; 循环左移
ROR     操作数, 位数     ; 循环右移
RCL     操作数, 位数     ; 循环左移（带进位）
RCR     操作数, 位数     ; 循环右移（带进位）
```

------

###  1.5跳转类（Jump Instructions）

#### 1.5.1 无条件跳转：

```
JMP     目标地址         ; 无条件跳转
CALL    过程名           ; 调用子程序
RET                      ; 返回
```

#### 1.5.2.条件跳转（根据标志位）：

```
JE/JZ    目标地址 ; 等于/零
JNE/JNZ  目标地址 ; 不等于/非零
JA/JNBE  目标地址 ; 大于（无符号）
JAE/JNB  目标地址 ; 大于等于
JB/JNAE  目标地址 ; 小于
JBE/JNA  目标地址 ; 小于等于
JG/JNLE  目标地址 ; 大于（有符号）
JGE/JNL  目标地址 ; 大于等于
JL/JNGE  目标地址 ; 小于
JLE/JNG  目标地址 ; 小于等于
JC       目标地址 ; 进位
JNC      目标地址 ; 无进位
JO       目标地址 ; 溢出
JNO      目标地址 ; 无溢出
JS       目标地址 ; 负数
JNS      目标地址 ; 非负
JP/JPE   目标地址 ; 奇偶标志为1
JNP/JPO  目标地址 ; 奇偶标志为0
```

------

###  1.6字符串处理（String Instructions）

```
MOVS/MOVSB/MOVSW       ; 移动字符串
CMPS/CMPSB/CMPSW       ; 比较字符串
SCAS/SCASB/SCASW       ; 扫描字符串
LODS/LODSB/LODSW       ; 加载字符串元素
STOS/STOSB/STOSW       ; 存储字符串元素
REP/REPE/REPNE 前缀    ; 重复执行
```

------

###  1.7标志控制类（Flag Control）

```
CLC     ; 清除进位标志
STC     ; 设置进位标志
CMC     ; 取反进位标志
CLD     ; 清除方向标志（向前处理字符串）
STD     ; 设置方向标志（向后处理字符串）
CLI     ; 禁用中断
STI     ; 启用中断
```

------

###  1.8处理器控制（Control Instructions）

```
HLT     ; 停机
WAIT    ; 等待处理器空闲
NOP     ; 空操作
ESC     ; 协处理器转义指令
LOCK    ; 总线锁定前缀
```

------

###  1.9伪指令（Pseudo-Instructions）

这些不是机器指令，但用于编程辅助：

```
DB      定义字节数据
DW      定义字数据（2字节）
DD      定义双字（4字节）
SEGMENT 定义段
ENDS    结束段
ASSUME  告诉汇编器段寄存器与段的对应关系
ORG     设置地址偏移
EQU     定义常量
LABEL   定义标签
END     程序结束
```

------

###  1.10宏和结构相关（可选）

```
MACRO     宏定义
ENDM      宏结束
STRUCT    定义结构体
ENDS      结构体结束
```



# 2.浮点数支持

**常见 x87 协处理器浮点指令的简洁示例**

---

##  准备工作（通用头部）

每个示例都需要以下基本结构（MASM 语法）：

```asm
.model small
.stack 100h
.data
    result REAL4 ?
.code
start:
```

每段代码后面都用：

```asm
    mov ax, 4c00h
    int 21h
end start
```

---

##  2.1 `fld` —— 加载一个浮点数到 FPU 栈顶

```asm
    fld REAL4 PTR [myNum]   ; 加载 float 数
    fstp result             ; 存回结果
```

完整示例：

```asm
.data
    myNum REAL4 5.5
    result REAL4 ?
.code
start:
    fld myNum
    fstp result
```

---

##  2.2 `fadd` —— 浮点加法：ST(0) = ST(0) + 操作数

```asm
.data
    a REAL4 3.0
    b REAL4 2.0
    result REAL4 ?
.code
start:
    fld a          ; ST(0) = 3.0
    fadd b         ; ST(0) = 3.0 + 2.0
    fstp result    ; result = 5.0
```

---

## 2.3`fsub` —— 浮点减法：ST(0) = ST(0) - 操作数

```asm
.data
    a REAL4 5.0
    b REAL4 1.5
    result REAL4 ?
.code
start:
    fld a          ; ST(0) = 5.0
    fsub b         ; ST(0) = 5.0 - 1.5 = 3.5
    fstp result
```

---

## 2.4 `fmul` —— 浮点乘法：ST(0) = ST(0) \* 操作数

```asm
.data
    a REAL4 2.0
    b REAL4 3.5
    result REAL4 ?
.code
start:
    fld a          ; ST(0) = 2.0
    fmul b         ; ST(0) = 2.0 * 3.5 = 7.0
    fstp result
```

---

## 2.5`fdiv` —— 浮点除法：ST(0) = ST(0) / 操作数

```asm
.data
    a REAL4 7.0
    b REAL4 2.0
    result REAL4 ?
.code
start:
    fld a          ; ST(0) = 7.0
    fdiv b         ; ST(0) = 7.0 / 2.0 = 3.5
    fstp result
```

---

##  2.6 `fcom` —— 比较 ST(0) 和指定值（设置状态位）

```asm
.data
    a REAL4 3.0
    b REAL4 5.0
.code
start:
    fld a          ; ST(0) = 3.0
    fcom b         ; 比较 ST(0) 和 b（不弹栈）

    ; 通常配合 fstsw ax 和 sahf 使用来检查比较结果
    fstsw ax       ; 取状态字
    sahf           ; 把 AH 转到标志寄存器
    ; 之后可以用 jz/jc/ja 等跳转
```

---

## 2.7 `fsqrt` —— 开平方根（√ST(0)）

```asm
.data
    a REAL4 9.0
    result REAL4 ?
.code
start:
    fld a          ; ST(0) = 9.0
    fsqrt          ; ST(0) = sqrt(9.0) = 3.0
    fstp result
```

---

##  2.8补充：`fst` 与 `fstp`

| 指令     | 说明                |
| ------ | ----------------- |
| `fst`  | 存储 ST(0) 到变量（不弹出） |
| `fstp` | 存储并弹出 ST(0)       |

```asm
    fld a
    fst result   ; 仅保存，不弹栈
    fstp result  ; 保存并弹栈
```

---

##  如何验证结果？

因为我们不能直接在汇编中输出浮点数（除非写字符串转换函数），可以：

* **用 C 调试**（将变量地址传给 printf）；
* **在 DOSBox 中用 Debug 工具查看 result 内存值**；
* **写一个把浮点数转换为字符串的例程**（可选进阶）；

---

##  2.9总结图表

| 指令      | 功能      |
| ------- | ------- |
| `fld`   | 加载浮点数入栈 |
| `fadd`  | 加法      |
| `fsub`  | 减法      |
| `fmul`  | 乘法      |
| `fdiv`  | 除法      |
| `fstp`  | 保存并弹栈   |
| `fcom`  | 比较      |
| `fsqrt` | 平方根（√）  |

---

需要我把这些例子打包成 `.asm` 文件或者写个 `.bat` 批处理脚本让你直接在 DOSBox 里运行吗？或者你想要将计算结果显示为字符串？欢迎告诉我。
