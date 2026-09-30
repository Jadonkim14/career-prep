# Tree 복습

# Tree 복습

## 1. Maximum Depth

```cpp
if (root == nullptr) return 0;

return max(maxDepth(root->left),
           maxDepth(root->right)) + 1;
```

- 핵심: `max(left, right) + 1`
- 시간복잡도: `O(n)`
- 이유: 모든 노드를 한 번씩 방문

---

## 2. Same Tree

```cpp
if (p == nullptr && q == nullptr) return true;
if (p == nullptr || q == nullptr) return false;
if (p->val != q->val) return false;

return isSameTree(p->left, q->left) &&
       isSameTree(p->right, q->right);
```

- 둘 다 `nullptr` → `true`
- 하나만 `nullptr` → `false`
- 값 다름 → `false`
- 왼쪽과 오른쪽 모두 같아야 함 → `&&`

---

## 3. Invert Binary Tree

```cpp
if (root == nullptr) return nullptr;

swap(root->left, root->right);

invertTree(root->left);
invertTree(root->right);
```

- 핵심: 왼쪽 ↔ 오른쪽 교환
- 시간복잡도: `O(n)`

---

## 4. Path Sum

```cpp
int remain = targetSum - root->val;
```

리프 노드에서:

```cpp
remain == 0
```

인지 확인.

재귀:

```cpp
return hasPathSum(root->left, remain) ||
       hasPathSum(root->right, remain);
```

- 핵심: 내려가면서 `targetSum - root->val`
- 반드시 **leaf에서 합 확인**
- 한쪽 경로만 성공해도 됨 → `||`

---

## 5. Balanced Binary Tree

조건:

```text
|leftHeight - rightHeight| <= 1
```

주의:

각 노드마다 서브트리 높이를 다시 계산하면 중복 계산 발생.

```text
최악 O(n²)
```

---

# Tree 문제별 핵심 패턴

| 문제 | 핵심 |
|---|---|
| Maximum Depth | `max(left, right) + 1` |
| Same Tree | 왼쪽과 오른쪽 모두 같아야 함 `&&` |
| Invert Tree | 왼쪽/오른쪽 교환 후 양쪽 재귀 |
| Path Sum | 한쪽 경로만 성공해도 됨 `||` |
| Balanced Tree | 높이 차이 확인 + 중복 높이 계산 주의 |

---

# DFS 재귀 기본 사고방식

Tree 문제를 보면 먼저 다음 순서로 생각한다.

```text
1. 종료 조건은 무엇인가?
        ↓
2. 왼쪽 서브트리에서 무엇을 얻는가?
        ↓
3. 오른쪽 서브트리에서 무엇을 얻는가?
        ↓
4. 두 결과를 어떻게 합치는가?
```

---

# 한 줄 요약

```text
Tree DFS =
종료 조건을 잡고
→ 왼쪽 재귀
→ 오른쪽 재귀
→ 두 결과를 문제에 맞게 결합한다.
```

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


# 0226. Invert Binary Tree (26.9.24)

### 유형

* Tree(树)

* DFS(深度优先搜索)

* BFS(广度优先搜索)

* Recursion(递归)

### 풀이

* 현재 Node의 왼쪽/오른쪽 Child를 서로 교환한다.

* 이후 왼쪽 Subtree와 오른쪽 Subtree에도 같은 작업을 반복한다.

* Leaf Node의 경우 `left`, `right`가 둘 다 `nullptr`이어도 교환이 가능하므로 따로 처리할 필요가 없다.

### 내 코드 개선 과정

* 처음에는 Child의 존재 여부에 따라 여러 경우로 나누었다.

```text
둘 다 nullptr
왼쪽만 nullptr
오른쪽만 nullptr
둘 다 존재
```

* 하지만 `nullptr`도 Pointer 값이므로 `left`와 `right`를 그대로 교환할 수 있다.

* 또한 재귀 함수의 Base Case에서 `nullptr`을 처리하므로 재귀 호출 전에 Child가 존재하는지 확인할 필요도 없다.

### BFS 풀이

* Queue를 이용해 Tree의 모든 Node를 순서대로 방문한다.

* 현재 Node를 Queue에서 꺼낸 뒤 왼쪽/오른쪽 Child를 교환한다.

* 교환한 후 존재하는 Child를 다시 Queue에 넣는다.

* 이 문제에서는 Level별 결과를 따로 구분할 필요가 없기 때문에 `q.size()`를 이용한 Level Size 저장이 필요하지 않다.

### 배운 점

* Tree 재귀에서는 `nullptr`을 Base Case에서 처리하면 호출 전에 불필요한 조건 검사를 줄일 수 있다.

