## 5.1
会置为0

## 5.2
版本1最快：n=1,2
版本2最快：n=3,4,5,6,7
版本3最快：n>=8

## 5.3
| code | min | max | incr | square |
|------|-----|-----|------|--------|
|  A   |  1  | 91  | 90   |  90    |
|  B   | 91  |  1  | 90   |  90    |
|  C   |  1  |  1  | 90   |  90    |

## 5.4
A. `%xmm0` 在 -O2 优化中有存储结果的功能，而在 -O1 优化中只作为乘法结果的临时存储
B. 没有，相当于引入了累乘变量
C. 发现 `(%rbx)` 和 `%xmm0` 在循环开始时值相等，因此可以用 `%xmm0` 替代反复取值

## 5.5
A. 2n次乘法和n次加法
B. 浮点数乘法延迟为5

## 5.6
A. n次加法和n次乘法
B. 要执行下一条要等待 加法和乘法串行结束

## 5.8
