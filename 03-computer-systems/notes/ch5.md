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

## 8. Memory Aliasing

\*\*Memory Aliasing(内存别名)\*\*은 서로 다른 포인터가 같은 메모리를 참조하는 현상이다.

```
*a = 10;
*b = 20;
```

`a`와 `b`가 같은 주소를 가리키면 `*a`의 최종 값은 `20`이다.

따라서 Compiler는 포인터 간 메모리 의존성을 확신할 수 없으면 최적화를 제한한다.

Scalar Replacement: 반복적인 메모리 접근을 지역 변수로 대체하는 기법. 단, Aliasing으로 인해 프로그램 결과가 달라질 수 있으므로 주의해야 한다.

## 9. Matrix Blocking

\*\*Matrix Blocking(矩阵分块)\*\*은 행렬을 작은 블록으로 나누어 처리하는 최적화 기법이다.

```
Matrix Blocking
→ Data Reuse 증가
→ Cache Locality 개선
→ Cache Miss 감소 가능
```

핵심은 같은 데이터를 캐시에 남아 있는 동안 최대한 재사용하는 것이다.

## 10. Loop Unrolling

\*\*Loop Unrolling(循环展开)\*\*은 반복문 본문을 펼쳐 반복 제어 비용을 줄이는 기법이다.

```
// Original
for (int i = 0; i < 8; i++)
    sum += a[i];

// 2-way Unrolling
for (int i = 0; i < 8; i += 2) {
    sum += a[i];
    sum += a[i + 1];
}
```

단, Loop Unrolling만으로는 \*\*Data Dependency(数据依赖)\*\*가 제거되지 않는다.

## 11. Instruction-Level Parallelism (ILP)

\*\*ILP(指令级并行)\*\*는 CPU가 여러 명령어를 동시에 또는 겹쳐 실행하는 능력이다.

- Superscalar(超标量): 한 Cycle에 여러 명령어 처리 가능
- Out-of-Order Execution(乱序执行): 데이터 의존성이 없다면 순서를 바꿔 실행 가능
- Latency(延迟): 연산 결과가 나오기까지 걸리는 시간
- Throughput(吞吐量): 단위 시간당 처리량

핵심: Data Dependency를 줄이면 CPU의 병렬 실행 능력을 더 잘 활용할 수 있다.

## 12. CPE & Multiple Accumulators

\*\*CPE(Cycles Per Element, 每元素时钟周期数)\*\*는 원소 하나를 처리하는 데 필요한 평균 CPU Cycle 수이다.

T(n) = CPE * n + Overhead

기존 코드:

```
sum += a[0];
sum += a[1];
```

이전 덧셈의 결과가 다음 덧셈에 필요하므로 의존성이 발생한다.

\*\*Multiple Accumulators(多累加器)\*\*를 사용하면:

```
sum0 += a[0];
sum1 += a[1];
```

독립적인 연산 흐름이 만들어져 병렬 실행이 가능해진다.

```
Multiple Accumulators
→ Data Dependency 완화
→ ILP 증가
→ CPE 감소 가능
```

Reassociation(运算重结合) 역시 연산 순서를 변경해 의존성을 완화하는 기법이다. 단, 부동소수점 연산에서는 결과가 달라질 수 있다.

## 13. Branch Prediction

\*\*Branch Prediction(分支预测)\*\*은 CPU가 조건문의 실행 경로를 미리 예측하는 기능이다.

예측이 틀리면:

```
Branch Misprediction
→ 잘못된 작업 무효화
→ Pipeline 재진행
→ 성능 저하
```

\*\*Conditional Move(条件传送, CMOV)\*\*는 조건에 따라 값을 복사하여 분기를 피할 수 있는 명령어이다.

핵심: 예측하기 어려운 분기가 반복되면 성능이 저하될 수 있다.

## 14. Cache-friendly Code

\*\*Cache-friendly Code(缓存友好代码)\*\*는 캐시에 저장된 데이터를 최대한 활용하도록 작성한 코드이다.

C의 2차원 배열은 Row-major Order로 저장된다.

```
// Cache-friendly
for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
        sum += a[i][j];
```

연속된 메모리에 접근하면 \*\*Spatial Locality(空间局部性)\*\*가 개선되어 Cache Miss가 감소할 수 있다.

자세한 내용은 Chapter 6에서 학습한다.

## 핵심 정리

```
Program Optimization (程序优化)
│
├─ 1. Removing Unnecessary Work (消除不必要的操作)
│  ├─ Removing Procedure Calls (减少函数调用)
│  ├─ Code Motion (代码移动, 계산 위치 변경)
│  ├─ Strength Reduction (强度削弱, 연산 종류 변경)
│  └─ Common Subexpression (公共子表达式消除)
│
├─ 2. Optimization Blockers (优化障碍)
│  ├─ Procedure Side Effects (函数副作用)
│  └─ Memory Aliasing (内存别名)
│
├─ 3. Instruction-Level Parallelism (指令级并行)
│  ├─ Loop Unrolling (循环展开)
│  ├─ Reassociation (运算重结合)
│  └─ Multiple Accumulators (多累加器)
│
├─ 4. Control Flow Optimization (控制流优化)
│  ├─ Branch Prediction (分支预测)
│  └─ Conditional Move (条件传送)
│
└─ 5. Memory Access Optimization (内存访问优化)
   ├─ Cache Locality (缓存局部性)
   └─ Matrix Blocking (矩阵分块)
```

Chapter 5 핵심: 프로그램의 동작을 유지하면서 불필요한 작업을 줄이고, \*\*ILP(指令级并行)\*\*와 \*\*Cache Locality(缓存局部性)\*\*를 활용해 실행 성능을 개선하는 것.