# Phase 0 — C Basic 빠른 복습

## Ch.2 — Hello, World (2026.08.24)

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


## Ch.3 — Variables and Statements (2026.08.24)

### 핵심 개념

* 변수(Variable / 变量)는 **메모리의 특정 주소에 저장된 데이터에 붙인 이름**이다.
* 변수는 사용하기 전에 타입(Type / 类型)을 지정하여 선언해야 한다.

```c
int i = 2;
float f = 3.14;
char c = 'A';
```

* 초기화하지 않은 변수는 불확정 값(Indeterminate Value / 不确定值)을 가지므로 사용 전에 초기화한다.
* C에서 `0`은 `false`, **0이 아닌 값은 `true`로 취급**된다.
* 주요 연산자:

```text
산술       + - * / %
비교       == != < > <= >=
논리       && || !
복합 대입   += -= *= /= %=
```

* 삼항 연산자(Ternary Operator / 三元运算符)는 조건에 따라 하나의 값을 선택한다.

```c
condition ? value1 : value2;
```

* 전위 `++i`는 **증가 후 사용**, 후위 `i++`는 **사용 후 증가**한다.
* `sizeof`는 변수나 타입이 차지하는 메모리 크기를 Byte 단위로 반환한다.

```c
printf("%zu\n", sizeof(int));
```

* 주요 흐름 제어(Flow Control / 流程控制):

```text
if / else
while
do-while
for
switch
```

* `while`은 조건을 먼저 검사하고, `do-while`은 실행 후 조건을 검사하므로 최소 한 번 실행된다.
* `switch`는 값에 해당하는 `case`를 실행하며 `break`를 만나면 빠져나간다.
* `switch`에서 `break`가 없으면 다음 `case`까지 실행되며 이를 **Fall Through**라고 한다. 

### 새로 알게 된 사실

* `true`는 숫자 `1`이다. 따라서:

```c
printf("%d\n", true == 12);
```

는 `1 == 12` → `false`이므로 `0`을 출력한다.

* **0이 아닌 값이 조건에서 `true`로 취급되는 것과 그 값이 `true`와 같은 것은 다르다.**

```c
if (12)        // true로 취급
true == 12     // false
```

* 연산자 우선순위 때문에:

```c
y += x > 10 ? 17 : 37;
```

는 다음과 같다.

```c
y += (x > 10 ? 17 : 37);
```

자주 쓰는 우선순위:

```text
()
→ ! ++ -- sizeof
→ * / %
→ + -
→ < <= > >=
→ == !=
→ &&
→ ||
→ ?:
→ = += -= ...
```

* `%zu`는 **`size_t` 타입을 출력하는 형식 지정자**이다. `sizeof`의 반환 타입이 `size_t`이므로 다음처럼 사용한다.

```c
printf("%zu\n", sizeof(int));
```

* `char`는 문자를 표현하는 데 사용하지만 C의 타입 체계에서는 **정수 타입(Integer Type / 整数类型)의 하나**이다.

```c
char c = 'A';

printf("%c\n", c);  // A
printf("%d\n", c);  // ASCII 환경에서는 65
```

* Fall Through를 의도적으로 사용하면 여러 `case`가 같은 코드를 공유하게 할 수 있다.

```c
switch (grade) {
    case 'A':
    case 'B':
    case 'C':
        printf("Pass\n");
        break;
}
```

### 기억할 것

> **변수 = 메모리의 데이터에 붙인 이름. C에서는 타입과 연산자에 주의하고, `if / while / for / switch`로 프로그램의 실행 흐름을 제어한다.**


## Ch.4 — Functions (2026.08.24)

### 핵심 개념

* 함수(Function / 函数)는 **특정 작업을 수행하는 코드 단위**이며, 인자를 받고 값을 반환할 수 있다.

```c
int plus_one(int n)
{
    return n + 1;
}
```

* `int plus_one(int n)`에서 앞의 `int`는 **반환 타입**, `int n`은 **매개변수(Parameter / 参数)**이다.

