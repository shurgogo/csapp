## 3.1

已知

| Address | Value | Register | Value |
|---------|-------|----------|-------|
|  0x100  | 0xFF  |  `%rax`  | 0x100 |
|  0x104  | 0xAB  |  `%rcx`  | 0x1   |
|  0x108  | 0x13  |  `%rdx`  | 0x3   |
|  0x10C  | 0x11  |          |       |

求

|     Operand   | Value |
|---------------|-------|
|  `%rax`         | 0x100  |
|  `0x104`        | 0xAB  |
|  `$0x108`       | 0x108  |
|  `(%rax)`       | 0xFF  |
|  `4(%rax)`      | 0xAB |
|  `9(%rax,%rdx)` | 0x11  |
|  `260(%rcx,%rdx)` | 0x13  |
|  `0xFC(,%rcx,4)`    | 0xFF  |
|  `(%rax,%rdx,4)`    | 0x11  |


## 3.2
```asm
movl %eax, (%rsp)
movw (%rax), %dx
movb $0xFF, %bl
movb (%rsp,%rdx,4), %dl
movq (%rdx), %rax
movw %dx, (%rax)
```

## 3.3

```asm
movb $0xF, (%ebx) # %ebx 不能用做寻址，要用 %rbx
movl %rax, (%rsp) # 要用 movq
movw (%rax), 4(%rsp) # 不能直接从内存移动到内存
movb %al, %sl # 没有叫 %sl 的寄存器 -.-
movq %rax, $0x123 # 方向错误
movl %eax, %rdx # 源32位，目标64位，要扩展，如用movslq
movb %si, 8(%rbp) # 用 movw
```

## 3.4
```asm
# 1. char -> int
movsbl (%rdi), %eax
movl %eax, (%rsi)
```

```asm
# 2. char -> unsigned
movsbl (%rdi), %eax # char先转为int类型
movl %eax, (%rsi)
```

```asm
# 3. unsigned char -> long
movzbl (%rdi), %eax
movl %eax, (%rsi)
# 为什么不是 
# movzbq (%rdi), %rax
# movl %eax, (%rsi)
# 因为 movzbl 会自动将 %rax 高32位清零，更高效
```

```asm
# 4. int -> char
movl (%rdi), %eax
movb %al, (%rsi)
# 为什么不是 
# movb (%rdi), %al
# movb %al, (%rsi)
# 因为 C 语言的语义是读取整个 int 再截断，而不是直接截断
```

```asm
# 5. unsigned -> unsigned char
movl (%rdi), %eax
movb %al, (%rsi)
```

```asm
# 6. char -> short
movsbw (%rdi), %ax
movw %ax, (%rsi)
```

## 3.5

```c
void decode1(long *xp, long *yp, long *zp) {
    long x = *xp;
    long y = *yp;
    long z = *zp;
    *yp = x;
    *zp = y;
    *xp = z
}
```

## 3.6

|     instruction   | Result |
|---------------|-------|
|  `leaq 6(%rax), %rdx`         | 6+x  |
|  `leaq (%rax,%rcx), %rdx`        | x+y  |
|  `leaq (%rax,%rcx,4), %rdx`       | x+4y  |
|  `leaq 7(%rax,%rax,8), %rdx`       | 7+9x  |
|  `leaq 0xA(,%rcx,4), %rdx`      | 10+4y |
|  `leaq 9(%rax,%rcx,2), %rdx` | 9+x+2y  |


## 3.7
```c
long scale2(long x, long y, long z) {
    long t = 5*x + 2*y + 8*z;
    return t;
}
```

## 3.8

已知

| Address | Value | Register | Value |
|---------|-------|----------|-------|
|  0x100  | 0xFF  |  `%rax`  | 0x100 |
|  0x108  | 0xAB  |  `%rcx`  | 0x1   |
|  0x110  | 0x13  |  `%rdx`  | 0x3   |
|  0x118  | 0x11  |          |       |

求

|         Instruction         | Destinatino | Value |
|-----------------------------|-------------|-------|
|  `addq %rcx, (%rax)         |    0x100    | 0x100 |
|  `subq %rdx, 8(%rax)`       |    0x108    | 0xA8  |
|  `imulq $16, (%rax,%rdx,8)` |    0x118    | 0x110 |
|  `incq 16(%rax)`            |    0x110    | 0x14  |
|  `decq %rcx`                |    `%rcx`   | 0x0   |
|  `subq %rdx,%rax`           |    `%rax`   | 0xFD  |

## 3.9

```c
long shift_left4_rightn(long x, long n) {
    x << 4;
    x >> n;
    return x;
}
```

```asm
shift_left_4_rightn:
    movq    %rdi, %rax
    salq    $4,%rax
    movl    %esi, %ecx
    sarq    %cl, %rax
```

## 3.10

```c
long arith2(long x, long y, long z) {
    long t1 = x | y;
    long t2 = t1 >> 3;
    long t3 = ~t2;
    long t4 = z - t3;
    return t4;
}
```

## 3.11

`xorq %rdx, %rdx`

A. 把 `%rdx` 寄存器清零
B. 相当于 `movq $0, %rdx`
C. `xorq` 3字节，`movq` 7字节。使用 `xorl %edx, %edx` 为2字节。

## 3.12

```asm
void uremdiv(unsigned long x, unsigned long y, 
unsigned long *qp, unsigned long *rp)
x in %rdi, y in %rsi, qp in %rdx, rp in %rcx