* Leaf Node의 `left`, `right`가 둘 다 `nullptr`이어도 `swap`은 정상적으로 동작한다.

* Tree 전체에 동일한 작업을 적용할 때 DFS 재귀를 사용할 수 있다.

* BFS에서는 Queue를 이용해 모든 Node를 방문하면서 같은 작업을 수행할 수 있다.

* BFS라고 해서 항상 Level Size가 필요한 것은 아니다.

* Level별 처리가 필요한 문제에서만 `q.size()`를 이용해 Level을 구분하면 된다.

### 복잡도

* DFS 시간복잡도: `O(n)`

* DFS 공간복잡도: `O(height)`

* BFS 시간복잡도: `O(n)`

* BFS 공간복잡도: `O(n)`

# 0101. Symmetric Tree (26.9.27)

### 유형

* Tree(树)
* DFS(深度优先搜索)
* BFS(广度优先搜索)
* Recursion(递归)

### 풀이

* Binary Tree가 좌우 대칭인지 확인하려면 왼쪽 Subtree와 오른쪽 Subtree가 서로 거울 관계인지 비교한다.

* 두 Node `left`, `right`를 비교하는 보조 함수 `check(left, right)`를 사용한다.

* 두 Node가 모두 `nullptr`이면 서로 대칭이므로 `true`를 반환한다.

* 둘 중 하나만 `nullptr`이면 Tree의 구조가 다르므로 `false`를 반환한다.

* 두 Node의 값이 다르면 대칭이 아니므로 `false`를 반환한다.

* 현재 Node가 같다면 바깥쪽 Child와 안쪽 Child를 서로 비교한다.

```text
left->left   ↔ right->right
left->right  ↔ right->left
```

* 위 두 비교가 모두 `true`여야 전체 Tree가 대칭이다.

### 보조 함수를 사용하는 이유

* 주어진 `isSymmetric(root)` 함수는 하나의 Node만 전달받는다.

* 하지만 대칭 여부를 판단하려면 왼쪽과 오른쪽의 두 Node를 동시에 비교해야 한다.

* 재귀 호출에서도 계속 두 Node를 한 쌍으로 비교해야 하므로 두 Node를 Parameter로 받는 보조 함수를 사용한다.

```cpp
bool check(TreeNode* left, TreeNode* right)
```

* 즉 보조 함수를 먼저 떠올리는 것이 아니라, 재귀에 필요한 정보가 기존 함수의 Parameter만으로 부족하기 때문에 보조 함수가 필요하다.

### BFS 풀이

* Queue에 서로 비교해야 하는 Node를 항상 두 개씩 한 쌍으로 저장한다.

```text
[u] [v] [a] [b]
 └───┘   └───┘
 비교     비교
```

* Queue에서 두 Node `u`, `v`를 꺼내 서로 대칭인지 확인한다.

* 둘 다 `nullptr`이면 해당 위치는 대칭이므로 다음 비교를 진행한다.

* 하나만 `nullptr`이거나 두 Node의 값이 다르면 대칭이 아니므로 `false`를 반환한다.

* 현재 두 Node가 같다면 다음에 비교해야 하는 Node를 거울 방향으로 Queue에 넣는다.

```text
u->left   ↔ v->right
u->right  ↔ v->left
```

* 재귀에서는 다음 비교를 함수 호출로 처리하지만, 반복 풀이에서는 다음에 비교할 Node Pair를 Queue에 저장한다.

```text
Recursion
check(u->left, v->right)
check(u->right, v->left)

        ↓

Iteration
Queue에
(u->left, v->right)
(u->right, v->left)
저장
```

### 배운 점

* 대칭 Tree에서는 같은 방향의 Child가 아니라 반대 방향의 Child를 비교해야 한다.

```text
왼쪽의 왼쪽   ↔ 오른쪽의 오른쪽
왼쪽의 오른쪽 ↔ 오른쪽의 왼쪽
```

* Tree 재귀에서 기존 함수의 Parameter만으로 필요한 정보를 전달할 수 없다면 보조 함수를 사용할 수 있다.

* 두 Subtree를 비교하는 문제에서는 두 Node를 Parameter로 받는 재귀 함수를 생각할 수 있다.

* 재귀에서는 Base Case를 이용해 두 Node가 모두 `nullptr`인 경우, 하나만 `nullptr`인 경우, 값이 다른 경우를 처리할 수 있다.

* BFS/Iteration 풀이에서는 Queue에 단순히 Node를 저장하는 것이 아니라 서로 비교해야 하는 Node를 Pair 형태로 저장할 수 있다.