* `void`는 **반환값이 없거나 매개변수가 없음**을 나타낸다.

```c
void hello(void);
```

* C의 함수 호출은 **값에 의한 전달(Passing by Value / 值传递)**이다. 즉, 인자의 **값을 복사해서 매개변수에 전달**한다.

```c
void increment(int a) {
    a++;
}

int i = 10;
increment(i);   // i는 여전히 10
```

* 함수 프로토타입(Function Prototype / 函数原型)은 함수를 사용하기 전에 **반환 타입과 매개변수 정보를 컴파일러에게 알려준다.**

```c
int foo(void);  // prototype

int main(void) {
    foo();
}

int foo(void) {
    return 3490;
}
```

* `printf()`도 `stdio.h`에 프로토타입이 선언되어 있기 때문에 사용할 수 있다.

### 새로 알게 된 사실

* **Argument(인자)**는 호출할 때 전달하는 값이고, **Parameter(매개변수)**는 함수가 그 값을 받아 저장하는 지역 변수이다.

```c
plus_one(i);
//       ↑ Argument

int plus_one(int n)
//               ↑ Parameter
```

* C에서는 매개변수를 변경해도 원래 인자는 변경되지 않는다. 원본이 아니라 **복사본을 수정하기 때문**이다.

* 교재 기준 기존 C에서는 다음 둘의 의미가 다르다.

```c
void foo();       // 매개변수 정보가 명확하지 않음
void foo(void);   // 매개변수 0개로 확정
```

따라서 매개변수가 없다면 `foo(void)`처럼 명확하게 작성한다.

### 기억할 것

> **함수 = 특정 작업을 수행하는 코드 단위. C에서는 인자의 값이 매개변수로 복사되며, 함수를 사용하기 전에 정의하거나 Function Prototype으로 미리 선언한다.**


## Ch.5 — Pointers (2026.08.24)

### 핵심 개념

* 포인터(Pointer / 指针)는 **메모리 주소(Address / 地址)를 저장하는 변수**이다.

```c
int i = 10;
int *p = &i;
```

* `&`는 **주소 연산자(Address-of Operator / 取地址运算符)**로 변수의 주소를 얻는다. `%p`는 주소를 출력할 때 사용한다.

```c
printf("%p\n", (void *)&i);
```

* `*`는 문맥에 따라 의미가 다르다.

```c
int *p = &i;  // 선언: p는 int를 가리키는 포인터
*p = 20;      // 역참조: p가 가리키는 값에 접근
```

* 역참조(Dereference / 解引用)는 **포인터가 가리키는 실제 데이터에 접근하는 것**이다.

```text
i = 10
p = &i

p  → i의 주소
*p → i의 값
```

* 포인터도 함수에 전달할 때 **복사본이 전달**된다. 하지만 복사된 포인터도 같은 주소를 가지므로 원본 변수에 접근할 수 있다.

```c
void increment(int *p)
{
    *p = *p + 1;
}

int i = 10;
increment(&i);   // i = 11
```

* `NULL`은 포인터가 **아무것도 가리키지 않음**을 나타낸다. `NULL`을 역참조하면 Undefined Behavior가 발생한다.

```c
int *p = NULL;
// *p = 10;  // 잘못된 사용
```

* 포인터 선언에서는 각 포인터 변수에 `*`를 붙여야 한다.

```c
int *p, q;   // p는 int*, q는 int
int *p, *q;  // 둘 다 int*
```

### 기억할 것

> **포인터 = 주소를 저장하는 변수. `&`로 주소를 얻고 `*`로 그 주소가 가리키는 값에 접근한다. 포인터도 함수에 복사되어 전달되지만 같은 주소를 가리키므로 원본 데이터를 수정할 수 있다.** 


## Ch.6 — Arrays (2026.08.24)

### 핵심 개념

* 배열(Array / 数组)은 **같은 타입의 여러 데이터를 연속된 메모리에 저장**하는 구조이며, 인덱스는 `0`부터 시작한다. 

