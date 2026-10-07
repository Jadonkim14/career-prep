# 0733. Flood Fill (26.10.2)

## 유형

- Graph(图)
- DFS(深度优先搜索)
- BFS(广度优先搜索)
- Recursion(递归)
- Matrix / Grid Traversal(矩阵遍历)

## 처음 접근

- 시작 픽셀의 색상을 변경한 뒤, 상하좌우 인접 픽셀을 재귀적으로 탐색하는 DFS 방식으로 접근했다.
- 처음에는 인접 픽셀의 색상을 확인하지 않고 재귀 호출했다.

```cpp
image[sr][sc] = color;

if (sr - 1 >= 0)
    floodFill(image, sr - 1, sc, color);
```

- **시작 픽셀과 원래 색상이 같고, 상하좌우로 연결된 픽셀만 변경해야 한다.**

## 1차 수정 — 원래 색상 검사 추가

- 현재 픽셀의 원래 색상을 `cur_color`에 저장하고, 인접 픽셀의 색상이 같을 때만 DFS를 수행하도록 수정했다.

```cpp
int cur_color = image[sr][sc];
image[sr][sc] = color;

if (sr - 1 >= 0 && image[sr - 1][sc] == cur_color)
    floodFill(image, sr - 1, sc, color);
```

- 이 방식으로 정답을 구할 수 있었다.
- 이미 방문한 픽셀은 새로운 색상으로 변경되므로 다시 탐색하지 않는다.
- 따라서 별도의 `visited` 배열이 필요하지 않다.

## 개선 — 불필요한 반환값 대입 제거

- 처음에는 재귀 호출 결과를 다시 `image`에 대입했다.

```cpp
image = floodFill(image, sr - 1, sc, color);
```

- 하지만 `image`는 참조(`&`)로 전달되므로 함수 내부의 변경 사항이 원본에 반영된다.

```cpp
floodFill(image, sr - 1, sc, color);
```

- C++에서는 반환 타입이 `void`가 아니더라도 반환값을 무시할 수 있다.
- 사용하지 않는 임시 `vector` 객체는 자동으로 소멸하므로 메모리 누수가 발생하지 않는다.
- 다만 값으로 반환하는 과정에서 불필요한 복사 비용이 발생할 수 있다.

## C++ 메모리 관리 — 반환값과 메모리 누수

- 반환값을 무시한다고 반드시 메모리 누수가 발생하는 것은 아니다.

- 반환된 임시 `vector`는 자동으로 소멸하며 소유한 메모리도 해제된다.

- 반면 다음 코드는 메모리 누수가 발생한다.

```cpp
int* func() {
    return new int(10);
}

int main() {
    func();
}
```

- `new`로 할당한 메모리의 주소를 잃어버려 `delete`할 수 없기 때문이다.
- 핵심은 **동적 메모리의 소유권을 올바르게 관리하는 것**이다.

## 공식 풀이 — DFS

- 공식 풀이에서는 `floodFill()`과 `dfs()`를 분리하고, 최초 색상을 모든 재귀 호출에 전달한다.
- 또한 방향 배열을 사용해 상하좌우 이동을 처리한다.

const int dx[4] = {1, 0, 0, -1};
const int dy[4] = {0, 1, -1, 0};

- 직접 작성한 코드와 공식 풀이 모두 올바른 DFS 구현이다.
- 공식 풀이는 원래 색상을 명시적으로 전달하고, 방향 배열을 사용해 중복 코드를 줄였다.

## 공식 풀이 — BFS

- BFS는 재귀 호출 대신 `queue`를 사용한다.
- 큐에서 좌표를 꺼내 상하좌우를 탐색하고, 조건에 맞는 픽셀을 다시 큐에 삽입한다.
- **BFS에서는 큐에 삽입하는 순간 방문 처리하는 것이 중요하다.**

```cpp
que.emplace(mx, my);
image[mx][my] = color;
```

