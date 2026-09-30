# Chapter 4. Computer Architecture

## 4.0 Overview

### Chapter 4의 목표

Chapter 3:
C → Assembly → Machine Code

Chapter 4:
Machine Instruction → CPU → Execution

핵심:

> Machine Instruction이 CPU 내부에서 어떻게 실행되는지 이해한다.

전체 흐름:

ISA
↓
Logic Design
↓
Sequential Processor
↓
Pipelining
↓
Performance

ISA는 CPU가 무엇을 해야 하는지 정의한다.

Microarchitecture(微体系结构)는
그 ISA를 CPU 내부에서 어떻게 구현하는지 다룬다.

주요 Hardware:

- Register
- ALU
- Memory
- MUX
- Control Logic

이들을 연결한 데이터 이동 경로를
Datapath(数据通路)라고 한다.

Sequential Processor의 기본 단계:

Fetch
↓
Decode
↓
Execute
↓
Memory
↓
Write Back

Pipeline은 여러 Instruction의 단계를 겹쳐 실행해
Throughput(吞吐量)을 높인다.

---

## 4.1 Instruction Set Architecture

### 1. ISA

ISA(Instruction Set Architecture, 指令集架构)는
Software와 Hardware 사이의 인터페이스이다.

Software
↓
ISA
↓
Hardware

Software는 CPU 내부 회로를 몰라도
Register, Instruction, Memory 등을 통해 Hardware를 사용한다.

---

### 2. Instruction과 Processor State

Instruction은 Processor State를 읽거나 변경한다.

예:

addq %rax, %rbx

의미:

%rbx = %rbx + %rax

Assembly는 실제 Memory에서
Machine Code byte로 저장된다.

Assembly
↓
Instruction Encoding
↓
Machine Code

Y86-64 Processor State:

- Program Registers
- Condition Codes
- Program Counter
- Program Status
- Memory

Condition Codes:

ZF → 결과가 0  
SF → 결과의 Sign  
OF → Signed Overflow

PC는 다음에 실행할 Instruction의 주소를 저장한다.

Memory는 Byte-addressable이며
Multi-byte 값은 Little Endian으로 저장한다.

핵심:

Processor State
+
Instruction
↓
New Processor State

즉:

> Instruction 실행은 Processor State를 변화시키는 과정이다.

---

### 3. Y86-64

Y86-64는 x86-64를 단순화한 교육용 ISA이다.

목적은 Y86-64 명령어 자체를 외우는 것이 아니라:

> ISA가 실제 Processor에서 어떻게 구현되는지 이해하는 것

이다.

---

### 4. Instruction Encoding

Y86-64 Instruction의 첫 byte:

icode | ifun

- icode → Instruction 종류
- ifun → 세부 기능

Register가 필요하면:

rA | rB

형태의 Register Byte를 추가한다.

각 Register ID는 4 bits이다.

사용하지 않는 Register field는 F를 사용한다.

중요:

> rA와 rB의 실제 역할은 Instruction마다 다르다.

64-bit Immediate, Displacement, Address가 필요하면
추가로 8 bytes를 사용한다.

따라서 Y86-64 Instruction 길이는:

1 ~ 10 bytes

이다.

---

### 5. 주요 Y86-64 Instructions

halt
→ 프로그램 종료

nop
→ 아무 작업 없음

rrmovq
→ Register → Register

irmovq
→ Immediate → Register

rmmovq
→ Register → Memory

mrmovq
→ Memory → Register

OPq
→ Arithmetic / Logic

cmovXX
→ Conditional Move

jXX
→ Conditional Jump

pushq / popq
→ Stack

call / ret
→ Function Call / Return

---

### 6. Byte Encoding

예:

addq %rax, %rsi

는 Instruction 종류와 Register ID를 이용해
Machine Code byte로 변환된다.

즉:

> Byte Encoding은 Assembly Instruction을
> 실제 Machine Code byte로 표현하는 과정이다.

Move Instruction에서는
rA / rB의 의미가 Instruction마다 달라진다.

예:

rmmovq rA, D(rB)

rA → 저장할 데이터  
rB → Memory Address 계산용 Base

mrmovq D(rB), rA

rB → Base Register  
rA → Memory에서 읽은 값을 받을 Register

---

### 7. OPq / Conditional Move / Jump

OPq는:

- addq
- subq
- andq
- xorq

같은 연산을 수행한다.

연산 결과와 함께:

ZF / SF / OF

도 갱신한다.

cmovXX는 Condition Code를 확인해
조건이 참일 때 Register 값을 이동한다.

jXX는 조건에 따라:

PC = Dest

를 수행한다.

즉:

> Jump의 핵심은 Program Counter를 변경하는 것이다.

---

### 8. Stack

Stack Top은 %rsp가 가리킨다.

Stack은 낮은 주소 방향으로 성장한다.

pushq:

%rsp -= 8  
Memory[%rsp] = value

popq:

value = Memory[%rsp]  
%rsp += 8

즉:

push
→ Stack Pointer 감소 후 저장

pop
→ 값 읽은 후 Stack Pointer 증가

---

### 9. Function Call

call Dest:

1. Return Address를 Stack에 저장
2. PC = Dest

Return Address는
call 다음 Instruction의 주소이다.

ret:

1. Stack에서 Return Address를 읽음
2. PC = Return Address

즉:

call
→ 돌아올 주소 저장 후 함수로 이동

ret
→ 저장했던 주소로 복귀

---

### 10. Program Status

AOK
→ 정상 실행

HLT
→ halt 실행

ADR
→ 잘못된 Memory Address

INS
→ 잘못된 Instruction

AOK이면 계속 실행하고,
그 외 상태에서는 실행을 중지한다.

---

### 11. Y86-64 Code의 특징

Y86-64는 x86-64보다
Instruction과 Addressing Mode가 단순하다.

x86-64에서는:

base + index × scale

같은 주소 계산이 가능하지만,
Y86-64에는 Scaled Addressing Mode가 없다.

따라서 배열을 순회할 때는:

현재 값 읽기
↓
Pointer 증가
↓
다음 값 읽기

형태로 직접 구현한다.

---

### 12. Condition Code를 이용한 검사

