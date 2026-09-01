## Ch.13 — Scope (2026.08.27)

### 핵심 개념

* Scope(스코프 / 作用域)는 **변수를 사용할 수 있는 코드의 범위**이다.

```text
Block Scope    → {} 블록 내부
File Scope     → 파일 내에서 선언 이후
for-loop Scope → for문 내부
```

---

### Block Scope

* `{}` 안에서 선언한 변수는 **해당 블록 안에서만 유효**하다.

```c
int a = 10;

if (a > 5) {
    int b = 20;
}

// b 사용 불가
```

* 안쪽 블록에서는 바깥쪽 변수를 사용할 수 있다.

---

### Variable Hiding

* 안쪽 Scope에서 같은 이름의 변수를 선언하면 **바깥쪽 변수를 숨긴다.**

```c
int i = 10;

{
    int i = 20;
    printf("%d\n", i);  // 20
}

printf("%d\n", i);      // 10
```

---

### File Scope

* 블록 밖에서 선언한 변수는 **선언 이후 파일의 여러 함수에서 공유**할 수 있다.

```c
int shared = 10;

void foo(void)
{
    shared++;
}
```

---

### `for`-loop Scope

* `for`문에서 선언한 변수는 **해당 for문 안에서만 유효**하다.

```c
for (int i = 0; i < 10; i++)
    printf("%d\n", i);

// i 사용 불가
```

---

### 기억할 것

> **Scope = 변수를 사용할 수 있는 영역. `{}` 안에서 선언하면 Block Scope, 블록 밖에서 선언하면 File Scope이며, 안쪽 Scope의 같은 이름 변수는 바깥쪽 변수를 숨긴다.**


## Ch.14 — Types II: Way More Types! (2026.08.27)

### 핵심 개념

* C에는 다양한 숫자 타입이 있으며 **표현 범위, 정밀도, 크기**가 다르다. 

```text
정수 → char, short, int, long, long long
실수 → float, double, long double
```

---

### Signed / Unsigned

* `signed` → 음수와 양수 표현
* `unsigned` → `0` 이상의 값만 표현 → 더 큰 양수까지 표현 가능

8비트 기준:

```text
signed char   → -128 ~ 127
unsigned char →    0 ~ 255
```

---

### `char`

* `char`는 문자를 저장하지만 내부적으로는 **정수 값**으로 다룰 수 있다.

```c
char c = 'B';

printf("%c\n", c);  // B
printf("%d\n", c);  // 66 (ASCII 환경)
```

* 일반 `char`가 signed인지 unsigned인지는 구현에 따라 다를 수 있다.

---

### 정수 타입

```text
short ≤ int ≤ long ≤ long long
```

* 정확한 크기는 시스템에 따라 다를 수 있으므로 항상 `short = 2B`, `int = 4B`처럼 가정하면 안 된다.
* `sizeof()`로 실제 크기를 확인할 수 있고, `<limits.h>`로 각 타입의 범위를 확인할 수 있다.

---

### 실수 타입

```text
float < double < long double
```

* 일반적으로 뒤로 갈수록 더 높은 정밀도를 제공한다.
* 부동소수점은 모든 실수를 정확하게 표현할 수 있는 것은 아니다.

---

### 숫자 상수

정수 상수의 타입을 접미사로 지정할 수 있다.

```text
1234    → 기본 정수 상수
1234U   → unsigned int
1234L   → long
1234LL  → long long
1234ULL → unsigned long long
```

실수 상수:

```text
3.14  → double
3.14f → float
3.14L → long double
```

진법:

```text
0x10 → 16진수 → 16
010  → 8진수  → 8
```

따라서 정수 앞에 무심코 `0`을 붙이지 않도록 주의한다.

---

### 기억할 것

> **C의 숫자 타입은 표현 범위·정밀도·크기가 다르다. `signed/unsigned`의 차이와 정수 타입의 크기가 시스템마다 달라질 수 있다는 점, 그리고 숫자 상수 자체에도 타입이 있다는 것을 기억한다.**


## Ch.15 — Types III: Conversions (2026.08.27)

### 핵심 개념

* Conversion(타입 변환 / 类型转换)은 **한 타입의 값을 다른 타입으로 변환하는 것**이다. 

```text
문자열 ↔ 숫자
숫자 타입 ↔ 숫자 타입
Implicit Conversion → 자동 변환
Explicit Conversion → 명시적 변환(Casting)
```

---

### 문자열 ↔ 숫자

숫자 → 문자열:

```c
snprintf(s, 10, "%f", f);
```

문자열 → 숫자:

```c
atoi("123");      // → int 123
atof("3.14");     // → 숫자 3.14
```

* 포인터의 **주소 자체를 숫자로 변환하는 것이 아니라**, 포인터가 가리키는 문자열을 읽어서 변환한다.
* `strtol()` 계열은 **오류 위치와 진법을 확인할 수 있어** 입력 검증에 더 유리하다.