- 큐에서 꺼낼 때 방문 처리하면 동일한 픽셀이 여러 번 삽입될 수 있다.
- 삽입과 동시에 색상을 변경하면 중복 방문을 방지할 수 있다.

## DFS / BFS 선택 기준

| 문제 유형 | 적합한 방식 |
|---|---|
| 연결된 영역 탐색 | DFS / BFS |
| 섬의 개수 및 크기 계산 | DFS / BFS |
| 가중치 없는 그래프 최단 거리 | BFS |
| 단계별 확산 | BFS |
| 백트래킹 / 조합 탐색 | 일반적으로 DFS |
| 재귀 깊이가 지나치게 큰 경우 | 반복형 DFS / BFS |

- Flood Fill에서는 DFS와 BFS 모두 자연스러운 풀이이다.
- DFS는 재귀 구조로 간단하게 구현할 수 있고, BFS는 큐를 이용해 탐색한다.
- 모든 문제를 두 방식으로 풀기보다는 문제의 특성에 따라 적절한 방법을 선택하는 것이 중요하다.

## 실행시간 비교

```text
직접 작성한 DFS → 4ms
공식 DFS → 3ms
다른 DFS 풀이 → 1ms 미만
```

- 세 코드 모두 시간복잡도는 `O(m × n)`이다.
- LeetCode 실행시간은 측정 환경과 컴파일러 최적화 등의 영향을 받으므로 수 ms 차이만으로 성능을 판단하기 어렵다.
- 코딩테스트에서는 미세한 실행시간 차이보다 **시간복잡도와 불필요한 연산을 줄이는 것**이 중요하다.

## 배운 점

- Flood Fill은 같은 색상으로 연결된 영역을 탐색하는 문제이며 DFS와 BFS 모두 사용할 수 있다.
- 2차원 배열 탐색에서는 **경계 검사, 색상 검사, 방문 처리**가 중요하다.
- 이번 문제에서는 색상 변경 자체를 방문 처리로 사용했다.
- 시작 색상과 목표 색상이 같으면 즉시 반환해야 한다.
- `dx`, `dy` 방향 배열을 사용하면 상하좌우 탐색을 간결하게 구현할 수 있다.
- 참조(`&`)로 전달한 객체는 함수 내부에서 수정하면 원본에 반영되므로 반환값을 다시 대입할 필요가 없다.
- BFS에서는 큐에 삽입하는 순간 방문 처리하여 중복 탐색을 방지한다.
- DFS와 BFS 중 하나만 익히기보다 문제의 특성에 맞게 선택할 수 있어야 한다.
- 실행시간의 작은 차이보다 알고리즘의 시간복잡도를 이해하는 것이 중요하다.

## 복잡도

- DFS
  - 시간복잡도: `O(m × n)`
  - 공간복잡도: `O(m × n)` — 최악의 경우 재귀 호출 스택

- BFS
  - 시간복잡도: `O(m × n)`
  - 공간복잡도: `O(m × n)` — 최악의 경우 큐

- `m`은 이미지의 행 수, `n`은 열 수이다.

# 0200. Number of Islands (26.10.3 ~ 4)

## 유형

- Graph(图)
- DFS(深度优先搜索)
- BFS(广度优先搜索)
- Union-Find(并查集)
- Connected Components(连通分量)
- Matrix / Grid Traversal(矩阵遍历)

## 처음 접근

- 방문하지 않은 육지(`'1'`)를 발견하면 `sum++`을 실행하고 DFS로 연결된 모든 육지를 방문 처리했다.
- 상하좌우 탐색을 위해 `dx`, `dy` 방향 배열을 사용했다.
- 방문 여부는 별도의 `isVisited` 배열로 관리했다.

```cpp
if (grid[i][j] == '1' && !isVisited[i][j]) {
    sum++;
    visitIsl(grid, isVisited, i, j);
}
```

