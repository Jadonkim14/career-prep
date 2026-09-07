# **0206. Reverse Linked List (26.9.7)**

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