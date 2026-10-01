# Chapter 5. Optimizing Program Performance

## 1. Overview

같은 Big-O라도 실제 실행 성능은 다를 수 있다.

성능에 영향을 주는 요소:

- Algorithm
- Data Representation
- Procedure
- Loop
- Compiler
- CPU / Memory Hierarchy 10-advanced-opt

핵심:

```text
Algorithm Complexity
≠
Actual Runtime Performance
```

---

## 2. Compiler Optimization

GCC 최적화 옵션:

```text
-O0 → 최적화 거의 없음
-O1 → 기본 최적화
-O2 → 일반적인 강한 최적화
-O3 → 더 공격적인 최적화
```

Compiler가 잘하는 것:

```text
Register Allocation
Instruction Scheduling
Dead Code Elimination
Minor Optimization
```

하지만 다음은 한계가 있다.

```text
Algorithm 자체 개선
Procedure Side Effect
Memory Aliasing
Runtime Information
```

Compiler는 **program behavior를 바꾸면 안 되므로**, 확신이 없으면 보수적으로 최적화한다. 10-advanced-opt

---

## 3. Removing Procedure Calls

Loop 안의 불필요한 함수 호출과 bound check는 큰 overhead가 될 수 있다.

```c
for (i = 0; i < n; i++) {
    get_vec_element(v, i, &val);
    *res += val;
}
```

개선:

```c
double *data = get_vec_start(v);

for (i = 0; i < n; i++)
    *res += data[i];
```

특히 **innermost loop**의 반복적인 overhead가 중요하다. 10-advanced-opt

---

## 4. Code Motion

Loop 동안 변하지 않는 계산을 밖으로 이동한다.

```c
for (j = 0; j < n; j++)
    a[n * i + j] = b[j];
```

↓

```c
long ni = n * i;

for (j = 0; j < n; j++)
    a[ni + j] = b[j];
```

즉:

```text
Repeated Computation
→ Compute Once
```

10-advanced-opt

---

## 5. Strength Reduction

비싼 연산을 더 단순한 연산으로 바꾼다.

```text
16 * x
→ x << 4
```

또는:

```text
Multiplication
→ Addition
```

10-advanced-opt

---

## 6. Common Subexpression

같은 계산을 여러 번 하지 않고 결과를 재사용한다.

```c
int inj = i * n + j;

up    = val[inj - n];
down  = val[inj + n];
left  = val[inj - 1];
right = val[inj + 1];
```

핵심:

```text
Same Expression
→ Compute Once
→ Reuse
```

10-advanced-opt

---

## 7. Procedure Call as Optimization Blocker

문제:

```c
for (i = 0; i < strlen(s); i++)
```

`strlen()`이 \(O(n)\)이고 loop도 \(n\)번이므로 전체가:

\[
O(n^2)
\]

이 될 수 있다. 10-advanced-opt

개선:

```c
int len = strlen(s);

for (i = 0; i < len; i++)
```

그러면 전체는 다시 대략:

\[
O(n)
\]

이 된다. 10-advanced-opt

Compiler가 자동으로 못 옮길 수 있는 이유:

```text
Procedure Side Effect 가능성
Global State 의존성
Memory 변경 가능성
```

즉 compiler가 안전하다고 확신하지 못하면 최적화를 하지 않는다. 10-advanced-opt

---

## 핵심 정리

```text
Program Optimization
├─ 불필요한 함수 호출 제거
├─ Code Motion
├─ Strength Reduction
└─ Common Subexpression
```

> **핵심은 프로그램의 의미는 유지하면서 불필요한 작업을 줄이는 것