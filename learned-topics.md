# 1. MIT Missing Semester

## A. The Shell

* Shell / Bash / Shebang
* PATH / 환경 변수
* 기본 파일·텍스트 명령어
* Glob / 와일드카드
* 따옴표
* stdin / stdout / stderr / Redirect
* 종료 상태와 조건 실행
* Bash 조건문
* Shell Builtin
* `set -x` / Strict Mode
* Command Substitution
* Script Arguments
* Pipe
* Regex 기초
* `xargs`
* `curl`
* `jq`
* `awk`
* Shell Pipeline
* 프로젝트 적용 및 트러블슈팅

## B. Command-line Environment

* Signals / Job Control
* Process 관리
* `tmux`
* Alias / Dotfiles
* SSH
* SSH Key / Config / 보안
* SSH Port Forwarding
* Mosh / SSH + tmux
* Process / Port / HTTP / Log 기반 장애 분석

## C. Development Environment and Tools

* Development Environment
* Vim
* Language Server / LSP
* Python Environment
* AI-powered Development
* IDE Extensions

## D. Debugging and Profiling

* Debugging 기본 흐름
* GDB / Debugger
* AddressSanitizer
* `strace`
* File Descriptor
* Profiling
* `perf` / Flame Graph / `hyperfine`
* Resource Monitoring
* Logging
* 문제 재현 → 원인 추적 → 검증

## E. Version Control and Git

* Git Data Model
* Blob / Tree / Commit
* Objects / References / HEAD
* Staging Area
* Branch / Merge
* Merge Conflict
* Rebase
* Remote / Fetch / Pull / Push
* Stash / Revert / `.gitignore`
* Git History 조사 및 수정
* GitHub / Pull Request
* Git Workflow

## F. Packaging, Dependencies & Containers

* Dependencies / Environments
* Packaging / Artifact
* Versioning / Lockfile / Reproducibility
* Docker
* Dockerfile / Image / Container
* Configuration / Environment Variables
* Docker Compose
* Registry / Publishing
* Build → Artifact → Deploy

## G. Agentic Coding

* Coding Agent
* Tool Use
* Feedback Loop
* Feature / Fix / Refactoring / Review 활용
* Model / Context
* Context Management
* `AGENTS.md`
* Skills / Subagents
* MCP
* Parallel Agents
* Privacy / Security
* AI Coding 검증
* Sandbox / 격리 환경

## H. Beyond the Code

* 좋은 Engineer의 기본 원칙
* Comment
* README
* Commit
* Bug Report
* Minimal Reproducible Example
* Pull Request
* Code Review
* 좋은 질문 방법
* AI 사용 원칙
* Why 기록하기

## I. Code Quality

* 코드 품질 자동화
* Formatter
* Linter / Static Analysis
* Compiler Warning
* Testing
* Code Coverage
* Pre-commit
* Continuous Integration
* Regex
* Semgrep
* Parser vs Regex
* Format → Analyze → Test → CI

---

# 2. Phase 1 — C Basic

## Ch.2 — Hello, World

* C Program Structure
* `main()`
* Preprocessor
* Header File
* `printf()`
* Compilation
* Executable

## Ch.3 — Variables and Statements

* Variables
* Types
* Initialization
* Boolean / Truth Value
* Operators
* Ternary Operator
* Prefix / Postfix Increment
* `sizeof`
* Flow Control
* `if / else`
* `while`
* `do-while`
* `for`
* `switch`
* Fall Through
* Operator Precedence
* `size_t`
* `char`

## Ch.4 — Functions

* Function
* Return Type
* Parameter
* Argument
* `void`
* Pass by Value
* Function Prototype
* Function Declaration
* Function Definition

## Ch.5 — Pointers

* Pointer
* Address
* Address-of Operator `&`
* Dereference `*`
* Pointer Passing
* `NULL`
* Pointer Declaration

## Ch.6 — Arrays

* Array
* Contiguous Memory
* Array Index
* `sizeof` Array
* Array Initialization
* Array Bounds
* Undefined Behavior
* Multidimensional Array
* Row-major Order
* Array-to-Pointer Conversion
* Array Parameter
* Array Length
* Pointer vs Array

## Ch.7 — Strings

* C String
* NUL Character
* String Literal
* `char` Array
* String Pointer
* `strlen()`
* String Copy
* `strcpy()`
* Pointer Assignment

## Ch.8 — Structs

* `struct`
* Structure Member
* Structure Initialization
* Designated Initializer
* Structure Passing
* Structure Pointer
* `.` Operator
* `->` Operator
* Structure Assignment
* Shallow Copy

## Ch.9 — File Input/Output

* `FILE *`
* File Stream
* Standard Streams
* `stdin`
* `stdout`
* `stderr`
* `fopen()`
* `fclose()`
* `fgetc()`
* `fgets()`
* `fscanf()`
* Text File
* Binary File
* `fread()`
* `fwrite()`
* Byte Representation
* Endianness
* Big Endian
* Little Endian
* Serialization

