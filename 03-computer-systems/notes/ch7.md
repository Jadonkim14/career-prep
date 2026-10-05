# Chapter 7 — Linking

### 1. Linking

- **Linking(链接)**: 여러 **Relocatable Object File(`.o`)** 을 결합하여 하나의 Executable을 만드는 과정.
- 각 `.c` 파일은 별도로 컴파일될 수 있음.
- 여러 소스 파일을 나누어 개발할 수 있어 **Modularity(模块化)** 가 좋아짐.
- 수정된 소스 파일만 다시 컴파일한 뒤 재링크할 수 있어 빌드 시간이 줄어듦. 12-linking-4up 12-linking-4up

### 2. Symbol Resolution & Relocation

- **Symbol Resolution(符号解析)**: Symbol Reference를 실제 Symbol Definition과 연결하는 과정.
- 함수나 전역 변수 등이 Linker가 처리하는 주요 Symbol.
- **Relocation(重定位)**: 각 코드와 데이터를 최종 위치에 배치하고 Symbol Reference를 실제 주소에 맞게 수정하는 과정.
- 핵심:
  - Symbol Resolution → **누구인가?**
  - Relocation → **어디에 있는가?** 12-linking-4up

### 3. Object File

- **Relocatable Object File (`.o`)**: 다른 Object File과 결합할 수 있는 파일.
- **Executable Object File**: 메모리에 Load하여 실행할 수 있는 파일.
- **Shared Object File (`.so`)**: Load Time 또는 Run Time에 Dynamic Linking할 수 있는 Object File.
- Linux에서는 이들을 주로 **ELF(Executable and Linkable Format)** 형식으로 표현. 12-linking-4up

### 4. ELF Sections

- **`.text`**: Machine Code.
- **`.rodata`**: Read-only Data.
- **`.data`**: 초기화된 Global Variable.
- **`.bss`**: 초기화되지 않은 Global Variable.
- **`.symtab`**: Symbol Table.
- **`.rel.text`**: `.text` 영역의 Relocation 정보.
- **`.rel.data`**: `.data` 영역의 Relocation 정보.
- **`.debug`**: Debugging 정보. 12-linking-4up 12-linking-4up

### 5. Linker Symbols

- **Global Symbol(全局符号)**: 현재 Module에서 정의되고 다른 Module에서도 참조할 수 있는 Symbol.
- 대표적으로 non-static Function과 Global Variable.
- **External Symbol(外部符号)**: 현재 Module에서 사용하지만 다른 Module에서 정의된 Symbol.
- **Local Linker Symbol(局部链接符号)**: 현재 Module 내부에서만 사용하는 Symbol.
- 대표적으로 `static` Function과 Variable.
- 함수 내부의 일반 Local Variable은 Local Linker Symbol과 다름. 12-linking-4up

### 6. Strong & Weak Symbols

- **Strong Symbol(强符号)**: Function, 초기화된 Global Variable.
- **Weak Symbol(弱符号)**: 초기화되지 않은 Global Variable.
- Strong Symbol이 여러 개 존재하면 Link Error.
- Strong + Weak가 존재하면 Strong Symbol 선택.
- Weak Symbol만 여러 개 존재하면 하나가 선택될 수 있음. 12-linking-4up

### 7. Header & Global Variables

- `#include`는 Linker가 처리하는 것이 아니라 **Preprocessor(预处理器)** 가 Header File의 내용을 Source에 삽입하는 과정.
- Global Variable은 가능하면 피하는 것이 좋음.
- 현재 파일 내부에서만 사용하면 `static` 사용.
- 다른 Module에 정의된 Global Variable을 사용하면 `extern` 사용. 12-linking-4up 12-linking-4up

### 8. Static Library

- **Static Library(静态库)**: 여러 Relocatable Object File을 하나의 Archive로 묶은 파일.
- Linux에서는 보통 `.a` 확장자 사용.
- `ar` 명령으로 여러 `.o` 파일을 하나의 Static Library로 만들 수 있음.
- Linker는 Library 전체를 무조건 넣는 것이 아니라 **현재 unresolved reference를 해결하는 데 필요한 `.o`만 선택하여 Link**. 12-linking-4up

### 9. Library Link Order

- Linker는 `.o`와 `.a`를 **Command Line의 왼쪽 → 오른쪽 순서**로 Scan.
- Scan하면서 **Unresolved External Reference** 목록을 관리.
- Library를 너무 먼저 읽으면 아직 필요한 Symbol이 없기 때문에 그냥 지나갈 수 있음.
- 일반적으로 **Object File을 먼저, Library를 뒤에 배치**.

```bash
gcc libtest.o -lmine   # 성공 가능
gcc -lmine libtest.o   # undefined reference 가능
```

12-linking-4up

### 10. Shared Library & Dynamic Linking

- **Shared Library(共享库)**: 여러 프로그램이 공유할 수 있는 Library.
- Linux에서는 `.so` 확장자 사용.
- Static Library와 달리 Library Code를 Executable마다 복사하는 중복을 줄일 수 있음.
- **Load-Time Linking**: 프로그램을 Load할 때 Dynamic Linker가 Library를 연결.
- **Run-Time Linking**: 프로그램 실행 중 필요한 Library를 동적으로 Load. 12-linking-4up 12-linking-4up

### 11. Run-Time Dynamic Linking

- `dlopen()`: 실행 중 Shared Library를 Load.
- `dlsym()`: Library에서 원하는 Symbol의 주소를 찾음.
- `dlclose()`: Shared Library 사용 종료.
- 프로그램이 실행된 이후 Library와 Function을 연결할 수 있음. 12-linking-4up

### 12. Library Interposition

- **Library Interposition(函数拦截)**: 원래 호출하려던 Library Function 대신 Wrapper Function을 먼저 실행하도록 만드는 기법.
- Monitoring, Profiling, `malloc` tracing, Memory Leak 분석 등에 활용 가능. 12-linking-4up
- **Compile-Time Interposition**: Macro / Preprocessor를 이용해 함수 호출을 Wrapper로 변경.
- **Link-Time Interposition**: Linker의 `--wrap`을 이용해 Symbol Resolution을 변경.
- **Load/Run-Time Interposition**: `LD_PRELOAD`로 내가 만든 Shared Library를 우선 Load하고 `dlsym(RTLD_NEXT, ...)`로 실제 함수를 찾음. 12-linking-4up 12-linking-4up

### 핵심 정리

- Linker의 핵심 역할은 **Symbol Resolution + Relocation**.
- `.o`, Executable, `.so`는 Linux에서 주로 **ELF** 형식을 사용.
- Static Library `.a`는 Link Time에 필요한 Object Code를 Executable에 포함.
- Shared Library `.so`는 Load Time 또는 Run Time에 Dynamic Linking.
- `undefined reference`는 Linker가 필요한 Symbol Definition을 찾지 못했을 때 발생할 수 있음.
- Library Interposition을 이용하면 기존 함수 호출을 Wrapper를 통해 가로채 Monitoring과 Profiling 등에 활용할 수 있음.