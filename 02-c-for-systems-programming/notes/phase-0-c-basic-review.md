# Phase 0 — C Basic 빠른 복습

## Ch.2 — Hello, World

### 핵심 개념

* C는 저수준 언어(Low-level Language / 低级语言)로, 메모리와 하드웨어에 가까운 동작을 이해하기 좋다.
* 프로그램은 main() 함수에서 실행을 시작한다.
* `#include`는 **전처리기(Preprocessor / 预处理器)**가 처리한다.
* `<stdio.h>`는 표준 입출력 **헤더 파일(Header File / 头文件)**이며 `printf()` 등을 사용할 수 있게 한다.

```c
#include <stdio.h>

int main(void)
{
    printf("Hello, World!\n");
}
```

* printf() → 출력 함수
* "Hello, World!\n" → 전달하는 문자열
* \n → 줄바꿈(Newline / 换行符)
* ; → 하나의 문장이 끝났음을 표시
* /* */, // → 주석(Comment / 注释)

### 컴파일

```text
C Source Code
→ Preprocessor
→ Compiler
→ Executable
```

```bash
gcc -o hello hello.c
./hello
```

* `gcc -o hello hello.c` → `hello.c`를 컴파일해 `hello` 실행 파일 생성
* `./hello` → 현재 디렉터리의 `hello` 실행

### 기억할 것

> **C 소스 코드가 전처리와 컴파일을 거쳐 CPU가 실행할 수 있는 프로그램이 된다.** 