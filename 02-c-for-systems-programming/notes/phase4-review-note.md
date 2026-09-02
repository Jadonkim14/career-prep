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


## Ch.21 — Characters and Strings II (2026.09.01)

### 핵심 개념

* **Escape Sequence(转义序列)**는 문자열이나 문자에서 특별한 문자를 표현하는 방법이다.

```c
\n   // 줄바꿈
\t   // 탭
\\   // 백슬래시
\"   // 큰따옴표
\'   // 작은따옴표
\r   // 현재 줄의 처음으로 이동
\b   // 커서를 한 칸 뒤로 이동
```

예:

```c
printf("\"Hello\"\n");
```

```text
"Hello"
```

* `\r`과 `\b`는 **Terminal(终端)**에서 출력 위치를 제어할 때 사용할 수 있다.

```text
\r → 현재 줄의 처음으로 이동
\b → 커서를 한 칸 뒤로 이동
```

이를 이용하면 진행률처럼 **같은 줄의 내용을 계속 갱신하는 출력**을 만들 수 있다.

* `fflush(stdout)`는 **stdout 버퍼(输出缓冲区)에 남아 있는 출력을 즉시 화면으로 내보내도록 요청**한다.

```c
printf("\rLoading...");
fflush(stdout);
```

* 문자나 byte를 **Hexadecimal(十六进制)** 또는 **Octal(八进制)** 값으로 표현할 수도 있다.

```c
printf("\x41\n");   // A
printf("\101\n");   // A
```

* Unicode Code Point(Unicode码点)는 `\u` 또는 `\U`로 표현할 수 있다.

```c
printf("\u2022\n");
```

### 기억할 것

> **Escape Sequence = 문자열에서 특수 문자를 표현하는 방법.**
>
> `\n`, `\t`, `\\`, `\"`, `\'`는 자주 사용하고, `\r`은 현재 줄의 처음으로, `\b`는 커서를 한 칸 뒤로 이동시킨다.
>
> Hex/Octal Escape와 Unicode Escape가 존재한다는 것도 알아둔다.

### 학습 기준

복잡한 Unicode/Multibyte 처리나 희귀 Escape Sequence를 암기할 필요는 없다.

현재는 **C 문자열에서 특수 문자와 byte 값을 표현할 수 있다는 것**을 이해하는 정도면 충분하다.


## Ch.22 — Enumerated Types: `enum` (2026.09.01)

### 핵심 개념

* `enum`(열거형 / 枚举)은 **이름을 가진 정수 상수(Integer Constant / 整数常量)를 묶어 표현하는 방법**이다.

```c
enum {
    RED,
    BLUE,
    GREEN
};
```

→ 기본적으로 `RED = 0`, `BLUE = 1`, `GREEN = 2`

* 값을 직접 지정할 수도 있다.

```c
enum {
    RED = 1,
    BLUE = 2,
    GREEN = 5,
    YELLOW
};
```

→ `YELLOW = 6`

* `enum`은 **하나의 타입(Type / 类型)**으로 사용할 수 있다.

```c
enum resource {
    SHEEP,
    WHEAT,
    WOOD
};

enum resource r = WOOD;
```

* `typedef`를 사용해 별칭(Alias / 别名)을 만들 수도 있다.

```c
typedef enum {
    RED,
    BLUE,
    GREEN
} COLOR;

COLOR c = RED;
```

* `enum`은 `#define`과 달리 **의미 있는 이름을 가진 정수 타입**으로 표현할 수 있어 코드의 가독성과 디버깅에 유리하다.

* `enum`의 Scope(作用域)는 선언된 위치에 따라 결정된다.

  * File Scope → 해당 파일에서 사용
  * Block Scope → 해당 블록에서만 사용
  * Header에 정의 → 여러 `.c` 파일에서 `#include`하여 사용 가능

### 기억할 것

> **`enum` = 의미 있는 이름을 가진 정수값을 묶어 표현하는 타입. 기본값은 0부터 1씩 증가하며, 직접 값을 지정할 수도 있다.**

```text
enum resource
      ↓
enum의 타입 이름(tag)

enum resource r;
```


## Ch.23 — Pointers III: Pointers to Pointers and More (2026.09.01)

### 핵심 개념

* **Pointer to Pointer(포인터의 포인터 / 指针的指针)**는 포인터의 주소를 저장하는 포인터다.

```c
int x = 10;
int *p = &x;
int **q = &p;
```

```text
q → p → x
         ↓
         10
```

* `&`는 포인터 단계를 하나 추가하고, `*`는 하나 제거한다.

```text
&x  → int*
&p  → int**
*q  → p
**q → x의 값
```

* **`int **`는 함수에서 원래 포인터 자체를 변경할 때 중요하다.**

```c
void change(int **p)
{
    *p = malloc(sizeof(int));
}
```

```text
int *  → 포인터가 가리키는 값 변경
int ** → 포인터 자체 변경
```

* **`const`는 위치에 따라 보호 대상이 달라진다.**

```c
const int *const *const q;
```

```text
q → p → x
🔒  🔒  🔒
```

```c
const int *const *q;
```

```text
q → p → x
🔓  🔒  🔒
```

```c
int **const q;
```

```text
q → p → x
🔒  🔓  🔓
```

---

### `unsigned char *`와 메모리

* `unsigned char *`를 사용하면 메모리를 **1 byte 단위로 접근**할 수 있다.
* `memcpy()`처럼 메모리 자체를 복사하는 작업에 사용된다.

```c
while (n-- > 0)
    *d++ = *s++;
```

→ 현재 byte를 복사하고 `d`, `s`를 다음 byte로 이동한다.

```text
*s++ → 현재 byte 읽기 → s++
*d++ → 현재 위치에 저장 → d++
n--  → 남은 byte 수 감소
```

* 객체의 실제 메모리 배치인 **Object Representation(객체 표현 / 对象表示)**을 확인할 때도 사용할 수 있다.
* Struct의 **Padding(填充)**과 **Endianness(字节序)**를 확인하는 데 활용할 수 있다.

