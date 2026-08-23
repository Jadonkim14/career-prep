# MIT Missing Semester 학습 정리

# MIT Missing Semester — 목차

## A. The Shell

1. Shell / Bash / Shebang
2. PATH / 환경 변수
3. 기본 파일·텍스트 명령어
4. Glob / 와일드카드
5. 따옴표
6. stdin / stdout / stderr / Redirect
7. 종료 상태와 조건 실행
8. Bash 조건문
9. Shell Builtin
10. `set -x` / Strict Mode
11. Command Substitution
12. Script Arguments
13. Pipe
14. Regex 기초
15. `xargs`
16. `curl`
17. `jq`
18. `awk`
19. Shell Pipeline
20. 프로젝트 적용 및 트러블슈팅

## B. Command-line Environment

1. Signals / Job Control
2. Process 관리
3. `tmux`
4. Alias / Dotfiles
5. SSH
6. SSH Key / Config / 보안
7. SSH Port Forwarding
8. Mosh / SSH + tmux
9. Process / Port / HTTP / Log 기반 장애 분석

## C. Development Environment and Tools

1. Development Environment
2. Vim
3. Language Server / LSP
4. Python Environment
5. AI-powered Development
6. IDE Extensions

## D. Debugging and Profiling

1. Debugging 기본 흐름
2. GDB / Debugger
3. AddressSanitizer
4. `strace`
5. File Descriptor
6. Profiling
7. `perf` / Flame Graph / `hyperfine`
8. Resource Monitoring
9. Logging
10. 문제 재현 → 원인 추적 → 검증

## E. Version Control and Git

1. Git Data Model
2. Blob / Tree / Commit
3. Objects / References / HEAD
4. Staging Area
5. Branch / Merge
6. Merge Conflict
7. Rebase
8. Remote / Fetch / Pull / Push
9. Stash / Revert / `.gitignore`
10. Git History 조사 및 수정
11. GitHub / Pull Request
12. Git Workflow

## F. Packaging, Dependencies & Containers

1. Dependencies / Environments
2. Packaging / Artifact
3. Versioning / Lockfile / Reproducibility
4. Docker
5. Dockerfile / Image / Container
6. Configuration / Environment Variables
7. Docker Compose
8. Registry / Publishing
9. Build → Artifact → Deploy

## G. Agentic Coding

1. Coding Agent
2. Tool Use
3. Feedback Loop
4. Feature / Fix / Refactoring / Review 활용
5. Model / Context
6. Context Management
7. `AGENTS.md`
8. Skills / Subagents
9. MCP
10. Parallel Agents
11. Privacy / Security
12. AI Coding 검증
13. Sandbox / 격리 환경

## H. Beyond the Code

1. 좋은 Engineer의 기본 원칙
2. Comment
3. README
4. Commit
5. Bug Report
6. Minimal Reproducible Example
7. Pull Request
8. Code Review
9. 좋은 질문 방법
10. AI 사용 원칙
11. Why 기록하기

## I. Code Quality

1. 코드 품질 자동화
2. Formatter
3. Linter / Static Analysis
4. Compiler Warning
5. Testing
6. Code Coverage
7. Pre-commit
8. Continuous Integration
9. Regex
10. Semgrep
11. Parser vs Regex
12. Format → Analyze → Test → CI

---

# 전체 통합 핵심

1. **Shell & Linux** — 시스템을 조작하고 관찰
2. **Development Tools** — 효율적으로 코드 작성·탐색
3. **Debugging & Profiling** — 문제의 Root Cause와 병목 분석
4. **Git** — 변경사항과 History 관리
5. **Packaging & Docker** — 재현 가능한 실행환경과 배포
6. **Agentic Coding** — AI + Tool + Feedback Loop
7. **Beyond the Code** — 협업과 Why 기록
8. **Code Quality** — 품질 검사를 자동화
9. **통합 Workflow** — `Problem → Reproduce → Root Cause → Fix → Verify → Automate → Document` 


# A. The Shell

> 날짜: 2026-08-13  
> 범위: Shell 기본, 파일/텍스트 처리, Bash, Pipe, Redirect, Regex, `xargs`, `curl`, `jq`, `awk`

## 1. Shell / Bash / Shebang

- **Shell(命令行外壳)**: 텍스트 명령어로 컴퓨터를 조작하는 인터페이스.
- **Bash(Bash Shell)**: Shell의 한 종류.
- **Terminal(终端)**: Shell을 화면에서 사용하는 프로그램.
- **Shebang(解释器声明)**: 스크립트를 어떤 인터프리터로 실행할지 지정.

```bash
#!/bin/bash
```

실행 권한(执行权限)이 있으면:

```bash
./check.sh test.txt
```

실행 권한(执行权限) 부여:

```bash
chmod +x check.sh
```

`./`는 **현재 디렉터리의 파일을 지정하는 경로**이지, 실행 명령어 자체가 아니다.

---

## 2. PATH / 환경 변수(环境变量)

```bash
echo $PATH
which echo
```

- `$PATH`: 환경 변수(环境变量). 명령어 실행 시 프로그램을 찾는 디렉터리 목록.
- `which`: 실제 실행되는 프로그램 경로(程序路径) 확인.

---

## 3. 기본 파일/텍스트 명령어

| 명령어 | 역할 |
|---|---|
| `cat file` | 파일 내용 출력(文件内容输出) |
| `sort file` | 줄 정렬(行排序) |
| `uniq file` | 연속 중복 줄 제거(去除连续重复行) |
| `uniq -c` | 연속 중복 줄의 개수 계산(计数)(计数) |
| `head` | 앞부분 출력 |
| `tail` | 뒷부분 출력 |
| `grep pattern file` | 패턴과 일치하는 줄 검색(模式匹配) |
| `find` | 조건에 맞는 파일 검색(文件查找) |
| `sed` | 텍스트 치환/편집(文本替换/编辑) |
| `awk` | 열 단위 데이터 처리(数据处理)(按列处理数据) |
| `wc -l` | 줄 수 계산 |

### `find` 예시

```bash
find ~/Downloads -type f -name "*.zip" -mtime +30
```

- `-type f`: 파일만
- `-name "*.zip"`: `.zip` 파일
- `-mtime +30`: 30일보다 오래된 파일

---

## 4. Glob / 와일드카드(通配符)

Glob은 **파일 이름 패턴 매칭(匹配)(文件名模式匹配)**에 사용한다.

```text
*         임의의 문자 0개 이상
?         임의의 문자 정확히 1개
{a,b,c}   a, b, c 각각으로 확장
```

예시:

```bash
ls *.txt
ls file?.txt
ls {a,b,c}.txt
```

---

## 5. 따옴표

```bash
name="Kim"
echo 'Hello $name'   # Hello $name
echo "Hello $name"   # Hello Kim
echo $'Hello\nWorld'
```

- `'...'`: 거의 그대로 해석.
- `"..."`: `$변수` 등의 확장 허용.
- `$'...'`: `\n` 같은 ANSI escape 사용 가능.

---

## 6. stdin / stdout / stderr와 Redirect(重定向)

표준 스트림(标准流):

```text
stdin   = 0  입력(标准输入)
stdout  = 1  정상 출력(标准输出)
stderr  = 2  오류 출력(标准错误)
```

예시:

```bash
command > stdout.txt
command 2> stderr.txt
command > all.txt 2>&1
```

`2>&1` = **stderr(2)를 현재 stdout(1)이 가는 곳으로 보내라.**

---

## 7. 종료 상태(退出状态)와 조건 실행

```bash
echo $?
```

- `0`: 성공
- `0 이외`: 실패

```bash
command1 && command2   # command1 성공 시 command2
command1 || command2   # command1 실패 시 command2
```

실습:

```bash
test -d /tmp/mydir || mkdir /tmp/mydir
```

→ `/tmp/mydir`가 없을 때만 생성.

---

## 8. Bash 조건문(条件语句)

```bash
#!/bin/bash

if [ -f "$1" ]; then
    echo "file exist"
else
    echo "file not exist"
fi
```

- `$1`: 첫 번째 인자.
- `[ -f "$1" ]`: 해당 파일이 존재하는 일반 파일인지 검사.

실행 예:

```bash
./check.sh test.txt
```

---

## 9. `cd`가 Shell Builtin(内置命令)인 이유

별도 프로그램(자식 프로세스(子进程))이 자신의 디렉터리를 바꿔도 **부모 Shell(父 Shell)의 현재 디렉터리는 바꿀 수 없다.**

따라서 현재 Shell 자체의 작업 디렉터리를 변경해야 하는 `cd`는 Shell 내부 명령어여야 한다.

---

## 10. `set -x`

```bash
set -x
```

스크립트가 실행하는 명령어를 화면에 표시한다. 디버깅(调试)할 때 유용하다.

강의에서 함께 나온 strict mode:

```bash
set -euo pipefail
```

- `-e`: 명령 실패 시 종료
- `-u`: 정의되지 않은 변수 사용 시 오류
- `pipefail`: pipe 중간 명령 실패도 전체 실패로 처리

---

## 11. Command Substitution / 명령 치환(命令替换)

```bash
$(command)
```

명령어 실행 결과를 그 자리에 넣는다.

예시:

```bash
cp test.txt test_$(date +%Y-%m-%d).txt
```

---

## 12. `$@`

