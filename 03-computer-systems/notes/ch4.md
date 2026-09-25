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