예:

andq %rdx, %rdx

x & x = x

이므로 값 자체는 변하지 않는다.

하지만 OPq이므로 Condition Code는 갱신된다.

%rdx == 0
→ ZF = 1

따라서:

andq
↓
ZF 설정
↓
je / jne

형태로 값이 0인지 검사할 수 있다.

---

### 13. Program Structure

Y86-64 Program은 실행 전에:

- Stack 설정
- Program Data 배치

를 해야 한다.

기본 흐름:

Initialization
↓
Main
↓
Function
↓
Return
↓
halt

Function Argument는 Register를 통해 전달할 수 있다.

예:

%rdi
→ Function Argument

---

### 14. Assembler / Simulator

Y86-64 Source:

len.ys

Assembler:

yas len.ys

결과:

len.yo

Assembler가 처리하는 것:

- Instruction Encoding
- Register ID
- Label Address
- Little Endian

Simulator:

yis len.yo

확인 가능:

- Register 변화
- Memory 변화
- PC
- Status
- Condition Code

즉 Simulator를 통해:

Instruction
↓
Processor State 변화

를 확인할 수 있다.

---

## CISC vs RISC

### 15. CISC

CISC(Complex Instruction Set Computer, 复杂指令集计算机)

특징:

- 복잡한 Instruction
- Memory Operand 사용 가능
- 복잡한 Address 계산 가능
- Condition Code 사용

하나의 Instruction이
여러 작업을 수행할 수 있다.

---

### 16. RISC

RISC(Reduced Instruction Set Computer, 精简指令集计算机)

특징:

- 단순한 Instruction
- Register 중심
- Load / Store Architecture

구조:

Memory
↓ Load
Register
↓ Arithmetic
Register
↓ Store
Memory

즉:

Arithmetic
→ Register끼리 수행

Memory 접근
→ Load / Store만 수행

핵심:

> Memory 접근과 연산을 분리해 Instruction을 단순하게 만든다.

---

### 17. MIPS

MIPS는 대표적인 RISC ISA이다.

예:

addu
→ Register끼리 Arithmetic

lw
→ Memory → Register

sw
→ Register → Memory

beq
→ 조건에 따라 Branch

즉:

lw / sw
→ Memory 접근

addu
→ Memory 접근 없이 Register 연산

둘 다 RISC Instruction이다.

---

### 18. CISC와 RISC의 현재

과거에는 CISC와 RISC 중
어느 구조가 더 좋은지 논쟁이 컸다.

현대 Processor에서는
ISA만으로 성능이 결정되지는 않는다.

x86-64도 여러 RISC 특징을 사용한다.

Embedded에서는 RISC가 잘 맞는다.

이유:

단순한 Hardware
↓
작은 Chip
↓
낮은 비용
↓
낮은 전력 소비

ARM이 대표적인 예이다.

---

### 4.1 최종 핵심

ISA
→ Software와 Hardware 사이의 Interface

Y86-64
→ x86-64를 단순화한 교육용 ISA

Processor State
→ Register + CC + PC + Status + Memory

Instruction Encoding
→ icode + ifun
→ 필요하면 Register Byte
→ 필요하면 8-byte 값

OPq
→ 연산 + Condition Code 변경

jXX
→ PC 변경

Stack
→ %rsp 사용

pushq
→ %rsp 감소 후 저장

popq
→ 값 읽은 후 %rsp 증가

call
→ Return Address 저장 + 함수로 이동

ret
→ Return Address 복원

Assembler
→ Assembly를 Machine Code로 변환

Simulator
→ Instruction 실행에 따른 State 변화 확인

CISC
→ 복잡한 Instruction 중심

RISC
→ 단순한 Instruction
→ Register 중심
→ Load / Store Architecture

최종 핵심:

> **Y86-64 명령어의 세부 번호를 외우는 것보다,
> Instruction이 Processor State를 어떻게 변화시키고
> 그것이 CPU에서 어떻게 구현되는지 이해하는 것이 중요하다.**

---

## 4.2 Logic Design

### 1. Logic Design의 기본 역할

Hardware의 기본 역할은 크게 세 가지이다.

- Communication
- Computation
- Storage

Digital Hardware는 정보를 bit:

0 / 1

로 표현한다.

실제 전압은 연속적이지만,
Low Voltage와 High Voltage 범위를 각각 0과 1로 해석한다.

중간에는 Guard Range를 두어
Noise가 있어도 안정적으로 0과 1을 구분한다.

---

### 2. Logic Gate와 Combinational Logic

Logic Gate는 Boolean Function을 계산한다.

예:

- AND
- OR
- NOT
- XOR

실제 Gate에는 Input 변화 후
Output이 바뀌기까지 약간의 Delay가 존재한다.

Combinational Circuit(组合逻辑电路)은
Logic Gate를 Cycle 없이 연결한 회로이다.

특징:

- 현재 Input만으로 Output 결정
- State 없음
- Input이 변하면 Output도 반응

즉:

Current Input
↓
Combinational Logic
↓
Current Output

이다.

---

### 3. Equality와 Multiplexor

Equality Circuit은 두 값이 같은지 검사한다.

A == B

결과:

같음 → 1  
다름 → 0

Multiplexor(MUX, 多路选择器)는
여러 Input 중 하나를 선택한다.

예:

s = 1
→ A 선택

s = 0
→ B 선택

즉:

Control Signal
↓
MUX
↓
선택된 Input

CPU에서는 여러 데이터 경로 중
어떤 값을 사용할지 선택할 때 MUX를 사용한다.

---

### 4. HCL Case Expression

HCL에서는 MUX와 같은 선택 동작을
Case Expression으로 표현한다.

예:

[
    a : A;
    b : B;
    1 : C;
]

위에서부터 조건을 확인하여
처음 참인 조건의 값을 선택한다.

마지막:

1 : C;

는 항상 참이므로 Default 역할을 한다.

---

### 5. ALU

ALU(Arithmetic Logic Unit, 算术逻辑单元)는
Arithmetic / Logic 연산을 수행하는 Combinational Logic이다.

Control Signal에 따라:

- Add
- Sub
- And
- Xor

등을 선택한다.

