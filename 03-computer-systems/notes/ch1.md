# Chapter 1. A Tour of Computer Systems (26.09.04)

## 1. Abstraction Is Good, But Don't Forget Reality

프로그램에서는 **추상화(Abstraction, 抽象)**를 사용해 복잡한 컴퓨터의 동작을 단순하게 표현한다.

하지만 추상화의 내부에서 실제 컴퓨터가 어떻게 동작하는지 이해해야 한다.

```text
Program
  ↓
Compiler
  ↓
Assembly
  ↓
Machine Code
  ↓
CPU / Memory / I/O
```

즉, 프로그래머는 높은 수준의 추상화를 사용하면서도 필요할 때 그 아래의 **실제 시스템 동작**을 이해할 수 있어야 한다.

---

## 2. Five Realities

### 2.1 Ints Are Not Integers, Floats Are Not Reals

수학적인 정수와 컴퓨터의 `int`는 동일하지 않다.

`int`는 제한된 수의 **bits(비트, 位)**로 값을 표현하기 때문에 표현할 수 있는 범위가 제한된다.

예:

```c
int x = 50000 * 50000;
```

수학적으로는:

```text
2,500,000,000
```

이지만 32-bit signed `int`의 표현 범위를 초과한다.

따라서 **컴퓨터의 데이터 표현 방식과 수학적 개념 사이에는 차이가 있다.**

> Signed integer overflow는 C에서 Undefined Behavior이다.

부동소수점 역시 실제 컴퓨터의 제한된 표현을 사용하기 때문에 수학적인 실수와 동일하게 동작하지 않는다.

---

### 2.2 You've Got to Know Assembly

C 프로그램은 CPU가 직접 실행하는 것이 아니다.

```text
C
↓
Compiler
↓
Assembly
↓
Machine Code
↓
CPU
```

C 코드의 추상적인 연산은 실제로는 CPU의 **register(레지스터, 寄存器)**와 명령어를 이용한 구체적인 동작으로 변환된다.

따라서 다음과 같은 내용을 이해하려면 Assembly를 볼 수 있어야 한다.

* 함수 호출
* 포인터
* 배열 접근
* 조건문과 반복문
* 레지스터
* 메모리 접근
* 성능
* 프로그램 실행 과정

목표는 Assembly 프로그래머가 되는 것이 아니라 **C 코드 아래에서 CPU가 실제로 무엇을 수행하는지 이해하는 것**이다.

---

### 2.3 Memory Matters

C에서는 메모리를 단순한 변수와 배열로 추상화한다.

```c
int a[3];
```

하지만 실제로는 메모리 주소에 데이터가 배치된다.

```text
a[0] → address
a[1] → address + 4
a[2] → address + 8
```

따라서 배열 접근과 포인터 연산은 결국 **메모리 접근**이다.

예를 들어 배열 범위를 벗어나면:

```c
a[3] = 999;
```

실제 메모리에서 예상하지 못한 위치에 데이터를 쓸 수 있다.

또한 CPU와 RAM 사이에는 **Cache(캐시, 高速缓存)**가 존재하기 때문에 메모리 접근 패턴은 프로그램 성능에도 큰 영향을 준다.

---

### 2.4 More to Performance Than Asymptotic Complexity

알고리즘의 **asymptotic complexity(점근적 복잡도, 渐近复杂度)**만으로 실제 프로그램의 성능을 모두 설명할 수 없다.

예를 들어 같은 `O(N²)` 연산이라도 메모리 접근 방식에 따라 성능이 크게 달라질 수 있다.

```c
for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
        sum += a[i][j];
```

행 방향으로 연속적인 메모리에 접근하면 **cache locality(캐시 지역성, 缓存局部性)**를 잘 활용할 수 있다.

반대로 메모리를 크게 건너뛰며 접근하면 cache를 제대로 활용하지 못해 성능이 떨어질 수 있다.

즉 실제 성능에는 다음과 같은 요소도 영향을 준다.

* CPU
* Cache
* Memory
* Compiler
* Instruction
* Memory access pattern
* Locality
* Vectorization
* Threads

---

### 2.5 Computers Do More Than Execute Programs

컴퓨터는 단순히 프로그램을 실행하는 것만 하지 않는다.

프로그램은 시스템을 통해 다음과 같은 작업도 수행한다.

* Input / Output
* File access
* Network communication
* Process management
* Concurrency

따라서 프로그래머는 CPU와 메모리뿐 아니라 **Operating System(OS, 操作系统)**과 시스템의 다른 구성 요소도 이해해야 한다.

---

# 3. Why Study Computer Systems?

Computer Systems를 이해하면 프로그래머가 작성하는 코드가 실제 시스템에서 어떻게 동작하는지 이해할 수 있다.

특히 다음과 같은 문제를 더 잘 이해할 수 있다.

* 프로그램이 왜 느린가?
* 왜 특정 환경에서만 문제가 발생하는가?
* 왜 segmentation fault가 발생하는가?
* 포인터는 실제로 무엇을 가리키는가?
* 함수 호출은 실제로 어떻게 이루어지는가?
* 메모리 접근은 성능에 어떤 영향을 주는가?
* OS는 프로그램 실행에 어떤 역할을 하는가?

결국 목표는 **C 언어의 추상적인 개념과 실제 Computer System의 동작을 연결하는 것**이다.

---

# 4. Course Structure

CMU 15-213에서는 다음과 같은 흐름으로 Computer Systems를 다룬다.

```text
Programs & Data
      ↓
Machine Programming
      ↓
Memory Hierarchy
      ↓
Performance
      ↓
Exceptional Control Flow
      ↓
Virtual Memory
      ↓
Networking
      ↓
Concurrency
```

각 영역을 실제 프로그래밍과 실험을 통해 학습한다.

대표적인 Lab:

* Data Lab — Data Representation
* Bomb Lab — Assembly / GDB
* Cache Lab — Cache
* Malloc Lab — Dynamic Memory Allocation
* Shell Lab — Processes / Signals
* Proxy Lab — Networking / Concurrency

---

# 5. Chapter 1 핵심

Chapter 1의 핵심은 **추상화와 실제 시스템 사이의 연결을 이해하는 것**이다.

```text
C Programming
     ↓
Compiler
     ↓
Assembly
     ↓
CPU
     ↓
Cache
     ↓
Memory
     ↓
OS
     ↓
I/O / Network
```

C 프로그램은 독립적으로 존재하는 것이 아니라 **하드웨어와 운영체제 위에서 실행되는 시스템의 일부**이다.

따라서 Computer Systems의 목표는 단순히 C 문법을 더 배우는 것이 아니라,

> **C 프로그램이 실제 컴퓨터에서 어떻게 실행되는지를 이해하는 것**

이다.