- **하나의 섬에 대해 `sum++`은 한 번만 실행되어야 한다.**
- DFS로 해당 섬 전체를 방문 처리하므로 중복 계산을 방지할 수 있다.

## 1차 오류 — 방문 배열 초기화 누락

처음에는 다음과 같이 선언했다.

```cpp
vector<vector<bool>> isVisited;
```

- 빈 벡터이므로 `isVisited[i][j]`에 접근하면 Undefined Behavior(未定义行为)가 발생한다.
- 입력 크기에 맞춰 초기화하여 해결했다.

```cpp
vector<vector<bool>> isVisited(m, vector<bool>(n, false));
```

- 또는 문제에서 제시된 최대 크기의 고정 배열을 사용할 수 있다.

## 공식 풀이 — DFS

- 공식 풀이에서는 별도의 방문 배열 대신 원본 `grid`를 수정한다.

```cpp
grid[r][c] = '0';
```

- 방문한 육지를 물로 변경해 중복 탐색을 방지한다.
- 별도 방문 배열은 필요 없지만 원본 데이터가 변경된다.
- 재귀 호출 스택 때문에 최악의 공간복잡도는 여전히 `O(MN)`이다.
- 재귀 깊이가 지나치게 깊으면 Stack Overflow(栈溢出)가 발생할 수 있다.

## 공식 풀이 — BFS

- BFS는 재귀 호출 대신 `queue`를 사용한다.
- 큐에서 좌표를 꺼내 상하좌우를 탐색하고, 연결된 육지를 큐에 추가한다.
- **큐에 삽입하는 순간 방문 처리해야 중복 삽입을 방지할 수 있다.**

```cpp
neighbors.push({row - 1, col});
grid[row - 1][col] = '0';
```

- 공식 BFS 구현은 원본 `grid`를 수정하므로 별도 방문 배열이 필요 없다.
- 재귀 호출이 없어 스택 오버플로 위험을 피할 수 있다.

## 직접 구현 — BFS 최적화 과정

**① DFS → BFS 전환**

- 기존 DFS의 재귀 호출을 `queue`를 사용하는 반복문으로 변경했다.
- 처음에는 `isVisited` 배열을 그대로 유지했다.

```cpp
queue<pair<int, int>> q;
q.push({row, col});

while (!q.empty()) {
    pair<int, int> p = q.front();
    q.pop();

    // Explore four directions
}
```

- 시작점과 새로 발견한 육지는 큐에 삽입할 때 방문 처리했다.
- 시간복잡도 `O(MN)`, 공간복잡도 `O(MN)`.

**② BFS + visited → BFS + grid 수정**

- 별도의 `isVisited` 배열을 제거했다.
- 방문한 육지를 `'0'`으로 변경하여 중복 탐색을 방지했다.

```cpp
grid[row][col] = '0';
q.push({row, col});
```

인접한 육지를 발견했을 때도 동일하게 처리했다.

```cpp
if (newRow >= 0 && newRow < m &&
    newCol >= 0 && newCol < n &&
    grid[newRow][newCol] == '1') {

    grid[newRow][newCol] = '0';
    q.push({newRow, newCol});
}
```

- 시간복잡도는 `O(MN)`으로 동일하다.
- 공간복잡도는 `O(MN)`에서 `O(min(M,N))`으로 개선되었다.
- 단, 원본 `grid`가 변경된다.

## C++ 문법 — pair와 queue

```cpp
queue<pair<int, int>> q;
```

- `pair<int, int>`: 두 정수를 하나의 객체로 저장한다.
- `queue<pair<int, int>>`: 정수 쌍을 FIFO(先进先出) 순서로 관리한다.
- BFS에서는 주로 `(row, col)` 좌표를 저장한다.

```cpp
q.push({2, 3});  // Insert
q.emplace(4, 5); // Construct and insert

pair<int, int> p = q.front();

int row = p.first;
int col = p.second;

q.pop(); // Remove front element
```