```c
strtoul("101010", NULL, 2);  // → 42
```

---

### `char` ↔ 숫자

```c
'6' - '0'  // → 숫자 6
6 + '0'    // → 문자 '6'
```

숫자 문자의 코드값이 연속되어 있기 때문에 가능하다.

---

### 숫자 타입 변환

* `0 → false`, `0 이외 → true`
* 실수 → 정수 변환 시 **소수 부분을 버린다.**

```c
int x = 3.99;  // → 3
```

* 범위를 벗어난 정수를 변환하면:

```text
→ unsigned : wrap around
→ signed   : implementation-defined
```

---

### Integer Promotion

* `char`, `short` 등은 계산할 때 `int`로 승격될 수 있다.

```c
char a = 10;
char b = 20;

int x = a + b;
```

```text
char + char
→ int + int
→ 계산
```

---

### Implicit Conversion

* 서로 다른 타입을 계산하면 **컴파일러가 자동으로 타입을 맞춘다.**

```c
int x = 3 + 1.9;
```

```text
int + double
→ double + double
→ 4.9
→ int에 저장
→ 4
```

---

### `void *`

* `void *`는 여러 **객체 포인터 타입**과 서로 변환할 수 있다.

```c
int x = 10;

void *p = &x;
int *q = p;
```

---

### Casting

* `(type)`을 사용해 프로그래머가 직접 타입을 변환할 수 있다.

```c
double x = 3.99;

int a = (int)x;  // → 3
```

```text
Implicit Conversion → 컴파일러가 자동 변환
Explicit Conversion → 프로그래머가 직접 명시
```

---

### 기억할 것

> **C에서는 타입이 자동으로 변환될 수 있다. 특히 `char/short → int` 정수 승격, `int + double → double` 같은 암시적 변환, 실수 → 정수에서 소수 부분이 버려지는 점, 그리고 `(type)`을 이용한 명시적 Casting을 기억한다.**


## Ch.16 — Types IV: Qualifiers and Specifiers (2026.08.27)

### 핵심 개념

* **Type Qualifier** → 타입에 추가적인 성질을 부여한다.
* **Storage-Class Specifier** → 변수의 저장 기간이나 다른 파일에서의 접근 등에 영향을 준다. 

---

### `const`

* 값을 수정하지 못하게 한다.

```c
const int x = 10;
```

포인터에서는 `const`의 위치가 중요하다.

```text
const int *p        → p 변경 O, *p 변경 X
int *const p        → p 변경 X, *p 변경 O
const int *const p  → p 변경 X, *p 변경 X
```

* `const` 객체를 일반 포인터로 받아 수정하려 하면 문제가 된다.

---

### `volatile`

* **외부 요인으로 값이 변경될 수 있음**을 컴파일러에게 알린다.
* 컴파일러가 해당 메모리 접근을 함부로 최적화하지 않도록 한다.

```c
volatile int *timer;
```

```text
volatile ≠ 멀티스레드 안전
volatile ≠ atomic 보장
```

하드웨어 레지스터 등을 다룰 때 중요하다.

---

### `restrict`

* 해당 객체에 다른 포인터를 통한 중복 접근(aliasing)을 하지 않겠다고 컴파일러에게 약속한다.
* 최적화에 활용될 수 있지만 잘못 사용하면 Undefined Behavior가 발생할 수 있다.

```c
swap(&x, &y);  // OK
swap(&x, &x);  // restrict 약속 위반
```

---

### `static` ⭐

`static`은 **어디에 선언했는지에 따라 의미가 다르다.**

함수 안:

```c
void foo(void)
{
    static int count = 0;
}
```

```text
Scope    → 함수 내부
Lifetime → 프로그램이 끝날 때까지

→ 함수 호출이 끝나도 값 유지
```

함수 밖:

```c
static int count = 0;
```

```text
Scope    → File Scope
Lifetime → 프로그램 전체

→ 다른 .c 파일에서는 접근 불가
→ 현재 .c 파일 전용
```

> **함수 안 `static` → Scope는 그대로, Lifetime이 길어진다.**
> **함수 밖 `static` → 현재 `.c` 파일에서만 사용할 수 있게 한다.**

---

### `extern` ⭐

* **다른 곳에 정의된 객체를 참조하기 위한 선언**이다.

```c
// a.c
int count = 10;

// b.c
extern int count;
```

`extern int count;`가 새로운 변수를 만드는 것이 아니라 `a.c`에 정의된 `count`를 참조한다.

단:

```c
// a.c
static int count = 10;
```

이면 다른 파일에서 `extern`으로 접근할 수 없다.

---

### 함수와 Linker

```c
void hello(void);
```

는 다른 `.c` 파일을 자동으로 읽어오는 것이 아니다.

```text
함수 선언
→ 이런 함수가 존재한다고 컴파일러에게 알림

실제 함수 구현 연결
→ Linker가 담당
```

