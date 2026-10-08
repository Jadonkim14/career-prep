# shlab-review

## eval()

### 핵심 구조

```text
parseline
→ empty check
→ builtin check
→ SIGCHLD block
→ fork
   ├─ child
   │   → signal mask 복원
   │   → setpgid
   │   → execve
   │
   └─ parent
       → addjob
       → signal mask 복원
       → FG: waitfg
       → BG: 바로 shell로 복귀
```

### 질문 및 헷갈렸던 내용

* `fork()` 직후 parent와 child는 둘 다 같은 `tsh` 코드의 다음 줄부터 실행
  * Parent → child PID 반환
  * Child → `0` 반환

* `setpgid(0, 0)`
  * child를 자기만의 process group으로 분리
  * `PID = PGID`
  * `Ctrl-C`, `Ctrl-Z` 같은 signal을 job 단위로 관리하기 위해 사용

* `execve(argv[0], argv, environ)`
  * child가 실행 중인 `tsh` 프로그램을 실제 명령 프로그램으로 교체
  * `fork()` = process 생성
  * `execve()` = 프로그램 교체

* `addjob()`
  * parent가 child를 job list에 등록
  * PID, JID, 상태(FG/BG/ST), command line 저장

* `pid2jid(pid)`
  * PID 하나를 해당 JID로 변환

* `listjobs(jobs)`
  * 현재 등록된 모든 job 출력

* `waitfg(pid)`
  * foreground job이 끝나거나 stop될 때까지 shell이 기다림
  * background job은 기다리지 않음

* `SIGCHLD` block
  * child가 너무 빨리 종료되어 `addjob()`보다 먼저 handler가 실행되는 race condition 방지
  * 흐름: `SIGCHLD block → fork → addjob → signal mask 복원`

* `SIG_UNBLOCK`
  * 모든 signal을 해제하는 것이 아니라 mask 안의 signal만 unblock

* 이전 signal mask 복원
  * 임시로 signal을 block하기 전 상태를 저장
  * 작업이 끝나면 `SIG_SETMASK`로 원래 상태 복원

## signal / job control

### 핵심 구조

```text
SIGINT   → foreground job 종료
SIGTSTP  → foreground job 정지
SIGCONT  → 정지된 job 재개
SIGCHLD  → child 상태 변화 알림
```

### 질문 및 헷갈렸던 내용

* `SIGCHLD`
  * child가 종료되거나 stop되면 parent인 shell이 받는 signal
  * `sigchld_handler()`에서 `waitpid()`로 child 상태를 확인

* `waitpid(-1, &status, WNOHANG | WUNTRACED)`
  * `-1` → 아무 child나 확인
  * `WNOHANG` → 상태 변화가 없으면 기다리지 않고 바로 반환
  * `WUNTRACED` → 종료뿐 아니라 stop된 child도 확인

* child 상태 확인
  * `WIFEXITED(status)` → 정상 종료
  * `WIFSIGNALED(status)` → signal 때문에 종료
  * `WIFSTOPPED(status)` → signal 때문에 정지

* `SIGINT`
  * 보통 `Ctrl-C`로 발생
  * foreground job을 종료시키는 signal
  * Linux에서 보통 signal 번호 `2`

* `SIGTSTP`
  * 보통 `Ctrl-Z`로 발생
  * foreground job을 종료하지 않고 잠시 정지
  * 정지된 job의 상태를 `ST`로 변경

* `SIGCONT`
  * 정지된 process를 다시 실행
  * `bg`, `fg` 모두 stopped job을 살릴 때 사용

* `kill(-pid, signal)`
  * `pid` 앞에 `-`를 붙이면 한 process가 아니라 **process group 전체**에 signal 전달
  * job 하나가 여러 process를 포함할 수 있기 때문에 필요

---

## bg / fg

### 핵심 구조

```text
bg %JID
→ job 찾기
→ SIGCONT
→ state = BG
→ shell은 바로 다음 명령 처리

fg %JID
→ job 찾기
→ SIGCONT
→ state = FG
→ waitfg()
```

### 질문 및 헷갈렸던 내용

* `bg`, `fg`
  * 사용자가 shell에 입력하는 **명령어 문자열**

* `BG`, `FG`, `ST`
  * shell 내부 job 상태를 나타내는 상수

```c
#define FG 1
#define BG 2
#define ST 3
```

* 예:

```text
bg %2
```

```text
"bg"
→ 명령어

BG
→ job->state에 저장되는 내부 상태
```

* `%2`
  * `%`가 붙으면 JID
  * `getjobjid(jobs, 2)`로 검색

* `3627`
  * `%`가 없는 숫자는 PID
  * `getjobpid(jobs, 3627)`로 검색

* `&argv[1][1]`
  * `%2`에서 `%` 다음 위치를 가리킴
  * `"2"`를 `atoi()`로 변환하기 위해 사용

---

## signal handler

### `sigint_handler()`

```text
SIGINT 수신
→ foreground PID 확인
→ foreground process group 전체에 SIGINT 전달
```

```c
pid = fgpid(jobs);
kill(-pid, SIGINT);
```

### `sigtstp_handler()`

```text
SIGTSTP 수신
→ foreground PID 확인
→ foreground process group 전체에 SIGTSTP 전달
```

### `sigchld_handler()`

```text
SIGCHLD 수신
→ waitpid()
→ child 상태 확인

정상 종료
→ job 삭제

signal 종료
→ 메시지 출력
→ job 삭제

stop
→ state = ST
→ job은 삭제하지 않음
```

* handler 안에서 `while`로 `waitpid()`를 반복
  * 한 번의 `SIGCHLD`가 왔을 때 여러 child의 상태가 변했을 수도 있기 때문

* `errno` 저장/복원

```c
int olderrno = errno;

/* handler */

errno = olderrno;
```

  * signal handler 실행 때문에 기존 코드의 `errno` 값이 망가지는 것을 방지

---

## Shell Lab 전체 흐름

```text
사용자 명령 입력
→ eval()
→ builtin인가?
   ├─ yes → jobs / bg / fg / quit
   └─ no
       → SIGCHLD block
       → fork()
       → child: setpgid + execve
       → parent: addjob
       → signal mask 복원
       → FG면 waitfg()
```

그리고 실행 중에는:

```text
Ctrl-C
→ SIGINT
→ foreground process group 종료

Ctrl-Z
→ SIGTSTP
→ foreground process group 정지

bg
→ SIGCONT + BG

fg
→ SIGCONT + FG + waitfg

child 종료/정지
→ SIGCHLD
→ job list 갱신
```
