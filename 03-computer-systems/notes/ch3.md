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


## 3.3 Machine Prog: Procedures (26.09.11)

### 1. Switch Statements

큰 `switch`는 **Jump Table(跳转表)** 로 구현될 수 있다.

```asm
cmpl $6, %eax
ja .L2
jmp *.L7(,%eax,4)
```

* `%eax = x`
* `ja` → unsigned 기준 `x > 6`이면 `default`
* `jmp *.L7(,%eax,4)` → jump table에서 target 주소를 읽어 간접 점프
* IA32에서는 주소가 4 byte라 `x * 4`
* fall-through는 코드 흐름을 이어서 구현

---

### 2. IA32 Stack

* Stack은 **낮은 주소 방향으로 성장**
* `%esp` = 현재 stack top 주소

```text
pushl:
%esp -= 4
Memory[%esp] = value

popl:
value = Memory[%esp]
%esp += 4
```

---

### 3. `call` / `ret`

`call`:

```text
Return Address를 stack에 저장
→ callee로 이동
```

`ret`:

```text
stack에서 Return Address를 꺼냄
→ caller로 복귀
```

예:

```text
%esp = 0x1000
call 실행
→ %esp = 0x0FFC
→ Memory[0x0FFC] = Return Address
```

---

### 4. `%eip` vs `%esp`

```text
%eip → 실행할 instruction 주소
%esp → 현재 stack top 주소
```

둘 다 CPU register이며, 같은 프로세스의 **Virtual Address Space(虚拟地址空间)** 안에서 서로 다른 영역을 가리킨다.

```text
Code / Text ← %eip
Stack       ← %esp
```

---

### 5. Stack Frame

함수가 호출될 때마다 해당 함수의 실행 정보를 저장하는 **Stack Frame(栈帧)** 이 생성된다.

```text
Arguments
Return Address
Old %ebp        ← %ebp
Saved Registers
Local Variables
Argument Build  ← %esp
```

* `%ebp` → 현재 함수 stack frame의 기준점
* `%esp` → 현재 stack top
* `8(%ebp)`, `12(%ebp)` 등으로 argument 접근
* `-4(%ebp)` 등으로 local variable 접근 가능

함수 시작:

```asm
pushl %ebp
movl %esp, %ebp
```

함수 종료 시 기존 `%ebp`를 복구한다.

---

### 6. Register Saving Convention

함수 호출 시 register 값이 망가지지 않도록 **Calling Convention(调用约定)** 을 사용한다.

```text
Caller-save
%eax %ecx %edx
→ caller가 필요하면 호출 전에 저장

Callee-save
%ebx %esi %edi
→ callee가 사용하면 저장 후 복구
```

예:

```asm
pushl %ebx
...
popl %ebx
```

`%eax`는 정수 반환값에도 사용된다.

---

### 7. Recursion

재귀 호출도 일반 함수 호출과 동일하다.

```text
pcount_r(5)
→ pcount_r(2)
→ pcount_r(1)
→ pcount_r(0)
```

호출될 때마다 새로운 stack frame이 생성된다.

```text
call
→ 새로운 frame 생성
→ recursive call
→ base case
→ ret을 통해 역순으로 복귀
```

재귀 호출 결과는 `%eax`에 반환된다.

예:

```asm
call pcount_r
```

이후:

```text
%eax = pcount_r(x >> 1)의 결과
```

그리고:

```asm
leal (%edx,%eax), %eax
```

를 통해:

```text
(x & 1) + recursive result
```

를 계산한다.

---

### 8. Pointer와 Local Variable

지역변수의 주소가 필요하면 해당 변수를 stack에 저장할 수 있다.

예:

```c
int localx = x;
incrk(&localx, 3);
```

PPT에서는:

```text
-4(%ebp) → localx
```

로 배치한다.

값 읽기:

```asm
movl -4(%ebp), %eax
```

→ `localx`의 값

주소 계산:

```asm
leal -4(%ebp), %eax
```

