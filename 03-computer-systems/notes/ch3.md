# Chapter 3. Machine-Level Programming

## 3.1 Machine Prog: Basics

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