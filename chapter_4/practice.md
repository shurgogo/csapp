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