→ `&localx`

즉:

```text
movl → 메모리의 값
leal → 메모리 주소
```

다른 함수에 주소를 넘기면 해당 함수가 원래 stack frame의 변수를 직접 수정할 수 있다.

---

### 9. Procedure 전체 흐름

```text
Caller
↓
Argument 준비
↓
call
↓
Return Address 저장
↓
Stack Frame 생성
↓
함수 실행
↓
Return Value → %eax
↓
Register / %ebp 복구
↓
ret
↓
Caller 복귀
```

### 최종 핵심

```text
%ebp → 현재 함수 frame의 기준점
%esp → 현재 stack top

call → Return Address push
ret  → Return Address pop

Caller-save → %eax %ecx %edx
Callee-save → %ebx %esi %edi

Recursion → 호출마다 별도의 stack frame 생성

Pointer → 결국 memory address
leal → 주소 계산
```


## 3.4 Machine Prog: Data

### 1. Array

배열은 **연속된 메모리(连续内存)** 에 저장된다.

```c
T A[L];
```

전체 크기:

```text
L × sizeof(T)
```

원소 주소:

```text
&A[i]
= base + i × element size
```

예:

```c
int A[5];
```

```text
A[0] → base
A[1] → base + 4
A[2] → base + 8
```

Assembly:

```asm
movl (%edx,%eax,4), %eax
```

```text
address = base + index × 4
```

---

### 2. 2D Array

```c
int A[R][C];
```

C의 2차원 배열은 **Row-Major Order(行优先)** 로 저장된다.

```text
A[0][0] ... A[0][C-1]
A[1][0] ... A[1][C-1]
...
```

원소 주소:

```text
&A[i][j]
= base + (i × C + j) × element size
```

`int`라면:

```text
= base + (i × C + j) × 4
```

---

### 3. Nested Array vs Multi-Level Array

Nested Array:

```c
int A[4][5];
```

모든 데이터가 연속 배치된다.

```text
A[i][j]
→ base + (i × 5 + j) × 4
```

Multi-Level Array:

```c
int *A[4];
```

각 원소가 pointer이므로:

```text
A[i]
↓
row pointer 읽기
↓
A[i][j] 접근
```

즉 memory access가 한 단계 더 필요하다.

---

### 4. Matrix Access

고정 크기:

```c
int A[16][16];
```

한 row 크기:

```text
16 × 4 = 64 bytes
```

따라서:

```text
&A[i][j]
= base + i × 64 + j × 4
```

가변 크기:

```c
int A[n][n];
```

```text
row size = 4n

&A[i][j]
= base + i × 4n + j × 4
```

---

### 5. Structure

구조체는 여러 member를 하나의 연속된 메모리 영역에 저장한다.

```c
struct rec {
    int a[3];
    int i;
    struct rec *n;
};
```

PPT 기준:

```text
offset

0   a[0]
4   a[1]
8   a[2]
12  i
16  n
```

Member 접근:

```text
structure base + member offset
```

예:

```c
r->i
```

```text
r + 12
```

---

### 6. Struct 내부 Array

```c
r->a[i]
```

주소:

```text
base + member offset + i × element size
```

위 구조체에서는 `a`의 offset이 0이므로:

```text
r->a[i]
→ r + i × 4
```

---

### 7. Linked List

```c
while (r) {
    int i = r->i;
    r->a[i] = val;
    r = r->n;
}
```

Assembly 흐름:

```text
r + 12
→ r->i

r + i × 4
→ r->a[i]

r + 16
→ r->n

next pointer를 읽어서 다음 node 이동
```

---

### 최종 핵심

```text
Array
→ base + index × element size

2D Array
→ base + (row × columns + column) × element size

Struct
→ base + member offset

Struct Array Member
→ base + member offset + index × element size

Linked List
→ next pointer를 읽어서 다음 node로 이동
```

결국 Machine-Level에서 **Array, Struct, Matrix, Linked List는 모두 주소 계산 + Memory Access 문제**이다.