구조:

Input A
+
Input B
+
Control
↓
ALU
↓
Result

ALU의 결과를 이용해:

ZF / SF / OF

같은 Condition Code도 계산할 수 있다.

---

### 6. Combinational Logic vs Sequential Logic

Combinational Logic:

- 현재 Input만으로 Output 결정
- State 없음
- Clock 필요 없음

Sequential Logic(时序逻辑):

- 이전 State를 저장
- 현재 Input + 이전 State 사용
- 보통 Clock을 이용해 State 갱신

즉:

Combinational Logic
→ 계산

Sequential Logic
→ 저장 / State 유지

---

### 7. Bistable Element와 Latch

Storage를 위해서는
0 또는 1의 상태를 유지할 수 있어야 한다.

Bistable Element는:

- State 0
- State 1

두 개의 안정된 상태를 가진다.

Feedback을 이용해
이전 상태를 유지한다.

Combinational Circuit에는 Feedback이 없지만,
Storage Circuit은 State를 유지하기 위해 Feedback을 사용한다.

---

### 8. Transparent Latch와 Edge-Triggered Storage

Transparent Latch는
Clock이 활성화된 동안 Input D가 Output Q로 전달된다.

즉:

Clock Active
→ D 변화
→ Q 변화

Edge-Triggered 방식은
Clock의 특정 Edge에서만 Input을 저장한다.

Rising Edge:

0 → 1

이 되는 순간:

D
↓
Register에 저장
↓
Q 갱신

그 외 시간에는 Q가 유지된다.

CPU에서는 여러 Register의 State를
같은 시점에 갱신해야 하므로
Edge-Triggered 방식이 중요하다.

흐름:

계산
↓
Clock Rising Edge
↓
새 State 저장
↓
다음 계산

---

### 9. Register

Register는 여러 개의
Edge-Triggered Storage Element를 묶어
하나의 Word를 저장하는 Hardware이다.

예:

8-bit Register
→ 8개의 bit 저장

동작:

Input = y
State = x
Output = x

Rising Edge
↓

State = y
Output = y

즉:

> Register는 대부분의 시간 동안 Input과 Output을 분리하고,
> Rising Edge에서만 Input을 새로운 State로 저장한다.

---

### 10. State Machine

State Machine의 기본 구조:

Current State
↓
Combinational Logic
↓
Next State 계산
↓
Clock Edge
↓
Register
↓
New State

즉:

> Combinational Logic은 다음 값을 계산하고,
> Register는 Clock Edge에서 그 값을 State로 저장한다.

Accumulator 예제에서는:

Load = 1
→ 현재 Input을 그대로 저장

Load = 0
→ 기존 Out + 현재 Input 저장

예:

x0
↓
x0 + x1
↓
x0 + x1 + x2
↓
Load
↓
x3
↓
x3 + x4
↓
x3 + x4 + x5

---

### 11. Random-Access Memory와 Register File

Random-Access Memory는
여러 개의 Word를 저장한다.

Address Input이:

> 어떤 Word를 읽거나 쓸지 지정한다.

Register File은 Program Register들의 값을 저장한다.

예:

- %rax
- %rsp
- %rdi

Register Identifier가 Address 역할을 한다.

Y86-64에서:

0xF
→ No Register
→ Read / Write하지 않음

---

### 12. Register File의 Port

Register File에는 여러 Port가 존재할 수 있다.

예:

Read Port A:

srcA
→ 읽을 Register ID

valA
→ 읽어온 값

Read Port B:

srcB
→ 읽을 Register ID

valB
→ 읽어온 값

Write Port:

dstW
→ 쓸 Register ID

valW
→ 저장할 값

따라서 한 Cycle에서
여러 Register를 동시에 읽거나 쓸 수 있다.

예:

addq %rax, %rbx

에서는:

%rax Read
+
%rbx Read
↓
ALU
↓
결과를 %rbx에 Write

하는 구조가 가능하다.

---

### 13. Register File Timing

Register File에서 Read와 Write는 동작 방식이 다르다.

Read:

srcA / srcB 변경
↓
약간의 Delay
↓
valA / valB 출력

즉:

> Read는 Combinational Logic처럼 동작한다.

Clock Edge가 필요하지 않다.

Write:

dstW + valW 준비
↓
Clock Rising Edge
↓
Register 값 변경

즉:

> Write는 State를 변경하므로 Rising Edge에서 수행한다.

핵심:

Read
→ 현재 State를 읽음
→ Combinational Logic

Write
→ 새로운 State를 저장
→ Sequential Logic

---

### 14. Hardware Control Language

HCL(Hardware Control Language)은
Processor의 Control Logic을 표현하기 위한
간단한 Hardware Description Language이다.

주요 Data Type:

bool
→ Boolean

int
→ Word

HCL의 int는 C의 32-bit int를 의미하지 않는다.

Word 크기는 Hardware에 따라 달라질 수 있다.

Statement 예:

bool a = bool-expr;

int A = int-expr;

---

### 15. HCL Operations

Boolean Logic:

a && b  
a || b  
!a

Word Comparison:

A == B  
A != B  
A < B  
A <= B  
A >= B  
A > B

Set Membership:

A in { B, C, D }

는:

A == B || A == C || A == D

와 같다.

Case Expression:

[
    a : A;
    b : B;
    c : C;
]

조건을 위에서부터 검사하여
처음 참인 조건의 Word를 반환한다.

이 구조는 Hardware의 MUX를 표현하는 데 사용된다.

---

### 4.2 최종 핵심

Combinational Logic
→ 현재 Input으로 Output 계산
→ State 없음

Sequential Logic
→ 이전 State 저장
→ Clock을 이용해 State 갱신

MUX
→ 여러 Input 중 하나 선택

ALU
→ Arithmetic / Logic 계산

Bistable / Latch
→ bit State 저장

Register
→ 하나의 Word 저장
→ Rising Edge에서 갱신

State Machine
→ Combinational Logic + Register

Register File
→ 여러 Program Register 저장
→ 여러 Read / Write Port 가능

Register File Read
→ Combinational
→ Address가 바뀌면 Output 변화

Register File Write
→ Sequential
→ Rising Edge에서 State 변경

