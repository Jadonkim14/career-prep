# 0104. Maximum Depth of Binary Tree (26.9.16)

**### 유형**

* Tree(树)

* DFS(深度优先搜索)

* BFS(广度优先搜索)

* Recursion(递归)

**### 풀이**

* 현재 Node의 최대 깊이는:

```text
max(왼쪽 Subtree 깊이, 오른쪽 Subtree 깊이) + 1
```

* 빈 Node는 깊이 0이다.

```cpp
if (!root) return 0;
```

* 따라서 DFS 재귀식은 다음과 같다.

```cpp
return max(maxDepth(root->left), maxDepth(root->right)) + 1;
```

**### 내 코드 개선 과정**

* 처음에는 자식 존재 여부를 따로 처리했다.

```cpp
if (!root->left)
return 1 + maxDepth(root->right);

else if (!root->right)
return 1 + maxDepth(root->left);
```

* 하지만 `nullptr`은 이미:

```cpp
if (!root) return 0;
```

에서 처리되므로 이 분기는 필요 없다.

* 이후 다음처럼 작성하려 했다.

```cpp
return maxDepth(root->left) >= maxDepth(root->right)
? 1 + maxDepth(root->left)
: 1 + maxDepth(root->right);
```

* 이 코드는 무한 재귀는 아니지만, 비교할 때 계산한 Subtree를 다시 계산하므로 중복 재귀 호출이 발생한다.

* 따라서 재귀 결과를 한 번만 계산해서 저장하거나:

```cpp
int left = maxDepth(root->left);
int right = maxDepth(root->right);

return max(left, right) + 1;
```

* 더 간단히 다음처럼 작성할 수 있다.

```cpp
return max(maxDepth(root->left), maxDepth(root->right)) + 1;
```

**### BFS 풀이**

* BFS는 Tree를 한 층씩 탐색한다.

* 현재 Queue의 크기를 저장하면 그 값이 현재 Level의 Node 개수다.

* **현재 Queue의 크기 만큼만 반복한다.**

```cpp
int sz = q.size();
```

* 현재 Level의 Node를 모두 처리한 뒤 깊이를 1 증가시킨다.

```text
Level 1 처리 → depth = 1
Level 2 처리 → depth = 2
Level 3 처리 → depth = 3
```

**### DFS / BFS 선택 기준**

* DFS를 먼저 생각할 상황:

```text
Subtree 결과 계산
모든 경로 탐색
재귀적으로 왼쪽/오른쪽 결과 결합
```

* BFS를 먼저 생각할 상황:

```text
최단 거리
최소 횟수
가장 가까운 Node
Level별 처리
동시에 퍼지는 문제
```

* 이번 문제는 DFS와 BFS 둘 다 가능하지만, DFS가 더 간단하다.

**### 배운 점**

* Tree 재귀에서는 먼저 함수의 의미를 정의한다.

```text
maxDepth(node)
= node를 root로 하는 Subtree의 최대 깊이
```

* `nullptr` 처리는 base case에 맡기면 불필요한 분기를 줄일 수 있다.

* 같은 재귀 결과를 여러 번 호출하면 Subtree를 다시 탐색하므로 중복 계산이 발생할 수 있다.

* BFS에서 `q.size()`를 저장하는 패턴은 한 Level을 처리할 때 자주 사용한다.

**### 복잡도**

* DFS 시간복잡도: `O(n)`

* DFS 공간복잡도: `O(height)`

* BFS 시간복잡도: `O(n)`

* BFS 공간복잡도: `O(n)`


# 0100. Same Tree (26.9.19)

### 유형

* Tree(树)
* DFS(深度优先搜索)
* BFS(广度优先搜索)
* Recursion(递归)

### 풀이

* 두 Tree가 같으려면:

```text
현재 Node 값이 같고
왼쪽 Subtree가 같고
오른쪽 Subtree가 같아야 한다.
```

* Base Case:

```cpp
if (!p && !q) return true;
if (!p || !q) return false;
```

* 재귀:

```cpp
return p->val == q->val
        && isSameTree(p->left, q->left)
        && isSameTree(p->right, q->right);
```

### BFS 풀이

* 두 Queue에 대응되는 Node를 같은 순서로 저장한다.
* 현재 Node의 값과 왼쪽/오른쪽 Child 구조를 비교한다.
* 구조가 같으면 존재하는 Child만 Queue에 넣는다.

```cpp
if ((left1 == nullptr) ^ (left2 == nullptr))
    return false;

if ((right1 == nullptr) ^ (right2 == nullptr))
    return false;
```

* `^`는 XOR이며, 두 조건이 서로 다를 때 `true`다.

### 내 코드 개선 과정

* 처음에는 `nullptr`도 Queue에 넣고 나중에 비교했다.
* 이 방식도 맞지만, 공식 풀이처럼 Child 구조를 먼저 검사하고 실제 Node만 Queue에 넣으면 더 깔끔하다.

```text
내 방식:
일단 Queue에 넣음 → 나중에 nullptr 확인

공식 방식:
구조 확인 → 존재하는 Node만 Queue에 삽입
```

### 배운 점

* Tree 비교에서는 값뿐 아니라 구조도 확인해야 한다.
* Base Case는 조건 순서가 중요하다.
* DFS에서는 왼쪽/오른쪽 Subtree를 재귀적으로 비교한다.
* BFS에서는 대응되는 Node의 순서를 유지해야 한다.

### 복잡도

* DFS 시간복잡도: `O(n)`
* DFS 공간복잡도: `O(height)`
* BFS 시간복잡도: `O(n)`
* BFS 공간복잡도: `O(n)`