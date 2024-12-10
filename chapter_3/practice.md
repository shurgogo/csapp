## 3.1

已知

| Address | Value | Register | Value |
|---------|-------|----------|-------|
|  0x100  | 0xFF  |  `%rax`  | 0x100 |
|  0x104  | 0xAB  |  `%rcx`  | 0x1   |
|  0x108  | 0x13  |  `%rdx`  | 0x3   |
|  0x10C  | 0x11  |          |       |

求

|      Operand      | Value |
|-------------------|-------|
|  `%rax`           | 0x100 |
|  `0x104`          | 0xAB  |
|  `$0x108`         | 0x108 |
|  `(%rax)`         | 0xFF  |
|  `4(%rax)`        | 0xAB  |
|  `9(%rax,%rdx)`   | 0x11  |
|  `260(%rcx,%rdx)` | 0x13  |
|  `0xFC(,%rcx,4)`  | 0xFF  |
|  `(%rax,%rdx,4)`  | 0x11  |


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

|          instruction         | Result |
|------------------------------|--------|
|  `leaq 6(%rax), %rdx`        | 6+x    |
|  `leaq (%rax,%rcx), %rdx`    | x+y    |
|  `leaq (%rax,%rcx,4), %rdx`  | x+4y   |
|  `leaq 7(%rax,%rax,8), %rdx` | 7+9x   |
|  `leaq 0xA(,%rcx,4), %rdx`   | 10+4y  |
|  `leaq 9(%rax,%rcx,2), %rdx` | 9+x+2y |


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
|  `addq %rcx, (%rax)`        |    0x100    | 0x100 |
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

## 3.33
a, b, u, v
int, short, long*, char*

汇编中可能出现两者交换顺序的情况
b, a, v, u
int, short, long*, char*

## 3.34
A. a0~a5
B. a6~a7
不够用

## 3.35

```c
long rfun(unsigned long x) {
    if (x == 0)
        return 0;
    unsigned long nx = x >> 2;
    long rv = rfun(nx);
    return x + rv;
}
```

## 3.36

| Array | Element size | Total size | Start address |Element i|
|-------|--------------|------------|---------------|---------|
|   S   |       2      |    14      |       s       |   s+2i  |
|   T   |       8      |    24      |       t       |   t+8i  |
|   U   |       8      |    48      |       u       |   u+8i  |
|   V   |       4      |    32      |       v       |   v+4i  |
|   W   |       8      |    32      |       w       |   w+8i  |

## 3.37

| Expression |   Type  |   Value  |         Assembly code        |
|------------|---------|----------|------------------------------|
| S+1        | short * |  s+2     | `leaq 2(%rdx), %rax`         |
| s[3]       | short   | M[s+6]   | `movw 6(%rdx), %ax`          |
| &S[i]      | short * | s+2i     | `leaq (%rdx,%rcx,), %rax`    |
| S[4*i+1]   | short   | M[s+8i+2]| `movw 2(%rdx,%rcx,8), %ax`   |
| S+i-5      |short *  | s+2i-10  | `leaq -10(%rdx,%rcx,2),%rax` |

## 3.38
M=5 N=7

## 3.39

$ \&D[i][j] = X_D + L * (C*i+j) $
Aptr 从 i 行 0 列开始，每次地址+1，增加 1 列
Bptr 从 0 行 k 列开始，每次地址+C，增加 1 行
*Aptr = D[i][j]
*Bptr = D[j][k]


## 3.40

```c
#define N 16;

void fix_set_diag_opt(fix_matrix A, int val) {
    int *Aptr = &A[0][0];
    int *end = &A[0][0] + N*(N+1);
    do {
        *Aptr = val;
        Aptr += (N+1);
    } while (Aptr != end);
}
```

## 3.41

A. 0, 8, 12, 16
B. 24
C.
```c
void sp_init(struct prob *sp) {
    sp->s.x = sp->s.y;
    sp->p   = &(sp->s.x);
    sp->next= sp;
}
```

## 3.42
A. 
```c
long fun(struct ELE *ptr) {
    long result = 0;
    while (ptr) {
        result += ptr->v;
        ptr = ptr->p;
    }
    return result;
}
```

## 3.43

