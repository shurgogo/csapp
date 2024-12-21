## 4.1
```asm
0x100:                        |.pos 0x100 # start code at address 0x100
0x100: 30f30f00000000000000   | irmovq $15, %rbx
0x10A: 2031                   | rrmovq %rbx, %rcx
0x10C:                        | loop:
0x10C: 4013FDFFFFFFFFFFFFFF   | rmmovq %rcx,-3(%rbx)
0x116: 6031                   | addq %rbx, %rcx
0x118: 700C01000000000000     | jmp loop
```

## 4.2
A. `irmovq $-2, %rbx`, `rmmovq %rsi, 2048(%rbx)`
B. `pushq %rsi`, `call 0x20c`(call proc), `halt`, proc: `irmovq $0xa, %rbx`, `ret`
C. `mrmovq 7(%rbp), %rsp`, `nop`, `pop %rcx`
D. `subq %rcx, %rbx`, `je 0x400`, `halt`
E. `xorq %rsi, %rdx`, 非法

## 4.3

```asm
sum:
    xorq %rax, %rax
    andq %rsi, %rsi
    jmp test
loop:
    mrmovq (%rdi), %r10
    addq %r10, %rax
    iaddq $8, %rdi
    iaddq $-1, %rsi
test:
    jne loop
    ret
```

## 4.4
```asm
rsum:
    andq %rsi, %rsi         # set cc
    je .L3                  # if count == 0， return 0
    pushq %rbx              # 被调用者保存寄存器
    mrmovq (%rdi), %rbx     # 获取 *Start
    irmovq $8, %r8          # 立即数保存到寄存器，用于后续计算
    irmovq $1, %r9
    addq %r8, %rdi          # start + 1
    subq %r9, %rsi          # count - 1
    call rsum               # 递归
    addq %rbx, %rax         # 保存的 *Start 加到返回值中
    popq %rbx               # 恢复寄存器上一个保存值
    ret
.L3:
    xorq %rax, %rax
    ret
```

## 4.5
```asm
sum:
    irmovq $8, %r8
    irmovq $1, %r9
    xorq %rax, %rax
    andq %rsi, %rsi
    jmp test
loop:
    mrmovq (%rdi), %r10
    xorq %r11, %r11     # 0
    subq %r10, %r11     # %r11 = -x
    jle ge0             # if *start >= 0
    rrmovq %r11, %r10   # %r10 = -x
ge0:
    addq %r10, %rax     # sum += *start
    addq %r8, %rdi
    subq %r9, %rsi
test:
    jne loop
    ret
```

## 4.6
```asm
sum:
    irmovq $8, %r8
    irmovq $1, %r9
    xorq %rax, %rax
    andq %rsi, %rsi
    jmp test
loop:
    mrmovq (%rdi), %r10
    xorq %r11, %r11     # 0
    subq %r10, %r11     # %r11 = -x
    cmovg %r11, %r10    # if 0 > *start, %r10 = %r11
    addq %r10, %rax     # sum += *start
    addq %r8, %rdi
    subq %r9, %rsi
test:
    jne loop
    ret
```

## 4.7
虽然 push 的定义是 %rsp-8 后在 %rsp-8 的位置保存，返回 0 说明有个类似于中间变脸 tmp 保存了 %rsp， 即 `pushq %rsp` 保存的是 tmp 中的旧值。

## 4.8
弹出了内存中的值，而不是 %rsp+8 的值

## 4.9

xor = (!a && b) || (!b && a)
xor = !eq

## 4.10

64个xor + or + not

## 4.11
word Min3 = [
A <= B && A <= C: A;
B <= C          : B;
1               : C;
]

## 4.12
word Med3 = [
B <= A && A <= C: A;
C <= A && A <= B: A;
A <= B && B <= C: B;
C <= B && B <= A: B;
1               : C;
]

## 4.13

| Stage     | `irmovq V, rB`       | `irmovq $128, %rsp`         |
|-----------|----------------------|-----------------------------|
| Fetch     | icode:ifun <- M1[PC] | icode:ifun <- M1[0x016]=3:0 |
|           | rA:rB <- M1[PC+1]    | rA:rB <- M1[0x017]=F:4      |
|           | valC <- M8[PC+2]     | valC <- M8[0x018]= 128      |
|           | valP <- PC+10        | valP <- 0x016+10=0x020      |
| Decode    |                      |                             |
| Execute   | valE <- 0+valC       | valE <- 0+valC=128          |
| Access    |                      |                             |
| Write     | R[rB] <- valE        | R[%rsp] <- valE=128         |
| PC Update | PC <- valP           | PC <- valP=0x020            |


