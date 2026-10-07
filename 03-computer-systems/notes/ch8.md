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

## 15. Signals and Nonlocal Jumps

이번 PPT의 핵심 흐름:

```text
Multitasking / Shell
→ Signal
→ Signal Handler
→ Pending / Blocked Signal
→ Process Group
→ SIGCHLD / waitpid()
→ Async-Signal-Safety
→ setjmp() / longjmp()
```

---

## 16. Shell과 Background Job

**Shell(命令解释器)** 은 사용자를 대신해 다른 프로그램을 실행하는 application program이다.

기본 흐름:

```text
Shell
→ Command 입력
→ fork()
→ Child에서 execve()
→ Program 실행
```

Foreground Job:

```text
Shell
→ Child 실행
→ waitpid()
→ Child가 끝날 때까지 대기
```

Background Job:

```text
Shell
→ Child 실행
→ 기다리지 않음
→ 바로 다음 command 처리
```

Background Job은 언제 종료될지 Shell이 알 수 없기 때문에,
종료된 Child를 적절히 처리하기 위해 **Signal**이 필요하다.

---

## 17. Signal

**Signal(信号)** 은 system에서 어떤 event가 발생했음을
Process에게 알려주는 작은 message이다.

대표적인 Signal:

```text
SIGINT  → Ctrl-C
SIGKILL → 강제 종료
SIGSEGV → Segmentation Violation
SIGALRM → Timer
SIGCHLD → Child 종료 또는 정지
```

Signal은 보통 Kernel이 Process에게 전달한다.

Signal이 발생하는 대표적인 경우:

```text
1. Kernel이 system event를 감지

2. 다른 Process가 kill()을 통해
   Kernel에게 Signal 전송을 요청
```

예:

```text
Child 종료
→ Kernel
→ SIGCHLD
→ Parent
```

---

## 18. Signal을 받았을 때의 동작

Process가 Signal을 받으면 다음 세 가지 방식으로 반응할 수 있다.

```text
1. Ignore
2. Terminate
3. Catch
```

### Ignore

```text
Signal 도착
→ 무시
→ 계속 실행
```

### Terminate

```text
Signal 도착
→ Process 종료
```

### Catch

사용자가 정의한 **Signal Handler(信号处理函数)** 를 실행한다.

```text
Signal 도착
→ Signal Handler 실행
→ 원래 Control Flow로 복귀
```

---

## 19. Pending / Blocked Signal

### Pending Signal

Signal이 보내졌지만 아직 Process가 처리하지 않은 상태.

```text
Signal Sent
→ Pending
→ Received
```

### Blocked Signal

Signal은 도착했지만 현재는 처리하지 않도록 막아둔 상태.

```text
Signal 도착
→ Blocked
→ Pending 상태 유지
→ Unblock
→ Receive
```

Kernel은 각 Process마다 다음 정보를 관리한다.

```text
pending bit vector
blocked bit vector
```

중요:

```text
Signals are not queued.
```

같은 종류의 Signal이 이미 Pending 상태라면
같은 Signal이 여러 번 더 발생해도 각각 저장되지 않는다.

예:

```text
SIGCHLD
SIGCHLD
SIGCHLD

≠

SIGCHLD 3개가 Queue에 저장
```

---

## 20. Process Group

모든 Process는 하나의 **Process Group(进程组)** 에 속한다.

예:

```text
Foreground Process Group
├─ Process A
├─ Process B
└─ Process C
```

### Ctrl-C

```text
Ctrl-C
→ Foreground Process Group 전체
→ SIGINT
→ 기본 동작: Terminate
```

### Ctrl-Z

```text
Ctrl-Z
→ Foreground Process Group 전체
→ SIGTSTP
→ 기본 동작: Stop / Suspend
```

즉 Ctrl-C와 Ctrl-Z는 일반적으로
하나의 Process가 아니라 **Foreground Process Group 전체**에 전달된다.

---

## 21. `kill()`

```c
kill(pid, signal);
```

특정 Process에 Signal을 보내도록 Kernel에 요청한다.

예:

```c
kill(pid, SIGINT);
```

흐름:

```text
Process
→ kill()
→ Kernel
→ Target Process에 SIGINT 전달
```

`kill()`은 이름과 달리 반드시 Process를 종료하는 함수는 아니다.

핵심 역할:

```text
Process에 지정한 Signal을 보내는 것
```

---

## 22. Signal Handler

기본 Signal 동작 대신 사용자가 직접 함수를 실행하도록 설정할 수 있다.

```c
signal(SIGINT, handler);
```

의 의미:

```text
SIGINT 도착
→ 기본 동작 대신
→ handler() 실행
```

설정 가능한 대표 값:

```text
SIG_IGN
→ Signal 무시

SIG_DFL
→ 기본 동작 사용

함수 주소
→ 해당 함수를 Signal Handler로 사용
```

Signal Handler는 새로운 Process가 아니다.

```text
Process A
├─ Main Logical Flow
└─ Signal Handler Logical Flow
```

Handler가 return하면 일반적으로 Signal 때문에 중단됐던
기존 Control Flow로 돌아간다.

---

## 23. `SIGCHLD`와 Zombie 처리

Child가 종료하면 Parent에게 `SIGCHLD`가 전달될 수 있다.

```text
Child 종료
→ Kernel
→ SIGCHLD
→ Parent
```

하지만 Signal은 Queue 방식이 아니다.

여러 Child가 거의 동시에 종료해도:

```text
Child A 종료 → SIGCHLD
Child B 종료 → SIGCHLD
Child C 종료 → SIGCHLD
```

Handler가 반드시 3번 실행된다고 보장할 수 없다.

따라서 Handler에서 Child 하나만 `wait()`하면
일부 Zombie가 남을 수 있다.