HCL
→ Hardware Control Logic을 표현

전체 핵심:

Current State
↓
Combinational Logic
↓
Next State 계산
↓
Clock Rising Edge
↓
Register에 저장
↓
New State

> **Logic Design의 핵심은
> Combinational Logic으로 값을 계산하고,
> Sequential Logic으로 State를 저장하는 구조를 이해하는 것이다.**

## 4.3 Sequential Implementation

### 1. SEQ 기본 구조

Y86-64 Instruction은 공통적으로 다음 단계를 따른다.

```text
Fetch
→ Decode
→ Execute
→ Memory
→ Write Back
→ PC Update
```

Instruction마다 구조가 달라지는 것이 아니라, **각 Stage에서 어떤 값과 Hardware를 사용하는지가 달라진다.**

---

### 2. 주요 Signal

```text
valA, valB
→ Register File에서 읽은 값

valC
→ Instruction 내부의 Constant / Destination / Displacement

valP
→ 다음 순차 Instruction 주소

valE
→ ALU 결과

valM
→ Memory에서 읽은 값
```

Register 선택 Signal:

```text
srcA, srcB
→ 읽을 Register

dstE
→ valE를 저장할 Register

dstM
→ valM을 저장할 Register
```

`F(0xF)`는 **No Register**를 의미한다.

---

### 3. Fetch

Fetch에서는 PC를 이용해 Instruction을 읽고 다음 정보를 추출한다.

```text
icode / ifun
rA / rB
valC
valP
```

Control Logic:

```text
need_regids
→ Register Byte 필요 여부

need_valC
→ 8-byte Constant 필요 여부

instr_valid
→ 유효한 Instruction인지 확인
```

`valP`:

```text
valP = PC + Instruction Length
```

---

### 4. Decode

Register File에서 operand를 읽는다.

```text
srcA → valA
srcB → valB
```

Register File은:

```text
Read
→ Combinational

Write
→ Clock Rising Edge
```

예:

```asm
addq %rax, %rbx
```

```text
srcA = %rax
srcB = %rbx
dstE = %rbx
dstM = F
```

---

### 5. Execute

ALU가 연산, 주소 계산, Stack Pointer 계산 등을 수행한다.

```text
aluA
aluB
alufun
↓
ALU
↓
valE
```

`OPq`:

```text
valE = valB OP valA
```

Memory Address 계산:

```text
valE = valB + valC
```

Stack:

```text
pushq / call
→ %rsp - 8

popq / ret
→ %rsp + 8
```

`OPq`는 `ifun`으로 ADD/SUB/AND/XOR를 선택하고,
대부분의 다른 Instruction은 `ALUADD`를 사용한다.

---

### 6. Condition Code

ALU는 필요할 경우:

```text
ZF
SF
OF
```

를 계산한다.

```text
Set CC = 1
→ Condition Code 갱신
```

Conditional Move에서 조건이 거짓이면:

```text
dstE = F
```

로 만들어 Register Write를 취소한다.

---

### 7. Memory

Memory 접근 여부와 주소를 Control Logic이 결정한다.

Memory Read:

```text
mrmovq
popq
ret
```

Memory Write:

```text
rmmovq
pushq
call
```

주소:

```text
rmmovq / mrmovq / pushq / call
→ mem_addr = valE

popq / ret
→ mem_addr = valA
```

`popq`, `ret`은 **old %rsp 위치를 읽어야 하므로 `valA`를 사용**한다.

---

### 8. Stack Instruction

#### `pushq`

```text
valE = %rsp - 8
Memory[valE] = valA
%rsp = valE
```

#### `popq`

```text
valA = old %rsp
valE = old %rsp + 8
valM = Memory[valA]

%rsp = valE
rA = valM
```

```text
valE
→ 새로운 %rsp

valM
→ Stack에서 꺼낸 데이터
```

---

### 9. `call` / `ret`

#### `call Dest`

```text
valC = Dest
valP = Return Address

valE = %rsp - 8
Memory[valE] = valP
%rsp = valE

PC = valC
```

```text
valP
→ 나중에 돌아올 주소

valC
→ 지금 이동할 함수 주소
```

#### `ret`

```text
valA = old %rsp
valE = old %rsp + 8
valM = Memory[valA]

%rsp = valE
PC = valM
```

`valM`은 Stack에서 읽은 **Return Address**이다.

---

### 10. Jump

조건 Jump:

```text
Cnd = Cond(CC, ifun)
```

다음 PC:

```text
Cnd = 1
→ PC = valC

Cnd = 0
→ PC = valP
```

즉:

```text
PC = Cnd ? valC : valP
```

MUX가 실제 CPU Control에 사용되는 대표적인 예이다.

---

### 11. PC Update

```text
일반 Instruction
→ PC = valP

call
→ PC = valC

Taken Jump
→ PC = valC

ret
→ PC = valM
```

---

### 12. Processor Status

```text
SAOK
→ 정상

SHLT
→ halt

SADR
→ Memory Address Error

SINS
→ Invalid Instruction
```

---

### 13. Clock과 State Update

SEQ는 다음 구조로 동작한다.

```text
Current State
→ Combinational Logic
→ Next State 계산
→ Clock Rising Edge
→ New State
```

Clock Rising Edge에서 갱신되는 주요 State:

```text
PC
Register File
Condition Codes
Data Memory
```

중간 계산값이 즉시 State를 바꾸는 것이 아니라, **Rising Edge에서 한 번에 갱신**된다.

---

### 14. SEQ의 한계

SEQ는 **한 Instruction을 한 Clock Cycle 안에 전부 실행**한다.

```text
Instruction Memory
↓
Register File
↓
ALU
↓
Data Memory
```

가장 오래 걸리는 Instruction의 전체 경로가 **Critical Path**가 된다.

따라서:

```text
긴 Critical Path
→ 긴 Clock Period
→ 낮은 Clock Frequency
→ 낮은 성능
```

또한 한 Instruction이 모든 Hardware를 항상 사용하는 것이 아니므로 Hardware가 쉬는 시간도 많다.

---

### 핵심 흐름