---

### `NULL`과 `'\0'`

* `NULL` → **아무것도 가리키지 않는 포인터**
* `'\0'` → **문자열의 끝을 나타내는 문자**

```text
p == NULL   → 포인터 자체 확인
*p == '\0'  → 가리키는 문자가 문자열 끝인지 확인
```

---

### Pointer와 Integer

* 포인터를 정수로 변환할 수 있으며, `uintptr_t`를 사용할 수 있다.
* 일반적인 프로그램에서는 거의 사용하지 않고 **Low-Level / Hardware**에서 활용될 수 있다.

---

### Strict Aliasing

* 호환되지 않는 타입의 포인터를 통해 같은 객체에 접근하면 **Undefined Behavior(未定义行为)**가 발생할 수 있다.

```c
int x = 10;
float *p = (float *)&x;  // 위험
```

→ `int` 객체를 `float *`로 접근하는 것은 피해야 한다.

---

### Pointer Difference

* 같은 배열 내부의 두 포인터를 빼면 원소 사이의 거리를 얻는다.

```c
int *p = arr + 20;
int *q = arr + 60;

ptrdiff_t d = q - p;  // 40
```

* 결과를 저장할 때 `ptrdiff_t`를 사용할 수 있다.

---

### Function Pointer

**Function Pointer(함수 포인터 / 函数指针)**는 특정 형태의 함수를 가리키는 포인터다.

```c
int (*op)(int, int);
```

→ `int` 두 개를 받아 `int`를 반환하는 함수를 가리킨다.

```c
int add(int a, int b)
{
    return a + b;
}

int (*op)(int, int) = add;

int result = op(3, 5);  // 8
```

* 함수 포인터를 다른 함수의 인자로 전달하여 **실행할 함수를 선택**할 수 있다.

```text
print_math(add, 5, 7)
        ↓
    op(5, 7)
        ↓
    add(5, 7)
```

### 기억할 것

> **`int **` → 포인터 자체를 변경할 때 사용**
>
> **`unsigned char *` → 메모리를 byte 단위로 접근**
>
> **`NULL` → 포인터 / `'\0'` → 문자열 종료**
>
> **Strict Aliasing → 호환되지 않는 타입으로 같은 객체를 접근하지 않기**
>
> **Function Pointer → 함수를 가리키고 다른 함수에 전달 가능**


## Ch.24 — Bitwise Operations (2026.09.01)

### 핵심 개념

* **Bitwise Operation(비트 연산 / 位运算)**은 정수의 개별 bit를 직접 조작한다.

```text
&  → AND
|  → OR
^  → XOR
~  → NOT
```

* `&`, `|`, `^`에는 각각 축약형이 있다.

```c
a &= c;
a |= c;
a ^= c;
```

→ `a = a & c` 등의 형태와 같다.

---

### Bitwise Shift

```text
<< → 왼쪽 시프트
>> → 오른쪽 시프트
```

* bit를 이동시키며, 새로 들어오는 bit는 `0`으로 채워진다.
* `<<=`, `>>=` 축약형도 사용할 수 있다.

```c
a <<= c;
a >>= c;
```

---

### 주의사항

* Shift 횟수가 음수이거나 피연산자의 bit 수 이상이면 **Undefined Behavior(未定义行为)**.

```c
a >> -1;   // ❌
a >> 32;   // ❌ 32-bit 기준
```

* 음수 값을 `>>`하는 것은 **Implementation-defined(实现定义)**.

```c
int x = -8;
x >> 1;     // Implementation-defined
```

```text
-8 >> 1  → 음수 값을 shift
8 >> -1  → 음수 shift count
             ↑
          Undefined Behavior
```

**### 기억할 것**

> **`&` / `|` / `^` / `~` → Bitwise Operation**
>
> **`<<` / `>>` → Bitwise Shift**
>
> **음수·범위 초과 shift count → Undefined Behavior**
>
> **음수 값의 `>>` → Implementation-defined**


## Ch.25 — Variadic Functions (2026.09.01)

### 핵심 개념

* **Variadic Function(가변 인자 함수 / 可变参数函数)**는 **인자의 개수가 정해지지 않은 함수**다.

```c
void func(int a, ...)
```

* `...` 앞에는 최소 하나의 **Named Parameter(명명된 매개변수)**가 필요하다.
* 대표적인 예가 `printf()`다.

---

### `stdarg.h`

가변 인자를 다룰 때 `<stdarg.h>`를 사용한다.

```c
va_list va;

va_start(va, count);

int n = va_arg(va, int);

va_end(va);
```

```text
va_list
→ 가변 인자의 현재 위치/상태 관리

va_start()
→ 가변 인자 처리 시작
→ 마지막 Named Parameter를 기준으로 초기화

va_arg()
→ 다음 인자를 가져옴
→ 가져올 타입을 지정

va_end()
→ 가변 인자 처리 종료
```

기본 순서:

```text
va_start()
    ↓
va_arg()
    ↓
va_arg()
    ↓
va_end()
```

---

### `va_copy()`

* `va_list`의 **현재 상태를 그대로 복사**한다.
* 현재 위치를 유지하면서 별도로 가변 인자를 탐색할 때 사용한다.

---

### `printf()`와 `vprintf()`

* `printf()`는 대표적인 Variadic Function이다.
* `vprintf()`는 **`va_list`를 받아 `printf()`와 같은 방식으로 출력**한다.
* `vfprintf()`, `vsprintf()`, `vsnprintf()`도 같은 계열이다.

```text
printf()
→ 가변 인자를 직접 받음

vprintf()
→ va_list를 받아 처리
```

따라서 Custom `printf()`를 만들 때 활용할 수 있다.

---

### 인자와 매개변수

```c
int add(int x, int y)
{
    return x + y;
}

add(10, 20);
```

```text
x, y     → Parameter(매개변수 / 参数)
10, 20   → Argument(인자 / 实参)
```