```c
int a[5] = {1, 2, 3, 4, 5};
```

* 배열이 선언된 곳에서는 `sizeof`로 원소 개수를 계산할 수 있다.

```c
sizeof a / sizeof(int);
```

* 초기화 값이 부족하면 나머지는 자동으로 `0`이 된다.

```c
int a[5] = {1, 2};  // {1, 2, 0, 0, 0}
int b[100] = {0};   // 전체 0
```

* 배열 범위를 벗어나 접근하면 **Undefined Behavior(未定义行为)**가 발생한다.

```c
int a[5];
a[5] = 10;  // 잘못된 접근
```

* 다차원 배열(Multidimensional Array / 多维数组)은 `[]`를 추가하여 선언하며 **Row-major Order(行优先)**로 연속 저장된다.

```c
int a[2][3];
int b[2][3][4];
int c[2][3][4][5];
```

* 대부분의 표현식에서 배열 이름은 **첫 번째 원소를 가리키는 포인터로 변환**된다.

```c
int a[5];

a == &a[0];  // 이 문맥에서는 같은 주소를 가리킴
```

* 배열을 함수에 전달하면 첫 번째 원소를 가리키는 포인터가 전달되므로, 함수에서 원본 배열을 수정할 수 있다.

```c
void double_array(int *a, int len)
{
    for (int i = 0; i < len; i++)
        a[i] *= 2;
}

double_array(a, 5);
```

### 새로 알게 된 사실

* 배열을 함수에 전달하면 배열의 **길이 정보는 함께 전달되지 않는다.**

```c
void foo(int x[12])
{
    sizeof x;  // 배열 크기가 아니라 포인터 크기
}
```

따라서 보통 길이도 별도의 매개변수로 전달한다.

```c
void foo(int *a, int len);
```

* 배열 자체와 포인터는 같지 않다.

```c
int a[5];
int *p = a;

sizeof a;  // 배열 전체 크기
sizeof p;  // 포인터 크기
```

* 함수 매개변수에서는 다음 세 표현이 동일하게 취급된다.

```c
void foo(int *a);
void foo(int a[]);
void foo(int a[5]);
```

따라서 함수 안의 sizeof a는 배열 전체가 아니라 포인터 크기를 구한다.

* 다차원 배열을 함수에 전달할 때는 **첫 번째 차원을 제외한 나머지 차원의 크기가 필요**하다.

```c
void foo(int a[][3]);        // 2차원
void foo(int a[][3][4]);     // 3차원
void foo(int a[][3][4][5]);  // 4차원
```

컴파일러가 다음 위치로 이동할 때 메모리에서 얼마나 건너뛰어야 하는지 계산하기 위해서다.

### 기억할 것

> **배열 = 같은 타입의 데이터를 연속된 메모리에 저장하는 구조. 배열 이름은 대부분의 표현식에서 첫 원소의 포인터로 변환되며, 함수에 배열을 전달하면 길이 정보 없이 첫 원소를 가리키는 포인터가 전달된다.** 


## Ch.7 — Strings (2026.08.24)

### 핵심 개념

* C의 문자열(String / 字符串)은 **`char`들의 연속이며 마지막에 `NUL` 문자 `'\0'`이 존재**한다.

```c
"Hello"  // 실제로는 "Hello\0"
```

* 문자열 리터럴(String Literal / 字符串字面量)은 **코드에 직접 작성한 문자열 값**이다.

```c
"Hello, world!"
```

* 문자열은 포인터 또는 `char` 배열을 이용해 다룰 수 있다.

```c
char *s = "Hello";
char t[] = "Hello";
```

* 두 방식에는 중요한 차이가 있다.

```c
char *s = "Hello";  // 문자열 리터럴을 가리킴 → 수정하면 Undefined Behavior
char t[] = "Hello"; // 배열을 초기화 → 수정 가능
```