|         expr       | type  |           code            |
|--------------------|-------|---------------------------|
| up->t1.v           | short | `movw 8(%rdi), %ax`       |
|                    |       | `movw %ax, (%rsi)`        |
| &up->t1.w          | char* | `addq $10, %rdi`          |
|                    |       | `movq %rdi, (%rsi)`       |
| up->t2.a           | int*  | `movq %rdi, (%rsi)`       |
| up->t2.a[up->t1.u] | int   | `movq (%rdi), %rax`       |
|                    |       | `movl (%rdi,%rax,4), %eax`|
|                    |       | `movl %eax, %(rsi)`       |
| *up->t2.p          | char  | `movq 8(%rdi), %rax`      |
|                    |       | `movb (%rax), %al`        |
|                    |       | `movb %al, (%rdi)`        |


## 3.44
A. i:0,c:4,j:8,d:12; size = 16; align=4
B. i:0,c:4,d:5,j:8; size = 16; align=8
c. w:0,c:6; size=10; align=2
d. w:0,c:16; size=40; align=8
d. a:0,t:24; size=40; align=8

## 3.45
A. a:0,b:8,c:16,d:24,e:28,f:32,g:40,h:48
B. 56
C. 
```c
struct {
    char *a;
    double c;
    long g;
    float e;
    int h;
    short b;
    char d;
    char f;
} rec;
```
size = 40

## 3.46
A.

|       stack           | description   |
|-----------------------|---------------|
|00 00 00 00 00 40 00 76| return address|
|01 23 45 67 89 AB CD EF| `%rbx`        |
|                       | empty         |
|                       | <-`%rsp`      | 

B.

|       stack           | description   |
|-----------------------|---------------|
|00 00 00 00 00 40 00 34| return address|
|33 32 31 30 39 38 37 36| `%rbx`        |
|35 34 33 32 31 30 39 38| overflow      |
|37 36 35 34 33 32 31 30| <-`%rsp`      | 

C. 试图返回到 400034
D. `%rbx`
E. malloc(strlen(buf)+1); return 要检查 NULL

## 3.47
A. 8192 bytes
B. 64次

## 3.48
A. 
a) buf: 0; v: -24(%rsp)
b) buf: -16(%rsp); v: -8(%rsp);canary:-40(%rsp)
B. v更靠近栈顶，buf 溢出后不会影响 v；溢出后先破坏 canary，不会破坏返回值

## 3.49
A.  
%rax = 22+8*n
%rax &= -16 向下舍入到最接近16的倍数
%rsp -= %rax

B. 
%rax = %rsp + 7
%rax >> 3
%r8 = %rax*8 向上舍入到最接近8的倍数
%rcx = %r8

C.

|n| s1 | s2 | p  |e1|e2|
|-|----|----|----|--|--|
|5|2065|2017|2024|1 |7 |
|6|2064|2000|2000|16|0 |

D. s1 - s2 是16的对齐，p以8的倍数对齐

## 3.50
val1: d
val2: i
val3: l
val4: f

## 3.51

gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0。long to float 用了 `cvtsi2ss`。

|   Tx   |   Ty   |         Instruction             |
|--------|--------|---------------------------------|
| long   | double | `vcvtsi2sdq %rdi, %xmm0`        |
| double | int    | `vcvttsd2si %xmm0, %eax`        |
| double | float  | `vcvtsd2ss %xmm0, %xmm0`        |
| long   | float  | `vcvtsi2ssq %rdi, %xmm0, %xmm0` |
| float  | long   | `vcvttss2siq %xmm0, %rax`       |

## 3.52
A. a： %xmm0, b: %rdi, c: %xmm1, d: %esi
B. a: %edi, b: %rsi, c: %rdx, d: %rcx
C. a: %rdi, b: %xmm0, c: %esi, d: %xmm1
D. a: %xmm0, b: %rdi, c: %xmm1, d: %xmm2

## 3.53
|arg1_t|arg2_t|arg3_t|arg4_t|
|------|------|------|------|
| int  | long | float|double|
| int  | float| long |double|

## 3.54
```c
double funct2(double w, int x, float y, long z) {
    return y*x - w/z;
}
```

## 3.55
1077936128=0x40400000。因为是 double 类型，符号位+指数位=12，即 0x404，得到符号位为0，$E = 0x404-Bias = 5$。
又因为小数位都为0，所以M=1。$V = (-1)^0 * M * 2^E = 32$

## 3.56
A. abs(x)
B. x^x = 0.0
C. -x

## 3.57
```c
double funct3(int *ap, double b, long c, float *dp) {
    int a = *ap;
    float d = *dp;
    if (a < b) {
        return c * d;
    } else {
        return c+d*2;
    }
}
```