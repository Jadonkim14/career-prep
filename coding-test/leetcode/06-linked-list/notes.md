# 0206. Reverse Linked List (26.9.7)

**### 유형**

* Linked List(链表)
* Pointer Manipulation(指针操作)
* Iteration(迭代)
* Recursion(递归)

**### 풀이**

* 처음에는 새로운 `ListNode`를 생성해 역순으로 연결하는 방식으로 접근했다.
* 이후 **기존 Node의 `next`를 직접 변경**하는 Iterative 방식으로 개선했다.
* `prev`, `cur`, `next` 세 포인터를 사용한다.

```text
① next = cur->next
② cur->next = prev
③ prev = cur
④ cur = next
```

* `next`를 먼저 저장하는 이유는 `cur->next`를 바꾸면 원래 다음 Node의 주소를 잃기 때문이다.

**### 헷갈렸던 부분**

* ListNode* newList = ListNode(cur->val);는 포인터에 객체를 대입한 것.
* 동적 생성하려면 ListNode* newList = new ListNode(cur->val);

* `ListNode* a, b;` → `a`만 Pointer.
* `ListNode* a, *b;` → `a`, `b` 모두 Pointer.

* 초기화하지 않은 지역 Pointer는 자동으로 `nullptr`이 되지 않는다. ListNode* prev = nullptr;

**### Recursive(递归) 풀이**

* `reverseList(head->next)`로 끝까지 내려간 후 돌아오면서 연결을 뒤집는다.
* 핵심:

```cpp
head->next->next = head;
head->next = nullptr;
```

* 재귀도 시간은 `O(n)`이지만 호출 스택 때문에 공간은 `O(n)`이다.
* 따라서 이 문제에서는 **Iterative 방식이 더 효율적**이다.

**### 배운 점**

* Linked List Reverse는 **값이 아니라 Pointer의 연결을 변경하는 문제**다.

* `prev / cur / next`의 역할을 이해하는 것이 핵심이다.

* `next`를 먼저 저장해야 기존 List의 연결을 잃지 않는다.

* Recursive 풀이도 가능하지만, 이 문제에서는 Iterative 풀이를 기본으로 한다.

* **시간복잡도(Time Complexity / 时间复杂度): `O(n)`**

* **공간복잡도(Space Complexity / 空间复杂度): `O(1)`** — Iterative 기준. 


# 0021. Merge Two Sorted Lists (26.9.8)

**### 유형**

* Linked List(链表)
* Pointer Manipulation(指针操作)
* Iteration(迭代)
* Dummy Node(哨兵节点)

**### 풀이**

* 두 정렬된 Linked List의 현재 Node를 비교하고, **값이 작은 Node를 결과 List에 연결**한다.
* `merged`는 결과 List의 시작점을 유지하는 **Dummy Node**이고, `cur`는 현재 결과 List의 마지막 Node를 가리킨다.

```text
① list1->val과 list2->val 비교

② 값이 작은 Node를 cur->next에 연결

③ 선택한 list 포인터를 다음 Node로 이동

④ cur를 방금 연결한 Node로 이동
```

* 두 List 중 하나가 `nullptr`이 되면 다른 List에는 아직 처리하지 않은 Node들이 남아 있다.
* 두 List 모두 이미 정렬되어 있으므로 **남은 List 전체를 한 번에 연결**할 수 있다.

**### 헷갈렸던 부분**

* `merged`와 `cur`의 역할을 분리해야 한다.
* `merged`는 처음 위치를 유지하고, `cur`만 이동한다.

```text
merged
  ↓
dummy → 1 → 2 → 3 → 4
                  ↑
                 cur
```

* `cur->next = list1`은 새로운 Node를 만드는 것이 아니라 **기존 `list1` Node를 결과 List에 연결하는 것**이다.
* 두 List 중 하나가 끝나면 남은 List는 이미 정렬되어 있으므로 하나씩 비교할 필요 없이 통째로 연결할 수 있다.
* Dummy Node를 사용하면 첫 번째 Node를 별도로 처리할 필요가 없어지고 마지막에 `merged.next`를 반환하면 된다.

**### Recursive(递归) 풀이**

* 두 List의 head 중 값이 작은 Node를 선택하고, 선택한 Node의 `next`에 나머지 List를 재귀적으로 merge한다.
* Recursive 방식도 시간은 `O(n + m)`이지만, 재귀 호출 스택 때문에 공간은 `O(n + m)`이다.
* 따라서 이 문제에서는 **Iterative 방식이 추가 공간 `O(1)`이므로 더 효율적**이다.