일반 함수는 다른 파일에서도 연결할 수 있지만:

```c
static void hello(void);
```

처럼 `static`을 붙이면 현재 `.c` 파일에서만 사용할 수 있다.

---

### `auto` / `register`

```c
auto int x;
```

* C++의 타입 추론 `auto`와 다르다.
* C에서는 **automatic storage duration**을 의미한다.
* 일반 지역 변수는 기본적으로 `auto`라 거의 직접 쓰지 않는다.

`register`는 자주 사용하는 변수를 빠르게 접근할 수 있도록 해달라는 힌트지만, 현대 컴파일러가 최적화를 알아서 하기 때문에 거의 사용하지 않는다.

---

### 기억할 것

> **`const` → 수정 제한**
> **`volatile` → 외부에서 값이 변할 수 있음을 알림**
> **함수 안 `static` → 값 유지, Scope는 함수 내부**
> **함수 밖 `static` → 현재 `.c` 파일 전용**
> **`extern` → 다른 곳에 정의된 객체 참조**
> **실제 여러 파일의 함수/변수 연결은 Linker가 담당**


## Ch.17 — Multifile Projects (2026.08.27)

### 핵심 개념

* 큰 C 프로젝트는 여러 `.c`, `.h` 파일로 나누어 작성한다.
* 각 `.c` 파일을 **Object File**로 컴파일한 뒤, **Linker**가 하나의 실행 파일로 연결한다.

```text
.c → Compile → .o → Link → 실행 파일
```

---

### `.h` / `.c`

보통 역할을 다음처럼 나눈다.

```text
.h → 다른 파일이 알아야 할 선언(Prototype)
.c → 실제 함수 구현
```

예:

```c
// math.h
int add(int, int);
```

```c
// math.c
int add(int x, int y)
{
    return x + y;
}
```

다른 파일에서는:

```c
#include "math.h"
```

로 `add()`의 선언을 가져온다.

---

### `#include`

```c
#include "math.h"
```

* 해당 헤더 파일의 **내용을 그 위치에 포함**한다.
* `math.c`의 실제 구현까지 가져오는 것은 아니다.

```text
#include <stdio.h>  → 보통 시스템/표준 헤더
#include "math.h"   → 보통 프로젝트 헤더
```

---

### Include Guard

헤더가 여러 번 포함되는 것을 방지한다.

```c
#ifndef MATH_H
#define MATH_H

int add(int, int);

#endif
```

```text
첫 include
→ MATH_H 없음
→ 내용 포함 + MATH_H 정의

두 번째 include
→ MATH_H 이미 있음
→ 내용 건너뜀
```

헤더를 만들 때 기본적으로 넣는 습관을 들인다.

---

### `static` / `extern`

```c
extern int x;
```

→ 다른 곳에 정의된 객체를 참조한다.

파일 스코프에서:

```c
static int x;
static void foo(void);
```

→ 현재 `.c` 파일에서만 사용할 수 있다.

---

### Object File

```bash
gcc -c foo.c
gcc -c bar.c
```

결과:

```text
foo.c → foo.o
bar.c → bar.o
```

`-c`는 **compile only**로, 아직 링크하지 않는다.

Windows에서는 보통 `.obj`, Unix 계열에서는 `.o`를 사용한다.

---

### Link

```bash
gcc -o foo foo.o bar.o
```

```text
foo.o ─┐
       ├─ Link → foo
bar.o ─┘
```

Linker는 한 Object File에서 필요한 함수/객체를 다른 Object File의 실제 구현과 연결한다.

예:

```text
main.o
→ "add()가 필요함"
        ↓
      Linker
        ↓
math.o
→ "add() 구현이 여기 있음"

        ↓
최종 실행 파일
```

---

### `gcc -o`

```bash
gcc -o foo foo.c bar.c
```

* `-o` → **output 이름 지정**
* `foo` → 최종 실행 파일 이름
* `foo.c`, `bar.c` → 소스 파일

GCC가 컴파일과 링크를 모두 처리한다.

---

### Object File을 사용하는 이유

파일이 많은 프로젝트에서 하나의 `.c`만 수정했다면 **그 파일만 다시 컴파일**하면 된다.

```text
foo.c 수정

foo.c
 ↓ Compile
foo.o
 ↓
기존 다른 .o들과 Link
 ↓
실행 파일
```

→ 전체 소스를 매번 다시 컴파일하지 않아도 되어 **빌드 시간이 크게 줄어든다.**

---

### 기억할 것

> **`.h` → 선언 / `.c` → 구현**
> **`#include` → 헤더 내용을 현재 파일에 포함**
> **Include Guard → 헤더 중복 포함 방지**
> **`.c → .o` = Compile**
> **여러 `.o → 실행 파일` = Link**
> **Linker가 함수 호출과 다른 Object File의 실제 구현을 연결한다.**