## Ch.10 — `typedef`

* `typedef`
* Type Alias
* Anonymous Struct
* Type Abstraction
* Pointer Typedef
* Array Typedef

## Ch.11 — Pointers II: Arithmetic

* Pointer Arithmetic
* Pointer Increment / Decrement
* Array and Pointer Relationship
* Pointer Comparison
* `void *`
* `memcpy()`
* Byte-wise Memory Access
* Pointer Casting

## Ch.12 — Manual Memory Allocation

* Dynamic Memory Allocation
* `malloc()`
* `free()`
* `calloc()`
* `realloc()`
* Memory Leak
* Dangling Pointer
* Use-After-Free
* Double Free
* Dynamic Buffer
* Heap Memory
* Ownership

---

# 3. Phase 2 — C Intermediate

## Ch.13 — Scope

* Scope
* Block Scope
* Variable Hiding
* File Scope
* `for`-loop Scope

## Ch.14 — Types II: Way More Types!

* Numeric Types
* Signed / Unsigned
* `char`
* Integer Types
* Floating-Point Types
* Integer Constants
* Floating-Point Constants
* Integer Suffix
* Hexadecimal
* Octal
* Type Range
* Precision

## Ch.15 — Types III: Conversions

* Type Conversion
* String ↔ Number
* `char` ↔ Number
* Numeric Conversion
* Integer Promotion
* Implicit Conversion
* Explicit Conversion
* Casting
* `void *`
* `atoi()`
* `atof()`
* `strtol()`
* `strtoul()`
* `snprintf()`

## Ch.16 — Types IV: Qualifiers and Specifiers

* Type Qualifier
* Storage-Class Specifier
* `const`
* `volatile`
* `restrict`
* `static`
* `extern`
* Scope vs Lifetime
* Linker
* `auto`
* `register`

## Ch.17 — Multifile Projects

* Multifile Project
* `.h` / `.c`
* Header File
* Source File
* `#include`
* Include Guard
* `static` / `extern`
* Object File
* Compilation
* Linking
* Linker
* `gcc -c`
* `gcc -o`
* Incremental Build

---

# 4. Phase 3 — C Advanced

## Ch.18 — The Outside Environment

* Command Line Arguments
* `argc`
* `argv`
* `argv[0]`
* `argv[argc]`
* Pointer Arithmetic with `argv`
* Exit Status
* `getenv()`
* Environment Variables
* `environ`

## Ch.19 — C Preprocessor

* Preprocessor
* `#include`
* `#define`
* Function-like Macro
* Conditional Compilation
* `#ifdef`
* `#ifndef`
* `#if`
* `#else`
* `#elif`
* Include Guard
* `__FILE__`
* `__LINE__`
* `__func__`
* Stringification `#`
* Token Pasting `##`

## Ch.20 — Structs II

* Nested Struct
* Struct Array
* Self-Referential Struct
* Flexible Array Member
* Padding
* `offsetof`
* Bit-field
* `union`

## Ch.21 — Characters and Strings II

* Escape Sequence
* `\n`
* `\t`
* `\\`
* `\"`
* `\'`
* `\r`
* `\b`
* `fflush()`
* Hexadecimal Escape
* Octal Escape
* Unicode Escape

## Ch.22 — Enumerated Types

* `enum`
* Enumeration Constant
* Enumeration Type
* Explicit Enum Values
* `typedef enum`
* Enum Scope

## Ch.23 — Pointers III

* Pointer to Pointer
* `int **`
* `unsigned char *`
* Byte-wise Memory Access
* Object Representation
* `NULL` vs `'\0'`
* Pointer ↔ Integer
* `uintptr_t`
* Strict Aliasing
* Pointer Difference
* `ptrdiff_t`
* Function Pointer
* Callback

## Ch.24 — Bitwise Operations

* Bitwise Operation
* AND
* OR
* XOR
* NOT
* Bitwise Assignment
* Left Shift
* Right Shift
* Shift Undefined Behavior

## Ch.25 — Variadic Functions

* Variadic Function
* `stdarg.h`
* `va_list`
* `va_start()`
* `va_arg()`
* `va_end()`
* `va_copy()`
* `printf()`
* `vprintf()`
* Variadic Arguments

## Ch.26 — Locale and Internationalization

* Locale
* Internationalization
* `setlocale()`
* `localeconv()`
* Locale Category
* `LC_ALL`
* `LC_NUMERIC`
* `LC_MONETARY`

## Ch.27 — Unicode, Wide Characters, and All That

* Unicode
* Code Point
* Encoding
* UTF-8
* C Unicode Escape
* Multibyte Character
* `strlen()` Byte Count
* Wide Character
* `wchar_t`
* Wide String
* `mbtowc()`
* `wctomb()`
* `mbstowcs()`
* `wcstombs()`