해결:

```c
while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    ...
}
```

의미:

```text
SIGCHLD 수신
→ 종료된 Child 확인
→ waitpid()
→ 또 종료된 Child가 있는지 확인
→ 없을 때까지 반복
```

`-1`:

```text
어떤 Child든 확인
```

`WNOHANG`:

```text
Child가 없으면 기다리지 말고 즉시 반환
```

---

## 24. Async-Signal-Safety

Signal Handler는 Main Program의 어느 시점에나 끼어들 수 있다.

따라서 Handler 안에서 아무 함수나 호출하면 위험할 수 있다.

**Async-Signal-Safe**:

```text
Signal Handler 안에서도 안전하게 호출할 수 있는 함수
```

대표 예:

```text
write()  → Async-Signal-Safe
printf() → Async-Signal-Safe 아님
```

### Reentrant

**Reentrant(可重入)** 는 함수가 실행 중일 때
같은 함수가 다시 호출되어도 내부 상태가 깨지지 않는 성질이다.

```text
Function 실행 중
→ Signal 발생
→ Handler에서 같은 Function 호출
→ 정상 동작

= Reentrant
```

Signal Handler에서는 가능한 한
Async-Signal-Safe 함수만 사용하는 것이 중요하다.

---

## 25. `setjmp()` / `longjmp()`

**Nonlocal Jump(非局部跳转)** 는 일반적인 함수 call / return 순서를 벗어나
Control Flow를 다른 위치로 이동시키는 기능이다.

### `setjmp()`

```c
setjmp(buf);
```

현재 실행 위치를 저장한다.

주요 저장 정보:

```text
Register Context
Stack Pointer
Program Counter
```

처음 실행할 때는:

```text
setjmp()
→ 0 반환
```

### `longjmp()`

```c
longjmp(buf, 1);
```

이전에 `setjmp()`로 저장한 위치로 즉시 이동한다.

예:

```text
P1
↓
P2
↓
P3
↓
longjmp()
↓
P1의 setjmp 위치
```

일반적인:

```text
P3 → return
P2 → return
P1
```

과정을 거치지 않는다.

`longjmp(buf, 1)`이 실행되면
복귀한 `setjmp()`는 이번에는 `1`을 반환한 것처럼 동작한다.

```text
처음:

setjmp()
→ 0


longjmp(buf, 1)


복귀 후:

setjmp()
→ 1
```

즉 두 번째 인자인 `1`은 Jump 횟수가 아니라
복귀한 `setjmp()`의 반환값이다.

---

## 26. Nonlocal Jump 제한

`longjmp()`는 아무 위치로나 Jump할 수 있는 것은 아니다.

핵심 조건:

```text
setjmp()를 실행한 함수가
아직 종료되지 않았을 때만
그 환경으로 돌아갈 수 있음
```

가능한 경우:

```text
P1: setjmp()
 ↓
P2()
 ↓
P3()
 ↓
longjmp()
 ↓
P1
```

P1이 아직 return하지 않았기 때문에 가능하다.

반대로:

```text
P1에서 setjmp()
↓
P1 return
↓
나중에 longjmp()
```

처럼 이미 종료된 함수의 환경으로 돌아가면 안 된다.

이유:

```text
함수가 종료됨
→ Stack Frame도 더 이상 유효하지 않음
```

---

## 27. Signal + Nonlocal Jump

Signal Handler와 Nonlocal Jump를 결합할 수도 있다.

```c
sigsetjmp(buf, 1);
```

로 복귀 위치를 저장한 뒤:

```c
siglongjmp(buf, 1);
```

를 실행하면 저장된 위치로 돌아간다.

예:

```text
sigsetjmp()
→ 실행 위치 저장
→ 처음에는 0 반환

Program 실행
→ Ctrl-C

SIGINT
→ Signal Handler
→ siglongjmp(buf, 1)

저장 위치로 복귀
→ sigsetjmp()가 1을 반환한 것처럼 동작
```

예시 구조:

```c
if (!sigsetjmp(buf, 1))
    printf("starting\n");
else
    printf("restarting\n");
```

처음:

```text
sigsetjmp()
→ 0

!0
→ true

"starting"
```

Signal 발생 후:

```text
siglongjmp(buf, 1)

→ sigsetjmp()가 1을 반환한 것처럼 동작

!1
→ false

"restarting"
```

---

## 핵심 정리

```text
Signal
→ Process-level Exceptional Control Flow

SIGCHLD
→ Child 상태 변화 알림

Signals are not queued
→ 같은 종류의 Signal이 개수대로 저장되지 않음

waitpid(..., WNOHANG)
→ 종료된 Child들을 반복해서 Reap

Ctrl-C
→ Foreground Process Group에 SIGINT

Ctrl-Z
→ Foreground Process Group에 SIGTSTP

kill()
→ Process에 Signal 전송 요청

Signal Handler
→ Signal 발생 시 사용자 함수 실행

Async-Signal-Safe
→ Handler에서 안전하게 호출 가능한 함수

setjmp()
→ 복귀할 실행 위치 저장

longjmp()
→ 일반적인 Return 순서를 건너뛰고 저장 위치로 이동

sigsetjmp() / siglongjmp()
→ Signal 처리 상황에서 사용하는 Nonlocal Jump
```

### 전체 Chapter 8 흐름

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
│  ├─ wait() / waitpid()
│  └─ execve()
│
├─ Signal
│  ├─ SIGINT
│  ├─ SIGCHLD
│  ├─ SIGSEGV
│  ├─ Pending / Blocked
│  ├─ Process Group
│  └─ Signal Handler
│
└─ Nonlocal Jump
   ├─ setjmp()
   ├─ longjmp()
   ├─ sigsetjmp()
   └─ siglongjmp()
```
