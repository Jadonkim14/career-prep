# Chapter 8 — Exceptional Control Flow & Processes

## 1. Control Flow

CPU는 시작부터 종료까지 **instruction(指令)** 을 순서대로 실행한다.

이 실행 순서를 **Control Flow(控制流)** 라고 한다.

기존의 control flow 변경 방법:

- Jump / Branch
- Call / Return

하지만 이것만으로는 다음과 같은 **system event**에 대응하기 어렵다.

- Disk / Network에서 데이터 도착
- Divide by zero
- `Ctrl-C`
- Timer 만료

따라서 **Exceptional Control Flow, ECF(异常控制流)** 가 필요하다. 13-exceptions-4up

---

## 2. Exceptional Control Flow

ECF는 **system state 변화에 반응하여 control flow를 바꾸는 메커니즘**이다.

```text
Exceptional Control Flow
├─ Low Level
│  └─ Exception
│
└─ Higher Level
   ├─ Process Context Switch
   ├─ Signal
   └─ setjmp / longjmp
```

Exception은 Hardware와 OS가 함께 처리한다. 13-exceptions-4up

---

## 3. Exception

**Exception(异常)** 은 어떤 event가 발생했을 때 control을 OS의 **Exception Handler**로 넘기는 것이다.

```text
User Process
    ↓
Event
    ↓
Exception
    ↓
OS Exception Handler
    ↓
현재 instruction 재실행
또는
다음 instruction 실행
또는
Abort
```

각 Exception은 고유한 **Exception Number**를 가지며, 이 번호를 **Exception Table / Interrupt Vector**의 index로 사용한다. 13-exceptions-4up 13-exceptions-4up

---

## 4. Exception 종류

```text
Exception
├─ Interrupt
├─ Trap
├─ Fault
└─ Abort
```

### Interrupt(中断)

CPU 외부에서 발생하는 event.

예:

- Keyboard 입력
- Network packet 도착
- Disk I/O 완료

보통 handler 처리 후 **다음 instruction**으로 돌아간다. 13-exceptions-4up

### Trap(陷阱)

의도적으로 발생시키는 exception.

대표 예:

- System Call
- Breakpoint

```text
User Program
→ System Call
→ Kernel
→ 처리
→ 다음 instruction
```

### Fault

의도하지 않았지만 복구 가능할 수도 있는 exception.

대표 예:

- Page Fault
- Protection Fault

Page Fault:

```text
Memory 접근
→ Page Fault
→ Kernel이 page를 RAM으로 load
→ 현재 instruction 재실행
```

### Abort

복구 불가능한 심각한 exception.

예:

- Machine Check
- Parity Error

현재 프로그램을 종료한다. 13-exceptions-4up

---

## 5. Process

**Process(进程)** 는 **실행 중인 프로그램의 instance**이다.

Process가 제공하는 핵심 추상화:

- **Logical Control Flow**
  - CPU를 혼자 사용하는 것처럼 보이게 함
- **Private Virtual Address Space**
  - Memory를 혼자 사용하는 것처럼 보이게 함

Single-core에서는 실제로 한 순간에 하나의 process만 실행된다. 13-exceptions-4up

---

## 6. Context Switch

**Context Switch(上下文切换)** 는 현재 process의 실행 상태를 저장하고 다른 process의 상태를 복원하여 CPU 실행 대상을 바꾸는 것이다.

```text
Process A
   ↓
Kernel
   ↓
A 상태 저장
   ↓
B 상태 복원
   ↓
Process B
```

이를 통해 여러 process가 동시에 실행되는 것처럼 보인다. 13-exceptions-4up

---

## 7. `fork()`

```c
pid_t pid = fork();
```

현재 process를 기반으로 새로운 **Child Process**를 생성한다.

특징:

```text
Parent → Child PID 반환
Child  → 0 반환
```

즉:

> `fork()`는 한 번 호출되지만 두 process에서 반환된다.

```text
Process
   ↓ fork()
Parent + Child
```

연속해서 모든 process가 `fork()`를 실행하면 process 수는 증가한다. 13-exceptions-4up

---

## 8. `exit()`

```c
exit(status);
```

현재 process를 종료한다.

보통 정상 종료는:

```c
exit(0);
```

으로 표현한다. 13-exceptions-4up

---

## 9. Zombie Process

Child가 종료했지만 Parent가 아직 종료 정보를 회수하지 않았다면:

**Zombie Process(僵尸进程)** 가 된다.

```text
Child
 ↓
exit()
 ↓
Zombie
 ↓
Parent wait()
 ↓
Reaped
```

Zombie는 실행 중은 아니지만 OS에 종료 정보가 남아 있다. 13-exceptions-4up

---

## 10. `wait()` / `waitpid()`

Parent가 종료된 Child의 정보를 회수하는 것을 **Reaping(回收)** 이라고 한다.

```c
wait(&status);
```

- Child 중 하나가 종료될 때까지 대기
- 종료한 Child의 PID 반환

```c
waitpid(pid, &status, options);
```

- 특정 Child를 기다릴 수 있음 13-exceptions-4up 13-exceptions-4up

---

## 11. Orphan Process

Parent가 먼저 종료되어도 Child는 계속 실행될 수 있다.

```text
Parent exit()
   ↓
Child still running
```

이런 Child를 **Orphan Process(孤儿进程)** 라고 한다.

강의자료에서는 Parent가 Child를 reap하지 않고 종료하면 Child가 이후 `init` process에 의해 reap될 수 있다고 설명한다. 13-exceptions-4up

---

## 12. `execve()`

```c
execve(filename, argv, envp);
```

새 process를 만드는 것이 아니라:

> **현재 process 안에서 실행되는 프로그램을 다른 프로그램으로 교체한다.**

```text
Before

PID 100
Shell Program
```

```text
execve()
```

```text
After

PID 100
New Program
```

교체되는 것:

- Code
- Data
- Stack

유지되는 것:

- PID
- Open Files
- Signal Context

성공하면 일반적으로 기존 코드로 돌아오지 않는다. 13-exceptions-4up

---

## 13. `fork()` + `execve()`

Unix/Linux에서 매우 중요한 조합.

```text
Shell
  ↓
fork()
  ↓
Child
  ↓
execve()
  ↓
New Program
```

정리:

```text
fork()
→ 새로운 Process 생성

execve()
→ 현재 Process의 프로그램 교체
```

---

## 14. Signal

**Signal(信号)** 은 OS가 Process에게 특정 event가 발생했음을 알려주는 메커니즘이다.

대표 예:

```text
Invalid Memory Access
→ Fault
→ Kernel
→ SIGSEGV
→ Process 종료
```

`SIGSEGV`:

```text
SIG  → Signal
SEGV → Segmentation Violation
```

잘못된 memory address 접근 시 발생할 수 있다. 13-exceptions-4up

---

## 핵심 정리

```text
Exceptional Control Flow
│
├─ Exception
│  ├─ Interrupt
│  ├─ Trap
│  ├─ Fault
│  └─ Abort
│
├─ Process
│  ├─ Context Switch
│  ├─ fork()
│  ├─ exit()
│  ├─ wait()/waitpid()
│  └─ execve()
│
└─ Signal
   └─ SIGSEGV
```

가장 중요한 연결:

```text
fork()   → Process 생성
exit()   → Process 종료
wait()   → Child 종료 정보 회수
execve() → 현재 Process의 프로그램 교체
```

그리고:

```text
Invalid Memory Access
→ Fault
→ Kernel
→ SIGSEGV
→ Process 종료
```