* `strlen()`은 `'\0'`을 만날 때까지 문자를 세어 **문자열 길이**를 구한다. `'\0'` 자체는 길이에 포함하지 않는다.

```c
strlen("Hello");  // 5
```

* 문자열은 `=`로 내용을 복사할 수 없다. 포인터를 대입하면 **주소만 복사되어 같은 문자열을 가리킨다.**

```c
char s[] = "Hello";
char *t = s;  // 문자열 복사 X, 같은 배열을 가리킴
```

* 문자열 내용을 별도의 배열로 복사하려면 `strcpy()`를 사용할 수 있으며, 목적지에 충분한 공간이 있어야 한다.

```c
char s[] = "Hello";
char t[20];

strcpy(t, s);  // destination, source
```

### 새로 알게 된 사실

* 리터럴(Literal / 字面量)은 **코드에 값을 직접 작성한 것**을 의미한다.

```c
10       // 정수 리터럴
'A'      // 문자 리터럴
"Hello"  // 문자열 리터럴
```

* 다음 두 코드는 겉보기에는 비슷하지만 다르다.

```text
char *s = "Hello";
→ s가 문자열 리터럴을 가리킴
→ 내용 수정 ❌

char s[] = "Hello";
→ "Hello"의 내용으로 별도의 char 배열을 초기화
→ 내용 수정 ⭕
```

### 기억할 것

> **C 문자열 = `char`들의 연속 + 마지막 `'\0'`. 문자열 리터럴은 수정하면 안 되며, 수정이 필요하면 `char` 배열로 만든다. `=`는 문자열 내용이 아닌 포인터를 복사하고, 실제 문자열 복사에는 `strcpy()` 등을 사용한다.**


## Ch.8 — Structs (2026.08.24)

### 핵심 개념

* `struct`(구조체 / 结构体)는 **서로 다른 타입의 여러 데이터를 하나로 묶는 사용자 정의 타입**이다.

```c
struct car {
    char *name;
    float price;
    int speed;
};

struct car saturn;
```

* 구조체 자체의 필드에는 `.`으로 접근한다.

```c
saturn.price = 1000;
```

* 선언과 동시에 초기화할 수 있으며, **필드명을 지정하는 방식이 더 안전**하다.

```c
struct car saturn = {
    .name = "Saturn",
    .speed = 175
};
```

지정하지 않은 필드는 `0`으로 초기화된다.

* 함수에 구조체 자체를 전달하면 **구조체 전체가 복사**된다. 원본 수정이나 복사 비용을 줄이려면 구조체 포인터를 전달한다.

```c
void set_price(struct car *c, float price)
{
    c->price = price;
}

set_price(&saturn, 799.99);
```

* 구조체 포인터의 필드에는 `->`를 사용한다.

```c
c->price
```

이는 다음과 완전히 같다.

```c
(*c).price
```

### 새로 알게 된 사실

* 구조체는 `=`로 복사할 수 있다.

```c
struct car a, b;
b = a;
```

하지만 **Deep Copy가 아니라 각 필드의 값을 그대로 복사**하므로, 포인터 필드가 있다면 포인터가 가리키는 데이터까지 새로 복사되는 것은 아니다.

* 구조체끼리는 직접 비교하지 않고 **각 필드를 하나씩 비교하는 것이 안전**하다.

### 기억할 것

> **`struct` = 여러 데이터를 하나로 묶는 타입. 구조체 자체는 `.`, 구조체 포인터는 `->`. 함수에서 원본 구조체를 수정하려면 구조체의 주소를 전달한다.**


## Ch.9 — File Input/Output (2026.08.24)

### 핵심 개념

* C에서 파일 입출력(File I/O / 文件输入输出)은 `FILE*`를 통해 처리한다.

```c
FILE *fp;

fp = fopen("hello.txt", "r");
```

`FILE*`는 열린 파일과 현재 읽고 있는 위치 등의 정보를 관리하는 **스트림(Stream / 流)**을 나타낸다.

* C에는 기본적으로 세 가지 표준 스트림(Standard Stream / 标准流)이 열려 있다.