## 4.14

| Stage     | `popq rA`            | `popq %rax`         |
|-----------|----------------------|-----------------------------|
| Fetch     | icode:ifun <- M1[PC] | icode:ifun <- M1[0x02c]=b:0 |
|           | rA:rB <- M1[PC+1]    | rA:rB <- M1[0x02D]=0:F      |
|           |                      |                             |                
|           | valP <- PC+2         | valP <- 0x02c+2=0x02e       |
| Decode    | valA <- R[%rsp]      | valA <- R[%rsp]=120         |
|           | valB <- R[%rsp]      | valB <- R[%rsp]=120         |
| Execute   | valE <- valB+8       | valE <- 120+8=128           |
| Access    | valM <- M8[valA]     | valM <- M8[128]=9           |
| Write     | R[%rsp] <- valE      | R[%rsp] <- valE=128         |
|           | R[rA] <- valM        | R[%rax] <- valM=9           |
| PC Update | PC <- valP           | PC <- valP=0x02e            |

## 4.15
会 push 旧值，与期望一致

## 4.16
会 pop 原来 %rsp 中的值到 %rsp，与期望一致

## 4.17

| Stage     | `cmovxx rA, rB`           |
|-----------|---------------------------|
| Fetch     | icode:ifun <- M1[PC]      |
|           | rA:rB <- M1[PC+1]         |               
|           | valP <- PC+2              |
| Decode    | valA <- R[rA]             |
| Execute   | Cnd <- Cond(CC, ifun)     |
|           | valE <- valA              |
| Access    |                           |
| Write     | if(Cnd)R[rB]              |
|           | R[rB] <- valE             |
| PC Update | PC <- valP                |

## 4.18

| Stage     | `call Dest`          | `call 0x041`                |
|-----------|----------------------|-----------------------------|
| Fetch     | icode:ifun <- M1[PC] | icode:ifun <- M1[0x037]=8:0 |
|           | valC <- M8[PC+1]     | valC <- M8[0x038]=0x041     |                
|           | valP <- PC+9         | valP <- 0x037+9=0x040       |
| Decode    | valB <- R[%rsp]      | valB <- R[%rsp]=128         |      
| Execute   | valE <- valB-8       | valE <- 128-8=120           |
| Access    | M8[valE] <- valP     | M8[120] <- valP=0x040       |
| Write     | R[%rsp] <- valE      | R[%rsp] <- valE=120         |
| PC Update | PC <- valC           | PC <- valC=0x041            |

## 4.19
```hcl
bool need_valC = icode in {IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL}
```

## 4.20
```hcl
word srcB = [
    icode in {IRMMOVQ, IMRMOVQ, IOPQ}: rB;
    icode in {IPUSHQ, IPOPQ, ICALL, IRET}: RRSP;
    1: RNODE;
]
```

## 4.21
```hcl
word dstM = [
    icode in {IMRMOVQ, IPOPQ}: rA;
    1: RNODE
]
```

## 4.22
dstM 端口优先级高

## 4.23
```hcl
word aluB = [
    icode in {IRMMOVQ, IMRMOVQ, IOPQ, IPOPQ, IPUSHQ, ICALL, IRET}: valB;
    icode in {IRRMOVQ, IIRMOVQ}: 0
]
```

## 4.24
```hcl
word dstE = [
    icode in {IRRMOVQ} && Cnd: rB;
    icode in {IIRMOVQ, IOPQ}: rB;
    icode in {IPUSHQ, IPOPQ, ICALL, IRET}: RRSP;
    1: RNODE;
]
```

## 4.25
```hcl
word mem_data = [
    icode in {IPUSHQ, IRMMOVQ}: valA;
    icode == ICALL: valP;
]
```

## 4.26
```hcl
word mem_write = icode in {IPUSHQ, IRRMMOVQ, ICALL};
```