* 재귀에서 Call Stack에 저장되던 다음 작업을 반복 풀이에서는 Queue와 같은 자료구조에 직접 저장할 수 있다.

* 이 문제 역시 Level별 결과가 필요한 문제가 아니므로 `q.size()`를 이용한 Level Size 처리는 필요하지 않다.

### 복잡도

* DFS 시간복잡도: `O(n)`
* DFS 공간복잡도: `O(height)`
* BFS 시간복잡도: `O(n)`
* BFS 공간복잡도: `O(n)`


# 0112. Path Sum (26.9.28)

#### 유형

* Tree(树)
* DFS(深度优先搜索)
* Recursion(递归)

#### 풀이

* Root에서 Leaf까지 내려가는 경로 중 Node 값의 합이 `targetSum`과 같은 경로가 존재하는지 확인한다.

* 현재까지의 합을 따로 저장하는 대신, 현재 Node의 값을 `targetSum`에서 빼면서 **앞으로 필요한 합**을 다음 재귀 호출에 전달한다.

* 현재 Node가 `nullptr`이면 더 이상 경로가 존재하지 않으므로 `false`를 반환한다.

```cpp
if (!root) return false;
```

* 현재 Node가 Leaf라면 현재 Node의 값이 남아 있는 `targetSum`과 같은지 확인한다.

```text
현재 Node가 Leaf
        ↓
root->val == targetSum ?
        ↓
true / false
```

* Leaf인지 확인하기 위해 왼쪽 Child와 오른쪽 Child가 모두 `nullptr`인지 확인한다.

```cpp
!root->left && !root->right
```

* 현재 Node가 Leaf가 아니라면 현재 Node의 값을 `targetSum`에서 빼고 왼쪽과 오른쪽 Subtree를 탐색한다.

```cpp
hasPathSum(root->left, targetSum - root->val)

hasPathSum(root->right, targetSum - root->val)
```

* 왼쪽 또는 오른쪽 Subtree 중 하나라도 조건을 만족하는 경로가 존재하면 전체 결과는 `true`이다.

```text
Left Subtree
     \
      OR → true
     /
Right Subtree
```

#### Leaf를 확인해야 하는 이유

* 이 문제는 단순히 경로 중간에서 합이 `targetSum`이 되는지를 확인하는 문제가 아니다.

* 반드시 **Root에서 Leaf까지의 경로**여야 한다.

* 따라서 현재까지의 합이 `targetSum`과 같더라도 현재 Node가 Leaf가 아니라면 정답이 아니다.

* 따라서 값뿐만 아니라 Leaf 조건도 함께 확인해야 한다.

```cpp
root->val == targetSum
&& !root->left
&& !root->right
```

#### BFS 풀이

* BFS로도 해결할 수 있다.

* BFS에서는 Queue에 방문할 Node뿐만 아니라 **Root에서 해당 Node까지의 경로 합**도 함께 저장해야 한다.

```text
Node Queue
[node1] [node2] [node3]

Sum Queue
[sum1 ] [sum2 ] [sum3 ]
```

* Node를 Queue에서 꺼낼 때 해당 Node까지의 경로 합도 함께 꺼낸다.

* 현재 Node가 Leaf이고 경로 합이 `targetSum`과 같다면 `true`를 반환한다.

* Child를 Queue에 넣을 때 현재까지의 경로 합에 Child의 값을 더해서 함께 저장한다.

* 이 문제는 Level별 결과가 필요한 문제가 아니므로 `q.size()`를 이용한 Level Size 처리는 필요하지 않다.

* 다만 이 문제는 Root에서 Leaf까지 하나의 경로를 끝까지 탐색하는 구조이므로 **DFS 재귀 풀이가 더 직접적이고 간단하다.**

#### 배운 점

* Root → Leaf 경로 문제에서는 단순히 목표값에 도달했는지만 확인하지 않고 **현재 Node가 Leaf인지 반드시 확인**해야 한다.

* Tree에서 하나의 경로를 끝까지 내려가며 조건을 확인하는 문제에서는 DFS를 먼저 생각할 수 있다.

* 경로 합을 직접 누적하는 대신 `targetSum - root->val`처럼 **남은 목표값을 재귀 Parameter로 전달**할 수 있다.

* Tree 재귀에서는 현재 호출에 필요한 상태를 Parameter로 넘기면서 탐색할 수 있다.

* 재귀의 기본 구조를 다음과 같이 생각할 수 있다.

```text
1. nullptr인가?
   → false

2. Leaf인가?
   → 현재 값과 남은 target 비교

3. Leaf가 아니라면?
   → target에서 현재 값을 빼고
      Left / Right Subtree 탐색
```