- `front()`: 맨 앞 원소 확인
- `pop()`: 맨 앞 원소 제거 (반환값 없음)
- `push()`: 원소 삽입
- `emplace()`: 원소를 큐 내부에서 직접 생성
- `empty()`: 큐가 비었는지 확인

## C++17 문법 — Structured Binding

구조적 바인딩(结构化绑定)은 `pair` 등의 객체를 여러 변수로 분리하는 문법이다.

기존 방식:

```cpp
pair<int, int> p = q.front();
int row = p.first;
int col = p.second;
```

C++17 방식:

```cpp
auto [row, col] = q.front();
q.pop();
```

- `auto`가 각 변수의 자료형을 추론한다.
- `row`에는 `first`, `col`에는 `second` 값이 저장된다.
- 기본 `auto`는 값을 복사하므로 이후 `q.pop()`을 호출해도 변수는 유효하다.
- 성능 최적화보다는 가독성 개선에 해당한다.

## 공식 풀이 — Union-Find

- Union-Find(并查集)는 연결된 육지를 같은 집합으로 병합하는 방법이다.
- `find()`로 대표 노드를 찾고 `unite()`로 두 집합을 합친다.
- 서로 다른 집합을 병합할 때마다 연결 요소 개수를 하나 감소시킨다.
- 경로 압축(路径压缩)과 랭크 기반 합병(按秩合并)을 사용한다.
- 이번 문제에서는 DFS/BFS보다 구현이 복잡하다.

## DFS / BFS / Union-Find 비교

| 구현 방식 | 시간복잡도 | 공간복잡도 |
|---|---|---|
| DFS + isVisited | O(MN) | O(MN) |
| DFS + grid 수정 | O(MN) | O(MN) |
| BFS + isVisited | O(MN) | O(MN) |
| BFS + grid 수정 | O(MN) | O(min(M,N)) |
| Union-Find | O(MNα(MN)) | O(MN) |

- BFS의 `O(min(M,N))`은 4방향 격자에서 발견 즉시 방문 처리하는 구현 기준이다.
- BFS 큐에는 방문한 모든 칸이 아니라 **앞으로 탐색할 경계 부근의 칸**만 저장된다.
- DFS는 방문 배열을 제거해도 재귀 호출 스택이 필요하다.
- BFS도 별도 방문 배열을 사용하면 전체 공간복잡도가 `O(MN)`이 된다.

## DFS / BFS 선택 기준

| 문제 유형 | 적합한 방식 |
|---|---|
| 연결 요소 탐색 | DFS / BFS |
| 섬 개수 및 넓이 계산 | DFS / BFS |
| 가중치 없는 그래프 최단 거리 | BFS |
| 단계별 확산 | BFS |
| 백트래킹 / 조합 탐색 | 일반적으로 DFS |
| 깊은 재귀가 예상되는 경우 | 반복형 DFS / BFS |

- 이번 문제에서는 DFS와 BFS 모두 적절하다.
- 원본 수정이 가능하다면 BFS + grid 수정은 공간 효율성과 재귀 안정성 측면에서 유리하다.
- 구현이 간단한 DFS도 충분히 적절한 풀이이다.

## 시간복잡도와 실제 실행시간

- 시간복잡도는 입력 크기에 따른 연산량의 증가 추세를 나타낸다.
- 실제 실행시간은 함수 호출, 메모리 접근, 자료구조, 컴파일러 최적화 등에 영향을 받는다.
- **시간복잡도가 낮다고 모든 입력에서 반드시 빠른 것은 아니다.**
- DFS와 BFS는 모두 `O(MN)`이지만 실제 실행시간은 다를 수 있다.
- 코딩테스트에서는 미세한 실행시간보다 시간·공간 제한과 구현 안정성을 우선한다.

## 배운 점