## 4.27
```hcl
word Stat = [
    imem_error || dmem_error: SADR;
    !instr_valid: SINS;
    icode == IHALT: SHLT;
    1: SAOK;
]
```

## 4.28
A. 
在 C 之后
吞吐量 = 5.26 GIPS
延迟 = 380 ps

B. 
在 B 和 D 之后
吞吐量 = 7.69 GIPS
延迟 = 390 ps

C.
在 A、C、D 之后
吞吐量 = 9.09 GIPS
延迟 = 440 ps

D.
5阶段，在A、B、C、D后分别加1个寄存器
吞吐量 = 10 GIPS
延迟 = 500 ps

## 4.29
$$
延迟 = (300/k+20)*k = 20k + 300 ps\\
\\
吞吐量 = \frac{1000}{(300/k+20)} GIPS
$$
当 k -> ∞ 时，吞吐量为 50 GIPS

## 4.30

```hcl
word f_stat = [
    imem_error: SADR;
    !instr_valid: SINS;
    f_icode == IHALT: SHLT;
    1: SAOK;
]
```

## 4.31

```hcl
word d_dstE = [
    D_icode in {IRRMOVQ,IIRMOVQ, IOPQ}: D_rB;
    D_icode in {IPUSHQ, IPOPQ, ICALL, IRET}: RRSP;
    1: RNODE;
]
```

## 4.32

Execute 阶段中 `popq` 的 e_dstE 为 0x108，之后在 Memory 阶段传递给 M_dstE，所以 `%rax` 会先拿到 0x108 而不是 5，与预期不符。

## 4.33

```asm
irmovq $5, %rdx
irmovq $0x100, %rsp
rmmovq %rdx, 0(%rsp)
nop
nop
popq %rax
```
同样会导致 `%rax` 拿到 0x108，只不过是从 Write 阶段转发的。

## 4.34

```hcl
word d_valB = [                        
    d_srcB == e_dstE: e_valE;           # 从 ALU 结果转发
    d_srcB == M_dstM: m_valM;           # 从 Memory 阶段转发
    d_srcB == M_dstE: M_valE;           # 从 Memory 阶段转发
    d_srcB == W_dstM: W_valM;           # 从 Write 阶段转发
    d_srcB == W_dstE: W_valE;           # 从 Write 阶段转发
    1: d_rvalB;                         # 从 rB 读取
];
```

## 4.35

```asm
irmovq $0x123,%rax
irmovq $0x321,%rdx
xorq %rcx,%rcx
cmovne %rax,%rdx
addq %rdx,%rdx
halt
```
如果使用 E_dstE，条件传送源值 0x123 被转发到 ALU 的输入 valA，而 valB 是正确的 0x321，得到结果为 0x444（正确结果为 0x666）。


## 4.36

```hcl
word m_stat = [
    dmem_error: SADR;
    1: M_stat;
]
```

## 4.37

```asm
    xorq %rdx, %rdx
    jne target
target: 
    ret
    irmovq $1, %rdx
    halt
```


## 4.38

```asm
    mrmovq 0(%rbx), %rsp  
    ret
    halt
rtnpt: 
    irmovq $5, %rsi
    halt
```

假设 0(%rbx) 保存了 ret 的地址 rpnpt（即 `irmovq $5, %rsi` 的地址）

## 4.39

```hcl
bool D_stall = 
    E_icode in {IMRMOVQ, IPOPQ} && E_dstM in {d_srcA, d_srcB}; # 加载/使用冒险
```

## 4.40

```hcl
bool E_bubble = 
    (E_icode == jXX && !e_Cnd) || # 分支预测错误
    E_icode in {IMRMOVQ, IPOPQ} && E_dstM in {d_srcA, d_srcB};  # 加载/使用冒险
```

## 4.41

```hcl
bool set_cc = 
    E_icode == OPq &&
    !m_stat in {SADR, SHLT, SINS} && !W_stat in {SADR, SHLT, SINS};
```

## 4.42
```hcl
bool M_bubble = m_stat in {SADR, SHLT, SINS} || W_stat in {SADR, SHLT, SINS};

bool W_stall = W_stat in {SADR, SHLT, SINS};
```

## 4.43
平均惩罚会降低 0.02 个周期，CPI 达到 1.25

## 4.44

A. 8.5, 8
B. 1, 0
C. 9.5, 8