* BFS에서는 Node만 저장하는 것이 아니라 각 Node에 대응하는 **경로 합과 같은 추가 상태**도 Queue에 함께 저장할 수 있다.

* 모든 Tree 문제를 DFS와 BFS 두 가지 방법으로 구현할 필요는 없다. 이 문제에서는 DFS 재귀 풀이를 확실하게 구현할 수 있으면 충분하다.

#### 복잡도

* DFS 시간복잡도: `O(n)`
* DFS 공간복잡도: `O(height)`
* BFS 시간복잡도: `O(n)`
* BFS 공간복잡도: `O(n)`


# 0110. Balanced Binary Tree (26.9.29)

#### 유형

* Tree(树)
* DFS(深度优先搜索)
* Recursion(递归)
* Postorder Traversal(后序遍历)

#### 처음 접근

* 처음에는 Balanced Binary Tree도 왼쪽과 오른쪽 Subtree가 모두 Balanced이면 된다고 생각하여 다음과 같이 재귀 구조를 만들었다.

```cpp
bool isBalanced(TreeNode* root) {
    if (!root) return true;

    return isBalanced(root->left)
        && isBalanced(root->right);
}
```

* 하지만 이 코드는 각 Subtree를 재귀적으로 방문할 뿐, **왼쪽과 오른쪽 Subtree의 높이 차이**를 확인하지 않는다.

* Balanced Binary Tree의 조건은 모든 Node에서 다음을 만족하는 것이다.

```text
|Left Height - Right Height| <= 1
```

* 따라서 단순히 왼쪽과 오른쪽 Subtree를 재귀적으로 방문하는 것만으로는 부족하고, 각 Node에서 두 Subtree의 Height도 확인해야 한다.

#### 1차 수정 — Height 계산 추가

* `depth()` 함수를 만들어 Tree의 Height를 구했다.

```cpp
int depth(TreeNode* root, int cur_depth) {
    if (!root) return cur_depth;

    cur_depth++;

    int left_depth = depth(root->left, cur_depth);
    int right_depth = depth(root->right, cur_depth);

    return left_depth >= right_depth ? left_depth : right_depth;
}
```

* 그리고 현재 Node에서 왼쪽과 오른쪽 Subtree의 높이 차이를 계산했다.

```cpp
int depth_diff = depth(root->left, 0)
               - depth(root->right, 0);
```

* 현재 Node의 높이 차이가 `1` 이하이고, 왼쪽과 오른쪽 Subtree도 모두 Balanced인지 재귀적으로 확인했다.

```cpp
return (depth_diff >= -1) && (depth_diff <= 1)
    && isBalanced(root->left)
    && isBalanced(root->right);
```

* 이 방법으로 정답을 구할 수 있다.

* 하지만 `isBalanced()`를 호출할 때마다 `depth()`를 다시 호출하기 때문에 **같은 Node의 Height를 반복해서 계산하는 문제**가 있다.

#### 1차 풀이의 시간복잡도 문제

* 한쪽으로 치우친 Tree를 생각하면 중복 계산이 쉽게 보인다.

```text
1
 \
  2
   \
    3
     \
      4
       \
        5
```

* Root에서 Height를 계산할 때 아래쪽 Node들을 모두 탐색한다.

* 이후 `isBalanced()`가 다음 Node로 이동하면 그 Node를 기준으로 다시 Height를 계산한다.

```text
1번 Node → 약 n개 탐색
2번 Node → 약 n-1개 탐색
3번 Node → 약 n-2개 탐색
...
마지막 Node → 1개 탐색
```

* 따라서 최악의 경우 전체 연산량은

```text
n + (n-1) + (n-2) + ... + 1
```

이 된다.

* 따라서 1차 풀이의 최악의 시간복잡도는 `O(n²)`이다.

#### 개선 아이디어

* 기존에는 두 가지 작업을 따로 수행했다.

```text
depth()
→ Height 계산

isBalanced()
→ Balanced 여부 확인
```

* 하지만 `depth()`가 이미 왼쪽과 오른쪽 Subtree를 끝까지 탐색하므로, **Height를 계산하는 과정에서 Balanced 여부도 같이 확인할 수 있다.**

* 이를 위해 `depth()`의 반환값에 두 가지 의미를 부여한다.

```text
0 이상의 값
→ 현재 Subtree는 Balanced
→ 반환값은 Subtree의 Height

-1
→ 현재 Subtree는 Unbalanced
```

* 정상적인 Height는 음수가 될 수 없기 때문에 `-1`을 Unbalanced 상태를 나타내는 특별한 값으로 사용할 수 있다.