스크립트에 전달된 **모든 인자(所有参数)**를 의미한다.

```bash
./script.sh cargo test my_test
```

스크립트 내부:

```bash
while "$@" > "$LOGFILE" 2>&1; do
    ...
done
```

`"$@"` → `cargo`, `test`, `my_test`를 각각의 인자로 유지하여 명령으로 실행.

---

## 13. Pipe(管道) `|`

```bash
command1 | command2
```

`command1`의 **stdout(标准输出)을 command2의 stdin(标准输入)으로 전달**한다.

예:

```bash
cat ~/.bash_history | awk '{print $1}' | sort | uniq -c | sort -nr | head
```

흐름:

```text
history 읽기
→ 첫 번째 단어 추출
→ 같은 명령끼리 정렬
→ 사용 횟수 계산
→ 횟수 내림차순 정렬
→ Top 10
```

실습 결과에서는 `git`이 가장 많이 사용된 명령어였다.

---

## 14. 정규표현식(Regex / 正则表达式) 기초

학습한 패턴:

```regex
\.[^/]+$
```

읽는 방법:

```text
\.      실제 점(.)
[^/]    /가 아닌 문자 하나
+       앞 패턴이 1개 이상
$       여기서 문자열/줄이 끝나야 함
```

즉 대략 **`.확장자` 형태로 끝나는 경로**를 찾는 패턴.

추가:

```text
^   시작
$   끝
```

Regex는 통째로 암기하기보다 **왼쪽부터 조건을 읽는 방식**으로 이해한다.

---

## 15. 확장자 사용 빈도 파이프라인

```bash
find ~ -type f \
| grep -E '\.[^/]+$' \
| awk -F'.' '{print $NF}' \
| sort \
| uniq -c \
| sort -nr \
| head -5
```

핵심:

- `find`: 파일 찾기(文件查找)
- `grep`: 필요한 경로만 필터링(过滤)
- `awk`: 확장자 추출(提取扩展名)
- 첫 `sort`: 같은 확장자끼리 모음
- `uniq -c`: 개수 계산(计数)
- 두 번째 `sort -nr`: 개수 내림차순(降序)
- `head -5`: Top 5

---

## 16. `xargs`

`xargs`는 **stdin(标准输入)의 텍스트를 다른 명령어의 argument(参数)로 바꿔준다.**

```bash
find . -name "*.sh" | xargs wc -l
```

개념적으로:

```bash
wc -l ./check.sh ./xtest.sh
```

처럼 실행된다.

### 공백이 있는 파일명 처리(处理含空格的文件名)

```bash
find . -name "*.sh" -print0 | xargs -0 wc -l
```

- `-print0`: 파일들을 NUL 문자로 구분
- `xargs -0`: NUL을 구분자로 읽음

따라서 `my script.sh` 같은 파일명도 안전하게 처리 가능.

---

## 17. `curl`

```bash
curl URL
curl -s URL
curl -sS URL
```

- `curl`: URL에 요청을 보내 데이터 가져오기(发送请求并获取数据).
- `-s`: 진행률 등 부가 출력 숨김.
- `-S`: `-s`를 사용해도 오류는 표시.

클라우드/인프라에서 API 호출(API 调用), 서버 응답 확인(服务器响应检查), health check 등에 자주 사용.

### 강의 개수 세기 실습

```bash
curl -s https://missing.csail.mit.edu/ \
| grep 'href="/2026/' \
| grep -v '<span class="nav-link">' \
| wc -l
```

- `grep -v`: 해당 패턴이 있는 줄을 **제외**.

---

## 18. `jq`

JSON 처리 도구(JSON 处理工具).

```bash
curl -sS URL | jq .
```

`jq .` → JSON 전체를 구조화해서 출력.

실습:

```bash
curl -sS https://microsoftedge.github.io/Demos/json-dummy-data/64KB.json \
| jq '.[] | select(.version > 6) | .name'
```

해석:

```text
.[]                    배열 요소(数组元素)를 하나씩 꺼냄
select(.version > 6)   version이 6보다 큰 객체만 통과
.name                  통과한 객체의 name 추출
```

`.version`은 현재 객체(当前对象)의 `version` 필드 값을 가져오는 표현이다. 다른 필드를 삭제하는 것은 아니다.

---

## 19. `awk`

열(column / 列) 단위 처리에 유용하다.

```awk
$1   첫 번째 열
$2   두 번째 열
$3   세 번째 열
```

조건 + 출력:

```bash
printf 'a 50 x\nb 150 y\nc 200 z\n' \
| awk '$2 > 100 {print $3, $2, $1}'
```

결과:

```text
y 150 b
z 200 c
```

### Regex 매칭(匹配)

```awk
$3 ~ /pattern/
```

→ 세 번째 열이 regex `pattern`과 매칭(匹配)되는지 검사.

```text
~    매칭(匹配)됨
!~   매칭(匹配)되지 않음
```

예:

```awk
awk '$3 ~ /ERROR/ {print}'
```

### 열 비우기

```awk
{$4=""; print}
```

→ 네 번째 열 값을 빈 문자열로 만든 후 전체 레코드를 출력.

---

## 20. SSH 로그 파이프라인 해석

강의 예제의 처리 흐름:

```text
ssh + journalctl
→ 원격 서버(远程服务器) SSH 로그

grep
→ 연결 종료 로그 필터링(过滤)

sed
→ username 추출

sort | uniq -c
→ username별 등장 횟수(出现次数) 계산

sort ... | tail -n10
→ 가장 많이 등장한 10개

awk
→ username만 추출

paste -sd,
→ 여러 줄을 쉼표로 한 줄에 결합
```

핵심은 **작은 명령어 여러 개를 pipe로 연결해 하나의 데이터 처리(数据处理) 작업을 만드는 것**.

---

# 오늘 반드시 기억할 핵심

```text
|            앞 명령 stdout(标准输出) → 뒤 명령 stdin(标准输入)
>            stdout redirect(标准输出重定向)
2>           stderr redirect(标准错误重定向)
2>&1         stderr를 stdout이 가는 곳으로
$?           직전 명령 exit status(退出状态)
&&           앞 명령 성공 시 실행
||           앞 명령 실패 시 실행
$1           첫 번째 스크립트 인자
"$@"         모든 인자(所有参数)를 각각 유지
$(command)   명령 실행 결과 삽입
```

그리고 도구의 역할:

```text
find    파일 찾기(文件查找)
grep    줄 필터링(过滤)
sed     텍스트 치환
awk     열/레코드 처리
sort    정렬
uniq -c 빈도 계산
head    앞부분/Top N
xargs   stdin(标准输入) → 명령어 인자(参数)
curl    HTTP 데이터 가져오기(获取 HTTP 数据)
jq      JSON 처리
```

## 학습 기준

복잡한 `grep`/`awk`/Regex 원라이너를 통째로 암기하는 것이 목표가 아니다.

중요한 것은:

1. 각 명령어가 대략 무엇을 하는지 안다.
2. `|`로 데이터를 다음 단계에 넘기는 구조를 이해한다.
3. 필요한 작업을 `찾기 → 필터링(过滤) → 추출 → 집계 → 정렬`처럼 단계로 나눌 수 있다.
4. 세부 옵션이나 복잡한 Regex는 필요할 때 문서에서 찾아 사용할 수 있다.

### 프로젝트 적용 — `晚上吃啥`

Shell에서 배운 명령어를 실제 Backend 상태 점검에 적용했다.

