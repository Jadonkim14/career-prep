## 0020. Valid Parentheses (26.8.21)

### 유형

* Stack(栈)
* String
* LIFO(后进先出)

### 처음 풀이

* 여는 괄호 `(`, `{`, `[`를 Stack에 `push`.
* 닫는 괄호가 나오면 `top()`과 같은 종류인지 확인.
* 다르면 `false`, 같으면 `pop()`.
* 마지막에 Stack이 남아 있으면 `false`.

### 개선점

* 마지막 검사는 간단하게 작성 가능.

```cpp
return st.empty();
```

* 문자열 길이가 홀수라면 유효할 수 없으므로 미리 처리할 수 있다.

```cpp
if(s.size() % 2 == 1) return false;
```

* 공식 풀이는 `unordered_map`에 **닫는 괄호 → 여는 괄호**를 저장하여 반복되는 조건문을 줄였다.

```cpp
unordered_map<char, char> pairs = {
    {')', '('},
    {']', '['},
    {'}', '{'}
};
```

### 배운 점

* 핵심은 **가장 나중에 열린 괄호가 가장 먼저 닫혀야 한다**는 것 → LIFO → Stack.
* 닫는 괄호를 만났을 때 Stack이 비어 있거나 `top()`과 짝이 다르면 `false`.
* `"([)]"`처럼 짝이 Stack 안에 존재하더라도 **top과 일치하지 않으면 잘못된 순서**이다.
* 모든 순회가 끝난 뒤 Stack이 비어 있어야 모든 괄호가 닫힌 것이다.
* 시간복잡도 **O(n)**, 공간복잡도 **O(n)**.
* **`Valid Parentheses = Stack`을 암기하기보다, 최근에 들어온 것을 먼저 처리해야 하는 중첩 구조(嵌套结构) → Stack(栈)**을 떠올린다.

### Stack 문법 정리

```cpp
#include <stack>

stack<int> st;

st.push(10);     // 원소 추가
st.pop();        // top 원소 제거
st.top();        // top 원소 확인
st.empty();      // 비어 있으면 true
st.size();       // 원소 개수
```

* Stack(栈)은 **LIFO(后进先出)** 구조이다.
* `push(x)` → Stack의 맨 위에 `x` 추가.
* `pop()` → 맨 위 원소 제거. **값을 반환하지 않는다.**
* `top()` → 맨 위 원소 확인.
* `empty()` → Stack이 비어 있는지 확인.
* `size()` → 저장된 원소 개수.
* 빈 Stack에서 `top()`이나 `pop()`을 사용하지 않도록 먼저 `empty()`를 확인한다.

```cpp
if (!st.empty()) {
    char ch = st.top();
    st.pop();
}
```


## 0225. Implement Stack using Queues (26.8.23)

### 유형

* Stack
* Queue
* 자료구조 구현
* `queue`

### 처음 풀이

* 두 개의 Queue `q1`, `q2`를 사용
* `push()`는 `q1`에 바로 삽입
* `pop()`에서는 마지막 원소만 남을 때까지 `q1`의 원소를 `q2`로 이동
* 마지막 원소를 제거한 후 다시 `q2 → q1`으로 복원
* `top()`도 같은 방식으로 마지막 원소를 찾되, 제거하지 않고 다시 Queue에 삽입

### 개선점

* 두 개의 Queue가 없어도 **하나의 Queue를 회전(Rotation)**시켜 Stack을 구현할 수 있다.
* Queue의 `front()`를 제거한 뒤 다시 `push()`하면 원소의 순서를 회전시킬 수 있다.
* `pop()` 시 앞의 `n-1`개를 뒤로 보내면 가장 최근에 삽입된 원소가 `front()`에 위치한다.
* 또는 `push()`할 때 회전시켜 항상 Stack의 top을 Queue의 `front()`에 유지하는 방법도 있다.

### 최종 풀이

* Queue 1개 사용
* `push()` → `O(1)`
* `pop()` → `O(n)`
* `top()` → `O(n)`
* 공간 → `O(n)`

### 배운 점

* Stack(栈) → **LIFO**, 마지막에 들어온 원소가 먼저 나온다.
* Queue(队列) → **FIFO**, 먼저 들어온 원소가 먼저 나온다.
* Queue 하나도 원소를 `front → pop → push`하면 순서를 회전시킬 수 있다.
* `queue.pop()`은 값을 반환하지 않으므로 먼저 `front()`로 값을 얻은 뒤 `pop()`해야 한다.
* `queue`의 기본 연산은 `push()`, `front()`, `pop()`, `size()`, `empty()`이다.
* 반복문 안에서 `pop()`하면 `size()`가 계속 변하므로 **반복 전에 크기를 변수에 저장**해야 한다.
* 두 Queue를 사용하는 것보다 한 Queue의 회전을 이용하면 추가 Queue 없이 같은 동작을 구현할 수 있다.
* `push()` 때 미리 회전시키는 풀이를 사용하면 `push()`는 `O(n)`, `pop()`과 `top()`은 `O(1)`로 만들 수도 있다.