**### 배운 점**

* 두 개의 정렬된 Linked List를 merge할 때는 **두 head를 비교하면서 작은 Node를 연결**한다.
* 새로운 Node를 생성할 필요 없이 기존 Node의 `next`만 변경할 수 있다.
* `Dummy Node + cur` 패턴을 사용하면 Linked List의 첫 Node 처리와 포인터 관리를 단순화할 수 있다.
* Linked List에서는 **값을 이동시키는 것이 아니라 Node의 연결 관계를 변경하는 것**이 핵심이다.
* Recursive 풀이도 가능하지만, 이 문제에서는 **Iterative 방식을 기본으로 한다.**

**### 복잡도**

* **시간복잡도(Time Complexity / 时间复杂度): `O(n + m)`**
* **공간복잡도(Space Complexity / 空间复杂度): `O(1)`** — Iterative 기준.


# 0141. Linked List Cycle (26.9.9)

**### 유형**

* Linked List(链表)

* Pointer Manipulation(指针操作)

* Iteration(迭代)

* Two Pointers(快慢指针)

* Floyd's Cycle Detection(弗洛伊德判圈算法)

**### 풀이**

* 처음에는 `vector<ListNode*>`에 지나간 Node의 주소를 저장하고, `find()`를 이용해 현재 Node가 이미 존재하는지 확인하는 방식으로 접근했다.

* 하지만 `vector + find()` 방식은 `find()`가 선형 탐색을 수행하기 때문에 시간복잡도가 `O(n²)`이 된다.

* 이후 **Floyd's Cycle Detection**을 사용하여 추가 메모리 없이 Cycle을 탐지했다.

* `slow`는 한 번에 한 Node씩 이동하고, `fast`는 한 번에 두 Node씩 이동한다.

* Cycle이 없다면 `fast`가 먼저 `nullptr`에 도달한다.

* Cycle이 존재한다면 두 Pointer가 Cycle 내부에서 계속 이동하게 되고, 빠른 `fast`가 느린 `slow`를 결국 따라잡아 **같은 Node를 가리키게 된다.**

* 따라서 `slow == fast`이면 Cycle이 존재한다고 판단하여 `true`를 반환한다.

**### 헷갈렸던 부분**

* 처음에는 지나간 `val`을 저장하면 된다고 생각했지만, **`val`이 같다고 같은 Node는 아니다.**

* 따라서 방문 기록을 저장하는 방법에서는 `ListNode*`를 저장하여 **Node의 주소를 비교**해야 한다.

* `fast`는 한 번에 두 칸 이동하기 때문에 이동하기 전에 `fast->next`와 `fast->next->next`가 존재하는지 확인해야 한다.

```cpp
while (fast->next != NULL && fast->next->next != NULL)
```

* Cycle이 없는 경우 `fast`가 먼저 List의 끝에 도달하므로 `false`를 반환한다.

* Dummy Node나 새로운 List를 만들 필요 없이 **두 Pointer만으로 Cycle을 판단할 수 있다.**

**### 배운 점**

* Linked List의 Cycle 여부는 **Node의 `val`이 아니라 Node 자체의 동일성**을 기준으로 판단해야 한다.

* 방문한 Node를 저장하는 방법에서는 `ListNode*`를 사용하여 Node의 주소를 비교할 수 있다.

* 이 문제에서는 **방문한 Node의 존재 여부만 확인하면 되므로 `unordered_set`이 더 적합하다.**

* `vector + find()` 방식보다 **Floyd's Cycle Detection을 사용하면 추가 메모리 없이 Cycle을 탐지할 수 있다.**
* 따라서 Linked List의 Cycle 문제에서는 **Two Pointer + Floyd's Cycle Detection 패턴**을 기억한다.

* **항상 nullptr의 next를 역참조하지 않는지 생각해야 한다.**

**### 복잡도**

* **시간복잡도(Time Complexity / 时间复杂度): `O(n)`**

* **공간복잡도(Space Complexity / 空间复杂度): `O(1)`** — Floyd's Cycle Detection 기준.


# 0876. Middle of the Linked List (26.9.10)

### 유형

* Linked List(链表)

* Pointer Manipulation(指针操作)

* Iteration(迭代)

* Two Pointers(快慢指针)

* Slow / Fast Pointer

### 풀이

