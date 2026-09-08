# Chapter 3. Machine-Level Programming

## 3.1 Machine Prog: Basics (26.09.07)

### 1. C → Machine Code

```text
C Program
   ↓
Compiler
   ↓
Assembly
   ↓
Assembler
   ↓
Object Code
   ↓
Linker
   ↓
Executable
```

* Compiler → C를 Assembly로 변환
* Assembler → Assembly를 Machine Code로 변환
* Linker → 여러 Object Code와 Library를 결합

### 2. Programmer-Visible State

* **PC (Program Counter, 程序计数器)** → 다음에 실행할 instruction의 주소

  * IA32 → `EIP`
  * x86-64 → `RIP`
* **Register (寄存器)** → CPU 내부의 빠른 저장 공간
* **Memory (内存)** → Code, Data, Stack 등을 저장
* **Condition Codes** → 연산 결과의 상태를 저장

### 3. Assembly

Assembly는 CPU가 수행할 **instruction(指令)**을 사람이 읽을 수 있는 형태로 표현한 것이다.

주요 연산:

* Arithmetic → 산술 연산
* Data Transfer → Memory ↔ Register
* Control Transfer → 실행 흐름 변경

### 4. `mov`

```asm
movl Source, Dest
```

* Source의 값을 Dest로 **복사**
* `l` → 32-bit (4 bytes)

주요 형태:

```asm
movl $10, %eax      // Immediate → Register
movl %eax, %edx     // Register → Register
movl (%eax), %edx   // Memory → Register
movl %eax, (%edx)   // Register → Memory
```

* **Memory → Memory는 한 instruction으로 직접 수행할 수 없음.**

### 5. Memory Addressing

```asm
D(Rb,Ri,S)
```

$$
Mem[Reg[Rb]+S\times Reg[Ri]+D]
$$

* `D` → displacement (상수)
* `Rb` → base register
* `Ri` → index register
* `S` → scale: `1, 2, 4, 8`

특수 형태:

```text
(Rb)          → Mem[Rb]
D(Rb)         → Mem[Rb + D]
(Rb,Ri)       → Mem[Rb + Ri]
D(Rb,Ri)      → Mem[Rb + Ri + D]
(Rb,Ri,S)     → Mem[Rb + Ri × S]
```

* Array 접근 등에 활용된다.

### 6. Address vs Value

```text
%rdi = 0x1000
Memory[0x1000] = 30
```

```asm
movl %rdi, %eax
```

→ `%eax = 0x1000` (**주소**)

```asm
movl (%rdi), %eax
```

→ `%eax = 30` (**주소가 가리키는 값**)

* `(%reg)` → 해당 Register의 값을 **주소로 사용**

### 7. IA32 vs x86-64

**IA32**

* 함수 인자를 주로 Stack을 통해 전달
* 32-bit Register

**x86-64**

* 함수 인자를 Register로 전달

  * 첫 번째 → `%rdi`
  * 두 번째 → `%rsi`
* 더 많은 Register 제공
* Pointer → 8 bytes
* `long` → 8 bytes
* `int` → 여전히 4 bytes

### 8. `l` vs `q`

```text
l → 4 bytes (32-bit)
q → 8 bytes (64-bit)
```

```asm
movl    // 4 bytes
movq    // 8 bytes

addl    // 32-bit 덧셈
addq    // 64-bit 덧셈
```

* x86-64에서 32-bit Register에 값을 쓰면 **상위 32 bits가 0으로 설정됨.**

---

###### 질문·헷갈린 내용

###### 32-bit vs x86-64

32-bit에서는 함수 인자를 주로 **Stack(栈)을 통해 전달.
x86-64에서는 함수 인자를 **Register(寄存器)로 전달.
x86-64에서 첫 번째 인자 → %rdi, 두 번째 인자 → %rsi
따라서 x86-64 swap()에서는 Stack을 거치지 않아 단계 간소화.
이것은 **Calling Convention(调用约定)에서 정해진 규칙.

---

### 핵심 정리

> C 코드가 실제 CPU에서 실행되기까지는 **C → Assembly → Machine Code**로 변환되며, CPU는 **Register와 Memory를 이용해 instruction을 실행한다.**

> 특히 Assembly에서는 **값(value), 주소(address), Register, Memory의 관계**를 구분해서 이해하는 것이 핵심이다.


## 3.2 Machine Prog: Control (26.09.08)