- 섬의 개수를 구하는 것은 연결 요소(连通分量)의 개수를 구하는 것과 같다.
- DFS/BFS로 하나의 섬 전체를 방문 처리하면 중복 계산을 방지할 수 있다.
- `vector`는 선언만 하면 비어 있으므로 사용 전에 크기를 초기화해야 한다.
- 방문 배열 대신 원본 `grid`를 수정해 추가 메모리를 절약할 수 있다.
- BFS에서는 큐에 삽입하는 순간 방문 처리해야 한다.
- `queue<pair<int, int>>`로 2차원 좌표를 관리할 수 있다.
- C++17의 `auto [row, col]`로 `pair`의 값을 간결하게 분리할 수 있다.
- BFS 큐에는 전체 방문 기록이 아닌 탐색 대기 좌표만 저장된다.
- DFS는 재귀 호출 스택, BFS는 큐를 사용한다.
- Union-Find는 연결된 육지를 같은 집합으로 병합하는 또 다른 방법이다.
- 알고리즘 선택 시 시간복잡도뿐 아니라 공간복잡도와 구현 안정성도 고려해야 한다.

## 복잡도

- **DFS**
  - 시간복잡도: `O(MN)`
  - 공간복잡도: `O(MN)` — 재귀 호출 스택 및 선택적 방문 배열

- **BFS (grid 수정)**
  - 시간복잡도: `O(MN)`
  - 공간복잡도: `O(min(M,N))` — BFS 큐

- **Union-Find**
  - 시간복잡도: `O(MNα(MN))`
  - 공간복잡도: `O(MN)` — parent, rank 배열

- `M`은 행 수, `N`은 열 수이다.

# 0695. Max Area of Island (26.10.7)

## 유형

- Graph(图)
- DFS(深度优先搜索)
- BFS(广度优先搜索)
- Connected Components(连通分量)
- Matrix / Grid Traversal(矩阵遍历)

## 처음 접근

- 방문하지 않은 육지(`1`)를 발견하면 DFS를 시작했다.
- DFS가 연결된 모든 육지를 방문하면서 해당 섬의 넓이를 반환하도록 구현했다.
- 방문 여부는 별도의 배열 대신 원본 `grid`를 `0`으로 변경해 처리했다.

```cpp
if (grid[i][j] == 1) {
    int temp = visit(grid, i, j);

    if (temp > maxArea) {
        maxArea = temp;
    }
}
```

- `Number of Islands`가 섬의 개수를 세는 문제라면, 이번 문제는 **각 Connected Component의 크기를 계산하는 문제**이다.

## 직접 구현 — DFS

DFS 함수는 현재 위치와 연결된 육지의 개수를 반환한다.

```cpp
int visit(vector<vector<int>>& grid, int r, int c) {
    grid[r][c] = 0;
    int sizeIsl = 1;

    for (int i = 0; i < 4; i++) {
        int newRow = r + dr[i];
        int newCol = c + dc[i];

        if (newRow < 0 || newRow >= grid.size() ||
            newCol < 0 || newCol >= grid[0].size()) {
            continue;
        }

        if (grid[newRow][newCol] == 1) {
            sizeIsl += visit(grid, newRow, newCol);
        }
    }

    return sizeIsl;
}
```

핵심은 다음 부분이다.

```cpp
sizeIsl += visit(grid, newRow, newCol);
```

- 현재 칸의 넓이를 `1`로 시작한다.
- 연결된 육지를 DFS로 탐색한다.
- 각 재귀 호출이 반환한 면적을 누적한다.
- 최종적으로 하나의 섬 전체 면적을 반환한다.

## 직접 구현 — BFS

DFS의 재귀 호출을 `queue` 기반 반복문으로 변경했다.

```cpp
queue<pair<int,int>> q;

grid[r][c] = 0;
q.push({r, c});

int sizeIsl = 1;
```

큐가 빌 때까지 상하좌우를 탐색한다.