```bash
# 실행 중인 Uvicorn 프로세스 확인
ps aux | grep uvicorn

# 8000번 포트가 LISTEN 중인지 확인
ss -ltnp | grep :8000

# HTTP 응답 확인
curl http://127.0.0.1:8000

# HTTP 상태 코드와 Header까지 확인
curl -i http://127.0.0.1:8000/health
curl -i http://127.0.0.1:8000/ready

# OpenAPI JSON 확인
curl -s http://127.0.0.1:8000/openapi.json | jq '.paths'
핵심 트러블슈팅 흐름

Process(进程) → Port(端口) → HTTP → Health Check(健康检查) → Readiness Check(就绪检查)

프로세스가 존재한다고 서비스가 정상이라는 뜻은 아니다.
포트가 LISTEN 중인지 확인한다.
curl로 실제 HTTP 요청에 응답하는지 확인한다.
/health, /ready를 통해 애플리케이션 상태를 추가로 확인한다.
/에서 404 Not Found가 발생해도 서버 장애라고 단정할 수 없다. 해당 Endpoint가 없을 수도 있다.
현재 작업 디렉터리(CWD / 当前工作目录)에 따라 프로젝트 환경과 상대 경로 탐색 결과가 달라질 수 있다.

실무 관점: 명령어 자체보다 프로세스 → 포트 → HTTP → 애플리케이션 상태 순서로 문제 범위를 좁혀가는 것이 중요하다.


# B. Command-line Environment

> 날짜: 2026-08-15
> 범위: Job Control, Process, tmux, Dotfiles, SSH, Port Forwarding

## 1. Signals / Job Control

### Foreground / Background

```bash
sleep 10000
```

`Ctrl+Z` → 프로세스를 종료하지 않고 **Stopped(暂停)** 상태로 전환.

```bash
bg
jobs
```

* `bg`: 멈춘 작업을 Background에서 계속 실행
* `jobs`: 현재 Shell의 Job 확인

```bash
kill %1
```

`%1`은 PID가 아니라 **현재 Shell의 1번 Job**.

### Process 검색 / 종료

```bash
pgrep -af uvicorn
pkill -f "sleep 10000"
```

* `pgrep`: Process 검색
* `pkill`: 이름/패턴으로 Process에 Signal 전달

### `kill -0`

```bash
kill -0 PID
```

Process를 죽이지 않고 **존재 여부를 exit status로 확인**한다.

```bash
pidwait() {
    while kill -0 "$1" 2>/dev/null; do
        sleep 1
    done
}
```

---

## 2. 파일 최신순 검색

```bash
find . -type f -printf '%T@ %p\n' | sort -rn
```

가장 최근 파일:

```bash
find . -type f -printf '%T@ %p\n' | sort -rn | head -1
```

전체 명령을 암기하기보다:

```text
find → 찾기
sort → 정렬
head → 일부 선택
```

처럼 조합을 이해한다.

---

## 3. tmux

`tmux`는 하나의 Terminal에서 여러 Shell을 관리하고 **Session을 유지**하는 도구.

```bash
tmux new -s wanshang
tmux attach -t wanshang
tmux ls
```

주요 조작:

```text
Ctrl+B → %       좌우 Pane
Ctrl+B → "       위아래 Pane
Ctrl+B → 방향키   Pane 이동
Ctrl+B → c       새 Window
Ctrl+B → d       Detach
```

SSH 연결이 끊겨도 서버의 tmux Session과 그 안의 작업은 계속 살아 있을 수 있다.

---

## 4. Alias / Dotfiles

Alias:

```bash
alias dc='cd'
alias gs='git status'
```

Dotfiles는 Shell, tmux 등의 사용자 설정 파일이다.

```text
~/dotfiles/
├── bash_aliases
├── tmux.conf
├── starship.toml
└── install.sh
```

Symbolic Link(符号链接):

```bash
ln -s ~/dotfiles/tmux.conf ~/.tmux.conf
```

설정을 GitHub에 저장하고 `install.sh`로 새 Linux 환경에서 자동 복원하도록 구성했다.

```text
Git clone
   ↓
install.sh
   ↓
Symbolic Link 생성
   ↓
개발환경 설정 복원
```

---

## 5. SSH

SSH(Secure Shell)는 원격 머신에 암호화된 연결을 생성한다.

```bash
ssh user@server
```

### SSH Key

```bash
ssh-keygen -a 100 -t ed25519
```

```text
id_ed25519      Private Key(私钥) → 공유 금지
id_ed25519.pub  Public Key(公钥)  → 서버에 등록 가능
```

Public Key 등록:

```bash
ssh-copy-id vm
```

이후:

```bash
ssh vm
```

으로 Key 기반 인증 가능.

### `~/.ssh/config`

```ssh
Host vm
    User username
    HostName server_ip
    IdentityFile ~/.ssh/id_ed25519
```

긴 접속 정보를 저장하여 `ssh vm`처럼 간단히 접속할 수 있다.

---

## 6. SSH 보안 설정

`/etc/ssh/sshd_config`:

```text
PasswordAuthentication no
PermitRootLogin no
```

* Password 인증 차단
* root 직접 SSH 로그인 차단
* SSH Key 인증 사용

설정 검사:

```bash
sudo sshd -t
```

---

## 7. SSH Port Forwarding

```ssh
LocalForward 9999 localhost:8888
```

구조:

```text
내 PC localhost:9999
        ↓
     SSH Tunnel
        ↓
서버 localhost:8888
```

즉 **원격/내부 서비스를 SSH 터널을 통해 내 컴퓨터에서 접근**할 수 있다.

Background Tunnel:

```bash
ssh -fN -L 9999:localhost:8888 vm
```

* `-N`: 원격 Shell 실행 안 함
* `-f`: Background 실행
* `-L`: Local Port Forwarding

세부 옵션보다 **내부 서비스를 외부에 직접 공개하지 않고 SSH를 통해 접근할 수 있다는 개념**이 중요하다.

---

## 8. Mosh / SSH + tmux

Mosh는 네트워크가 불안정하거나 IP가 변경되는 환경에서 원격 Shell 연결 복구에 강하다.

하지만 SSH의 상위 버전은 아니다.

일반적인 서버 관리에서는 우선:

```text
SSH       필수
tmux      중요
mosh      선택
```

SSH + tmux만으로도 연결이 끊긴 뒤 작업을 복구할 수 있다.

---

## 9. `晚上吃啥` 프로젝트 적용

### Process 확인

```bash
pgrep -af uvicorn
```

### Port 확인

```bash
ss -ltnp | grep 8000
```

* `ss`: Socket / Port 상태 확인
* `LISTEN`: 요청을 받을 준비가 된 상태

### HTTP 확인

```bash
curl http://127.0.0.1:8000
```

`404 Not Found`가 나와도 서버 장애라는 의미는 아니다. 서버가 요청을 받고 있지만 해당 Endpoint가 없을 수 있다.

### Log 저장 / 관찰

```bash
uv run uvicorn app.main:app > logs/backend.log 2>&1 &
tail -f logs/backend.log
```

실제 요청이 Uvicorn access log에 기록되는 것을 확인했다.

---

## 10. 장애 유형 구분

### Process가 죽은 경우

```text
Process 없음
    ↓
8000 LISTEN 없음
    ↓
curl
    ↓
Connection refused
```

확인:

```bash
pgrep -af uvicorn
ss -ltnp | grep 8000
```

### Application 오류

```text
Process 정상
    ↓
Port 정상
    ↓
HTTP 연결 성공
    ↓
Application 실행 실패
    ↓
500 Internal Server Error
```

이 경우 로그와 traceback을 확인한다.

### 실제 API 테스트

```text
GET /api/v1/decisions/candidates
```

* 정상 parameter → `200 OK`
* 필수 parameter 누락 → `422 Validation Error`

---

# 오늘 반드시 기억할 핵심

```text
pgrep      Process 확인
jobs       Shell Job 확인
kill       Signal 전달

ss         Port / Socket 확인
curl       HTTP 요청 확인
tail -f    실시간 Log 확인

tmux       Session 유지 / Terminal 분할
ssh        원격 서버 접속
```

가장 중요한 트러블슈팅 흐름:

```text
서비스가 안 됨
    ↓
curl
HTTP 요청이 되는가?
    ↓
ss
Port가 열려 있는가?
    ↓
pgrep / ps
Process가 살아 있는가?
    ↓
logs
Application에서 무슨 일이 발생했는가?
```

## 학습 기준

명령어 옵션을 전부 암기하는 것이 목표가 아니다.

**Process(进程) → Port(端口) → HTTP → Application → Log**

순서로 문제 범위를 좁히는 사고방식을 익히는 것이 핵심이다.


# C. Development Environment and Tools

> 날짜: 2026-08-15
> 범위: Vim, Language Server, AI Development, IDE Extensions

## 1. Development Environment

개발에 필요한 도구들의 집합.

```text
IDE → VS Code처럼 여러 개발 기능을 통합
Terminal Workflow → Vim, tmux, Shell 등의 도구를 조합
```

둘 다 기본적인 사용법을 익히는 것이 좋다.

---

## 2. Vim

Vim은 **키보드 중심 Text Editor(文本编辑器)**.

주요 Mode:

```text
i        Insert Mode
ESC      Normal Mode
v / V    Visual Mode
:        Command-line Mode
```

주요 이동:

```text
h j k l    ← ↓ ↑ →
w / b      단어 이동
0 / $      줄 처음 / 끝
gg / G     파일 처음 / 끝
/word      검색
n / N      다음 / 이전 검색 결과
%          대응하는 괄호
```

핵심은 명령어 조합:

```text
Verb + Motion

dw       단어 삭제
d$       줄 끝까지 삭제
cw       단어 변경
```

Text Object:

```text
i = inside
a = around

ci"      따옴표 내부 변경
da"      따옴표 포함 전체 삭제
```

기타:

```text
o / O    아래 / 위 새 줄
u        Undo
Ctrl-r   Redo
y / p    Copy / Paste
```

---

## 3. Language Server / LSP

Language Server(语言服务器)는 IDE에서 **코드를 의미적으로 분석**한다.

```text
VS Code
   ↓
Extension
   ↓
Language Server
```

Python에서는 Pylance 등이 사용된다.

주요 기능:

```text
Autocomplete
Hover Documentation
Go to Definition
Find All References
Import / Type 검사
```

**LSP(Language Server Protocol)**는 Editor와 Language Server가 통신하는 표준.

---

## 4. Python Environment

Language Server가 올바른 Dependency를 찾으려면 **프로젝트가 실제 사용하는 Python Environment**를 선택해야 한다.

`晚上吃啥`에서는:

```text
backend/.venv/bin/python
```

을 선택했고,

```text
Pylance
→ FastAPI Dependency 인식
→ Go to Definition 정상
```

을 확인했다.

---

## 5. AI-powered Development

주요 형태:

```text
Autocomplete
→ Cursor 뒤의 코드 생성

Inline Chat
→ 기존 코드 선택 후 수정