### Queue 문법

```cpp
queue<int> q;

q.push(x);    // 뒤에 삽입
q.front();    // 맨 앞 원소
q.back();     // 맨 뒤 원소
q.pop();      // 맨 앞 원소 제거
q.size();     // 원소 개수
q.empty();    // 비었는지 확인
```

* Queue(队列)는 **FIFO** 구조
* `pop()`은 값을 반환하지 않음

```cpp
int x = q.front();
q.pop();
```

* 회전:

```cpp
q.push(q.front());
q.pop();
```

`[1,2,3] → [2,3,1]`


## 0232. Implement Queue using Stacks (26.8.24)

### 유형

* Stack(栈)
* Queue(队列)
* FIFO(先进先出)
* Amortized Analysis(均摊分析)

### 처음 풀이

* `s1`에 새 원소를 `push`.
* `pop()` 또는 `peek()`할 때 `s1`의 마지막 원소 하나만 남을 때까지 `s2`로 이동.
* 가장 먼저 들어온 원소를 꺼내거나 확인.
* 이후 `s2`의 원소를 다시 `s1`으로 복구.
* Queue의 FIFO를 Stack 두 개의 순서 뒤집기로 구현.

```cpp
while (s1.size() > 1) {
    s2.push(s1.top());
    s1.pop();
}
```

### 개선점

* 매 `pop()` / `peek()`마다 `s1 → s2 → s1`으로 옮기면 한 번의 연산에 **O(n)**이 걸린다.
* 두 Stack의 역할을 분리한다.

```text
inStack  = 새 원소를 받는 Stack
outStack = Queue의 front를 꺼내는 Stack
```

* `push()`는 항상 `inStack`에 넣는다.

```cpp
void push(int x) {
    inStack.push(x);
}
```

* `pop()` / `peek()` 시 `outStack`이 비어 있을 때만 `inStack → outStack`으로 이동한다.
* 한 번 `outStack`으로 옮긴 원소는 다시 `inStack`으로 돌려놓지 않는다.

```cpp
void in2out() {
    while (!inStack.empty()) {
        outStack.push(inStack.top());
        inStack.pop();
    }
}
```

* `in2out()`은 외부에서 직접 사용할 기능이 아니라 Queue 내부 구현을 위한 보조 함수이므로 `private`에 둔다 → Encapsulation(封装).

* Queue가 비었는지는 두 Stack을 모두 확인해야 한다.

```cpp
return inStack.empty() && outStack.empty();
```

### 배운 점

* Stack은 LIFO(后进先出), Queue는 FIFO(先进先出)이므로 **Stack을 한 번 뒤집으면 Queue의 순서를 만들 수 있다.**
* `inStack`은 새로 들어오는 원소를 저장하고, `outStack`은 먼저 들어온 원소부터 꺼내는 역할을 한다.
* 핵심은 **`outStack`이 비어 있을 때만 `inStack → outStack`으로 이동하는 것**이다.
* `outStack`에 아직 원소가 있다면 새로 `push`된 원소보다 먼저 처리되어야 하므로 그대로 사용한다.

```text
push(1), push(2), pop(), push(3)

inStack  = {3}
outStack = {2}

→ 다음 front는 3이 아니라 2
```

* `move` 자체는 한 번 실행할 때 **O(n)**일 수 있지만, 각 원소는 `inStack → outStack`으로 최대 한 번만 이동한다.
* 따라서 `push`, `pop`, `peek`, `empty`의 **均摊时间复杂度(amortized time complexity)는 O(1)**이다.
* 공간복잡도는 두 Stack에 전체 원소를 저장하므로 **O(n)**.
* `Implement Queue using Stacks = Stack 2개`를 암기하기보다, **FIFO가 필요한데 LIFO 자료구조만 사용할 수 있음 → 한 Stack에 입력하고 다른 Stack으로 순서를 뒤집는다**는 구조를 떠올린다.
* 외부에서 필요한 동작은 `public`, 내부 구현에만 필요한 데이터와 보조 함수는 `private`으로 숨길 수 있다 → Encapsulation(封装).


## 0704. Binary Search (26.8.27)

### 유형

