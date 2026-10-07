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