Coding Agent
→ 여러 파일을 포함한 더 큰 작업 수행
```

함수명, 타입, Docstring, Comment 등이 AI에게 Context가 된다.

**AI가 생성한 코드는 정답이 아니라 검토해야 할 제안**이다.

---

## 6. IDE Extensions

Extension으로 IDE 기능을 확장할 수 있다.

예:

```text
Remote SSH       원격 서버 개발
Dev Containers   Container 기반 개발환경
Live Share       공동 편집
markdownlint     Markdown 스타일 검사
```

실습에서는 `markdownlint`를 설치하고 `MD041` Warning을 확인했다.

---

# 오늘 반드시 기억할 핵심

```text
Vim
→ 키보드로 빠르게 탐색/편집
→ Verb + Motion 조합

Language Server
→ 코드 구조와 Dependency 분석

LSP
→ Editor ↔ Language Server 통신 표준

Python
→ 프로젝트에 맞는 Interpreter 선택

AI
→ Autocomplete / Inline Chat / Agent

Extension
→ 필요한 IDE 기능 추가
```

**핵심:** 도구 사용법을 전부 암기하는 게 아니라 **각 도구가 어떤 역할을 하고 언제 사용하는지 이해하는 것**.


# D. Debugging and Profiling

> 날짜: 2026-08-16
> 범위: Debugging, Profiling, Resource Monitoring, Logging

## 1. Debugging

프로그램의 문제를 **재현 → 관찰 → 원인 추적 → 수정 → 검증**하는 과정.

주요 도구:

```text
GDB / VS Code Debugger
→ Breakpoint, 변수, Call Stack 확인

rr
→ 실행을 기록하고 Reverse Debugging
→ watchpoint로 값이 변경된 시점 추적

AddressSanitizer
→ 잘못된 Memory 접근 탐지
→ heap-use-after-free 확인

strace
→ 프로그램의 System Call 추적
```

---

## 2. File Descriptor

프로세스가 열어놓은 파일·Socket 등의 **번호표**.

```text
0 → stdin
1 → stdout
2 → stderr
3... → 파일, Socket 등
```

```text
PID  → Process 식별
FD   → Process 내부의 열린 자원 식별
Port → Network Endpoint 식별
```

---

## 3. Profiling

프로그램의 **성능 병목을 측정하고 찾는 과정**.

```text
perf stat
→ CPU cycles, instructions, page faults 등 확인

perf record / report
→ CPU 시간을 많이 사용하는 함수 탐색

Flame Graph
→ Profiling 결과 시각화

hyperfine
→ 여러 프로그램의 실행 성능 비교
```

**핵심:**

```text
추측해서 최적화 ❌
측정 → 병목 발견 → 최적화 ✅
```

---

## 4. Resource Monitoring

주요 Linux 도구:

```text
htop   → CPU / Memory / Process
free   → Memory
iotop  → Disk I/O
lsof   → 열린 파일
ss     → Network Socket
```

Port를 사용하는 Process 확인:

```bash
ss -tlnp | grep :8000
```

문제가 있다면:

```text
Port 확인
→ PID 확인
→ Process 확인
→ 필요 시 kill
```

---

## 5. 晚上吃啥 적용

FastAPI Backend에서 실제로:

```text
Process
→ Port
→ HTTP
→ /health
→ /ready
```

순서로 서비스 상태를 확인했다.

```bash
ss -tlnp | grep :8000

curl http://127.0.0.1:8000/health
curl http://127.0.0.1:8000/ready
```

---

## 6. Request Logging

FastAPI Middleware를 사용해 모든 HTTP 요청을 관찰했다.

```text
Request
→ Middleware
→ Endpoint
→ Middleware
→ Response
```

기록 정보:

```text
method
path
status
latency_ms
```

예:

```text
2026-08-16 15:09:20 INFO wanshang
method=GET path=/health status=200 latency_ms=3.65
```

Logging 구조:

```text
Logger
→ Handler
→ Formatter
→ Terminal
```

Log Level:

```text
2xx / 3xx → INFO
4xx       → WARNING
5xx       → ERROR
느린 요청 → WARNING
```

실제로 `200 OK`라도 약 1초가 걸린 요청을 `WARNING`으로 탐지했다.

---

# 오늘 반드시 기억할 핵심

```text
Debugger → 실행 상태 확인
ASan     → Memory 오류 탐지
strace   → System Call 추적
perf     → CPU 병목 분석
Flame Graph → 병목 시각화
hyperfine → 성능 비교
htop / ss → System 상태 확인

Logging
→ Request의 status + latency 관찰
```

**핵심 흐름:**

```text
문제 재현
→ 관찰
→ 측정
→ 원인 추적
→ 수정
→ 검증
```

명령어를 전부 외우는 것보다 **어떤 문제가 생겼을 때 어떤 도구로 관찰해야 하는지 아는 것이 중요하다.**


# E. Version Control and Git

> 날짜: 2026-08-17
> 범위: Git Data Model, Staging, Branch, Merge, Remote, History, Git Workflow

## 1. Git Data Model

Git은 파일의 변경 이력을 **Snapshot** 단위로 관리한다.

```text
Blob
→ 파일의 내용

Tree
→ 파일·디렉터리 구조

Commit
→ Snapshot + Parent + Metadata
```

Commit들은 이전 Commit을 `parent`로 가리키며 **DAG(Directed Acyclic Graph)** 형태의 History를 만든다.

```text
A ← B ← C
     \
      D
```

Merge Commit은 여러 개의 Parent를 가질 수 있다.

---

## 2. Objects와 References

Git의 주요 Object:

```text
Blob
Tree
Commit
```

Object는 내용에 기반한 **Hash**로 식별된다.

```text
Object
→ Hash 계산
→ Object ID
```

Tree나 Commit은 다른 Object 자체를 포함하는 대신 **Hash를 통해 참조**한다.

```text
Commit
→ Tree Hash
→ Tree
→ Blob Hash
→ 파일 내용
```

Object는 **Immutable**이다. 내용을 변경하면 기존 Object를 수정하는 것이 아니라 새로운 Object가 생성된다.

Reference는 Commit을 가리키는 변경 가능한 Pointer다.

```text
main / master
→ 특정 Commit을 가리킴

HEAD
→ 현재 History에서의 위치
```

**Repository의 핵심:**

```text
Git Repository
≈ Objects + References
```

---

## 3. Staging Area

Git은 Working Directory 전체를 바로 Commit하지 않고 **다음 Snapshot에 포함할 변경사항을 선택**할 수 있다.

```text
Working Directory
→ git add
→ Staging Area
→ git commit
→ Commit / Snapshot
```

```bash
git status
git diff
git add <file>
git commit
```

**핵심:**

```text
git add
→ 다음 Commit에 포함할 변경사항 선택

git commit
→ Staging Area를 기반으로 Snapshot 생성
```

---

## 4. Branch와 Merge

Branch는 특정 Commit을 가리키는 **Reference**다.

```bash
git branch
git branch <name>
git switch <name>
git switch -c <name>
git merge <branch>
```

Branch에서 새로운 Commit을 만들면 해당 Branch Reference가 새로운 Commit으로 이동한다.

```text
        C ← feature
       /
A ← B
    ↑
   main
```

Merge를 통해 서로 다른 개발 흐름을 합칠 수 있다.

같은 부분을 서로 다르게 수정하면 **Merge Conflict**가 발생할 수 있다.

```text
<<<<<<< HEAD
현재 Branch 내용
=======
상대 Branch 내용
>>>>>>> sweet
```

Conflict는 Git이 자동으로 어느 변경을 선택할지 결정할 수 없는 상태다.

```text
Conflict 확인
→ 원하는 내용으로 직접 수정
→ Conflict Marker 제거
→ git add
→ git commit
```

---

## 5. Rebase

`rebase`는 현재 Branch의 Commit들을 **새로운 Base 위에 다시 생성**한다.

```text
Merge
→ 두 History를 합침

Rebase
→ Commit을 새로운 Base 위에 다시 쌓음
→ Linear History 생성 가능
```

Commit은 Immutable이므로 기존 Commit을 실제로 이동시키는 것이 아니라 **새 Commit을 생성**한다.

따라서 Commit Hash도 변경된다.

---

## 6. Remote

Remote는 다른 위치에 존재하는 Git Repository다.

```text
origin
→ Clone한 Remote Repository의 기본 이름

main
→ Local Branch

origin/main
→ 마지막으로 확인한 Remote main의 상태
```

주요 명령:

```bash
git clone
git remote
git fetch
git pull
git push
```

```text
git fetch
→ Remote의 Objects / References 가져오기
→ 현재 Local Branch는 직접 변경하지 않음

git pull
≈ git fetch + git merge

git push
→ Local Objects 전송
→ Remote Reference 업데이트
```

---

## 7. Stash / Revert / Gitignore

### Stash

아직 Commit하기 애매한 변경사항을 **임시 보관**한다.

```bash
git stash
git stash list
git stash pop
```

```text
작업 중
→ stash
→ 다른 작업
→ stash pop
→ 기존 작업 계속
```

Stash는 일반적인 Branch History에 작업 Commit으로 추가되는 것은 아니지만, 내부적으로 Commit Object를 활용하며 `refs/stash`로 관리되는 것을 확인했다.

### Revert

특정 Commit을 삭제하는 것이 아니라 **그 Commit의 효과를 취소하는 새로운 Commit을 생성**한다.

```bash
git revert <commit>
```

```text
A ← B ← C
        ↓ revert
A ← B ← C ← D
            ↑
        C의 효과 취소