```text
Instruction
↓
Fetch
↓
Decode
↓
Execute
↓
Memory
↓
Write Back
↓
PC Update
↓
Clock Rising Edge
↓
New State
```

SEQ의 핵심 문제:

> **Instruction 전체를 한 Cycle 안에 끝내야 하므로 Clock을 가장 긴 Critical Path에 맞춰야 한다.**

이 한계를 해결하기 위해 다음 단계에서 **Pipelining**을 사용한다.


## 4.4 Pipelined Implementation

### 4.4.1 파이프라이닝의 기본 개념

파이프라이닝(Pipelining, 流水线)은 명령어 처리를 여러 **Stage(阶段)** 로 나누고, 여러 명령어를 서로 다른 Stage에서 동시에 처리하는 방식이다.

핵심 목적은 **Latency 감소가 아니라 Throughput(吞吐量) 증가**이다.

예를 들어 조합 논리 `300 ps`와 Register Overhead `20 ps`가 있다면:

$$
T_{clock} = 300 + 20 = 320\text{ ps}
$$

$$
Throughput \approx 3.12\text{ GIPS}
$$

이를 3개의 `100 ps` Stage로 나누면:

$$
T_{clock} = 100 + 20 = 120\text{ ps}
$$

$$
Throughput \approx 8.33\text{ GIPS}
$$

하지만 하나의 연산이 3개의 Stage를 모두 통과하는 Latency는:

$$
Latency = 120 \times 3 = 360\text{ ps}
$$

즉, **개별 작업은 더 빨라지지 않아도 전체 처리량은 증가할 수 있다.**

---

### 4.4.2 Pipeline의 한계

Clock Cycle은 **가장 느린 Stage의 Delay + Register Overhead**에 의해 결정된다.

$$
T_{clock} = \max(\text{Stage Delay}) + \text{Register Overhead}
$$

따라서 Stage 간 Delay가 불균형하면 빠른 Stage에서 대기 시간이 발생한다.

또한 Pipeline을 너무 잘게 나누면 각 Stage의 Logic Delay는 감소하지만 **Register Overhead의 비중은 증가**한다.

---

### 4.4.3 5-Stage Pipeline

Y86-64 PIPE는 명령어 실행을 다음 5단계로 나눈다.

| Stage | 역할 |
|---|---|
| **F - Fetch (取指)** | 명령어 읽기 및 PC 계산 |
| **D - Decode (译码)** | Register 읽기 |
| **E - Execute (执行)** | ALU 연산 |
| **M - Memory (访存)** | Memory 읽기/쓰기 |
| **W - Write Back (写回)** | Register 갱신 |

```text
I1: F → D → E → M → W
I2:     F → D → E → M → W
I3:         F → D → E → M → W
```

각 Stage 사이에는 **Pipeline Register**가 존재하여 중간 결과를 저장한다.

Signal 표기법:

- `S_Field`: Stage S의 Pipeline Register에 저장된 값
- `s_Field`: Stage S에서 현재 계산된 값

예:

$$
e\_valE \rightarrow M\_valE
$$

즉 `e_valE`는 Execute Stage에서 현재 계산된 값이고, 다음 Clock 이후 `M_valE`로 저장된다.

---

### 4.4.4 Data Hazard

한 명령어가 Register에 값을 쓰고 다음 명령어가 그 값을 읽는 대표적인 의존성이 **RAW (Read After Write)** 이다.

```asm
irmovq $50, %rax      # Write %rax
addq   %rax, %rbx     # Read %rax
```

관계는 다음과 같다.

```text
WRITE %rax
    ↓
READ %rax
    ↓
RAW Dependency
```

Pipeline에서는 앞 명령어가 아직 Write Back을 하지 않았는데 뒤 명령어가 Decode에서 Register를 읽으면 **오래된 값**을 읽을 수 있다.

이를 **Data Hazard(数据冒险)** 라고 한다.

올바른 값이 `M_valE`, `e_valE` 등에 이미 존재하더라도 Register File에는 아직 반영되지 않았을 수 있다.

`nop`을 삽입하여 명령어 사이의 간격을 벌리면 문제를 피할 수 있지만 성능이 감소한다.

---

### 4.4.5 PC Prediction과 Control Hazard

Pipeline은 매 Clock Cycle마다 새로운 명령어를 Fetch해야 하므로 **다음 PC를 미리 예측**한다.

| Instruction | Predicted PC |
|---|---|
| 일반 명령어 | `valP` |
| `call`, Unconditional Jump | `valC` |
| Conditional Jump | `valC` (Taken 예측) |
| `ret` | 예측하지 않음 |

Conditional Branch에서:

- **Taken**: Branch Target으로 이동
- **Not Taken**: 바로 다음 명령어인 **Fall-through**로 진행

예측과 실제 결과가 다르면:

```text
Branch
  │
  ├─ 예측 경로 → 잘못 Fetch한 명령어 → 폐기
  │
  └─ 실제 경로 → 올바른 PC에서 다시 Fetch
```

이러한 문제를 **Control Hazard(控制冒险)** 라고 한다.

---

### 4.4.6 핵심 정리

```text
Pipelining
│
├─ 목적: Throughput 증가
│
├─ 5 Stages
│   └─ F → D → E → M → W
│
├─ Data Dependency
│   └─ RAW → Data Hazard
│
└─ Control Dependency
    ├─ Branch Misprediction
    └─ Return
        ↓
      Control Hazard
```

핵심은 다음과 같다.

> **Pipeline은 여러 명령어를 동시에 처리하여 Throughput을 높이지만, 명령어 간 Data Dependency와 Control Dependency로 인해 Hazard가 발생할 수 있다.**

좋아. 네가 작성한 **4.4.5 → 4.4.6 뒤에 그대로 붙일 수 있는 형태**로 이번 Part II PPT를 정리하면 아래가 적당해. PPT의 순서인 **Stalling → Forwarding → Load/Use → Control Hazard → Control Combination**을 유지할게. :chatgpt-content-reference{index="0"}

### 4.4.7 Data Hazard와 Stalling

앞선 명령어가 Register에 값을 쓰고, 뒤따르는 명령어가 그 Register를 Source로 사용하면 **Data Hazard(数据冒险)** 가 발생할 수 있다.

