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

그 아래에 이 정도만 붙이면 충분합니다.

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