# 四元式示例

## 1.转化规则
```
    设 quat(E),res(E)分别为表达式E的四元式和结果变量。

    ① quat(E1ωE2)= 	quat(E1)
                    quat(E2)
                    q:(ω res(E1) res(E2) ti )
    ② quat((E))= quat(E)    
    ③ quat(i)= 空 , res(i)= i
    其中：ω(运算符); i运算对象（变量或常数）
    ⑴ 方框内的三个定义式 ，为四元式翻译法则；
    ⑵ 定义式中的ω为 E1ωE2 中最后运算的算符！
```

## 2.算术表达式
```
    (1) ( * a b t ) 
    (2) ( / a b t ) 
    (3) ( - a b t ) 
    (4) ( + a b t ) 
    (5) ( = t _ x )
    (6) ( > a b t )
    (7) ( < a b t )
    (8) ( == a b t )
    (9) ( != a b t )
   	(10) ( && a b t )
    (11) ( || a b t )
    (12) ( ! a _ t )
    
```
## 3.转向语句（跳转语句）
```
    ⑴ (gt _  _  i )

    ......
    ⑻ (lb  _  _  i  )
    ⑼ ( +  a  b  t1 )
    ⑽ ( /  t1 c  t2 )
    ⑾ (:=  t2 _  x  )
```

## 4.条件语句
```
1. 文法：S -> if（E）S1 [ else S2 ]

        quat(E)
    q1(if res(E)_ _ )
        quat(S1)
    q2(el  _  _  _  )
        quat(S2)
    q3(ie  _  _  _  )

2. 例子：
    ⑴ ( <  a  b  t1 )
    ⑵ (if  t1 _  _  )
    ⑶ ( +  a  b  t2 )
    ⑷ ( =  t2 _  x  )
    ⑸ (el  _  _  _  )
    ⑹ ( -  a  b  t3 )
    ⑺ ( =  t3 _  y  )
    ⑻ (ie  _  _  _  )

    【注】 q1：当 res(E)=false 则 转向S2入口四元式；
    q2：无条件转向出口四元式；
    q3：条件语句出口四元式。

```

## 5.循环语句
```
(1) 文法：S -> while（E）S
    q1(wh _  _  _ )
        quat(E)
    q2(do res(E)_ _ )
        quat(S)
    q3(we _  _  _ )

(2)例子
    ⑴ (wh  _  _  _  )
    ⑵ ( <  a  b  t1 )
    ⑷ (∧  t1 t2 t3 )
    ⑸ (do t3  _  _  )
    ⑹ ( +  b  c  t4 )
    ⑺ ( /  t4 2  t5 )
    ⑻ ( =  t5 _  x  )
    ⑼ ( -  a  1  t6 )
    ⑽ ( =  t6 _  a  )
    ⑾ (we  _  _  _  )

```
## 6.返回语句
```
    (1) return 文法：return a;

    (2)(rt _ _ a)
```

## 7.声明语句
```
（1）标识符声明文法：<类型说明符><标识符>
	（df idt type name）

例子：
	source code: [int | float | void] a;

四元式：
    （df idt [int | float | void] a）
    （define identfier）

（2）函数声明文法：<类型说明符> <标识符> '(' [ <参数列表> ] ')' <块>

    q1(df fun type name)
        (para _ type name)
        ...
        (pe _ _ _)
        quats(S)		//块的内部语句
    q2(fe _  _  _ )

例子：
    source code: int add(int a, int b){
        return a + b;
    }
    四元式：
        (df fun int add)
        (para _ int a)
        (para _ int b)
        (pe _ _ _)
        (+ a b t1)
        (rt _ _ t1)
        (de _ _ _)
```

## 8.函数调用
```
假设已声明函数： int add(int a, int b){...}

调用文法：<标识符> '(' [ <参数列表> ] ')'';'

	(fun fun_name _ temp_renturn_val)
	
例子：
	source code: add(1, 2)
	四元式：
		(fun add _ rst)
		(para _ _ 1)
		(para _ _ 2)
		(fe _ _ _)
```

## 9.汇总表

| Statement Type   | Examples                       |
| ---------------- | ------------------------------ |
| Expression       | `x = y + 1;`, `func();`        |
| Declaration      | `int a = 0;`                   |
| Compound (Block) | `{ ... }`                      |
| Selection        | `if`, `else`, `switch`, `case` |
| Iteration        | `for`, `while`, `do...while`   |