* **Argument** → 함수를 호출할 때 전달하는 실제 값
* **Parameter** → 함수 정의에서 값을 받아 사용하는 변수

---

**### 기억할 것**

> **Variadic Function → 인자 개수가 가변적인 함수**

> **`...` → 가변 인자**

> **`va_list` → 가변 인자의 현재 상태 관리**

> **`va_start()` → 시작 / `va_arg()` → 다음 인자 / `va_end()` → 종료**

> **`va_copy()` → `va_list`의 현재 상태 복사**

> **Argument → 호출할 때 전달하는 값 / Parameter → 함수가 받는 변수**


## Ch.26 — Locale and Internationalization (2026.09.01)

### 핵심 개념

* **Localization(지역화 / 本地化)** → 국가·지역에 맞게 프로그램의 숫자, 통화, 문자 등의 형식을 조정한다.

### `setlocale()`

* `<locale.h>` 사용.
* 현재 시스템의 Locale 사용:

```c
setlocale(LC_ALL, "");
```

* 기본 C Locale:

```c
setlocale(LC_ALL, "C");
```

* 특정 Locale 이름은 시스템에 따라 달라질 수 있어 비이식적일 수 있다.

### `localeconv()`

* 통화 및 숫자 관련 Locale 정보를 가져온다.

```c
struct lconv *x = localeconv();
```

주요 정보:

```text
mon_decimal_point → 통화 소수점
mon_thousands_sep → 천 단위 구분자
currency_symbol   → 통화 기호
frac_digits       → 소수점 이하 자릿수
```

### Locale Category

```text
LC_ALL       → 전체
LC_COLLATE   → 문자열 비교/정렬
LC_CTYPE     → 문자 처리
LC_MONETARY  → 통화
LC_NUMERIC   → 숫자
LC_TIME      → 날짜/시간
```

**### 기억할 것**

> **`setlocale()` → Locale 설정**
>
> **`localeconv()` → Locale 정보 확인**
>
> **`LC_*` → 어떤 Locale 영역을 설정할지 지정**

Systems / Embedded / Driver 개발의 핵심 내용은 아니므로 **개념만 이해하고 넘어가면 충분하다.** 


## Ch.27 — Unicode, Wide Characters, and All That (2026.09.01)

### 핵심 개념

* **Unicode(유니코드 / Unicode)** → 다양한 문자를 하나의 문자 집합으로 표현한다.
* **Code Point(코드 포인트 / 代码点)** → 문자를 나타내는 숫자 값.
* **Encoding(인코딩 / 编码)** → Code Point를 메모리에 표현하는 방법.

```text
Code Point → 어떤 문자인가
Encoding   → 메모리에 어떻게 저장하는가
```

### Unicode Encoding

```text
UTF-8  → 가변 길이 byte 기반
UTF-16 → 16-bit 기반
UTF-32 → 32-bit 기반
```

* UTF-8은 **ASCII와 하위 128개 코드 포인트에서 호환**된다.
* UTF-16/UTF-32는 Endianness(字节序)에 따라 byte 순서가 달라질 수 있다.

### C에서 Unicode

```c
"\u20AC"       // 16-bit Unicode code point
"\U0001D4D1"   // 32-bit Unicode code point
```

* `\u` → 정확히 4개의 hexadecimal digit
* `\U` → 정확히 8개의 hexadecimal digit
* 이것들은 특정 Encoding 자체가 아니라 **Unicode Code Point**를 나타낸다.

### UTF-8과 C 문자열

* UTF-8 문자열은 일반적인 C 문자열처럼 `NUL`로 끝낼 수 있다.
* `strlen()`은 **문자 수가 아니라 byte 수**를 반환한다.

```text
"€"
→ UTF-8에서는 여러 byte
→ strlen()은 byte 수를 계산
```

### Wide Character

```text
char
→ Multibyte Character

wchar_t
→ Wide Character
```

* `wchar_t`는 현재 Locale에서 문자를 표현하기 위한 타입이다.
* Wide String은 `L` 접두사를 사용한다.

```c
wchar_t *s = L"Hello";
```

### 문자 변환

```text
mbtowc()   → Multibyte Character → Wide Character
wctomb()   → Wide Character → Multibyte Character

mbstowcs() → Multibyte String → Wide String
wcstombs() → Wide String → Multibyte String
```

### 기억할 것

> **Unicode → 문자 집합**
>
> **Code Point → 문자를 나타내는 숫자**
>
> **Encoding → Code Point를 메모리에 표현하는 방법**
>
> **UTF-8 → 가변 길이 byte Encoding + ASCII 호환**
>
> **`strlen()` → UTF-8에서 문자 수가 아닌 byte 수**
>
> **`wchar_t` → Wide Character 타입**

Systems / Embedded / Driver의 핵심 내용은 아니므로 **Unicode / UTF-8 / `strlen()`의 byte 단위 정도만 이해하고 넘어가면 충분하다.** 


## Ch.28 — Exiting a Program (2026.09.01)

**### 정상 종료**

* **`main()` 끝까지 도달** → `return 0`과 동일 → **성공**
* **`exit(status)`** → 어디서든 종료하며 Exit Status(종료 상태)를 전달한다.
* 정상 종료 시 **I/O stream flush + temporary file 제거**가 수행된다.

```text
0       → 성공
non-zero → 실패/오류를 나타내는 것이 일반적
```

### 종료 Handler

* **`atexit()`** → 프로그램 종료 시 실행할 함수 등록
* 여러 개 등록 가능하며 **등록 역순으로 실행**된다.

```text
등록: 1 → 2 → 3
실행: 3 → 2 → 1
```

### 빠른 종료

```text
quick_exit()
→ 일부 cleanup 생략
→ atexit() 실행 ❌
→ at_quick_exit() 사용

_Exit()
→ 즉시 종료
→ Handler ❌
→ flush / temporary file cleanup ❌
```

### 오류 종료

