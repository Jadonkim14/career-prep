# cache-lab-review

## Part A — Cache Simulator

### 1. 전체 구현 정리

**목표:** Memory Trace를 읽어 Cache의 Hit, Miss, Eviction 횟수를 계산하는 시뮬레이터 구현.

**① Cache 구조 정의**

```c
typedef struct {
    int valid;
    unsigned long long tag;
    unsigned long long last_used;
} CacheLine;
```

- `valid`: 해당 Cache Line에 유효한 데이터가 있는지 표시
- `tag`: 메모리 블록 식별
- `last_used`: LRU 교체 정책에 사용할 최근 접근 시점

**② Cache 초기화 — `initCache()`**

```c
int S = 1 << s;

cache = calloc(S, sizeof(CacheLine *));

for (int i = 0; i < S; i++) {
    cache[i] = calloc(E, sizeof(CacheLine));
}
```

- `S = 2^s`: 전체 Set 개수
- Set별로 `E`개의 Cache Line 할당
- `calloc()`으로 모든 Cache Line을 0으로 초기화

**③ Cache 접근 — `accessCache()`**

```c
set_index = (address >> b) & ((1ULL << s) - 1);
tag = address >> (s + b);
```

접근 처리 순서:

1. 주소에서 Set Index와 Tag 추출
2. 해당 Set에서 Valid Bit와 Tag를 비교하여 Hit 검사
3. Hit라면 `hits++` 후 최근 접근 시점 갱신
4. Miss라면 `misses++` 후 비어 있는 Cache Line 탐색
5. 비어 있는 Line이 없다면 `evictions++` 후 LRU 기준으로 교체

**④ Trace 처리 — `main()`**

1. `getopt()`로 `-s`, `-E`, `-b`, `-t`, `-v` 옵션 파싱
2. 입력값 검증 및 Cache 초기화
3. `fopen()`으로 Trace 파일 열기
4. `fgets()`와 `sscanf()`로 메모리 접근 기록 읽기
5. Operation에 따라 `accessCache()` 호출
6. `printSummary()`로 결과 출력
7. 파일 닫기 및 동적 메모리 해제

| Operation | 처리 |
|---|---|
| `I` | 무시 |
| `L` | Cache 접근 1회 |
| `S` | Cache 접근 1회 |
| `M` | Cache 접근 2회 |

**⑤ 메모리 해제 — `freeCache()`**

```c
for (int i = 0; i < S; i++) {
    free(cache[i]);
}

free(cache);
```

각 Set의 메모리를 먼저 해제한 후 Set 포인터 배열을 해제.

### 2. 테스트 결과

```text
./test-csim

TEST_CSIM_RESULTS=27
```

- 총 8개 테스트 통과
- Reference Simulator와 Hit, Miss, Eviction 결과 모두 일치
- Part A 기본 기능 검증 완료

---

### 3. 질문 및 헷갈렸던 내용 정리

**① 주소에서 Set Index와 Tag 추출**

```c
set_index = (address >> b) & ((1ULL << s) - 1);
tag = address >> (s + b);
```

- `address >> b`: Block Offset 제거
- `((1ULL << s) - 1)`: 하위 `s`비트가 1인 Mask 생성
- `&`: 필요한 Set Index 비트만 추출
- `address >> (s + b)`: Set Index와 Block Offset을 제거하고 Tag 추출
- `1ULL`: `unsigned long long` 타입의 정수 1

**② `calloc()` 문법**

```c
void *calloc(size_t nmemb, size_t size);
```

- `nmemb`: 할당할 요소의 개수
- `size`: 각 요소의 크기(Byte)
- 총 `nmemb * size` Byte의 메모리를 할당하고 모든 Byte를 0으로 초기화
- 성공 시 할당된 메모리의 포인터, 실패 시 `NULL` 반환

```c
cache = calloc(S, sizeof(CacheLine *));
cache[i] = calloc(E, sizeof(CacheLine));
```

첫 번째는 Set 포인터 `S`개를 저장할 공간, 두 번째는 Cache Line `E`개를 저장할 공간을 할당.

`malloc()`과 달리 `calloc()`은 할당된 메모리를 0으로 초기화하므로 초기 `valid` 값을 별도로 0으로 설정할 필요가 없음.

**③ 이중 포인터와 동적 메모리**

```c
CacheLine **cache;
```

- `cache`: Set 포인터 배열을 가리키는 포인터
- `cache[i]`: i번째 Set의 첫 번째 Cache Line을 가리키는 포인터
- `cache[i][j]`: i번째 Set의 j번째 Cache Line
- 각 Set을 별도로 할당했으므로 해제할 때도 각 Set을 먼저 `free()`해야 함

**④ `getopt()`**