```

### `.gitignore`

Git이 의도적으로 추적하지 않을 파일을 지정한다.

```text
.env
*.log
.DS_Store
```

Global Ignore도 설정했다.

```bash
git config --global core.excludesfile ~/.gitignore_global
```

```text
.gitignore
→ Project 단위

~/.gitignore_global
→ 사용자 환경 전체
```

---

## 8. Git History 조사와 수정

특정 파일의 History:

```bash
git log -- README.md
```

특정 줄을 마지막으로 수정한 Commit:

```bash
git blame <file>
```

Commit 상세 확인:

```bash
git show <commit>
```

History를 그래프로 확인:

```bash
git log --all --graph --decorate --oneline
```

Alias도 설정했다.

```bash
git graph
```

→ 위의 긴 `git log` 명령을 간단하게 실행.

민감한 파일을 단순히 삭제하는 것과 **History 전체에서 제거하는 것**은 다르다.

```text
rm + commit
→ 과거 Snapshot에는 파일이 남음

History Rewrite
→ 과거 Snapshot에서도 제거
```

`git filter-repo`를 이용해 `secret.txt`를 History 전체에서 제거하는 실습을 진행했고, History가 다시 작성되면서 Commit Hash가 변경되는 것도 확인했다.

---

## 9. GitHub와 Pull Request

Git과 GitHub는 다르다.

```text
Git
→ Version Control System

GitHub
→ Git Repository Hosting + Collaboration
```

Pull Request(PR)는:

> **내 변경사항을 검토하고 원본 Branch/Repository에 합쳐달라고 요청하는 협업 방식**

기본 Workflow:

```text
Branch 생성
→ 작업
→ Commit
→ Push
→ Pull Request
→ Code Review / CI
→ Merge
```

---

# 오늘 반드시 기억할 핵심

```text
Blob   → 파일 내용
Tree   → 디렉터리 구조
Commit → Snapshot

Object
→ Immutable
→ Hash로 식별

Reference
→ Commit을 가리키는 Pointer
→ Mutable

HEAD
→ 현재 위치

Staging Area
→ 다음 Commit에 포함할 변경사항 선택
```

주요 작업 흐름:

```text
수정
→ git diff / status
→ git add
→ git commit
→ Branch
→ Merge / PR
→ Push
```

Remote:

```text
fetch → 가져오기
pull  → 가져오기 + 합치기
push  → 보내기
```

**핵심 사고방식:**

```text
Git 명령어 암기 ❌

이 명령이
Working Directory
Staging Area
Object
Reference
Commit DAG

중 무엇을 어떻게 변경하는지 이해 ✅
```

앞으로 `晚上吃啥`에서도 의미 있는 작업은 **Branch → 수정 → Commit → Merge/PR** 흐름으로 실제 적용한다.


# F. Packaging, Dependencies & Containers

> 날짜: 2026-08-19
> 범위: Dependencies, Environments, Packaging, Versioning, Docker, Configuration, Publishing

## 1. Dependencies & Environments

프로그램은 여러 Library에 의존한다.

```text
Application
→ Dependency
   → Transitive Dependency
```

프로젝트마다 필요한 버전이 다를 수 있으므로 **Virtual Environment**로 분리한다.

```text
pip  → Package 설치
venv → 가상환경 생성
uv   → 환경 + Dependency + Package 관리
```

---

## 2. Packaging & Artifact

```text
Source Code
→ Build
→ Artifact
→ 배포/설치
```

Python에서는:

```text
pyproject.toml → 프로젝트/Dependency 정의
uv.lock        → 정확한 Dependency Version
.whl           → 배포 가능한 Python Artifact
```

---

## 3. Versioning & Reproducibility

Semantic Versioning:

```text
MAJOR.MINOR.PATCH

PATCH → Bug Fix
MINOR → 호환되는 기능 추가
MAJOR → Breaking Change
```

Pinning:

```text
requests>=2.0      → Version 범위
requests==2.32.3   → 정확한 Version
```

Lockfile을 사용하면 다른 환경에서도 동일한 Dependency를 재현하기 쉽다.

---

## 4. Docker

Package Dependency를 넘어 OS/System Library까지 실행환경을 묶을 수 있다.

```text
Dockerfile
→ docker build
→ Image
→ docker run
→ Container
```

```text
Dockerfile → Image 생성 방법
Image      → 실행 가능한 Artifact
Container  → 실행 중인 Image Instance
```

Docker는 Layer Cache를 사용하므로 Dependency 파일을 먼저 복사하면 Build를 효율화할 수 있다.

---

## 5. Configuration

환경마다 달라지는 설정은 코드와 분리한다.

```text
Code
+
Environment Variable / Config
```

특히:

```text
API Key
Password
Database URL
```

등은 Source/Image에 넣지 않고 Runtime에 전달한다.

---

## 6. Services & Docker Compose

Application, DB, Cache 등을 별도 Container로 실행할 수 있다.

```text
Docker Compose
├── app
├── database
└── cache
```

Compose는 여러 Container의 실행, Network, Volume 등을 함께 관리한다.

---

## 7. Publishing

만든 Artifact를 Registry에 배포할 수 있다.

```text
Python Package
→ PyPI / TestPyPI

Docker Image
→ GHCR / Docker Hub
```

전체 흐름:

```text
Source
→ Build
→ Artifact
→ Version/Tag
→ Registry
→ Deploy
```

---

## 8. `晚上吃啥` 적용

```text
backend/Dockerfile 추가
backend/.dockerignore 추가
uv.lock 기반 Dependency 설치
Docker Image 생성
FastAPI Container 실행
/health 정상 응답 확인 ✅
```

DB, Frontend, Docker Compose는 추후 적용.

---

# 오늘 반드시 기억할 핵심

```text
Dependency → 프로그램이 의존하는 Software
Environment → 프로젝트별 실행환경 격리
Lockfile → Dependency Version 고정
Artifact → 배포 가능한 결과물
Dockerfile → Image 만드는 방법
Image → 실행환경을 Package한 Artifact
Container → 실행 중인 Image
Registry → Artifact 저장/배포 장소
```

**핵심 흐름:**

```text
Source
→ Dependencies
→ Build
→ Artifact
→ Registry
→ Deploy
→ Run
```

**명령어 암기보다 `왜 환경을 격리하고, 재현 가능한 Artifact를 만들어 배포하는가`를 이해하는 것이 핵심.**


# G. Agentic Coding

> 날짜: 2026-08-21
> 범위: Coding Agent, Tool Use, Feedback Loop, Context Management, AGENTS.md, Skills, Subagents, AI Coding Risks

## 1. Coding Agent

Coding Agent(코딩 에이전트)는 AI 모델이 단순히 답변만 생성하는 것이 아니라 **파일과 개발 도구를 직접 사용하면서 작업하는 형태**다.

```text
User Request
→ Agent
→ File Read / Edit
→ Shell Command
→ Test / Type Check
→ Result 확인
→ 수정 반복
```

주요 Tool:

```text
File Read / Write
Web Search
Shell Command
Compiler
Linter
Type Checker
Test
Git
```

일반적으로 위험한 명령을 실행하기 전에는 사용자에게 권한 확인을 요청한다.

---

## 2. Agent와 Feedback Loop

Coding Agent의 중요한 장점은 한 번 코드를 생성하고 끝나는 것이 아니라 **검사 결과를 보고 반복 수정할 수 있다는 것**이다.

```text
코드 작성
→ Compiler / Test 실행
→ Error 확인
→ 코드 수정
→ 다시 검사
→ 통과
```

예:

```text
Agent
→ 코드 수정
→ mypy 실행
→ Type Error
→ 원인 분석
→ 수정
→ mypy 재실행
```

따라서 단순히:

```text
"코드를 만들어줘"
```

보다:

```text
"코드를 수정하고 테스트까지 실행해서
실패하면 원인을 고쳐라"
```

처럼 **검증 도구와 연결하는 방식**이 효과적이다.

---

## 3. Coding Agent 사용 방식

Coding Agent는 여러 종류의 개발 작업에 사용할 수 있다.

### Feature Implementation

```text
Requirement
→ 코드 분석
→ 기능 구현
→ Test
→ 수정
```

Test-Driven Development 방식과 잘 맞는다.

```text
Test 작성
→ Test 검토
→ Agent에게 구현 요청
→ Test 실행
→ 실패 시 수정
```

### Error Fixing

Compiler, Linter, Type Checker, Test 등의 Error를 Agent에게 직접 확인시키고 수정하도록 할 수 있다.

```text
Failing Check
→ Agent
→ 원인 분석
→ 수정
→ Check 재실행
```

### Refactoring

```text
Method Rename
Module 분리
중복 코드 제거
구조 개선
```

등을 수행할 수 있다.

### Code Review

```text
현재 변경사항
→ Agent Review
→ Bug / Security / 구조 문제 확인
```

GitHub CLI 등을 사용할 수 있다면 Pull Request 검토에도 활용할 수 있다.

### Code Understanding

처음 보는 Codebase를 분석할 때:

```text
Entry Point
→ Function Call
→ Module
→ Data Flow
```

등을 추적하도록 할 수 있다.

---

## 4. AI Model과 Context

LLM은 입력이 주어졌을 때 가능한 출력의 **확률 분포를 기반으로 결과를 생성**한다.

따라서 출력이 항상 정확한 것은 아니다.

Coding Agent도 기본적으로:

```text
Conversation
+
Files
+
Tool Results
+
Instructions
```

를 Context로 사용한다.

LLM에는 한 번에 처리할 수 있는 **Context Window**의 크기 제한이 있다.

---

## 5. Context Management

Agent에게 많은 정보를 무조건 제공하는 것이 좋은 것은 아니다.

```text
필요한 Context
→ 제공 ✅

