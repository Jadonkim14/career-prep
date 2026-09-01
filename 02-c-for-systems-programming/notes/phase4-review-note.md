**## Ch.18 — The Outside Environment (2026.08.31)**

**### 핵심 개념**

* C 프로그램은 **Shell / OS와 상호작용**할 수 있다.

* 주요 내용: **Command Line Arguments / Exit Status / Environment Variables**

---

**### `argc` / `argv`**

```text
./program hello 123

argc = 3

argv[0] → "./program"
argv[1] → "hello"
argv[2] → "123"
argv[3] → NULL
```

* `argc` → 명령행 인자의 개수

* `argv` → 명령행 인자 문자열을 가리키는 포인터

* `argv[0]` → 일반적으로 프로그램 이름

* `argv[argc] == NULL`

---

**### `char *argv[]` / `char **argv`**

```c
int main(int argc, char *argv[])
int main(int argc, char **argv)
```

→ `main()`의 매개변수에서는 동일하게 동작한다.

```c
argv[i] == *(argv + i)
```

→ 배열 인덱싱과 포인터 연산은 같은 의미로 표현할 수 있다.

---

**### Exit Status**

프로그램이 종료할 때 **Shell 등 호출자에게 종료 상태를 전달**한다.

```c
return 0;              // 성공
return EXIT_FAILURE;   // 실패
```

```bash
echo $?
```

→ 직전에 실행한 프로그램의 Exit Status 확인

---

**### Environment Variable**

프로그램 실행 환경에서 전달되는 변수.

```text
HOME=/home/user
PATH=/usr/bin:...
```

```c
char *value = getenv("HOME");
```

* 존재 → 값의 포인터 반환

* 없음 → `NULL` 반환

---

**### `environ`**

```c
extern char **environ;
```

→ 환경 변수 전체에 접근할 수 있다.

```text
"HOME=/home/user"
"PATH=/usr/bin:..."
"SHELL=/bin/bash"
...
NULL
```

* `"KEY=VALUE"` 형태의 문자열 배열이다.

---

**### 헷갈렸던 부분**

* `argv[argc]`는 `NULL`이며, 마지막 인자를 의미하는 것이 아니다.

* `getenv()`가 실패했는지는 `*value == NULL`이 아니라 **`value == NULL`**로 확인한다.

* `value`는 포인터이므로 **문자열의 시작 주소**를 가지고 있다.

```c
char *value = "hello";
```

```text
value  → "hello"의 시작 주소
*value → 'h'
```

* `printf()`에서:

```c
printf("%p", (void *)value);  // 주소
printf("%c", *value);         // 문자
printf("%s", value);          // 문자열
```

→ **포인터라고 항상 주소가 출력되는 것은 아니다. `%s`는 포인터가 가리키는 문자열을 출력한다.**

* Exit Status는 **정상 출력/비정상 출력이 아니라 프로그램이 호출자에게 전달하는 종료 상태**이다.


## Ch.19 — C Preprocessor (2026.08.31)

### 핵심 개념

\* C 코드는 컴파일 전에 **Preprocessor(전처리기)** 단계를 거친다.

\* `#include`, `#define`, 조건부 컴파일 등이 전처리 단계에서 처리된다.

---

### `#include`

```c
#include <stdio.h>
#include "math.h"
````

* 헤더 파일의 **내용을 현재 파일에 포함**한다.

* `< >` → 시스템/표준 헤더

* `" "` → 프로젝트/사용자 헤더

* `.c` 파일의 실제 구현까지 가져오는 것은 아니다.

---

### `#define`

```c
#define MAX_SIZE 100
```

→ 전처리 단계에서 `MAX_SIZE`를 `100`으로 치환한다.

매크로에 인자를 사용할 수도 있다.

```c
#define SQR(x) ((x) * (x))
```

```c
SQR(3 + 4)
```

→ `((3 + 4) * (3 + 4))`

* 매크로 인자는 **괄호로 감싸는 습관**이 중요하다.

---

### 조건부 컴파일

```c
#ifdef DEBUG
    printf("Debug\n");
#endif
```

* `DEBUG`가 정의되어 있으면 해당 코드가 컴파일된다.

* `#ifndef` → 정의되어 있지 않을 때

* `#if` / `#else` / `#elif` → 조건에 따라 컴파일할 코드 결정

```c
#if 0
    printf("Hello");
#endif
```