* **`assert()`** → 조건이 거짓이면 프로그램 종료. 개발 단계에서 **절대 발생하면 안 되는 상태**를 검사할 때 사용한다.
* **`abort()`** → 심각한 오류 발생 시 비정상 종료. `SIGABRT` 발생.

**### 기억할 것**

> **`exit()` → 정상 종료**
>
> **`atexit()` → 종료 Handler 등록**
>
> **`quick_exit()` → cleanup을 일부 생략한 종료**
>
> **`_Exit()` → 즉시 종료**
>
> **`assert()` → 조건 검증**
>
> **`abort()` → 비정상 종료 + `SIGABRT`**


## Ch.29 — Signal Handling (2026.09.01)

**### 핵심 개념**

* **Signal(시그널 / 信号)** → 프로그램에 갑자기 전달되는 외부 이벤트.
* **Signal Handler(시그널 핸들러 / 信号处理器)** → Signal 발생 시 실행되는 함수.

```text
외부 이벤트
    ↓
 Signal
    ↓
 Handler
```

### 주요 Signal

```text
SIGINT  → Ctrl+C 인터럽트
SIGTERM → 종료 요청
SIGSEGV → 잘못된 메모리 접근
SIGABRT → abort()에 의한 비정상 종료
```

### `signal()`

```c
signal(SIGINT, handler);
```

→ `SIGINT`가 발생하면 `handler()`를 실행한다.

```text
SIG_IGN → Signal 무시
SIG_DFL → 기본 동작
Handler → 지정한 함수 실행
```

### Signal Handler 주의점

* Signal은 **실행 중 갑자기 끼어들 수 있다.**
* 따라서 Handler에서 `printf()` 같은 일반적인 표준 라이브러리 함수를 사용하는 것은 안전하다고 보장되지 않는다.
* Handler에서는 가능한 한 **간단한 상태만 기록**하고, 실제 처리는 일반 코드에서 수행하는 방식이 좋다.

### `sig_atomic_t`

* Signal Handler와 일반 코드 사이에서 상태를 전달할 때 사용할 수 있는 정수 타입.
* `volatile sig_atomic_t` 형태로 사용할 수 있다.

```c
volatile sig_atomic_t flag = 0;

void handler(int sig)
{
    flag = 1;
}
```

### `sigaction()`

* C의 `signal()`은 제한과 이식성 문제가 있다.
* Unix/Linux에서는 **`sigaction()` 계열을 사용하는 것이 더 적절하다.**

**### 기억할 것**

> **Signal → 갑자기 전달되는 외부 이벤트**
>
> **`SIGINT` → Ctrl+C**
>
> **`SIGTERM` → 종료 요청**
>
> **`SIGSEGV` → 잘못된 메모리 접근**
>
> **Handler → Signal 발생 시 실행**
>
> **`SIG_IGN` → 무시 / `SIG_DFL` → 기본 동작**
>
> **Linux → `signal()`보다 `sigaction()`이 중요**


## Ch.30 — Variable-Length Arrays (VLA) (2026.09.01)

**### 핵심 개념**

* **VLA(가변 길이 배열 / 变长数组)** → 실행 시간에 크기가 결정되는 배열.

```c
int n = 10;
int v[n];
```

* 배열 크기는 선언 시점에 결정되며 **이후 변경할 수 없다.**
* `malloc()`과 달리 직접 `free()`할 필요가 없다.
* VLA는 **Stack에 할당**되므로 큰 크기를 사용하면 Stack Overflow 위험이 있다.

### `sizeof`

* 일반 배열 → `sizeof`가 컴파일 시간에 계산된다.
* VLA → `sizeof`가 **실행 시간에 계산**된다.

```c
size_t n = sizeof v / sizeof v[0];
```

→ VLA의 원소 개수를 구할 수 있다.

### 다차원 VLA

```c
int h = 10;
int w = 20;

int matrix[h][w];
```

* 여러 차원을 VLA로 만들 수 있다.
* VLA를 함수에 전달할 때 크기를 함께 전달할 수 있다.

```c
void print_matrix(int h, int w, int m[h][w]);
```

### 일반 배열과 호환

* 차원이 맞으면 **일반 배열 ↔ VLA**를 함수에 전달할 수 있다.
* 차원이 맞지 않으면 Undefined Behavior(정의되지 않은 동작)가 발생할 수 있다.

### Linux Kernel과 VLA

Linux Kernel에서는 VLA를 사용하지 않는다.

주요 이유:

* Stack 크기가 제한되어 있음 → **Stack Overflow 위험**
* VLA 처리에 추가적인 성능 비용
* 컴파일러 지원이 동일하지 않음
* 많은 경우 고정 크기 배열로 충분함

**### 기억할 것**

> **VLA → 실행 시간에 크기 결정**
>
> **VLA → Stack에 할당**
>
> **크기는 이후 변경 불가**
>
> **`sizeof` → VLA에서는 실행 시간에 계산**
>
> **큰 VLA → Stack Overflow 위험**
>
> **Linux Kernel → VLA 사용 금지**


## Ch.31 — `goto`

### 핵심 개념

* **`goto`** → 같은 함수의 **Label(레이블)**로 실행 흐름을 이동한다.
* Label은 **Function Scope(함수 스코프)**를 가진다.
* 중첩된 반복문이나 여러 단계의 구조를 한 번에 탈출할 수 있다.

### Error Handling / Cleanup

* 여러 Resource(자원)를 확보하다 실패하면 `goto`로 **이미 확보한 자원만 역순으로 해제**할 수 있다.

```text
Resource 1 → Resource 2 → 실패
                  ↓
             Resource 1 해제
```

* 실패한 위치에 따라 적절한 Cleanup Label을 사용해야 한다.

### 주의점

* `goto`로 **변수 초기화 코드를 건너뛰면** 문제가 발생할 수 있다.
* **VLA 선언을 건너뛰어 Scope 안으로 이동하는 것은 불가능**하다.
* Unix/Linux 코드에서는 **Error Handling, Retry, Cleanup** 등에 `goto`가 사용될 수 있다.