#### 2차 수정 — Bottom-up 방식

* 먼저 왼쪽 Subtree의 Height를 구한다.

```cpp
int left = depth(root->left);
```

* 왼쪽 Subtree에서 이미 Unbalanced가 발견되어 `-1`이 반환되었다면 현재 Tree도 Unbalanced이다.

```cpp
if (left == -1) return -1;
```

* 오른쪽도 같은 방식으로 확인한다.

```cpp
int right = depth(root->right);
if (right == -1) return -1;
```

* 두 Subtree가 모두 Balanced라면 Height 차이를 확인한다.

```cpp
if (abs(left - right) > 1) return -1;
```

* Height 차이도 정상이라면 현재 Subtree의 Height를 반환한다.

```cpp
return (left >= right ? left : right) + 1;
```

* 따라서 `depth()` 하나에서 다음 두 작업을 동시에 수행하게 된다.

```text
            depth(root)
                 ↓
       Left / Right 먼저 탐색
                 ↓
        Balanced 여부 확인
             /       \
        Unbalanced   Balanced
            ↓           ↓
           -1       Height 반환
```

#### 최종 풀이

```cpp
int depth(TreeNode* root) {
    if (!root) return 0;

    int left = depth(root->left);
    if (left == -1) return -1;

    int right = depth(root->right);
    if (right == -1) return -1;

    if (abs(left - right) > 1) return -1;

    return (left >= right ? left : right) + 1;
}
```

* 이제 `depth(root)` 자체가 전체 Tree의 Balanced 여부까지 검사하기 때문에 `isBalanced()`에서 다시 왼쪽과 오른쪽 Subtree를 재귀적으로 확인할 필요가 없다.

```cpp
bool isBalanced(TreeNode* root) {
    return depth(root) != -1;
}
```

#### Bottom-up / Postorder

* 최종 풀이에서는 현재 Node를 판단하기 전에 먼저 Left와 Right Subtree의 결과를 얻는다.

```text
Left Subtree
     ↓
Right Subtree
     ↓
현재 Node 판단
```

* 즉 Child의 결과를 먼저 계산한 후 Parent의 결과를 결정하는 **Bottom-up(自底向上)** 방식이다.

* Tree Traversal 관점에서는 다음 순서이므로 Postorder Traversal(后序遍历) 구조와 같다.

```text
Left
 ↓
Right
 ↓
Root
```

* 아래쪽에서 Unbalanced가 발견되면 `-1`이 Parent 방향으로 계속 전달된다.

```text
Unbalanced 발견
       ↓
      -1
       ↓
    Parent
       ↓
      -1
       ↓
    Parent
       ↓
      ...
       ↓
     Root
```

#### 배운 점

* 처음에는 Tree 문제에서 왼쪽과 오른쪽 Subtree를 단순히 재귀적으로 확인하면 된다고 생각했지만, 문제의 조건이 **Subtree의 Height 정보까지 필요로 한다는 것**을 확인해야 했다.

* Tree의 Height는 다음 재귀식으로 구할 수 있다.

```text
nullptr
→ 0

일반 Node
→ max(Left Height, Right Height) + 1
```

* `isBalanced()`에서 매번 `depth()`를 호출하는 방식도 정답이지만, 같은 Subtree의 Height를 반복해서 계산하여 최악의 경우 `O(n²)`이 된다.

* `1 + 2 + ... + n` 형태의 연산량은 `O(n²)`으로 볼 수 있다.

* 재귀 함수가 이미 Subtree 전체를 탐색하고 있다면, **그 과정에서 필요한 다른 정보도 함께 계산할 수 있는지 생각해볼 수 있다.**

* 이번 문제에서는 `depth()`가 단순히 Height만 반환하는 것이 아니라 다음 두 정보를 동시에 표현하도록 개선했다.

```text
Height >= 0 → Balanced + Height 정보
-1          → Unbalanced 상태
```

* 즉 재귀 함수의 반환값을 **계산 결과 + 상태 전달** 용도로 사용할 수 있다.

* Child의 결과를 이용해서 Parent의 결과를 결정하는 문제에서는 Bottom-up 재귀와 Postorder Traversal 구조를 생각할 수 있다.

#### 복잡도

* 처음 완성한 Top-down 풀이
  * 시간복잡도: 최악의 경우 `O(n²)`
  * 공간복잡도: `O(height)`

* 최종 Bottom-up 풀이
  * 시간복잡도: `O(n)`
  * 공간복잡도: `O(height)`
  * 한쪽으로 치우친 Tree에서는 `height = n`이므로 최악의 공간복잡도는 `O(n)`