remdiv:
    movq    %rdx, %r8      拷贝指针，空出 %rdx 寄存器
    movq    %rdi, %rax     x作为被除数移动到 %rax
    movl    $0, %edx       无符号数高位填0
    divq    %rsi           商和余数分别保存到 %rax，%rdx
    movq    %rax, (%r8)    商移动到 %r8 指向的地址
    movq    %rdx, (%rcx)   余数移动到 %rcx 指向的地址
```

## 3.13

已知

```c
int comp(data_t a, data_t b) {
    return a COMP b;
}
```

当汇编代码如下时，列出所有 data_t 和 COMP 的组合，例如(int, <)，(char, >)等。

A.
```asm
cmpl    %esi, %edi
setl    %al
```
(int, <)


B.

```asm
cmpw     %si, %di
setge    %al
```
(short, >=)

C.

```asm
cmpb     %sil, %dil
setbe    %al
```
(unsigned char, <=)

D.

```asm
cmpq     %rsi, %rdi
setne    %al
```
(long, !=) (unsigned long, !=) (指针, !=)

## 3.14

A. (long, >=)
B. (short, ==), (unsigned short, ==)
C. (unsigned char, >)
D. (int, <=)

## 3.15

A. 4003fe
B. 400425
C. ja: 400543 pop:400545
D. 400560


## 3.16

A.
```c
void cond(long a, long *p) {
    if (p == 0) goto done;
    if (a <= *p) goto done;
    *p = a;
done:
    return;
}
```
B. && 有截断的作用，即 p == 0

## 3.17

A. 
```c
long absdiff_se(long x, long y) {
    long result;
    if (x < y)
        goto x_lt_y;
    ge_cnt++;
    result = x - y;
    return result;
x_lt_y:
    lt_cnt++;
    result = y - x;
    return result;
}
```

B. 个人觉得判断一个表达式正确后不直接执行 then 语句而是通过 goto 跳过 else 语句，语义上太曲折不直观。

## 3.18

```c
long test(long x, long y, long z) {
    long val = x + y + z;
    if (x < -3) {
        if (y < z) {
            val = x * y;
        } else {
            val = y * z;
        }
    } else if (x > 2) {
        val = x * z;
    }
    return val;
}
```

## 3.19
A. 2*(31-16) = 30
B. 16+30 = 46

## 3.20
A. /
B. 如果被除数是负数的话，必须加上 2^k-1 = 7的偏移量

## 3.21

```c
long test(long x, long y) {
    long val = 8 * x;
    if (y > 0) {
        if (x >= y) {
            val = y & x;
        } else {
            val = y - x;
        }
    } else if (y <= -2) {
        val = x + y;
    }
    return val;
}
```

## 3.23

A. x: %rax, y: %rcx, n: %rdx
B. 用 `leaq` 
C.
```asm
dw_loop:
    movq    %rdi, %rax              Copy x to %rax 
    movq    %rdi, %rcx              
    imulq   %rdi, %rcx              Compute y = x*x
    leaq    (%rdi,%rdi), %rdx       Compute n = 2*x
.L2:
    leaq    1(%rcx, %rax), %rax     Compute x = x + y + 1
    subq    $1, %rdx                Decrement n
    testq   %rdx, %rdx              Compare n: 0
    jg      .L2 %rdx                if >, goto L2
    rep; ret                        Return
```

## 3.24

```c
long loop_while(long a, long b) {
    long result = 1;
    while (a < b) {
        result = result * (a+b); 
        a = a+1;
    }
    return result;
}
```

## 3.25

```c
long loop_while2(long a, long b) {
    long result = b;
    while (b > 0) {
        result = result * a;
        b = b-a;
    }
    return result;
}
```

## 3.26

B. 
```c
long fun_a(unsigned long x) {
    long val = 0;
    while (x != 0) {
        val ^= x;
        x >> 1;
    }
    return val & 1;
}
```

A. jump to middle
C. 计算x中1个数的奇偶性

## 3.27

```c
long fact_for_gd_goto(long n) {
    long i = 2;
    long result = 1;
    if (i > n)
        goto done;
loop:
    result *= i;
    i++;
    if (i <= n)
        goto loop;
done:
    return result;
}
```

## 3.28

A. 
```c
long fun_b(unsigned long x) {
    long val = 0;
    long i;
    for (i = 64; i != 0; i--) {
        val = (val << 1) | (x & 1);
        x >>= 1;
    }
    return val;
}
```

B. 因为 i 初始值为 64，必然!=0

C.将 x 的二进制反向

## 3.29

A. 会得到 45。本来是加偶数的，变成加所有数
B. 
```asm
loop:
    if (i & 1)
        goto next;
    sum += i;
next:
    i++
    if (i < 10)
        goto loop;
```

## 3.30
A. -1, 0, 1, 2, 4, 5, 7
B. .L5: 0, 7; .L7: 2, 4

## 3.31

```c
void switcher(long a, long b, long c, long *dest) {
    long val;
    switch (a) {
    case 5:  /* Case A */
        c = b ^ 15;
    case 0:  /* Case B */ 
        val = 112 + c;
        break;
    case 2:  /* Case C */    
    case 7:  /* Case D */
        val = (c+b) << 2;
        break;
    case 4:  /* Case E */    
        val = a;
        break;
    default:
        val = b;
    }
    *dest = val;
}
```
分析：
- `cmpq $7, %rdi   ja .L2` 所以跳转表 index 为0～7
- `.L2` 为 default 分支
- `.L6` 为 `*dest = val`
- 到 `.L2` 的分支 a = 1，3，6 都没有在 case 中
- 剩下按顺序排：5, 0, (2,7), 4