## Ch.28 — Exiting a Program

* `return`
* `exit()`
* `atexit()`
* `quick_exit()`
* `at_quick_exit()`
* `_Exit()`
* `assert()`
* `abort()`
* Exit Handler
* Cleanup

## Ch.29 — Signal Handling

* Signal
* Signal Handler
* `SIGINT`
* `SIGTERM`
* `SIGSEGV`
* `SIGABRT`
* `signal()`
* `SIG_IGN`
* `SIG_DFL`
* `sig_atomic_t`
* `sigaction()`
* Async Signal Safety

## Ch.30 — Variable-Length Arrays

* Variable-Length Array
* VLA
* Runtime Array Size
* VLA `sizeof`
* Multidimensional VLA
* VLA and Function Parameters
* VLA and Stack
* VLA Stack Overflow
* Linux Kernel VLA

## Ch.31 — `goto`

* `goto`
* Label
* Function Scope
* Error Handling
* Cleanup
* Resource Cleanup
* Reverse-order Cleanup

## Ch.32 — Compound Literals & Generic Selections

* Compound Literal
* Anonymous Object
* Compound Literal Scope
* Compound Literal Lifetime
* `_Generic`
* Generic Selection

## Ch.33 — Arrays Part II

* Array Parameter
* Type Qualifier
* `static` Array Parameter
* Multidimensional Array Initialization

## Ch.34 — `setjmp` / `longjmp`

* `setjmp()`
* `longjmp()`
* Non-local Jump
* `setjmp()` Return Value
* `volatile`
* Automatic Variable
* Register Variable
* Control Flow Recovery

## Ch.35 — Incomplete Types

* Incomplete Type
* Forward Declaration
* Self-Referential Struct
* `extern` Array
* Complete Type
* `void`
* Incomplete Array Type

## Ch.36 — Complex Numbers

* Complex Number
* Complex Type
* `complex`
* `I`
* `CMPLX()`
* `creal()`
* `cimag()`
* Complex Arithmetic
* Complex Math

## Ch.37 — Fixed Width Integer Types

* Fixed-width Integer
* `int8_t`
* `int16_t`
* `int32_t`
* `int64_t`
* `uint8_t`
* `uint16_t`
* `uint32_t`
* `uint64_t`
* `intmax_t`
* Fixed-width Constants
* Integer Limits
* Format Specifier
* `PRIu32`
* `PRId64`

## Ch.38 — Date and Time Functionality

* `time_t`
* `struct tm`
* Calendar Time
* Time Conversion
* `localtime()`
* `gmtime()`
* `mktime()`
* `strftime()`
* `timespec`
* Nanosecond Precision
* Time Difference

## Ch.39 — Multithreading

* Thread
* Multithreading
* Thread Creation
* Thread Execution
* Thread Join
* Shared Data
* Race Condition
* Mutex
* Critical Section
* Thread Synchronization

## Ch.40 — Atomics

* Atomic Operation
* `atomic_int`
* Atomicity
* Atomic RMW
* Synchronization
* Memory Visibility
* Acquire
* Release
* Sequential Consistency
* Memory Order
* `memory_order_seq_cst`
* Mutex vs Atomic
* Race Condition

## Ch.41 — Function Specifiers & Alignment

* `noreturn`
* `_Noreturn`
* Alignment
* `alignas`
* `_Alignas`
* `alignof`
* `_Alignof`
* `max_align_t`
* `memalignment()`

## logicalneg

* `x | (-x)`를 이용하면 `x != 0`일 때 최상위 비트(MSB)가 `1`이 된다.
* `x | (-x)`를 `>> 31`하면 `x == 0`일 때 `0`, `x != 0`일 때 `0xFFFFFFFF`가 된다. (0x1 아님!)

## howmanybits

* 2의 보수(two's complement)에서 `x`를 표현하는 데 필요한 최소 비트 수를 구한다.
* 양수와 음수를 같은 방식으로 처리하기 위해 음수이면 `~x`로 변환한다.
* `sign = x >> 31`을 이용하면 양수는 `0`, 음수는 `0xFFFFFFFF`가 된다.
* `x ^ sign`을 사용하면 양수는 그대로, 음수는 `~x`가 된다.
* 가장 높은 `1`의 위치를 **16 → 8 → 4 → 2 → 1** 순서로 범위를 절반씩 줄이며 찾는다.
* `!!(x >> n)`은 `n`비트보다 높은 영역에 `1`이 있는지 `0/1`로 확인한다.
* `b16`, `b8`, `b4`, `b2`, `b1`은 각 범위에서 찾은 비트 수를 나타낸다.
* `x >>= bN`을 통해 이미 확인한 상위 영역을 제거하고 다음 범위를 탐색한다.
* 마지막 `x`는 binary search 이후 남은 마지막 `1`비트를 나타낸다.
* 마지막 `+1`은 sign bit(부호 비트)를 포함하기 위한 것이다.