```text
stdin   → 표준 입력
stdout  → 표준 출력
stderr  → 표준 오류 출력
```

예를 들어 다음 두 코드는 같은 의미다.

```c
printf("Hello\n");
fprintf(stdout, "Hello\n");
```

* `fopen()`으로 파일을 열고 `fclose()`로 닫는다.

```c
FILE *fp = fopen("hello.txt", "r");

fclose(fp);
```

주요 파일 모드:

```text
"r"   → Text Read
"w"   → Text Write
"rb"  → Binary Read
"wb"  → Binary Write
```

`"w"`로 기존 파일을 열면 기존 내용은 지워지고 처음부터 다시 작성된다.

* `fgetc()`는 파일에서 **문자 하나씩** 읽는다.

```c
int c;

while ((c = fgetc(fp)) != EOF)
    printf("%c", c);
```

`fgetc()`는 모든 byte 값뿐 아니라 `EOF`도 표현해야 하기 때문에 반환형으로 `char`가 아니라 `int`를 사용한다.

* `EOF`(End Of File / 文件结束)는 더 이상 읽을 데이터가 없음을 나타내는 특별한 값이다.

```c
(c = fgetc(fp)) != EOF
```

의 흐름은 다음과 같다.

```text
fgetc(fp)
    ↓
c에 저장
    ↓
EOF와 비교
    ↓
EOF가 아니면 반복
```

* `fgets()`는 파일에서 **한 줄 또는 지정된 버퍼 크기까지 문자열로 읽는다.**

```c
char s[1024];

while (fgets(s, sizeof s, fp) != NULL)
    printf("%s", s);
```

`fgets()`는 읽은 문자열 끝에 NUL 문자 `\0`도 추가한다.

* `fscanf()`는 파일의 데이터를 지정된 형식에 맞춰 읽는다.

```c
char name[1024];
float length;
int mass;

fscanf(fp, "%1023s %f %d", name, &length, &mass);
```

```text
%1023s → 최대 1023개의 문자
%f     → float
%d     → int
```

`name`은 배열 이름이므로 대부분의 표현식에서 첫 번째 원소를 가리키는 포인터로 변환된다. 따라서 `&name`이 아니라 `name`을 사용한다.

```c
name       // char *
&length    // float *
&mass      // int *
```

`"%1023s"`처럼 길이를 제한하는 이유는 `char name[1024]`의 마지막 한 칸을 문자열 종료 문자 `\0`에 남겨두기 위해서다.

* 텍스트 파일에는 `fputc()`, `fputs()`, `fprintf()` 등을 사용해 데이터를 쓸 수 있다.

```c
fputc('B', fp);
fputs("Hello\n", fp);
fprintf(fp, "x = %d\n", x);
```

### Binary File

* Binary File(바이너리 파일 / 二进制文件)은 데이터를 문자로 제한하지 않고 **raw byte 그대로 저장하고 읽는 방식**이다.

```c
unsigned char bytes[6] = {5, 37, 0, 88, 255, 12};

FILE *fp = fopen("output.bin", "wb");

fwrite(bytes, sizeof(char), 6, fp);
```

위 데이터는 실제 byte 값으로 다음과 같이 저장된다.

```text
Decimal:   5   37   0   88   255   12
Hex:      05   25  00   58    FF   0C
```

Binary I/O에서는 주로 다음을 사용한다.

```c
fread()
fwrite()
```

* Text와 Binary 모두 컴퓨터 내부에서는 결국 byte로 저장된다. 차이는 **그 byte를 어떤 의미로 사용하는가**이다.

예를 들어:

```text
문자 '5'
→ ASCII 53
→ 0x35

숫자 5
→ 값 5
→ 0x05
```

따라서:

```c
char a = '5';          // 0x35
unsigned char b = 5;   // 0x05
```

`0x35`는 **문자 `'5'`를 표현하는 값**이고, `0x05`는 **숫자 값 `5`를 표현하는 값**이다.