* 처음에는 `vector<ListNode*>`에 모든 Node의 주소를 저장한 뒤, `v[v.size() / 2]`를 반환하는 방식으로 접근했다.

* 이 방법은 List를 한 번 순회하므로 시간복잡도는 `O(n)`이지만, 모든 Node의 주소를 저장하기 때문에 공간복잡도가 `O(n)`이다.

* 이후 **Slow / Fast Pointer**를 사용하여 추가 메모리 없이 중간 Node를 찾았다.

* `slow`는 한 번에 한 Node씩 이동하고, `fast`는 한 번에 두 Node씩 이동한다.

### 헷갈렸던 부분

* 처음에는 Linked List의 중간 Node를 찾으려면 모든 Node를 저장해야 한다고 생각했지만, **Pointer의 이동 속도 차이를 이용하면 Node의 개수를 직접 셀 필요가 없다.**

* 특히 Linked List에서는 **현재 Pointer 자체가 `nullptr`인지 먼저 확인한 뒤 `->next`를 접근하는 습관**이 중요하다.

### 배운 점

* Linked List의 특정 위치를 찾을 때 모든 Node를 저장하지 않고 **Pointer의 이동 속도 차이**를 활용할 수 있다.

* `slow`가 1칸, `fast`가 2칸 이동하면 `fast`가 끝에 도착했을 때 `slow`가 중간 지점에 위치한다.

* 따라서 Middle Node 문제에서는 **Slow / Fast Pointer 패턴**을 먼저 생각할 수 있다.

* Linked List에서는 **항상 `nullptr`의 `next`를 역참조하지 않는지 확인해야 한다.**

* Slow / Fast Pointer는 단순히 Middle Node를 찾는 것뿐만 아니라 Cycle 탐지 등 여러 Linked List 문제에 활용된다.

### Slow / Fast Pointer 활용 유형

* **중간 Node 찾기**
* **Cycle 존재 여부 확인**
* **Cycle 시작점 찾기**
* **뒤에서 N번째 Node 찾기**
* **Linked List를 절반으로 분할**

```text
속도 차이
→ 중간 위치 / Cycle 탐지

거리 차이
→ 뒤에서 N번째 위치
```

* 즉, `slow / fast`를 단순히 **"1칸 / 2칸 이동"**으로 외우기보다, **두 Pointer의 속도 또는 거리 차이를 이용하여 원하는 위치 관계를 만든다**고 이해한다.

### 복잡도

* **시간복잡도(Time Complexity / 时间复杂度): `O(n)`**

* **공간복잡도(Space Complexity / 空间复杂度): `O(1)`** — Slow / Fast Pointer 기준.


# 0083. Remove Duplicates from Sorted List (26.9.12)

**### 유형**

* Linked List(链表)

* Pointer Manipulation(指针操作)

* Iteration(迭代)

**### 풀이**

* `cur`와 `cur->next`를 비교해서 값이 같으면 다음 Node를 삭제한다.

* 중복을 삭제한 경우에는 `cur`를 이동하지 않고 다시 비교한다.

* 값이 다를 때만 `cur = cur->next`로 이동한다.

**### 헷갈렸던 부분**

* 중복 삭제 후 `cur`까지 이동하면 연속된 중복을 놓칠 수 있다.

```text
1 → 1 → 1 → 2
```

* 따라서:

```text
중복
→ next 삭제
→ cur 유지

다름
→ cur 이동
```

* 처음에 `head == nullptr`을 처리하면 `cur`는 항상 유효하므로:

```cpp
while (cur->next)
```

만 사용해도 된다.

**### 배운 점**

* Linked List 삭제에서는 Node 값보다 `next` 연결을 어떻게 바꾸는지가 중요하다.

* Node를 삭제한 뒤 현재 Pointer를 이동할지 유지할지 판단해야 한다.

* `nullptr` 검사는 무조건 넣는 것이 아니라 Pointer가 유효한지 논리적으로 판단한다.

* LeetCode 및 일반 코테에서는 입력으로 주어진 Node의 메모리 소유권이 채점 환경에 있을 수 있으므로,    특별한 요구가 없다면 직접 delete하지 않고 연결만 수정한다.

* 따라서 내가 직접 할당한 메모리는 내가 해제하고, 문제에서 받은 Node는 소유권이 명확하지 않으면 임의로 해제하지 않는다.

* 노드 삭제 문제에서는 **"Node를 제거한 뒤 Pointer를 이동할 것인지 유지할 것인지 판단하는 것"**이 중요하다.

**### 복잡도**