불필요한 Context
→ 줄이기 ✅
```

Context가 너무 커지면 필요한 정보를 찾기 어려워지고 성능이 떨어질 수 있다.

### Context 초기화

관련 없는 새로운 작업을 시작하면 새 Conversation을 사용하는 것이 좋다.

```text
Task A 완료
→ Context Clear
→ Task B 시작
```

### Rewind

Agent가 잘못된 방향으로 갔다면 계속 수정 지시를 추가하는 것보다 이전 상태로 돌아가 다시 진행하는 것이 나을 수 있다.

### Compaction

Conversation이 너무 길어지면 이전 내용을 요약하여 Context 크기를 줄일 수 있다.

```text
Long Conversation
→ Summary
→ 중요 Context 유지
```

---

## 6. AGENTS.md

`AGENTS.md`는 **Coding Agent를 위한 프로젝트 README**처럼 사용할 수 있다.

Agent가 프로젝트를 시작할 때 반복적으로 알아야 하는 정보를 기록한다.

예:

```text
Build 방법
Test 방법
Lint 방법
Coding Convention
Project Structure
주의사항
```

예를 들어:

```text
코드를 수정한 뒤 항상 Test 실행

Python 수정 후 mypy 실행

C 코드 수정 후 Compiler Warning 확인
```

같은 규칙을 넣을 수 있다.

일부 Agent는 `CLAUDE.md` 등 다른 이름을 사용한다.

---

## 7. Skills / Subagents

### Skills

`AGENTS.md`는 보통 항상 Context에 포함된다.

반면 Skill은 필요한 상황에만 불러올 수 있다.

```text
AGENTS.md
→ 항상 Load

Skill
→ 필요할 때 Load
```

따라서 Context가 불필요하게 커지는 것을 줄일 수 있다.

### Subagent

특정 작업을 담당하는 별도의 Agent를 둘 수도 있다.

```text
Main Agent
   ↓
Subagent
   ↓
특정 작업 수행
   ↓
결과 요약
   ↓
Main Agent
```

예:

```text
Main Agent
   ↓
Code Checker Agent
   ↓
mypy / ruff 실행
   ↓
결과 반환
```

각 Agent가 필요한 Context만 가지게 하여 작업을 분리할 수 있다.

---

## 8. MCP

MCP(Model Context Protocol)는 **Agent와 외부 Tool/System을 연결하기 위한 Protocol**이다.

예:

```text
Coding Agent
→ MCP
→ Notion
→ GitHub
→ 외부 Tool
```

이를 이용하면:

```text
Notion Spec 읽기
→ 구현 계획 작성
→ 코드 구현
```

같이 여러 시스템을 연결하는 Workflow를 만들 수 있다.

현재는 MCP 구현 자체보다:

> **Agent가 외부 Tool과 연결될 수 있는 표준 인터페이스가 있다**

정도로 이해하면 충분하다.

---

## 9. Parallel Agents

여러 Agent를 동시에 사용할 수도 있다.

```text
Agent A → Feature A

Agent B → Feature B

Agent C → 같은 문제의 다른 해결책
```

LLM의 결과가 확률적이므로 같은 작업을 여러 Agent에게 시켜 결과를 비교할 수도 있다.

서로 다른 Agent가 같은 Repository를 수정할 경우 변경사항 충돌을 막기 위해 `git worktree` 등을 사용할 수 있다.

현재 학습 단계에서는 개념만 알고 있으면 된다.

---

## 10. Privacy

많은 Coding Agent는 코드와 Context를 Cloud의 AI Model로 전송한다.

따라서 다음 정보는 특히 주의해야 한다.

```text
Private Source Code
API Key
Password
Credential
내부 문서
사용자 데이터
```

특히 회사나 조직의 코드에서는 허용된 AI Tool과 정책을 먼저 확인해야 한다.

---

## 11. AI Coding에서 주의할 점

AI가 생성한 코드는 **정답이 아니라 검증해야 하는 결과물**이다.

```text
AI Code
→ Compile
→ Test
→ Static Analysis
→ Code Review
→ 직접 이해
```

특히 다음 문제를 놓칠 수 있다.

```text
Logic Bug
Security Bug
Memory Error
Race Condition
Undefined Behavior
잘못된 Error Handling
```

중요한 코드는 AI가 작성한 코드를 검증하는 것이 직접 작성하는 것보다 더 어려울 수도 있다.

따라서:

```text
AI 의존
→ 이해 없이 코드 사용 ❌

AI 활용
→ 반복 작업 절약
→ 결과 검증
→ 직접 이해 ✅
```

가 중요하다.

---

## 12. Autonomous Agent와 격리 환경

일부 Coding Agent는 사용자 승인 없이 명령을 자동으로 실행하는 Mode를 지원한다.

하지만 실제 PC에서 강한 권한을 그대로 주는 것은 위험할 수 있다.

```text
Agent
+
Host System 직접 접근
→ 위험 증가
```

필요하다면:

```text
Virtual Machine
Container
Sandbox
```

같은 격리된 환경에서 실행한다.

```text
Agent
→ Isolated Environment
→ Code / Command 실행
```

Docker를 이용한 격리 개념과 연결해서 이해하면 된다.

---

## 13. System SW / Embedded 로드맵 적용

현재 목표에서는 AI 자체를 깊게 공부하는 것이 중요한 것은 아니다.

AI는 앞으로의 System Programming 학습을 **보조하는 Tool**로 사용한다.

```text
C / Linux / OS 공부
        ↓
직접 코드 작성
        ↓
Compiler / Test
        ↓
Debugging
        ↓
AI Review / 분석 보조
```

AI에게 맡기기 좋은 영역:

```text
Boilerplate
반복 코드
Test 초안
Documentation
Code Review 보조
Error Log 분석 보조
기존 Codebase 탐색
```

직접 이해해야 하는 영역:

```text
Pointer / Memory
System Call
Concurrency
Race Condition
Undefined Behavior
Architecture
Debugging
Root Cause Analysis
HW/SW Interaction
```

특히 앞으로 C 프로젝트에서는:

```text
gcc Warning
+
Sanitizer
+
Test
+
Agent Review
```

형태로 사용하면 좋다.

---

# 오늘 반드시 기억할 핵심

```text
Coding Agent
→ 파일/도구를 사용해 개발 작업을 수행하는 AI

Feedback Loop
→ 수정 → Test → Error → 재수정

Context
→ Agent가 판단할 때 사용하는 정보

Context Management
→ 필요한 정보만 제공

AGENTS.md
→ Agent용 Project README / 공통 규칙

Skill
→ 필요할 때 불러오는 Agent 지침

Subagent
→ 특정 작업을 담당하는 별도 Agent