* Binary Search(二分查找)
* Sorted Array(有序数组)
* Divide and Conquer(分治)
* Search(查找)

### 처음 풀이

* 탐색 범위를 `first`, `last`로 설정.
* 가운데 위치 `middle`의 값을 `target`과 비교.
* `nums[middle] < target`이면 왼쪽 절반을 버리고 오른쪽 영역을 탐색.
* `nums[middle] > target`이면 오른쪽 절반을 버리고 왼쪽 영역을 탐색.
* `first > last`가 되면 탐색할 범위가 없으므로 `-1` 반환.
* 매 반복마다 탐색 범위를 절반으로 줄여 `O(log n)`으로 탐색.

```cpp
if (nums[middle] < target) {
    first = middle + 1;
}
else {
    last = middle - 1;
}
```

### 개선점

* 처음 `middle`을 계산할 때도 반복문 내부와 동일하게 다음 형태를 사용하는 것이 좋다.

```cpp
int middle = first + (last - first) / 2;
```

* `(first + last) / 2`보다 위 형태를 사용하면 `first + last`에서 발생할 수 있는 Integer Overflow(整数溢出)를 피할 수 있다.

* `while (nums[middle] != target)`보다 **탐색 가능한 구간이 남아 있는가**를 기준으로 반복하는 것이 Binary Search의 구조를 더 명확하게 표현한다.

```cpp
while (first <= last) {
    int middle = first + (last - first) / 2;

    if (nums[middle] == target) {
        return middle;
    }

    if (nums[middle] < target) {
        first = middle + 1;
    }
    else {
        last = middle - 1;
    }
}

return -1;
```

* 현재 탐색 구간을 `[first, last]`처럼 **양쪽 끝을 모두 포함하는 구간**으로 정의했기 때문에 탐색 조건은 다음과 같다.

```cpp
first <= last
```

* `first > last`가 되면 탐색 가능한 원소가 하나도 남지 않은 상태다.

### 배운 점

* Binary Search는 **정렬된 데이터에서 비교 결과를 이용해 탐색 범위의 절반을 버리는 알고리즘**이다.

* Binary Search에서 중요한 것은 `middle` 계산 공식을 암기하는 것보다 **현재 탐색 범위에서 어떤 영역을 확실하게 버릴 수 있는지 판단하는 것**이다.

```text
nums[middle] < target
→ [first, middle] 제거

nums[middle] > target
→ [middle, last] 제거
```

* 매번 탐색 범위가 절반으로 줄어들기 때문에 시간복잡도(Time Complexity / 时间复杂度)는 **O(log n)**이다.

* 추가적인 자료구조를 사용하지 않으므로 공간복잡도(Space Complexity / 空间复杂度)는 **O(1)**이다.

* `Binary Search = 정렬 배열에서 값 찾기`만 암기하지 않는다. 이후에는 다음과 같은 문제로 확장된다.

```text
정확한 값 찾기
→ 704. Binary Search

들어갈 위치 / 경계 찾기
→ 35. Search Insert Position

조건을 만족하는 값 찾기
→ Binary Search on Answer

회전된 정렬 배열 탐색
→ 33. Search in Rotated Sorted Array
```

* `704. Binary Search = while + mid 공식`을 암기하기보다, **정렬되어 있음 → middle과 비교 → 정답이 존재할 수 없는 절반을 제거 → 남은 범위에서 반복**이라는 구조를 떠올린다.

* 실무에서도 정렬된 대량 데이터의 검색뿐 아니라 **특정 조건을 처음 만족하는 위치나 최소/최대 가능한 값을 효율적으로 찾는 문제**로 Binary Search 사고방식이 활용될 수 있다.

> **핵심:** `정렬된 탐색 공간 + 비교 결과로 절반을 확실히 제거할 수 있음 → Binary Search를 고려한다.`


## 0035. Search Insert Position (26.9.1)

**### 유형**

* Binary Search(二分查找)
* Sorted Array(有序数组)
* Insert Position(插入位置)

**### 처음 풀이**

* `left`, `right`로 탐색 범위를 설정.
* `middle`의 값과 `target`을 비교하여 탐색 범위를 절반으로 줄인다.
* `target`을 찾으면 해당 인덱스를 반환.
* 찾지 못하면 `left`가 target이 들어갈 위치이므로 `left`를 반환한다.

```cpp
if (nums[middle] < target)
    left = middle + 1;
else
    right = middle - 1;

return left;
```

**### 개선점**

* `middle`은 Integer Overflow(整数溢出)를 피하기 위해 다음 형태를 사용한다.

```cpp
int middle = left + (right - left) / 2;
```