* **시간복잡도(Time Complexity / 时间复杂度): `O(n)`**

* **공간복잡도(Space Complexity / 空间复杂度): `O(1)`**


# 0203. Remove Linked List Elements (26.9.13)

### 유형

* Linked List(链表)
* Pointer Manipulation(指针操作)
* Iteration(迭代)
* Dummy Node(哑节点)

### 풀이

* 삭제 대상이 `head`에 연속해서 존재할 수 있으므로, 먼저 `head->val == val`인 동안 `head`를 이동시키는 방법으로 처리할 수 있다.
* 이후 `cur`와 `cur->next`를 이용해 `cur->next->val == val`이면 해당 Node를 건너뛰도록 연결을 수정한다.
* Node를 삭제한 경우에는 `cur`를 이동하지 않고 새롭게 연결된 `cur->next`를 다시 검사한다.
* 값이 다를 때만 `cur = cur->next`로 이동한다.
* 또는 `dummy node`를 `head` 앞에 두면 `head` 삭제를 별도로 처리하지 않고 모든 Node를 동일한 방식으로 삭제할 수 있다.

### 헷갈렸던 부분

* 반복 조건을 처음에 다음과 같이 잘못 작성했다.

```cpp
while (!cur->next)
```

* `!cur->next`는 `cur->next == nullptr`이라는 뜻이므로, 다음 Node가 존재할 때 순회하려면 다음과 같이 작성해야 한다.

```cpp
while (cur->next)
```

* `cur`는 `head`를 복사한 Pointer이지만:

```cpp
cur->next = cur->next->next;
```

처럼 `cur`가 가리키는 실제 Node의 `next` 값을 변경하면 원본 Linked List의 연결도 변경된다.

* 다음과 같이 연속된 삭제 대상이 있을 수 있다.

```text
1 → 6 → 6 → 6 → 2
↑
cur
```

* 첫 번째 `6`을 삭제하면:

```text
1 → 6 → 6 → 2
↑
cur
```

가 되므로 삭제 후 `cur`를 이동하면 안 된다.

따라서:

```text
삭제 대상
→ cur->next 삭제
→ cur 유지

삭제 대상 아님
→ cur 이동
```

* 일반적인 Node 삭제는 이전 Node의 `next`를 변경하면 되지만, `head` 자체가 삭제 대상이면 앞에 Node가 없기 때문에 별도 처리가 필요하다.

```text
head
 ↓
6 → 1 → 2
```

* 이를 해결하는 대표적인 방법은 두 가지다.

```text
1. head가 val인 동안 head를 먼저 이동한다.
2. head 앞에 dummy node를 추가한다.
```

* `dummy node`를 사용하면:

```text
dummy
 ↓
0 → 6 → 1 → 2
↑
cur
```

첫 실제 Node도 항상 `cur->next`가 되므로:

```cpp
cur->next = cur->next->next;
```

라는 동일한 로직으로 삭제할 수 있다.

### 배운 점

* Linked List에서 Node 삭제는 실제 Node 값을 지우는 것보다 `next` 연결을 변경하는 것이 핵심이다.
* Pointer 자체를 복사해도 해당 Pointer가 가리키는 Node를 수정하면 원본 Linked List가 변경된다.
* Node를 삭제한 뒤 현재 Pointer를 이동할지 유지할지 반드시 판단해야 한다.
* 연속해서 삭제해야 하는 Node가 존재할 수 있으므로, 삭제한 경우에는 현재 Pointer를 유지하고 새로운 `next`를 다시 검사해야 한다.
* `head`는 다른 Node와 달리 이전 Node가 없기 때문에 삭제 시 예외 처리가 발생할 수 있다.
* `dummy node`를 사용하면 `head` 삭제를 포함한 모든 Node를 동일한 방식으로 처리할 수 있어 코드가 단순해진다.
* 따라서 **head가 변경될 가능성이 있는 Linked List 문제에서는 dummy node 사용을 먼저 고려할 수 있다.**
* 재귀(递归) 방식으로도 해결할 수 있지만 호출 스택 때문에 공간복잡도가 `O(n)`이 된다.
* 이 문제에서는 반복(迭代) + dummy node 방식이 `O(1)` 추가 공간으로 해결할 수 있어 코딩테스트 관점에서는 더 적합하다.

### 복잡도

* **시간복잡도(Time Complexity / 时间复杂度): `O(n)`**
* **공간복잡도(Space Complexity / 空间复杂度): `O(1)`**