MCP
→ Agent와 외부 Tool을 연결하는 Protocol
```

**핵심 흐름:**

```text
Requirement
→ Agent
→ Code
→ Compiler / Test / Tool
→ Result
→ Review
→ 수정
→ 검증
```

**명령어나 특정 AI Tool 사용법을 외우는 것이 목표가 아니다. `AI가 코드를 작성하게 하는 능력`보다 `AI가 만든 코드가 맞는지 직접 검증할 수 있는 능력`이 중요하다.**

현재 System SW / Embedded 로드맵에서는 Agentic Coding을 별도 전문 분야로 깊게 공부하지 않고, 이후 C·Linux·OS·System Programming 프로젝트에서 **코드 이해, 검증, Debugging을 보조하는 도구로 활용**하면 충분하다. 


# H. Beyond the Code

> 날짜: 2026-08-21
> 범위: Comments, README, Commit, Bug Report, PR, Code Review, 질문법, AI Etiquette

## 1. 핵심 원칙

좋은 Engineer는 단순히 동작하는 코드를 만드는 사람이 아니다.

```text
Code 작성
→ 다른 사람이 이해
→ 유지보수
→ 수정 / 확장
```

가 가능하도록 만들어야 한다.

가장 중요한 원칙:

```text
What → Code가 보여줌
Why  → 사람이 기록해야 함
```

---

## 2. 좋은 Comment

Comment는 코드 동작을 그대로 설명하기보다 **왜 이렇게 구현했는지**를 설명한다.

유용한 Comment:

```text
TODO + 미룬 이유
Reference / Specification
Correctness 근거
Debugging으로 얻은 교훈
Magic Number의 이유
Why-not 설명
```

특히 System/Embedded에서는:

```text
왜 volatile인가?
왜 이 Lock 순서인가?
왜 malloc을 사용하지 않는가?
왜 이 Timeout 값인가?
```

같은 이유를 남기는 것이 중요하다.

---

## 3. README

README는 다음 순서로 이해하기 쉽게 만든다.

```text
What
→ Why
→ Usage
→ Installation
→ Architecture / Detail
```

포트폴리오에서는 단순 기능 설명보다:

```text
Architecture
Design Decision
Debugging 경험
Trade-off
```

까지 보여주면 좋다.

---

## 4. Commit

Commit은 코드 변경의 History다.

복잡한 변경은:

```text
Problem
→ Solution
→ Implications
```

구조로 생각한다.

또한 하나의 Commit에는 가능하면 **하나의 논리적인 변경**만 넣는다.

```text
Refactoring
Feature
Bug Fix
```

를 불필요하게 한 Commit에 섞지 않는다.

---

## 5. Bug Report / Minimal Reproducer

좋은 Bug Report:

```text
Environment
Expected
Actual
Reproduction Steps
What I Tried
```

특히 중요한 것은 **Minimal Reproducible Example**이다.

```text
큰 Program
→ 관련 없는 부분 제거
→ 문제 유지 확인
→ 최소 코드로 축소
→ Root Cause 분석
```

System Debugging에서도 매우 중요한 방식이다.

---

## 6. Pull Request / Code Review

PR에서는 단순히 무엇을 바꿨는지보다:

```text
왜 필요한가?
왜 이 방법을 선택했는가?
어떻게 Test했는가?
어디를 집중해서 Review해야 하는가?
```

를 설명한다.

Code Review 원칙:

```text
사람이 아니라 Code를 Review
구체적인 수정 의견
Why 설명
질문 형태 활용
Blocking / Suggestion 구분
```

---

## 7. 좋은 질문 방법

좋은 질문은 먼저 자신의 현재 이해를 설명한다.

예:

```text
제가 이해하기로 Thread들은 Heap을 공유하고
Stack은 각각 따로 가지는 것으로 알고 있는데 맞나요?
```

추천 방식:

```text
현재 이해 설명
→ 헷갈리는 부분 명시
→ 구체적으로 질문
→ Follow-up으로 확인
```

사람뿐 아니라 LLM에게 질문할 때도 동일하다.

---

## 8. AI 사용

AI가 작성한 코드는 그대로 믿지 않는다.

```text
AI 도움
→ 직접 이해
→ Compile / Test
→ Review
→ 검증
```

특히 학습 중에는:

```text
직접 구현
→ 막힘
→ Hint / AI
→ 다시 직접 수정
```

순서가 좋다.

면접이나 회사에서는 AI 사용 정책을 먼저 확인한다.

---

## 9. System SW / Embedded 적용

앞으로 프로젝트에서는 다음 흐름을 남긴다.

```text
Problem
→ Reproduce
→ Debugging
→ Root Cause
→ Fix
→ Verification
→ Commit / README 기록
```

단순히:

```text
"프로젝트 완성"
```

보다:

```text
"문제를 어떻게 발견하고 해결했는가"
```

를 보여주는 것이 더 중요하다.

---

# 오늘 반드시 기억할 핵심

```text
Why              → 설계 이유를 기록
Comment          → 코드가 설명 못하는 이유 설명
README           → What / Why / Usage / Installation
Commit           → Problem / Solution / Implications
Minimal Reproducer → 문제 최소화
Bug Report       → Expected / Actual / Reproduction
Good Question    → 현재 이해부터 설명
AI               → 도움은 받되 직접 이해하고 검증
```

**핵심 흐름:**

```text
Problem
→ Reproduce
→ Root Cause
→ Fix
→ Verify
→ Why 기록
```


# I. Code Quality

> 날짜: 2026-08-21
> 범위: Formatting, Linting, Testing, Coverage, Pre-commit, CI/CD, Regex, Semgrep

## 1. 핵심 원칙

코드 품질은 사람이 매번 확인하는 것이 아니라 **도구로 자동화**한다.

```text
Code 작성
→ Format
→ Lint
→ Test
→ Commit
→ CI
```

핵심:

```text
사람 → Logic / Design에 집중
도구 → 반복적인 품질 검사
```

---

## 2. Formatter / Linter

**Formatter**는 코드 스타일을 자동으로 통일한다.

```text
clang-format
→ 들여쓰기
→ 공백
→ 줄 정리
```

**Linter / Static Analysis**는 잠재적인 코드 문제를 찾는다.

```text
clang-tidy
Compiler Warning
Semgrep
```

C/C++에서는 특히:

```bash
-Wall -Wextra -Wpedantic
```

같은 Compiler Warning도 적극 활용한다.

---

## 3. Testing

Test는 코드가 예상대로 동작하는지 검증한다.

```text
Unit Test        → 작은 단위
Integration Test → 여러 Component 연결
Regression Test  → 과거 Bug 재발 방지
```

System SW에서는 정상 경로뿐 아니라:

```text
Boundary
Error
Invalid Input
Resource Failure
```

같은 실패 경로도 중요하다.

---

## 4. Code Coverage

Coverage는 **Test가 실제로 실행한 코드 범위**를 보여준다.

```text
Test 실행
→ Coverage 측정
→ 실행되지 않은 Line 확인
→ 필요한 Test 추가
```

단:

```text
Coverage 100%
≠
좋은 Test
```

Coverage는 **테스트가 부족한 곳을 찾는 도구**이지 테스트 품질 자체가 아니다.

---

## 5. Pre-commit

Commit 전에 품질 검사를 자동 실행한다.

```text
git commit
↓
clang-format
↓
clang-tidy
↓
Pass
↓
Commit
```

목적:

```text
"검사해야지"라고 기억 ❌
자동으로 검사되게 구성 ✅
```

---

## 6. CI

CI는 Push / PR마다 별도 환경에서 자동 검증한다.

```text
git push
↓
GitHub Actions
↓
Format Check
↓
Lint
↓
Build
↓
Test
↓
PASS / FAIL
```

중요한 차이:

```text
Pre-commit → 개발자 PC에서 검사
CI         → 서버의 깨끗한 환경에서 검사
```

일부러 코드를 깨뜨려 **CI가 실제로 실패하는지도 확인**해야 한다.

---

## 7. Regex / Semgrep

Regex는 문자열 Pattern을 검색한다.

```text
^       → 시작
$       → 끝
.*      → 여러 문자
.*?     → Non-greedy
[]      → 문자 집합
()      → Capture Group
|       → OR
```

하지만 Regex는 **코드의 구조나 의미를 이해하지 않는다.**

```text
grep / Regex
→ Text Pattern

Semgrep
→ Code Pattern / Syntax
```

JSON처럼 구조화된 데이터는 Regex보다 **전용 Parser**를 사용하는 것이 좋다.

---

## 8. System SW / Embedded 적용

앞으로 C/C++ 프로젝트에서는:

```text
C/C++ Code
↓
clang-format
↓
Compiler Warning
↓
clang-tidy / Static Analysis
↓
Unit Test
↓
Coverage
↓
Pre-commit
↓
CI
```

흐름을 만드는 것이 중요하다.

특히:

```text
Compiler Warning
Static Analysis
Testing
Sanitizer
CI
```

를 실제 프로젝트에서 활용할 수 있어야 한다.

---

# 오늘 반드시 기억할 핵심

```text
Formatter       → 코드 Style 자동 통일
Linter          → 잠재적 문제 탐지
Compiler Warning → 컴파일 단계 문제 탐지
Test            → 동작 검증
Coverage        → Test가 놓친 코드 확인
Pre-commit      → Commit 전 자동 검사
CI              → Push/PR 후 자동 검증
Regex           → Text Pattern 검색
Semgrep         → Code Pattern 검색
```

**핵심 흐름:**

```text
Write
→ Format
→ Analyze
→ Test
→ Commit
→ CI
→ Verify
```

**명령어를 외우는 것보다 `사람이 반복해서 검사하지 말고 품질 검사를 자동화한다`는 사고방식을 이해하는 것이 핵심.**


좋아. 전체 정리를 보면 MIT Missing Semester에서 배운 걸 **9개 강의로 따로 기억하기보다 하나의 개발 흐름으로 묶는 것**이 가장 중요해 보인다. 네 정리에서도 Shell → 개발환경 → 디버깅 → Git → 패키징 → AI → 협업 → 품질 자동화가 반복적으로 하나의 흐름으로 연결된다. 

# MIT Missing Semester — 전체 핵심 정리

> **완강: 2026-08-21**

## 1. 이 강의에서 진짜 배운 것

Missing Semester의 핵심은 명령어 암기가 아니다.

```text
문제 발생
↓
도구로 상태 관찰
↓
문제 범위 축소
↓
원인 분석
↓
수정
↓
자동 검증
↓
변경사항 기록
↓
재현 가능한 형태로 배포
```

즉,

> **코드를 작성하는 것뿐 아니라 개발·디버깅·검증·협업·배포하는 방법**

을 배운 강의라고 보면 된다.

---

# 2. Shell — 컴퓨터를 조합 가능한 도구로 다루기

핵심 개념:

```text
stdin → Process → stdout / stderr
```

그리고 작은 프로그램들을 연결한다.

```bash
find ... | grep ... | awk ... | sort ... | uniq ...
```

핵심 도구:

```text
find   → 파일 탐색
grep   → 필터링
sed    → 텍스트 변환
awk    → 데이터 처리
jq     → JSON 처리
curl   → HTTP 요청
xargs  → 입력을 argument로 변환
```

### 가장 중요한 사고방식

```text
큰 작업
↓
작은 단계로 분해
↓
각 단계에 맞는 Tool 선택
↓
Pipe로 조합
```

복잡한 원라이너를 외울 필요는 없다.

---

# 3. Command-line Environment — Linux 환경을 실제로 운영하기

중요한 개념:

```text
Process
Signal
Job
Port / Socket
SSH
tmux
Dotfiles
Port Forwarding
```

특히 서버 문제를 볼 때:

```text
Process
↓
Port
↓
HTTP
↓
Application
↓
Log
```

이 흐름이 중요하다.

예:

```bash
pgrep -af uvicorn
ss -ltnp | grep :8000
curl http://localhost:8000/health
tail -f app.log
```

즉,

> **"서버가 안 돼요"가 아니라 어느 Layer에서 문제가 생겼는지 좁혀간다.**

---

# 4. Development Environment — 개발 도구를 효율적으로 사용하기

```text
Editor
+
Terminal
+
Language Server
+
Extensions
+
AI
```

### Vim

핵심은 명령 암기보다:

```text
Verb + Motion
```

```text
dw
d$
ci"
```

같은 조합 방식.

### LSP

```text
Editor
↕ LSP
Language Server
↓
Source Code / Dependency
```

Autocomplete뿐 아니라:

```text
Definition
References
Type Error
Import
Symbol
```

등을 코드의 **의미를 기반으로 분석**한다.

---

# 5. Debugging & Profiling — 추측하지 말고 관찰하기

이 강의 전체에서 특히 중요한 부분이다.

## Debugging

```text
Reproduce
↓
Observe
↓
Hypothesis
↓
Root Cause
↓
Fix
↓
Verify
```

도구를 문제에 맞게 선택한다.

```text
GDB       → 실행 상태 / Call Stack
ASan      → Memory Error
strace    → System Call
ss        → Network / Socket
lsof      → 열린 Resource
logs      → Application 상태
```

## Profiling

```text
느린 것 같다
→ 바로 최적화 ❌