```cpp
while (!q.empty()) {
    pair<int,int> cur = q.front();
    q.pop();

    for (int i = 0; i < 4; i++) {
        int newRow = cur.first + dr[i];
        int newCol = cur.second + dc[i];

        if (newRow < 0 || newRow >= gridRow ||
            newCol < 0 || newCol >= gridCol) {
            continue;
        }

        if (grid[newRow][newCol] == 1) {
            grid[newRow][newCol] = 0;
            q.push({newRow, newCol});
            sizeIsl++;
        }
    }
}
```

- 새로운 육지를 발견하면 즉시 `0`으로 변경한다.
- **큐에 삽입하는 순간 방문 처리해야 중복 삽입을 방지할 수 있다.**
- 새로운 육지를 하나 발견할 때마다 `sizeIsl++` 한다.

## DFS / BFS 차이

| 방식 | 면적 계산 방법 | 사용 자료구조 |
|---|---|---|
| DFS | 재귀 반환값을 누적 | Call Stack |
| BFS | 새 육지를 발견할 때 카운트 | Queue |

DFS:

```cpp
sizeIsl += visit(grid, newRow, newCol);
```

BFS:

```cpp
grid[newRow][newCol] = 0;
q.push({newRow, newCol});
sizeIsl++;
```

두 방식 모두 하나의 섬 전체를 방문한 뒤 그 면적을 계산한다.

## 0200. Number of Islands와 차이

`Number of Islands`:

```text
DFS/BFS 한 번 시작
→ 섬 하나 전체 방문
→ 섬 개수 +1
```

`Max Area of Island`:

```text
DFS/BFS 한 번 시작
→ 섬 하나 전체 방문
→ 방문한 칸의 개수 계산
→ 최대값 갱신
```

즉 두 문제 모두 Connected Component 탐색 문제이지만,

- `0200`은 **Connected Component의 개수**
- `0695`는 **Connected Component의 최대 크기**

를 구한다.

## 방문 처리

별도의 `visited` 배열 대신 원본 `grid`를 수정했다.

```cpp
grid[r][c] = 0;
```

장점:

- 별도 방문 배열이 필요 없다.
- 이미 방문한 육지를 다시 탐색하지 않는다.

단점:

- 원본 `grid`가 변경된다.

원본 데이터를 보존해야 한다면 별도의 `visited` 배열을 사용해야 한다.

## DFS / BFS 선택 기준

| 상황 | 적합한 방식 |
|---|---|
| 연결 요소 탐색 | DFS / BFS |
| 섬의 개수 계산 | DFS / BFS |
| 섬의 넓이 계산 | DFS / BFS |
| 가중치 없는 최단 거리 | BFS |
| 단계별 확산 | BFS |
| 깊은 재귀가 위험한 경우 | 반복형 DFS / BFS |

이번 문제에서는 DFS와 BFS 모두 적절하다.

## 배운 점

- 섬 하나는 하나의 Connected Component(连通分量)로 볼 수 있다.
- DFS 함수가 단순 방문뿐 아니라 **연결된 영역의 크기를 반환하도록 설계할 수 있다.**
- BFS에서는 큐에 삽입하는 순간 방문 처리해야 한다.
- 원본 `grid`를 수정하면 별도의 방문 배열을 제거할 수 있다.
- DFS는 재귀 호출 스택을 사용하고, BFS는 `queue`를 사용한다.
- `Number of Islands`와 기본 탐색 구조는 같고, 무엇을 계산하느냐만 다르다.
- DFS/BFS 모두 모든 칸을 최대 한 번 방문하므로 시간복잡도는 `O(MN)`이다.

## 복잡도

### DFS

- 시간복잡도: `O(MN)`
- 공간복잡도: `O(MN)`
  - 최악의 경우 재귀 호출 깊이가 전체 육지 수까지 증가할 수 있다.

### BFS

- 시간복잡도: `O(MN)`
- 공간복잡도: `O(MN)`
  - 최악의 경우 큐에 많은 탐색 대기 좌표가 저장될 수 있다.

`M`은 행 수, `N`은 열 수이다.