### 기억할 것

> **`goto` → 같은 함수의 Label로 이동**
>
> **중첩 구조 → 여러 단계 탈출**
>
> **Cleanup → 확보한 자원을 역순으로 해제**
>
> **초기화/VLA 선언을 건너뛰는 `goto` → 주의**


## Ch.32 — Compound Literals & Generic Selections

### 핵심 개념

* **Compound Literal(복합 리터럴 / 复合字面量)** → 이름 없는 객체를 즉석에서 생성한다.

```c
(int []){1, 2, 3, 4}

(struct coord){.x=10, .y=20}
```

* 임시 변수를 만들지 않고 함수에 배열이나 `struct`를 직접 전달할 수 있다.

```c
sum((int []){1, 2, 3, 4}, 4);

print_coord((struct coord){.x=10, .y=20});
```

### Scope / Lifetime ⭐

* Compound Literal도 일반 지역 변수처럼 **Scope와 Lifetime의 영향을 받는다.**
* Scope를 벗어난 객체를 가리키는 포인터를 사용하면 **Undefined Behavior**가 발생한다.

```c
int *p;

{
    p = &(int){10};
}

printf("%d\n", *p);  // ❌
```

→ 객체의 Lifetime이 끝났기 때문에 `p`는 더 이상 유효하지 않다.

### `_Generic`

* **`_Generic`** → 표현식의 **타입에 따라 다른 코드를 선택**한다.
* 선택은 **Runtime이 아니라 Compile Time**에 이루어진다.

```c
_Generic(x,
    int: "int",
    float: "float",
    default: "other"
);
```

* 주로 Macro와 함께 사용해 **타입에 따라 다른 동작을 구현**할 수 있다.

### 기억할 것

> **Compound Literal → 이름 없는 객체를 즉석에서 생성**
>
> **Compound Literal → 일반 객체처럼 Scope / Lifetime을 가짐**
>
> **Scope를 벗어난 Compound Literal을 가리키는 포인터 → ❌**
>
> **`_Generic` → 타입에 따라 Compile Time에 다른 표현식 선택**


## Ch.33 — Arrays Part II

### 핵심 개념

* 배열 Parameter는 실제로 **Pointer로 처리**된다.

```c
int func(int *p);
int func(int p[]);
```

→ 동일한 의미

### Array Parameter + Type Qualifier

* Array Parameter에서도 `const`, `volatile` 등의 **Type Qualifier(타입 한정자 / 类型限定符)**를 사용할 수 있다.
* `[]` 안에 작성한다.

```c
int func(int p[const volatile]);
```

→ Pointer에 `const volatile`이 적용된 형태와 대응된다.

### `static` Array Parameter

```c
int func(int p[static 4]);
```

* 함수에 전달되는 배열이 **최소 4개 원소 이상**이라는 것을 프로그래머가 보장한다.
* 조건을 지키지 않으면 **Undefined Behavior**가 발생할 수 있다.
* 컴파일러가 반드시 잘못된 전달을 잡아주는 것은 아니다.
* 이 기능은 네 로드맵에서는 중요도가 낮으므로 **개념만 이해**하면 충분하다.

### 다차원 배열 초기화

* 초기화 값이 부족하면 나머지 원소는 **0으로 초기화**된다.

```c
int a[3][2] = {
    {1, 2},
    {3},
    {5, 6}
};
```

```text
1 2
3 0
5 6
```

* 2차원 배열도 1차원 initializer로 순서대로 채울 수 있다.

```c
int a[3][2] = {1, 2, 3, 4, 5, 6};
```

```text
1 2
3 4
5 6
```

* `{0}`을 사용하면 전체 배열을 0으로 초기화할 수 있다.

```c
int a[3][2] = {0};
```

### 기억할 것

> **배열 Parameter → Pointer로 처리**
>
> **`const` / `volatile` → Array Parameter의 `[]` 안에서도 사용 가능**
>
> **`static N` → 전달되는 배열이 최소 N개 원소라는 약속**
>
> **부족한 초기화 값 → 나머지는 0**
>
> **`{0}` → 전체 배열을 0으로 초기화**


## Ch.34 — `setjmp` / `longjmp`

### 핵심 개념

* **`setjmp()`** → 현재 실행 상태를 저장한다.
* **`longjmp()`** → 저장된 `setjmp()` 지점으로 돌아간다.
* `goto`와 달리 **함수 호출 스택을 거슬러 올라갈 수 있다.**

```c
setjmp(env);

longjmp(env, 3490);
```

### `setjmp()`의 반환값 ⭐

* 처음 `setjmp()`를 실행하면 **`0` 반환**
* `longjmp()`로 돌아오면 **`longjmp()`의 두 번째 인자 반환**

```text
setjmp()
   ↓
0
   ↓
함수 호출
   ↓
longjmp(env, 3490)
   ↓
setjmp() 위치로 복귀
   ↓
3490
```

→ 이를 이용해 **처음 실행인지, `longjmp()`로 돌아온 것인지 구분**할 수 있다.

### `volatile`

* `setjmp()` 이후 값이 변경되는 **자동 지역 변수**는 `longjmp()` 후 올바른 값을 유지하려면 `volatile`이 필요할 수 있다.

```c
volatile int x = 20;
```

### 주의점

* `longjmp()`는 실행 상태를 강제로 되돌리므로 **Undefined Behavior(정의되지 않은 동작 / 未定义行为)**가 발생할 수 있는 조건이 많다.
* `setjmp()`를 호출한 함수가 이미 종료된 경우 사용할 수 없다.
* VLA의 Scope를 벗어난 뒤 `longjmp()`하는 것도 문제가 된다.
* `longjmp(env, 0)`은 `setjmp()`가 `1`을 반환하도록 변경된다.

### 기억할 것