측정
→ 병목 확인
→ 최적화
→ 다시 측정 ✅
```

```text
perf
Flame Graph
hyperfine
htop
```

핵심 원칙:

> **추측보다 측정.**

---

# 6. Git — 파일 저장이 아니라 History 관리

Git의 핵심 구조:

```text
Blob
Tree
Commit
```

그리고:

```text
Commit → Snapshot
Branch → Commit을 가리키는 Reference
HEAD → 현재 위치
```

작업 흐름:

```text
Working Directory
↓
git add
↓
Staging Area
↓
git commit
↓
History
```

실제 개발에서는:

```text
Branch
→ Code
→ Test
→ Commit
→ Push
→ PR
→ Review
→ CI
→ Merge
```

### 중요한 사고방식

Git 명령어를 외우는 것보다:

> **이 명령이 Working Directory / Staging Area / Commit / Reference 중 무엇을 변경하는가?**

를 이해하는 것이 중요하다.

---

# 7. Packaging & Docker — 내 컴퓨터 밖에서도 실행되게 만들기

소스 코드만 있다고 프로그램을 배포할 수 있는 것이 아니다.

```text
Source
↓
Dependencies
↓
Build
↓
Artifact
↓
Registry
↓
Deploy
↓
Run
```

핵심 개념:

```text
Virtual Environment → Dependency 격리
Lockfile            → Version 재현
Artifact            → 배포 결과물

Dockerfile          → Image 생성 방법
Image               → Package된 실행환경
Container           → 실행 중인 Image
Registry            → Image/Artifact 저장소
```

결국 핵심은:

> **"내 컴퓨터에서는 되는데요"를 줄이는 것.**

---

# 8. Agentic Coding — AI를 검증 가능한 개발 도구로 사용하기

AI에게 단순히 코드를 생성시키는 것보다 중요한 것은 **Feedback Loop**다.

```text
AI 코드 작성
↓
Compiler
↓
Linter
↓
Test
↓
Error
↓
AI 수정
↓
다시 검사
```

Agent에게 연결할 수 있는 것:

```text
Files
Shell
Compiler
Linter
Tests
Git
Web
```

하지만 최종 책임은 개발자에게 있다.

```text
AI Output
≠ Correct Code
```

따라서:

```text
AI
↓
Compile
↓
Test
↓
Static Analysis
↓
Diff Review
↓
직접 이해
```

### 핵심

> **AI가 코드를 얼마나 많이 작성하는지가 아니라 내가 그 코드가 맞는지 검증할 수 있는지가 중요하다.**

---

# 9. Beyond the Code — Why를 남기기

코드 자체는 보통 **What**을 보여준다.

사람이 남겨야 하는 것은:

```text
Why
```

좋은 Comment:

```text
왜 이 구현인가?
왜 다른 방법은 안 썼는가?
왜 이 값인가?
왜 이 순서인가?
```

좋은 Commit:

```text
Problem
→ Solution
→ Implications
```

좋은 Bug Report:

```text
Environment
Expected
Actual
Reproduction
What I Tried
```

좋은 PR:

```text
Why
Implementation
Testing
Review Point
```

특히 중요한 기술:

```text
큰 Bug
↓
관련 없는 요소 제거
↓
Minimal Reproducer
↓
Root Cause
```

---

# 10. Code Quality — 품질 검사를 자동화하기

개발자가 매번 직접 확인하지 않는다.

```text
Code
↓
Formatter
↓
Compiler Warning
↓
Linter / Static Analysis
↓
Test
↓
Coverage
↓
Commit
↓
CI
```

C/C++ 기준:

```text
clang-format
-Wall -Wextra -Wpedantic
clang-tidy
Sanitizer
Unit Test
Coverage
pre-commit
CI
```

역할은 다르다.

```text
Formatter → Style
Compiler  → Compile-time 문제
Linter    → 잠재적 문제
Sanitizer → Runtime 문제
Test      → Behavior
Coverage  → Test가 지나간 영역
CI        → 자동 검증
```

그리고:

```text
Coverage 100% ≠ 좋은 Test
```

---

# 11. 전체를 하나로 연결하면

Missing Semester 전체를 한 그림으로 압축하면 이렇다.

```text
                    ┌─ Shell / Linux
                    │
Requirement
    ↓
Code 작성 ──────────┼─ Editor / LSP / AI
    ↓               │
Format / Lint ──────┼─ clang-format / clang-tidy
    ↓               │
Build ──────────────┼─ Compiler
    ↓               │
Test ───────────────┼─ Unit Test / Coverage
    ↓               │
문제 발생 ──────────┼─ GDB / ASan / strace / perf
    ↓               │
Root Cause          │
    ↓               │
Fix                 │
    ↓               │
Verify              │
    ↓               │
Commit ─────────────┼─ Git
    ↓               │
PR / Review ────────┼─ GitHub
    ↓               │
CI ─────────────────┼─ 자동 검증
    ↓               │
Artifact ───────────┼─ Package / Docker
    ↓               │
Deploy              │
    ↓               │
Operate ────────────┼─ SSH / Process / Port / Log
    ↓
문제 발생 → 다시 Debugging
```

즉 **각 강의가 독립된 주제가 아니었다.**

---

# 12. 앞으로 반드시 몸에 남겨야 할 7가지

```text
1. 문제를 작은 단계로 분해한다.

2. 추측하기 전에 상태를 관찰한다.

3. 문제를 재현하고 범위를 줄인다.

4. Compiler / Test / Static Analysis를 적극 사용한다.

5. 반복 검사는 자동화한다.

6. Git에는 코드뿐 아니라 변경의 이유를 남긴다.

7. AI 결과도 반드시 직접 검증한다.
```

이 7개가 명령어 몇십 개를 외우는 것보다 중요하다.

---

# 13. 특히 중요한 통합 Debugging 사고방식

네 정리 전체에서 가장 반복적으로 등장하는 패턴을 하나만 남긴다면 이것이다. 

```text
Problem
↓
Reproduce
↓
Observe
↓
Narrow Down
↓
Root Cause
↓
Fix
↓
Verify
↓
Automate
↓
Document Why
```

예를 들어 서버 장애라면:

```text
curl
↓
ss
↓
Process
↓
Log
↓
strace / Debugger
↓
Root Cause
```

Memory 문제라면:

```text
Reproduce
↓
ASan
↓
GDB
↓
Call Stack / Memory 확인
↓
Root Cause
```

성능 문제라면:

```text
Reproduce
↓
Measure
↓
perf
↓
Flame Graph
↓
Bottleneck
↓
Optimize
↓
Benchmark
```

도구는 달라도 **사고 과정은 같다.**

---

# 최종 압축본

앞으로 세부 내용이 기억 안 나더라도 이것만 기억하면 된다.

```text
Shell
→ 작은 Tool을 조합한다.

Linux
→ Process / Port / Resource를 관찰한다.

Editor / LSP
→ 코드를 빠르게 탐색하고 이해한다.

Debugging
→ 재현하고 관찰해서 Root Cause를 찾는다.

Profiling
→ 추측하지 말고 측정한다.

Git
→ 변경사항과 History를 관리한다.

Docker / Packaging
→ 실행환경을 재현 가능하게 만든다.

AI
→ 작업을 도와주지만 결과는 검증한다.

Beyond the Code
→ What보다 Why를 기록한다.

Code Quality
→ Format / Lint / Test / CI를 자동화한다.
```

> **좋은 개발자는 코드를 작성하는 사람을 넘어, 문제를 관찰하고 원인을 찾고 검증하며 그 과정을 재현 가능하게 만드는 사람이다.**