### 새로 알게 된 사실

* `fwrite()`는 다음 구조로 사용한다.

```c
fwrite(data, size, count, fp);
```

즉:

```c
fwrite(bytes, sizeof(char), 6, fp);
```

는 `bytes`에서 `sizeof(char)` 크기의 데이터를 6개 파일에 기록한다는 뜻이다.

* 여러 byte로 이루어진 숫자를 메모리 그대로 저장하면 컴퓨터 아키텍처에 따라 byte 순서가 달라질 수 있다.

```c
unsigned short v = 0x1234;

fwrite(&v, sizeof v, 1, fp);
```

시스템의 Endianness(엔디언 / 字节序)에 따라 파일에:

```text
12 34
```

또는:

```text
34 12
```

처럼 저장될 수 있다.

대표적으로:

```text
Big Endian     → 중요한 byte부터 저장
Little Endian  → 덜 중요한 byte부터 저장
```

따라서 `struct`나 여러 byte 숫자를 메모리에서 그대로 `fwrite()`하는 방식은 다른 시스템에서도 동일하게 해석된다고 보장할 수 없다.

이러한 데이터를 이식 가능한 형식으로 변환하여 저장하는 것을 **Serialization(직렬화 / 序列化)**이라고 한다.

### 기억할 것

> **`FILE*` = 파일 스트림. `fgetc()`는 한 문자, `fgets()`는 한 줄/문자열, `fscanf()`는 형식에 맞춰 읽는다. Text와 Binary 모두 결국 byte지만 Text는 byte를 문자로 표현하고, Binary는 raw byte 데이터로 다룬다. 여러 byte 데이터는 Endianness 등의 차이가 있으므로 이식성이 필요하면 Serialization을 사용한다.**


## Ch.10 — `typedef`: Making New Types (2026.08.24)

### 핵심 개념

* `typedef`는 **새로운 타입을 만드는 것이 아니라 기존 타입에 새로운 이름(alias / 别名)을 붙인다.**

```c
typedef int age;

age x = 25;   // int x = 25;와 같음
```

* `struct`와 함께 사용하면 `struct`를 반복해서 쓰지 않아도 된다.

```c
typedef struct animal {
    char *name;
    int speed;
} animal;

struct animal a;  // 가능
animal b;         // 가능
```

* **익명 구조체(Anonymous Struct / 匿名结构体)**에도 이름을 붙일 수 있다.

```c
typedef struct {
    int x, y;
} point;

point p;          // 가능
// struct point p; // 불가능
```

`struct` 자체의 이름은 없고 `point`라는 `typedef` 이름만 있기 때문이다.

* 타입을 추상화하면 실제 타입을 나중에 쉽게 변경할 수 있다.

```c
typedef float app_float;
```

나중에:

```c
typedef long double app_float;
```

로 한 줄만 변경하면 `app_float`를 사용하는 모든 변수의 실제 타입이 바뀐다.

* 포인터 타입에도 사용할 수 있다.

```c
typedef int *intptr;

intptr p;   // int *p;
```

단, `*`가 보이지 않아 **포인터라는 사실이 숨겨질 수 있다.**

* 배열 타입에도 사용할 수 있다.

```c
typedef int five_ints[5];

five_ints a;     // int a[5];
five_ints b[5];  // int b[5][5];
```

### 새로 알게 된 사실

* `typedef`는 `struct`, 포인터, 배열 등에도 사용할 수 있다.
* 익명 `struct`를 `typedef`하면 `struct 이름`은 존재하지 않고 `typedef`로 만든 이름만 사용할 수 있다.
* 포인터나 배열 `typedef`는 편리하지만 실제 타입이 코드에서 잘 보이지 않을 수 있다.

### 기억할 것

> **`typedef` = 기존 타입에 별명 붙이기. `struct`를 간결하게 만들거나 타입을 추상화할 때 유용하며, 포인터와 배열에도 사용할 수 있다.**


## Ch.11 — Pointers II: Arithmetic (2026.08.24)