> **`setjmp()` → 실행 상태 저장**
>
> **`longjmp()` → 이전 `setjmp()` 지점으로 복귀**
>
> **처음 `setjmp()` → `0`**
>
> **`longjmp(env, n)` 후 → `n` 반환**
>
> **`goto`와 달리 함수 호출 스택을 거슬러 올라갈 수 있음**
>
> **강력하지만 제약과 Undefined Behavior 위험이 많음**


## Ch.35 — Incomplete Types

### 핵심 개념

* **Incomplete Type(불완전 타입 / 不完整类型)** → 크기를 아직 알 수 없는 타입.
* Incomplete Type은 **포인터로 선언할 수 있지만**, 역참조하거나 `sizeof`할 수 없다.

```c
struct foo;

struct foo *p;   // ⭕
struct foo x;    // ❌
sizeof(struct foo); // ❌
*p;              // ❌
```

### 자기참조 구조체 ⭐

* 구조체가 아직 완성되지 않았어도 **자기 자신에 대한 포인터**는 선언할 수 있다.

```c
struct node {
    int value;
    struct node *next;
};
```

→ **Linked List(연결 리스트 / 链表)**에 사용.

### `extern` 배열

* 크기를 지정하지 않은 `extern` 배열은 Incomplete Type이다.

```c
// bar.h
extern int my_array[];

// bar.c
int my_array[1024];
```

→ `.h`에서는 선언하고, `.c`에서 실제 크기와 저장 공간을 정의한다.

### Complete Type

```c
struct foo;

struct foo {
    int x;
    int y;
};
```

→ `};`까지 도달하면 `struct foo`가 **Complete Type(완전 타입 / 完整类型)**이 된다.

### `void`

* `void`도 Incomplete Type이다.
* 따라서 `void *`는 가능하지만 `void` 자체의 객체는 만들 수 없다.

```c
void *p;  // ⭕
void v;   // ❌
```

### 기억할 것

> **Incomplete Type → 크기를 알 수 없음**
>
> **Incomplete Type → 포인터는 가능**
>
> **역참조 / `sizeof` → ❌**
>
> **자기참조 구조체 → `struct node *next` 가능**
>
> **`extern int array[]` → Incomplete Array Type**


## Ch.36 — Complex Numbers

### 핵심 개념

* **Complex Number(복소수 / 复数)** → 실수부와 허수부로 이루어진 수.
* C에서는 `<complex.h>`를 사용한다.
* 복소수 지원은 C 표준에서 **optional feature**이므로 모든 컴파일러가 지원한다고 보장되지는 않는다.

### Complex Type

```c
#include <complex.h>

double complex x = 5 + 2*I;
```

* `complex` → 복소수 타입
* `I` → 허수 단위
* `float complex`, `double complex`, `long double complex` 사용 가능.

### 복소수 생성 / 분리

```c
double complex x = CMPLX(5, 2);

creal(x);  // 실수부
cimag(x);  // 허수부
```

* `CMPLX()` → 복소수 생성
* `creal()` → Real Part(실수부 / 实部)
* `cimag()` → Imaginary Part(허수부 / 虚部)

### 복소수 연산

일반적인 산술 연산을 사용할 수 있다.

```c
x + y
x - y
x * y
x / y
```

`==`, `!=`를 이용한 비교도 가능하다.

### Complex Math

주요 함수:

```text
ccos / csin / ctan  → 삼각함수
cexp / clog         → 지수 / 로그
cabs                → 절댓값
cpow                → 거듭제곱
csqrt               → 제곱근
carg                → 위상각
conj                → 켤레복소수
```

`float`, `long double` 버전은 함수명 뒤에 `f`, `l`을 붙인다.

### 기억할 것

> **`<complex.h>` → C 복소수 지원**
> **`complex` → 복소수 타입**
> **`I` → 허수 단위**
> **`CMPLX()` → 복소수 생성**
> **`creal()` / `cimag()` → 실수부 / 허수부**
> **`cabs()` / `cpow()` / `csqrt()` → 복소수 수학 연산** 


## Ch.37 — Fixed Width Integer Types

### 핵심 개념

* **Fixed Width Integer Type(고정 폭 정수 타입 / 固定宽度整数类型)** → 정수의 비트 수를 명확하게 지정하는 타입.
* `<stdint.h>`에서 제공한다.

```c
int32_t x;   // 정확히 32비트 signed
uint16_t y;  // 정확히 16비트 unsigned
```

### 주요 타입 ⭐

```text
intN_t       → 정확히 N비트
int_leastN_t → 최소 N비트
int_fastN_t  → 최소 N비트 중 가장 빠른 타입
```

unsigned는 앞에 `u`가 붙는다.

```text
int32_t
uint32_t
int_least16_t
uint_fast64_t
```

일반적으로 사용할 수 있는 고정 폭 타입:

```text
int8_t    uint8_t
int16_t   uint16_t
int32_t   uint32_t
int64_t   uint64_t
```

### `intmax_t`

* 시스템에서 표현할 수 있는 **가장 큰 signed / unsigned 정수 타입**이다.

```c
intmax_t
uintmax_t
```

### 고정 폭 상수

* 특정 타입에 맞는 정수 상수를 만들 때 사용한다.

```c
UINT16_C(12)
INTMAX_C(3490)
```

### Limits

* 각 타입의 최댓값과 최솟값을 제공한다.

```text
INT32_MAX
INT32_MIN
UINT32_MAX

INT_LEAST64_MAX
INT_FAST32_MAX

INTMAX_MAX
```

* unsigned의 최솟값은 항상 `0`이다.

### Format Specifier ⭐

고정 폭 정수는 `<inttypes.h>`의 매크로를 사용해 `printf()` / `scanf()`의 형식 지정자를 결정한다.

```c
#include <inttypes.h>

uint32_t x = 123;

printf("%" PRIu32 "\n", x);
```

주요 패턴:

```text
PRId32 → signed decimal
PRIu32 → unsigned decimal
PRIx32 → hexadecimal lowercase
PRIX32 → hexadecimal uppercase
```

### 기억할 것