### 1. Address Computation

```asm
D(Rb, Ri, S)
```

→ `D + Rb + Ri × S`

* `D`: Displacement(偏移量)
* `Rb`: Base Register(基址寄存器)
* `Ri`: Index Register(索引寄存器)
* `S`: Scale(比例因子), `1, 2, 4, 8`

`leal`은 메모리를 읽지 않고 주소 계산식을 계산한다.

### 2. Arithmetic

* `add`, `sub`, `imul`: 산술 연산
* `sal/shl`: 왼쪽 시프트
* `sar`: 산술 오른쪽 시프트
* `shr`: 논리 오른쪽 시프트
* `and`, `or`, `xor`: 논리 연산

컴파일러는 C 코드의 구조를 그대로 유지하지 않고 더 효율적인 instruction으로 최적화할 수 있다.

### 3. Condition Codes

산술 연산 결과에 따라 CPU가 설정하는 Flag:

* `CF`: Carry → unsigned
* `ZF`: 결과가 0
* `SF`: 결과의 sign
* `OF`: signed overflow

### 4. `cmp` / `test`

```asm
cmp
```

→ `Src1 - Src2`를 계산한 것처럼 Flag 설정. 결과는 저장하지 않음.

```asm
test
```

→ `Src1 & Src2`를 계산한 것처럼 Flag 설정. 결과는 저장하지 않음.

### 5. `setcc`

Condition Code에 따라 `0` 또는 `1`을 저장.

* `sete`: equal
* `setne`: not equal
* `setg/setge`: signed `>/<`
* `setl/setle`: signed `</<=`
* `seta`: unsigned `>`
* `setb`: unsigned `<`

### 6. Signed vs Unsigned

같은 비트 패턴도 signed와 unsigned에 따라 값이 다르게 해석된다.

* `jg`: signed greater
* `ja`: unsigned greater

Signed 비교에서는 `SF ^ OF`를 고려하고, unsigned 비교에서는 `CF`를 사용한다.

### 7. Branch / CMOV

**Conditional Branch(条件分支)**
→ 조건에 따라 **실행 위치**를 변경.

**Conditional Move(条件移动)**
→ 조건에 따라 **값**을 선택.

### 8. Loops

C의 반복문은 결국 **조건 검사 + Jump**로 구현된다.

* `do-while`: body 실행 후 조건 검사
* `while`: 조건 검사 후 body 실행
* `for`: `init → condition → body → update → condition`

`for`는 `while` + `update` 형태로 이해할 수 있다.

### 질문·헷갈린 내용

**CF vs OF**

### CF vs OF

**`CF` — Carry Flag(캐리 플래그)**

Unsigned(无符号) 연산에서 **표현 범위를 넘어 carry가 발생했는지** 나타낸다.

```text
8-bit unsigned: 0 ~ 255

255 + 1

  11111111
+ 00000001
-----------
1 00000000
↑
carry
```

8-bit 결과는 `00000000`이고, 최상위 비트 밖으로 carry가 발생했으므로:

```text
CF = 1
```

→ **Unsigned 범위를 넘었는지 판단할 때 사용**

---

**`OF` — Overflow Flag(오버플로우 플래그)**

Signed(有符号) 연산에서 **표현 범위를 넘어 overflow가 발생했는지** 나타낸다.

```text
8-bit signed: -128 ~ 127

127 + 1

  01111111
+ 00000001
-----------
  10000000
```

`10000000`은 signed에서 `-128`이므로:

```text
127 + 1 → -128
OF = 1
```

→ **Signed 범위를 넘었는지 판단할 때 사용**

---

### 핵심

```text
CF → Carry → Unsigned
OF → Overflow → Signed
```

같은 연산에서도 두 Flag의 의미는 다르다.

```text
127 + 1  → CF = 0, OF = 1
255 + 1  → CF = 1, OF = 0
```

따라서 `CF`와 `OF`를 구분해야 **signed 비교(`jg`)와 unsigned 비교(`ja`)**를 이해할 수 있다.

### 핵심 정리

* `cmp/test` → Condition Code(条件码) 설정
* `CF/ZF/SF/OF` → 조건 판단에 사용
* `Branch` → 실행 흐름 변경
* `CMOV` → 값 선택
* `Loops` → 조건 검사와 Jump의 조합
* 컴파일러는 의미를 유지하면서 더 효율적인 Assembly로 최적화한다.