가장 단순한 해결 방법은 **Stall(停顿)** 이다.

```text
Stall 발생
│
├─ Fetch  → 현재 상태 유지
├─ Decode → 현재 Instruction 유지
└─ Execute → Bubble 삽입
```

- **Stall**: 올바른 Instruction을 현재 Stage에 유지
- **Bubble**: Pipeline에 동적으로 삽입되는 `nop`과 같은 상태

즉, 필요한 값이 준비될 때까지 Consumer Instruction을 Decode에서 기다리게 하고 Execute에는 Bubble을 삽입한다. :chatgpt-content-reference{index="1"}

Pipeline Register는 다음 세 가지 방식으로 동작한다.

| Mode | 동작 |
|---|---|
| Normal | 다음 값을 정상적으로 저장 |
| Stall | 현재 값을 그대로 유지 |
| Bubble | `nop` 상태를 저장 |

Pipeline Control Logic이 Hazard를 감지하여 각 Pipeline Register의 동작을 결정한다. :chatgpt-content-reference{index="2"}

---

### 4.4.8 Data Forwarding

모든 Data Hazard를 Stall로 해결하면 Pipeline 성능이 크게 떨어진다.

따라서 **Data Forwarding(数据转发)** 을 사용한다.

```text
값을 생성한 Instruction
        │
        ├─ E
        ├─ M
        └─ W
        │
        ↓
Decode Stage에서 직접 사용
```

Register File에 Write Back될 때까지 기다리지 않고, Pipeline 내부에 이미 존재하는 값을 필요한 Instruction으로 직접 전달한다.

Forwarding Source는 다음과 같다.

- Execute: `valE`
- Memory: `valE`, `valM`
- Write Back: `valE`, `valM`

:chatgpt-content-reference{index="3"} :chatgpt-content-reference{index="4"}

여러 Stage에서 같은 Register에 대한 Forwarding 후보가 존재한다면 **Program의 순차 실행 결과와 동일하도록 가장 최근 값을 선택**해야 한다. PPT에서는 이를 위해 가장 앞쪽 Pipeline Stage의 일치하는 값을 우선 사용한다. :chatgpt-content-reference{index="5"}

---

### 4.4.9 Load/Use Hazard

Forwarding으로도 해결할 수 없는 대표적인 경우가 **Load/Use Hazard**이다.

```asm
mrmovq 0(%rax), %rdx
addq   %rdx, %rbx
```

`mrmovq`가 Memory에서 읽은 값은 **Memory Stage에서야 준비**되지만, 바로 뒤의 `addq`는 그 값을 더 일찍 필요로 한다.

따라서 Forwarding만으로 해결할 수 없다. :chatgpt-content-reference{index="6"}

해결 방법은:

```text
Load
 ↓
1 Cycle Stall
 ↓
Memory에서 값 생성
 ↓
Forwarding
 ↓
Use
```

즉 **1 Cycle Stall + Forwarding**을 사용한다.

Load/Use Hazard 발생 시:

```text
F → Stall
D → Stall
E → Bubble
M → Normal
W → Normal
```

Load Instruction은 계속 진행시키면서 Consumer Instruction을 Decode에서 한 Cycle 기다리게 한다. :chatgpt-content-reference{index="7"}

---

### 4.4.10 Branch Misprediction 처리

PIPE는 Conditional Branch를 기본적으로 **Taken으로 예측**한다.

실제 결과가 Not Taken이면 Branch Target에서 가져온 Instruction들은 잘못된 경로의 Instruction이다.

```text
Branch
 │
 ├─ Predicted Taken
 │      ↓
 │   Target Fetch
 │      ↓
 │   Prediction 실패
 │
 └─ Wrong-path Instructions
          ↓
        Bubble
```

Misprediction을 감지하면 잘못 Fetch된 Instruction을 **Bubble로 교체하여 취소**한다. 이 시점에는 해당 Instruction들이 아직 부작용을 발생시키지 않았기 때문에 안전하게 제거할 수 있다. :chatgpt-content-reference{index="8"}

PPT의 Control은 다음과 같다.

```text
F → Normal
D → Bubble
E → Bubble
M → Normal
W → Normal
```

Branch Misprediction으로 인해 **2 Clock Cycles의 손실**이 발생한다. :chatgpt-content-reference{index="9"} :chatgpt-content-reference{index="10"}

---

### 4.4.11 `ret` Control Hazard

`ret`은 Conditional Branch와 다른 문제가 있다.

Conditional Branch는 Branch Target을 알고 있기 때문에 다음 PC를 예측할 수 있지만, `ret`의 Return Address는 **Stack에서 읽어야 한다.**

따라서 Return Address가 준비될 때까지 Fetch를 진행할 수 없다.

```text
ret
 │
 ├─ D
 ├─ E        → Fetch Stall
 ├─ M
 │
 └─ W
     ↓
Return Address 준비
     ↓
Fetch 재개
```

`ret`이 Decode, Execute, Memory Stage를 통과하는 동안 **Fetch를 Stall**하고 Decode에는 Bubble을 삽입한다. Write Back Stage에 도달하면 Stall을 해제한다. :chatgpt-content-reference{index="11"}

```text
F → Stall
D → Bubble
E → Normal
M → Normal
W → Normal
```

이 과정에서 **3 Clock Cycles의 손실**이 발생한다. :chatgpt-content-reference{index="12"} :chatgpt-content-reference{index="13"}

---

### 4.4.12 Multiple Hazards와 Pipeline Control

실제 Pipeline에서는 여러 Hazard가 **같은 Clock Cycle에 동시에 발생**할 수 있다.

대표적인 경우가:

```text
Load/Use Hazard
      +
     ret
```

이다.

각각 따로 처리하면:

```text
Load/Use → D = Stall
ret      → D = Bubble
```

가 되어 같은 Pipeline Register에 **Stall과 Bubble이 동시에 요구되는 충돌**이 발생한다. PPT에서는 이러한 조합이 초기 Control Logic에서 Pipeline Error를 발생시킨다고 설명한다. :chatgpt-content-reference{index="14"}

이를 해결하기 위해 **Load/Use Hazard에 우선순위**를 준다.