> **`<stdint.h>` → 고정 폭 정수 타입**
> **`int32_t` → 정확히 32비트 signed**
> **`uint32_t` → 정확히 32비트 unsigned**
> **`least` → 최소 N비트**
> **`fast` → 최소 N비트 중 가장 빠른 타입**
> **`intmax_t` → 가장 큰 signed 정수 타입**
> **`<inttypes.h>` → `printf()` / `scanf()`용 Format Macro**


## Ch.38 — Date and Time Functionality

### 핵심 개념

* **UTC(협정 세계시 / 协调世界时)** → 전 세계의 공통 기준 시간.
* **Local Time(현지 시간 / 当地时间)** → 컴퓨터가 위치한 지역의 시간.
* 한 번 발생한 사건이나 로그 → **UTC** 사용을 권장.
* 날짜/시간 기능은 직접 구현하지 않고 **라이브러리 기능을 사용한다.**

### `time_t` / `struct tm` ⭐

```text
time_t
→ Calendar Time을 표현

struct tm
→ 년·월·일·시·분·초 등으로 분해된 시간
```

`struct tm`의 주요 필드:

```text
tm_sec    → 초 [0, 60]
tm_min    → 분 [0, 59]
tm_hour   → 시 [0, 23]
tm_mday   → 일 [1, 31]
tm_mon    → 월 [0, 11]
tm_year   → 1900년 이후의 연도
tm_wday   → 요일 [0, 6]
tm_yday   → 1월 1일 이후의 일 [0, 365]
tm_isdst  → 서머타임 적용 여부
```

### 시간 변환 ⭐

현재 시간:

```c
time_t now = time(NULL);
```

`time_t → struct tm`:

```text
localtime() → Local Time
gmtime()    → UTC
```

`struct tm → time_t`:

```c
mktime()
```

```text
time_t
 ↓ localtime() / gmtime()
struct tm
 ↓ mktime()
time_t
```

### `strftime()` ⭐

* `struct tm`의 날짜/시간을 **원하는 형식의 문자열로 변환**한다.

```c
char s[128];

strftime(s, sizeof s, "%F %T", localtime(&now));
```

주요 형식:

```text
%F → yyyy-mm-dd
%T → hh:mm:ss
%z → 시간대 offset
```

### `timespec`

더 높은 해상도의 시간을 다룬다.

```c
struct timespec {
    time_t tv_sec;  // 초
    long tv_nsec;   // 나노초
};
```

```c
timespec_get(&ts, TIME_UTC);
```

### 시간 차이

두 `time_t`의 차이는 `difftime()`을 사용한다.

```c
double diff = difftime(time_b, time_a);
```

→ 두 시간 사이의 **초 단위 차이**를 반환한다.

### 기억할 것

> **`time_t` → Calendar Time**
> **`struct tm` → 분해된 날짜/시간**
> **`time()` → 현재 시간 획득**
> **`localtime()` → Local Time 변환**
> **`gmtime()` → UTC 변환**
> **`mktime()` → `struct tm` → `time_t`**
> **`strftime()` → 원하는 형식의 문자열**
> **`timespec` → 초 + 나노초**
> **`difftime()` → 두 시간의 차이** 


## Ch.39 — Multithreading（多线程）(26.09.02)

### 핵심 개념

* **Thread（线程）**

  * 하나의 프로그램 안에서 독립적으로 실행되는 작업 단위.
  * 여러 스레드는 실행 순서가 보장되지 않는다.

* **`thrd_create()`**

  * 새로운 스레드를 생성하고 지정한 함수를 실행한다.

  ```c
  int worker(void *arg)
  ```

  * 스레드 시작 함수는 **`void *`를 받고 `int`를 반환**하는 형태.
  * 반환값은 `thrd_join()`으로 받을 수 있다.

* **`thrd_join()`**

  * 해당 스레드가 **종료될 때까지 기다림**.
  * 스레드의 `return` 값도 받을 수 있다.

  ```c
  int result;
  thrd_join(t, &result);
  ```

* **Race Condition（竞态条件）**

  * 여러 스레드가 **공유 데이터에 동시에 접근**하면서 실행 순서에 따라 결과가 달라지는 문제.
  * 특히 `count++`처럼 읽기 → 계산 → 쓰기가 필요한 작업에서 발생할 수 있다.

* **Mutex（互斥锁）**

  * 공유 데이터를 한 번에 하나의 스레드만 접근하도록 보호.

  ```c
  mtx_lock(&mutex);

  count++;

  mtx_unlock(&mutex);
  ```

  * `lock`과 `unlock` 사이가 **Critical Section（临界区, 임계 영역）**.

* **Condition Variable（条件变量）**

  * 어떤 조건이 만족될 때까지 스레드를 **대기시켰다가 깨우는 데 사용**.

  ```c
  cnd_wait(&cond, &mutex);
  ```

  * `cnd_wait()`는:

    1. mutex를 풀고
    2. 스레드를 대기시킨 뒤
    3. 깨어나면 mutex를 다시 획득한다.

* **`while + cnd_wait()`**

  ```c
  while (!ready)
      cnd_wait(&cond, &mutex);
  ```

  * **Spurious Wakeup（허위 기상）** 때문에 `if`가 아니라 `while`을 사용.
  * 깨어났다고 해서 조건이 만족됐다는 보장은 없다.
  * 따라서 깨어날 때마다 조건을 다시 확인한다.

* **Thread Local（线程局部）**

  ```c
  _Thread_local int x;
  ```

  * 스레드마다 독립적인 `x`를 갖는다.
  * 일반적인 지역 변수도 각 스레드의 호출 스택에 존재하므로 기본적으로 스레드별로 독립적이다.

### 기억할 것

> **Thread = 독립적인 실행 단위**

> **Race Condition = 공유 데이터 + 동시 접근 + 실행 순서 문제**

> **Mutex = 공유 데이터 보호**

> **Critical Section = 보호해야 하는 코드 영역**

> **Condition Variable = 조건이 될 때까지 대기**