### 핵심 개념

* 포인터에 정수를 더하거나 빼면 **byte 단위가 아니라 포인터가 가리키는 타입의 크기만큼 이동**한다.

```c
int *p = a;

p + 1;   // 다음 int
p + 2;   // 2칸 뒤 int
```

즉 `int *`라면 내부적으로 `sizeof(int)`를 기준으로 이동한다.

* 포인터 변수 자체도 이동시킬 수 있다.

```c
p++;   // 다음 원소를 가리킴
p--;   // 이전 원소를 가리킴
```

* 같은 배열 내부의 두 포인터를 빼면 **byte 차이가 아니라 원소가 몇 칸 떨어져 있는지** 나온다.

```c
int *p = &a[1];
int *q = &a[4];

q - p;   // 3
```

서로 다른 배열의 포인터끼리 빼면 안 된다.

---

### 배열과 포인터

* 배열 접근과 포인터 연산은 다음 관계를 가진다.

```c
a[i] == *(a + i)
```

따라서 `p = a`라면:

```c
a[2]
p[2]
*(a + 2)
*(p + 2)
```

모두 같은 원소에 접근한다.

* 하지만 **배열 이름 자체와 포인터 변수는 다르다.**

```c
int a[5];
int *p = a;

p++;   // 가능
a++;   // 불가능
```

`a`는 대부분의 표현식에서 첫 번째 원소의 주소로 변환되지만, **수정 가능한 포인터 변수는 아니다.**

* 함수 매개변수에서는 다음 두 표현이 같은 의미로 처리된다.

```c
int func(char *s);
int func(char s[]);
```

---

### `void *`

* `void *`는 **주소는 알고 있지만 그 주소의 데이터 타입은 모르는 포인터**다.

```c
int x = 10;
void *p = &x;
```

타입을 모르기 때문에 직접 역참조할 수 없다.

```c
*p;   // 불가능
```

실제 타입의 포인터로 변환한 뒤 사용한다.

```c
int *q = p;

printf("%d\n", *q);   // 10
```

`void *`에서는 직접 다음 연산을 할 수 없다.

```c
*p
p++
p[i]
p->member
```

---

### `memcpy()`

* `memcpy()`는 데이터의 타입과 관계없이 **byte 단위로 메모리를 복사**한다.

```c
memcpy(destination, source, byte_count);
```

예:

```c
int a[] = {10, 20, 30};
int b[3];

memcpy(b, a, 3 * sizeof(int));
```

`memcpy()`는 원소 개수가 아니라 **복사할 byte 수**를 받으므로 `3 * sizeof(int)`를 사용한다.

`void *`를 사용하기 때문에 `int`, `char`, `float`, `struct` 등 다양한 타입의 메모리를 처리할 수 있다.

### 새로 알게 된 사실

* 포인터 산술은 가리키는 타입의 크기를 자동으로 고려한다.
* `q - p`는 주소의 byte 차이가 아니라 **원소 단위 거리**를 반환한다.
* 배열 이름은 포인터처럼 사용할 수 있지만 **포인터 변수 자체는 아니다.**
* `void *`는 타입 정보가 없기 때문에 실제 데이터를 사용하려면 적절한 포인터 타입으로 변환해야 한다.

### 기억할 것

> **`a[i] == *(a+i)`. 포인터 산술은 타입 크기 기준으로 이동하고, 배열 이름 자체는 변경할 수 없다. `void *`는 타입을 모르는 주소이므로 실제 타입의 포인터로 변환한 뒤 사용한다. `memcpy()`는 byte 단위로 메모리를 복사한다.**


## Ch.12 — Manual Memory Allocation (2026.08.26)

### 핵심 개념

* C에서는 Heap(힙 / 堆) 메모리를 직접 할당하고 해제할 수 있다.

```text
malloc / calloc → 메모리 할당
realloc         → 메모리 크기 변경
free            → 메모리 해제
```