```text
Load/Use + ret
       ↓
Load/Use 우선
       ↓
F → Stall
D → Stall
E → Bubble
M → Normal
W → Normal
```

따라서 `ret`을 제거하지 않고 Decode Stage에서 한 Cycle 더 유지한다. :chatgpt-content-reference{index="15"}

Control Logic에서는 `ret`으로 `D_bubble`을 발생시키기 전에 **Load/Use Hazard가 아닌지 확인**하도록 조건을 수정한다. :chatgpt-content-reference{index="16"}

---

### 4.4.13 Pipelined Implementation 최종 정리

```text
Pipeline Hazard
│
├─ Data Hazard
│   │
│   ├─ 일반적인 Dependency
│   │      └─ Forwarding
│   │          → Performance Penalty 없음
│   │
│   └─ Load/Use Hazard
│          └─ 1 Cycle Stall + Forwarding
│
└─ Control Hazard
    │
    ├─ Branch Misprediction
    │      └─ Wrong-path → Bubble
    │          → 2 Cycles 손실
    │
    └─ ret
           └─ Fetch Stall
               → 3 Cycles 손실

Multiple Hazards
└─ 동시에 발생할 수 있음
    └─ Control Priority 필요
```

핵심은 다음과 같다.

> **Pipeline의 Hazard 처리는 Forwarding, Stall, Bubble을 적절히 조합하는 문제이며, 여러 Hazard가 동시에 발생하는 경우까지 고려하여 Pipeline Control Logic을 설계해야 한다.**

## 4.5 Processor Architecture — Wrap-Up

### 1. Exception Handling

**Exception(예외, 异常)**은 processor가 정상적인 실행을 계속할 수 없는 상황이다.

대표적인 원인:

- `halt` instruction
- 잘못된 instruction/data address
- invalid instruction

Pipeline에서는 Exception을 일으킨 instruction 뒤의 명령이 이미 실행 중일 수 있다.

따라서 Exception 처리의 핵심은:

> **Exception 이전 instruction은 정상적으로 완료하고, 이후 instruction은 Processor State를 변경하지 않도록 하는 것**

이다.

Processor State에는 다음과 같은 것들이 포함된다.

- Register
- Memory
- Condition Code
- PC

#### Exception Ordering

각 pipeline register에 `stat` 값을 함께 전달한다.

주요 상태:

- `SAOK` — 정상
- `SADR` — 잘못된 address
- `SINS` — invalid instruction
- `SHLT` — halt
- `SBUB` — bubble

Exception은 발견 즉시 확정하지 않고 instruction과 함께 pipeline을 통과시킨 뒤 **Write-back stage에 도달했을 때 처리**한다.

이를 통해 이전 instruction이 먼저 완료될 수 있다.

#### Side Effect 방지

Exception 이후 instruction은 Register, Memory, Condition Code 등을 변경하면 안 된다.

이를 위해:

- Condition Code update 차단
- Memory stage에 Bubble 삽입
- Exception instruction이 Write-back에 도달하면 Stall

등의 Control Logic을 사용한다.

실제 시스템에서는 Exception 발생 시 PC 등을 저장하고 **Exception Handler(异常处理程序)**로 이동한다.

---

### 2. Pipeline Performance

Pipeline의 이상적인 성능은:

\[
CPI \approx 1
\]

이다.

**CPI(Cycles Per Instruction)**는 instruction 하나를 완료하는 데 평균적으로 필요한 clock cycle 수이다.

5-stage pipeline에서는 instruction 하나의 latency는 약 5 cycles이지만, pipeline이 채워지면 거의 매 cycle 하나의 instruction을 완료할 수 있다.

#### Bubble과 CPI

총 cycle을 `C`, 완료된 instruction 수를 `I`, bubble 수를 `B`라고 하면:

\[
C = I + B
\]

따라서:

\[
CPI = \frac{C}{I}
     = 1 + \frac{B}{I}
\]

즉:

> **Bubble이 증가하면 CPI가 증가하고 성능이 감소한다.**

PIPE에서 주요 penalty는 다음과 같다.

- Load/Use Hazard
- Branch Misprediction
- `ret`

일반적인 계산 방식:

\[
Penalty =
Instruction\ Frequency
\times Problem\ Frequency
\times Bubble\ Count
\]

PPT의 수치는 계산 방법을 설명하기 위한 예시이며 고정된 값이 아니다.

---

### 3. Fetch Stage Optimization

기본 Fetch Stage는 다음 순서로 동작한다.

```text
PC 선택
↓
Instruction Memory Read
↓
Instruction 길이 확인
↓
PC Increment
```

문제는 instruction 길이를 알아야 다음 PC를 계산할 수 있기 때문에 여러 작업이 순차적으로 연결된다는 것이다.

이를 개선하기 위해 일부 PC 계산을 Memory Read와 **병렬(Parallel, 并行)**로 수행한다.

```text
        Memory Read
       ↗
PC
       ↘
        Increment 계산
```

목적은 **Critical Path(关键路径)**를 줄여 clock cycle을 짧게 만드는 것이다.

실제 고성능 processor에서는 Fetch Logic이 Instruction Cache와 결합되어 있으며 한 instruction만 읽는 것이 아니라 **16 또는 32 byte 정도의 cache block을 한 번에 가져올 수 있다.**

또한 현재 block을 처리하면서 다음 block을 미리 fetch한다.

---

### 4. Modern CPU Design

현대 CPU는 단순히 instruction을 program order대로 하나씩 실행하지 않는다.

Instruction Control에서는:

- 현재 PC를 이용한 Instruction Fetch
- Branch Prediction
- Instruction을 더 작은 Operation으로 변환
- Register reference를 내부 Tag로 변환

등을 수행한다.

Tag는 한 operation의 결과와 이후 operation의 입력 사이의 **Data Dependency(数据依赖)**를 추적하는 데 사용된다.

---

### 5. Out-of-Order Execution

현대 CPU에서는 operation이 반드시 program order대로 실행될 필요가 없다.

다음 조건이 만족되면 먼저 실행할 수 있다.

- Operand가 준비됨
- 필요한 Functional Unit이 사용 가능함