```c
getopt(argc, argv, "s:E:b:t:v")
```

- `:`가 붙은 옵션은 인자가 필요함. 예: `-s 5`
- `-v`는 인자가 없는 옵션
- `optarg`: 옵션에 전달된 인자
- `optopt`: 인식하지 못한 옵션 문자 또는 인자가 누락된 옵션 문자
- `getopt()`는 처리할 옵션이 없으면 `-1` 반환
- 이번 `-std=c99` 컴파일 환경에서는 `<getopt.h>`를 추가해 선언 오류 해결

**⑤ `sscanf()`**

```c
sscanf(line, " %c %llx,%d",
       &operation, &address, &size);
```

- `%c`: Operation 문자
- `%llx`: 16진수 주소를 `unsigned long long`으로 읽음
- `%d`: 접근 크기를 정수로 읽음
- `%c` 앞의 공백: 선행 공백과 줄바꿈 등을 건너뜀
- 반환값이 `3`이면 세 항목을 정상적으로 읽은 것

**⑥ `switch`, `break`, `return`**

```c
case 'L':
case 'S':
    accessCache(address);
    break;
```

- `L`과 `S`는 동일한 동작을 수행하므로 Case를 공유
- `break`: 가장 가까운 `switch` 또는 반복문 종료
- `return 0`: `main()` 정상 종료
- `return 1`: `main()` 오류 상태로 종료
- `default`에서 `break`를 사용하면 현재 `switch`만 종료하고 다음 Trace를 처리할 수 있음
- 이번 구현에서는 알 수 없는 Operation을 오류로 처리하고 자원 정리 후 `return 1`을 사용하기로 결정

**⑦ `fopen()`과 `fclose()`**

- `fopen()` 성공 시 파일 스트림 포인터 반환
- 실패 시 `NULL` 반환
- `fclose(NULL)`은 Undefined Behavior이므로 호출하면 안 됨
- 파일 열기에 실패해도 이미 `calloc()`으로 할당한 Cache 메모리는 해제해야 함

**⑧ 컴파일 옵션과 미사용 변수**

```text
-Wall    : 여러 컴파일 경고 활성화
-Werror  : 경고를 오류로 취급
-std=c99 : C99 언어 표준 사용
```

`verbose` 변수를 설정만 하고 읽지 않아 컴파일 오류 발생.

```c
(void)verbose;
```

미사용 변수 경고를 피하기 위한 임시 처리이며, `-v` 상세 출력 기능을 구현한 것은 아님.

**⑨ 실행 권한 문제**

```bash
chmod +x ./test-csim
chmod +x ./csim-ref
```

- `Permission denied`: 실행 권한이 없어 프로그램을 실행하지 못한 상황
- `chmod +x`: 파일에 실행 권한 추가
- `csim-ref`: 작성한 시뮬레이터와 결과를 비교하는 Reference Simulator

## Part B — Matrix Transpose

### 구현 내용

Cache Miss를 최소화하기 위해 행렬 크기에 따라 서로 다른 전치 알고리즘을 구현했다.

- **32×32:** 8×8 Blocking과 지역 변수 8개를 사용했다. A의 연속된 원소를 먼저 읽은 후 B에 기록하여 공간 지역성을 활용했다.
- **64×64:** 8×8 블록을 네 개의 4×4 영역으로 나눴다. B의 일부 공간에 데이터를 임시 저장한 후 최종 위치로 옮기는 방식으로 Cache Set 충돌을 줄였다.
- **61×67:** 경계 검사를 포함한 Blocking을 구현했다. 블록 크기별 성능을 비교한 결과, 실험한 크기 중 16×16에서 가장 적은 Cache Miss가 발생했다.

### 최종 결과

| 행렬 크기 | 기본 전치 Miss | 최적화 Miss | 감소율 |
|---|---:|---:|---:|
| 32×32 | 1,184 | 288 | 75.7% |
| 64×64 | 4,724 | 1,668 | 64.7% |
| 61×67 | 4,424 | 1,993 | 54.9% |

세 가지 행렬 모두 `correctness=1`을 통과했다.

### 핵심 학습

- **Blocking:** 행렬을 작은 블록으로 나누어 Cache 지역성을 개선한다.
- **Spatial Locality:** 연속된 메모리 주소에 접근하면 하나의 Cache Block을 효율적으로 활용할 수 있다.
- **Conflict Miss:** 서로 다른 메모리 블록이 같은 Cache Set에 매핑되면 반복적으로 축출될 수 있다.
- **Access Pattern:** 블록 크기가 같아도 데이터의 접근 순서에 따라 Cache Miss가 크게 달라질 수 있다.
- **Boundary Handling:** 행렬 크기가 블록 크기의 배수가 아니라면 배열 경계를 검사해야 한다.

---