→ 전처리 단계에서 제거되어 컴파일되지 않는다.

---

### Include Guard

```c
#ifndef MATH_H
#define MATH_H

int add(int, int);

#endif
```

→ 헤더 파일이 여러 번 포함되는 것을 방지한다.

---

### 디버깅 관련 매크로

```text
__FILE__ → 현재 파일 이름
__LINE__ → 현재 줄 번호
__func__ → 현재 함수 이름
```

→ 오류 메시지나 디버깅에 활용할 수 있다.

---

### 문자열화 / 토큰 연결

```c
#define STR(x) #x
```

→ 매크로 인자를 문자열로 변환한다.

```c
#define CAT(a, b) a ## b
```

→ 두 토큰을 연결한다.

---

### 헷갈렸던 / 틀렸던 부분

* Include Guard는 `MATH_H`가 **다른 곳에 선언되어 있는지** 확인하는 것이 아니라, **이미 정의되어 있는지** 확인한다.

* `#if 0`은 `if (0)`과 다르다.

```text
#if 0
→ 전처리 단계에서 코드 제거

if (0)
→ 컴파일은 하지만 실행하지 않음
```

* 매크로의 `#` → 문자열화

* 매크로의 `##` → 토큰 연결

* `#ifdef`에서 매크로는 **값보다 정의되어 있는지 여부**가 중요하다.

---

### 기억할 것

> **Preprocessor → 컴파일 전에 C 코드를 처리**

> **`#include` → 파일 내용 포함**

> **`#define` → 매크로 정의/치환**

> **`#ifdef / #ifndef / #if` → 조건부 컴파일**

> **Include Guard → 헤더 중복 포함 방지**

> **매크로 인자 → 괄호 주의**

> **`#` → 문자열화 / `##` → 토큰 연결**


## Ch.20 — Structs II (2026.08.31)

### 핵심 개념

\* **중첩 `struct` / 배열 초기화** → 구조체 안의 구조체·배열도 초기화 가능

\* **Self-Referential `struct`** → 자기 자신을 가리키는 포인터를 멤버로 사용

```c
struct Node {
    int data;
    struct Node *next;
};
````

→ 연결 리스트 등에 사용

* **Flexible Array Member** → 구조체 마지막에 `[]`를 두고 필요한 만큼 추가 공간을 할당

```c
struct Packet {
    int length;
    char data[];
};
```

* **Padding** → 메모리 정렬 등을 위해 컴파일러가 구조체에 추가하는 공간

* **`offsetof`** → 구조체 멤버의 메모리상 위치 확인

* **Bit-field** → 멤버가 사용할 비트 수를 지정

```c
unsigned int enable : 1;
unsigned int mode : 3;
```

→ Hardware Register 등에서 활용

* **`union`** → 여러 멤버가 같은 메모리 공간을 공유

```c
union Data {
    int a;
    float b;
};
```

→ 여러 형태 중 하나만 사용하거나 같은 메모리를 다른 방식으로 다룰 때 활용

---

### 헷갈렸던 / 틀렸던 부분

* **Flexible Array Member**

→ 단순히 "가변 배열"이 아니라 **구조체 마지막에 선언하고, 구조체 뒤의 추가 메모리를 함께 사용하는 배열 멤버**이다.

* **`malloc(sizeof *p + size)`**

→ 구조체 공간 + 가변 데이터 공간을 **한 번에 할당**하는 방식

* **Padding**

→ 구조체 멤버가 선언된 순서대로 바로 붙어 있다고 보장되지 않는다.

* **Bit-field**

→ 1/3/4 "칸"이 아니라 **1/3/4비트**를 사용하는 것이다.

* **`union`**

→ 멤버마다 별도의 공간을 가지는 것이 아니라 **같은 메모리 공간을 공유**한다.

* `union`에서 한 멤버에 값을 저장하면 같은 공간을 사용하는 다른 멤버의 값에 영향을 준다.

---

### 기억할 것

> **`struct` → 멤버들이 각각 공간을 사용**

> **`union` → 멤버들이 같은 공간을 공유**

> **Self-Referential `struct` → 자기 자신을 가리키는 포인터**

> **Flexible Array Member → 구조체 뒤의 가변 데이터**

> **Padding → 구조체 내부의 정렬용 추가 공간**

> **`offsetof` → 멤버의 위치**

> **Bit-field → 비트 단위로 멤버 사용**