예:

```text
I1 → I2
I3
```

`I2`가 `I1`의 결과를 기다리고 있지만 `I3`가 독립적이라면:

```text
I1 실행
I2 대기
I3 실행
I2 실행
```

처럼 실행할 수 있다.

이를 **Out-of-Order Execution(비순차 실행, 乱序执行)**이라고 한다.

다만 내부 실행 순서가 달라도:

> **최종적으로 프로그램에서 관찰되는 결과는 Sequential Execution과 동일해야 한다.**

---

### 6. Multiple Functional Units

현대 CPU에는 여러 **Functional Unit(功能单元)**이 존재한다.

예:

- Integer Unit
- Branch Unit
- Load Unit
- Store Unit
- Floating-Point Add
- Floating-Point Multiply / Divide

따라서 서로 독립적인 여러 operation을 동시에 실행할 수 있다.

Intel Haswell의 경우 여러 Load, Store, Integer, Floating-Point 연산을 병렬로 처리할 수 있다.

PPT의 구체적인 unit 개수는 특정 processor의 예시이므로 암기할 필요는 없다.

---

### 7. Latency vs Throughput

**Latency(延迟)**는 하나의 operation이 시작해서 결과가 나오기까지 걸리는 cycle 수이다.

예:

```text
Integer Multiply Latency = 3 cycles
```

하지만 execution unit 자체가 pipelined되어 있다면 매 cycle 새로운 연산을 시작할 수도 있다.

```text
Cycle 1 → Multiply A 시작
Cycle 2 → Multiply B 시작
Cycle 3 → Multiply C 시작
```

따라서:

> **Latency가 길다고 해서 새로운 operation을 그만큼 오래 기다렸다가 시작해야 하는 것은 아니다.**

PPT의 `Cycles/Issue`는 새로운 operation을 얼마나 자주 시작할 수 있는지를 나타낸다.

---

### 8. Uops and Reservation Stations

Intel Haswell에서는 instruction을 내부적으로 **Uop(Micro-operation)**으로 변환한다.

```text
Instruction
↓
Uop
Uop
Uop
```

Uop은 다음 조건이 만족되면 실행된다.

- Operand available
- Functional Unit available

이 실행을 **Reservation Station**이 관리한다.

Reservation Station의 주요 역할:

- Uop 대기
- Data Dependency 추적
- Operand 준비 여부 확인
- Functional Unit 할당

즉 CPU는 매 순간 **현재 실행 가능한 operation을 동적으로 선택**한다.

---

### 9. Branch Prediction

Branch 결과가 나올 때까지 기다리면 Pipeline이 정지하기 때문에 현대 CPU는 Branch 결과를 미리 예측한다.

```text
Branch
↓
Taken / Not Taken 예측
↓
예측한 경로의 instruction을 미리 Fetch
```

예측이 맞으면 Pipeline을 계속 진행할 수 있다.

하지만 **Branch Misprediction(分支预测错误)**이 발생하면 잘못 가져온 instruction을 버리고 올바른 경로를 다시 Fetch해야 한다.

고성능 CPU에서는 misprediction penalty가 매우 클 수 있기 때문에 Branch Prediction이 성능에 중요하다.

---

### 10. Branch Target Buffer

**BTB(Branch Target Buffer)**는 과거 branch와 target 정보를 저장하여 다음 branch의 목적지를 빠르게 예측하는 데 사용된다.

개념적으로:

```text
Branch PC
↓
BTB 검색
↓
예상 Branch Target
↓
Instruction Fetch
```

PPT의 BTB 크기와 history bit 수는 Intel Haswell의 구체적인 예시이므로 암기할 필요는 없다.

---

### 11. Branch History and State Machine

Branch Predictor는 이전 Branch 결과를 이용해 다음 결과를 예측할 수 있다.

단순히 직전 결과만 보는 것이 아니라 반복되는 패턴도 활용할 수 있다.

예:

```text
Taken
Not Taken
Taken
Not Taken
...
```

PPT에서는 다음과 같은 4-state predictor를 사용한다.

```text
No! ← No? ← Yes? ← Yes!
```

- `Yes!` — 강하게 Taken 예측
- `Yes?` — 약하게 Taken 예측
- `No?` — 약하게 Not Taken 예측
- `No!` — 강하게 Not Taken 예측

Branch가 Taken이면 오른쪽으로, Not Taken이면 왼쪽으로 이동한다.

이렇게 하면 한 번의 예외적인 결과 때문에 예측 방향이 즉시 바뀌는 것을 방지할 수 있다.

---

### 12. Processor Architecture 핵심 정리

Processor는 모든 instruction마다 별도의 hardware를 만드는 것이 아니라 공통 hardware를 공유하고 **Control Logic(控制逻辑)**으로 동작을 결정한다.

주요 구성:

- Register
- Memory
- ALU
- MUX
- Pipeline Register
- Control Logic

Processor 동작은 크게 두 부분으로 볼 수 있다.

```text
State
→ Memory / Clocked Register

Computation
→ Combinational Logic
```

Clock에 따라:

```text
Current State
↓
Combinational Logic
↓
Next State
↓
Clock
↓
New State
```

형태로 상태가 변화한다.

성능 향상을 위해:

- Pipelining
- Forwarding
- Branch Prediction
- Parallel Execution
- Out-of-Order Execution

등을 사용할 수 있다.

하지만 어떤 최적화를 사용하더라도 최종적으로는:

> **ISA가 정의한 프로그램의 동작을 유지해야 한다.**

### 최종 핵심

```text
Pipeline
↓
여러 instruction을 겹쳐 실행
↓
Hazard 발생
↓
Forwarding / Stall / Bubble로 처리
↓
Branch Prediction으로 Control Hazard 감소
↓
여러 Functional Unit으로 병렬 실행
↓
실행 가능한 Operation부터 Out-of-Order 실행
↓
최종 결과는 Sequential ISA와 동일하게 유지
```

Processor Architecture의 핵심은 **성능을 높이기 위해 내부 실행은 점점 더 병렬적이고 복잡하게 만들면서도, 외부에서는 ISA가 정의한 정확한 프로그램 동작을 유지하는 것**이다.