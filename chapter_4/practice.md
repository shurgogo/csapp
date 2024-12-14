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