> **`cnd_wait()` = mutex 해제 → 대기 → 깨어남 → mutex 재획득**

> **Condition Variable은 `while`로 조건을 다시 확인한다.**


## Ch.40 — Atomics (26.09.02)

### 핵심 개념

* **Atomic(원자적 연산 / 原子操作)**

  * 읽기·쓰기를 다른 스레드가 중간 상태로 관찰하지 않도록 **하나의 연산처럼 처리**.
  * `atomic_int` 등을 사용.

* **Atomic ≠ 모든 동시성 문제 해결**

  * 단순 연산은 Atomic으로 처리할 수 있음.
  * `if (x == 0) x = 100;`처럼 여러 연산으로 이루어진 논리는 여전히 **Race Condition(竞态条件)**이 발생할 수 있음.

* **Atomic RMW(Read-Modify-Write)**

  * 읽기 → 수정 → 쓰기를 하나의 Atomic 연산으로 처리.
  * `++`, `--`, `+=`, `-=` 등의 연산이 해당.

### Synchronization

* 여러 스레드에서 공유 데이터를 사용할 때는 **Atomicity(原子性)**뿐 아니라 **Synchronization(同步)**도 중요.
* 컴파일러/CPU의 재배치와 메모리 가시성 때문에 소스 코드의 순서대로 다른 스레드가 보지 않을 수 있음.

### Acquire / Release

```text
Thread A
data = 100;
ready = 1;       ← Release
                  ↓
               동기화
                  ↓
ready == 1       ← Acquire
data 읽기
```

* **Release(释放)** → 이전에 수행한 작업을 다른 스레드가 볼 수 있도록 공개.
* **Acquire(获取)** → 다른 스레드가 Release한 이전 작업을 볼 수 있도록 함.

### Sequential Consistency

* **Sequential Consistency(순차적 일관성 / 顺序一致性)**
* Atomic 연산을 모든 스레드가 **일관된 하나의 순서**로 보는 메모리 순서.
* `memory_order_seq_cst`가 기본값.
* 가장 강하고 이해하기 쉬운 메모리 순서.

### Mutex와 비교

```text
Atomic
→ 개별 값의 Atomic 연산
→ 카운터, 플래그 등

Mutex
→ 여러 작업으로 이루어진 Critical Section(临界区) 보호
→ 복잡한 공유 데이터 처리
```

### 실무 연결

```text
C11 Atomic
    ↓
Multithreading
    ↓
Race Condition / Synchronization
    ↓
Linux System Programming
    ↓
Linux Kernel
    ↓
Driver / BSP의 Concurrency
```

### 기억할 것

> **Atomic = 하나의 연산처럼 처리**

> **Atomic이라고 여러 연산으로 된 논리까지 안전해지는 것은 아니다.**

> **Atomicity = 중간 상태 방지 / Synchronization = 스레드 간 작업 전달**

> **Release → 공개, Acquire → 확인**

> **Atomic은 단순 공유 상태, Mutex는 여러 작업을 묶어서 보호**


# Ch.41 — 함수 지정자와 정렬 (26.09.02)

### 1. `noreturn` / `_Noreturn`

함수가 **호출자에게 반환하지 않는다**는 것을 컴파일러에게 알리는 지정자.

```c
#include <stdnoreturn.h>

noreturn void foo(void)
{
    exit(1);
}
```

* 컴파일러 최적화에 활용될 수 있음
* 다른 개발자에게 함수가 반환하지 않는다는 의도를 전달
* 실제로 반환하면 **Undefined Behavior**
* 실무에서 직접 사용할 일은 많지 않음
* `exit()`, `abort()` 같은 함수에서 볼 수 있음

> **`noreturn` = 반환값이 없다는 뜻이 아니라, 호출자에게 돌아오지 않는다는 뜻**

---

### 2. Alignment (정렬 / 对齐)

객체를 **어떤 주소 경계에 맞춰 저장해야 하는지**에 대한 개념.

예:

```c
char alignas(8) c;
```

→ `c`를 **8바이트 경계에 맞춰 정렬**하도록 지정.

저수준 코드에서 중요하다.

* Memory Allocator
* Embedded
* Hardware
* Atomic
* SIMD 등

---

### 3. `alignas` / `_Alignas`

변수의 **정렬을 지정**한다.

```c
char alignas(8) c;
```

또는:

```c
char alignas(int) c;
```

`<stdalign.h>`의 `alignas`를 사용할 수 있다.

---

### 4. `alignof` / `_Alignof`

특정 타입의 **Alignment 요구사항을 확인**한다.

```c
alignof(int)
```

예를 들어 시스템에 따라:

```text
char  → 1
int   → 4
double → 8
```

등이 될 수 있다.

값은 **시스템마다 다를 수 있다.**

---

### 5. `max_align_t`

시스템에서 일반적인 타입들이 사용하는 **최대 Alignment**를 나타내는 타입.

```c
char alignas(max_align_t) c;
```

→ 시스템의 최대 정렬 기준에 맞춰 `c`를 정렬하도록 지정.

---

### 6. `memalignment()` — C23

포인터가 가리키는 **실제 메모리의 Alignment를 확인**한다.

특히 `void *`처럼 구체적인 타입을 모르는 포인터를 사용할 때 의미가 있다.

```c
void foo(void *p)
{
    if (memalignment(p) >= alignof(atomic int)) {
        atomic int *i = p;
        do_things(i);
    }
}
```

→ `p`가 `atomic int`로 사용하기에 충분히 정렬되어 있는지 확인.

저수준 코드가 아니라면 사용할 일이 거의 없다.

---

## 기억할 것

> **`noreturn` → 호출자에게 반환하지 않음**

> **`alignas` → Alignment 지정**

> **`alignof` → Alignment 확인**

> **`max_align_t` → 시스템의 최대 Alignment**

> **`memalignment()` → 포인터가 가리키는 메모리의 Alignment 확인**

> **Alignment는 저수준 시스템/임베디드에서 중요하다.**