* 직접 할당한 메모리를 해제하지 않으면 **Memory Leak(메모리 누수 / 内存泄漏)**이 발생할 수 있다.

---

### `malloc()` / `free()`

* `malloc()`은 지정한 **Byte 수만큼 메모리를 할당**하고 시작 주소를 반환한다.

```c
int *p = malloc(sizeof(int) * 10);
```

* `malloc()`으로 할당한 메모리는 초기화되지 않는다.

* 사용이 끝나면 `free()`한다.

```c
free(p);
```

* `free()` 후 해당 메모리를 다시 사용하면 **Undefined Behavior**이다.

```c
free(p);
*p = 10;   // Undefined Behavior
```

* `free(p)`가 `p`를 자동으로 `NULL`로 만드는 것은 아니다.

```c
free(p);
p = NULL;
```

---

### `calloc()`

* `calloc()`은 **원소 개수와 원소 크기**를 받아 메모리를 할당하고 0으로 초기화한다.

```c
int *p = calloc(10, sizeof(int));
```

```text
malloc → 초기화 X
calloc → 0으로 초기화
```

사용 후에는 똑같이:

```c
free(p);
```

---

### `realloc()`

* 기존에 할당된 메모리의 **크기를 변경**한다.

```c
int *new_p = realloc(p, sizeof(int) * 20);

if (new_p != NULL)
    p = new_p;
```

* 두 번째 인자는 **원소 개수가 아니라 Byte 수**이다.

* `realloc()`은 메모리를 새로운 위치로 옮길 수도 있으므로 반환 주소가 기존 주소와 다를 수 있다.

* 실패하면 `NULL`을 반환하지만 **기존 메모리는 그대로 유지된다.**

따라서 다음 방식이 안전하다.

```c
int *new_p = realloc(p, new_size);

if (new_p != NULL)
    p = new_p;
```

* `realloc(NULL, size)`는 새로운 메모리를 할당하는 용도로 사용할 수 있다.

---

### Memory Leak / Dangling Pointer

* 기존 메모리를 `free()`하지 않고 주소를 잃어버리면 **Memory Leak**이다.

```c
int *p = malloc(sizeof(int) * 10);

p = malloc(sizeof(int) * 20);  // 첫 번째 주소를 잃음
```

* 이미 해제된 메모리를 가리키는 포인터는 **Dangling Pointer(댕글링 포인터 / 悬空指针)**이다.

```c
int *q = p;

free(p);

// q는 해제된 메모리를 가리킴
```

---

### 동적 버퍼

* 필요한 크기를 모르면 작은 크기로 시작해서 `realloc()`으로 증가시킬 수 있다.

```c
bufsize *= 2;
char *new_buf = realloc(buf, bufsize);
```

크기를 2배씩 늘리면 `realloc()` 호출 횟수를 줄일 수 있다.

```c
while ((line = readline(fp)) != NULL) {
    printf("%s\n", line);
    free(line);
}
```

실행 순서:

```text
readline(fp)
→ line에 대입
→ line != NULL 비교
→ 참이면 while 실행
```

---

### 새로 알게 된 사실

* `malloc()` → 메모리 할당, 초기화 X
* `calloc()` → 메모리 할당 + 0 초기화
* `free()` → 메모리 해제, **포인터 자체가 `NULL`이 되는 것은 아님**
* `realloc()` → 기존 메모리 크기 변경, 주소가 바뀔 수 있음. 확장할 공간이 충분하면 그대로 사용하고, 부족하면 새 위치로 이동
* `realloc()` 실패 → NULL 반환 → 기존 메모리는 유지됨
* 할당된 메모리 주소를 잃으면 **Memory Leak**
* 해제된 메모리를 가리키면 **Dangling Pointer**

### 기억할 것
기
> **`malloc/calloc`으로 할당 → 필요하면 `realloc` → 마지막에 `free`. 동적 메모리에서는 "현재 어떤 포인터가 어떤 메모리 블록을 가리키는가"를 추적하는 것이 핵심이다.** 