* 탐색 종료 시 `left > right`가 되고, `left`는 **target보다 작은 값과 큰 값 사이의 경계**가 된다.

```text
[ target보다 작은 값 ][ target보다 큰 값 ]
                     ↑
                    left
```

**### 배운 점**

* Binary Search는 정확한 값뿐 아니라 **삽입 위치 / 경계(Boundary / 边界)**를 찾는 데도 사용할 수 있다.
* `704`는 target을 찾지 못하면 `-1`을 반환하지만, `35`는 찾지 못해도 `left`를 반환한다.
* 시간복잡도(Time Complexity / 时间复杂度): `O(log n)`
* 공간복잡도(Space Complexity / 空间复杂度): `O(1)`

> **핵심:** `nums[middle] < target`이면 `left`를 오른쪽으로, `nums[middle] > target`이면 `right`를 왼쪽으로 이동시키면, 탐색 종료 후 `left`가 target의 삽입 위치가 된다.


## 0374. Guess Number Higher or Lower (26.9.2)

### 유형

* Binary Search(二分查找)
* Sorted Range(有序范围)
* Search Boundary(边界查找)

### 풀이

* `left`, `right`로 정답이 존재할 수 있는 탐색 범위를 설정.
* `middle`의 값에 대해 `guess()`의 결과를 확인하여 탐색 범위를 절반으로 줄인다.
* `guess(middle) == 0`이면 정답이므로 해당 값을 반환한다.
* `guess(middle) == -1`이면 정답이 `middle`보다 작으므로 `right`를 왼쪽으로 이동한다.
* `guess(middle) == 1`이면 정답이 `middle`보다 크므로 `left`를 오른쪽으로 이동한다.

### 개선점

* `guess(middle)`은 한 번만 호출하고 결과를 변수에 저장한다.

```cpp
int result = guess(middle);
```

* while (left <= right)

### 배운 점

* `35`번처럼 값 자체가 아니라 삽입 위치나 경계(Boundary / 边界)를 찾는 형태로도 확장할 수 있다.
* `35`번은 `left`를 반환하여 삽입 위치를 찾지만, `374`번은 `guess()`를 통해 **정확한 정답을 찾으면 즉시 반환**한다.
* 시간복잡도(Time Complexity / 时间复杂度): `O(log n)`
* 공간복잡도(Space Complexity / 空间复杂度): `O(1)`


좋아. **첫 번째 풀이의 방식(`mid`와 `mid + 1`을 비교해서 정답을 바로 반환)**으로 바꾸면 이렇게 정리하면 돼.

## 0069. Sqrt(x) (26.9.3)

**### 유형**

* Binary Search(二分查找)
* Sorted Range(有序范围)
* Search Boundary(边界查找)

**### 풀이**

* `left`, `right`로 정답이 존재할 수 있는 탐색 범위를 설정.
* `middle`의 제곱과 `x`를 비교하여 탐색 범위를 절반으로 줄인다.
* `middle² <= x`이고 `(middle + 1)² > x`이면 `middle`이 정답이므로 반환한다.
* `middle² > x`이면 정답이 `middle`보다 작으므로 `right`를 왼쪽으로 이동한다.
* `(middle + 1)² <= x`이면 정답이 더 크므로 `left`를 오른쪽으로 이동한다.
* middle 제곱할 때 오버플로우 발생

**### 개선점**

* (long long) middle² <= x 으로 오버플로우를 피할 수 있다.
* 혹은 `middle <= x / middle` 형태로 비교할 수 있다.
* 단, 나눗셈을 사용하는 경우 `middle == 0`에서 Division by Zero(除零)가 발생할 수 있으므로 주의한다.

**### 배운 점**

* 값 자체가 아니라 **조건을 만족하는 경계값**을 찾는 형태로도 Binary Search를 사용할 수 있다.
* `374`번은 `guess()`를 통해 정확한 정답을 찾으면 즉시 반환한다.
* `69`번은 `middle² <= x < (middle + 1)²`라는 조건을 만족하는 경계값을 찾는다.
* 곱셈에서는 Integer Overflow(整数溢出), 나눗셈에서는 Division by Zero(除零)를 주의해야 한다.
* int ans = exp(0.5 * log(x));
  return ((long long)(ans + 1) * (ans + 1) <= x ? ans + 1 : ans);
  이 방법으로 풀 시 부동소수점 계산 후 정수값을 사용할 때는 경계 후보를 정확한 연산으로 재검증 해야 한다.
* 시간복잡도(Time Complexity / 时间复杂度): `O(log n)`
* 공간복잡도(Space Complexity / 空间复杂度): `O(1)`
