## 0217. Contains Duplicate (26.8.13)

### 유형
- Array
- Hash Table
- `unordered_set`

### 처음 풀이
- `unordered_map<int, bool>`로 숫자의 등장 여부를 저장
- 이미 `true`이면 중복으로 판단

### 개선점

* 이 문제는 `key → value`가 아니라 **숫자의 존재 여부만 필요**하다.
* 따라서 `unordered_map<int, bool>`보다 `unordered_set<int>`이 더 적절하다.
* `hash[num]`은 key가 없으면 `{num: false}`를 새로 생성한다.

### 최적 풀이

* `unordered_set`
* 평균 `O(n)`, 공간 `O(n)`

```cpp
unordered_set<int> seen;

for (int num : nums) {
    if (seen.count(num)) return true;
    seen.insert(num);
}

return false;
```

### 배운 점

* `unordered_map` → `key → value`가 필요할 때 사용
* `unordered_set` → 값의 존재 여부만 필요할 때 사용
* 둘 다 Hash Table 기반으로 검색/삽입이 평균 `O(1)`
* `seen.count(num)` → 존재하면 `1`, 없으면 `0`
* `unordered_map<int, bool> hash;`는 처음에는 빈 상태 `{}`이다.
* `hash[num]`에서 key가 없으면 value가 기본값으로 생성된다.

  * `bool` → `false`
  * `int` → `0`
  * `string` → `""`
* 존재 여부만 확인하면서 새 key를 만들고 싶지 않다면 `find()` 또는 C++20의 `contains()`를 사용한다.


## 0242. Valid Anagram (26.8.15)

### 유형

* String
* Hash Table
* Frequency Array(频次数组)

### 처음 풀이

* `unordered_map<char, int>`로 각 문자의 등장 횟수를 저장
* `s`의 문자는 `+1`, `t`의 문자는 `-1`
* 마지막에 `a~z`의 값이 모두 `0`인지 확인하여 애너그램 여부를 판단

### 개선점

* 문제 조건에서 `s`, `t`는 **소문자 알파벳 26개만 포함**한다.
* 따라서 `unordered_map<char, int>`보다 크기 26의 배열 또는 `vector<int>`가 더 단순하고 적절하다.
* `ch - 'a'`를 이용하면 `'a'~'z'`를 `0~25`의 인덱스로 변환할 수 있다.
* 두 문자열의 길이가 다르면 애너그램일 수 없으므로 처음에 바로 `false`를 반환할 수 있다.
* `t`의 빈도를 빼는 과정에서 값이 `0`보다 작아지면 `t`에 해당 문자가 더 많이 존재한다는 뜻이므로 바로 `false`를 반환할 수 있다.

### 배운 점

* **字母异位词(Anagram)** → 문자의 순서는 달라도 각 문자의 종류와 등장 횟수가 동일한 문자열
* 문자의 **빈도(频次)**를 비교하면 애너그램인지 판단할 수 있다.
* `table[ch - 'a']` → 문자 `ch`에 해당하는 빈도 저장 위치
* 가능한 key의 범위가 `a~z`처럼 **작고 고정되어 있으면 배열**을 우선 고려한다.
* key의 범위가 크거나 미리 알 수 없다면 `unordered_map` 같은 Hash Map을 고려한다.
* `unordered_map`에서 존재하지 않는 key를 `hash[key]`로 접근하면 기본값이 생성되므로 처음부터 26개의 key를 `0`으로 초기화할 필요는 없다.
* Unicode처럼 문자 집합이 고정된 26개가 아니라면 Hash Map을 이용한 빈도 관리가 필요할 수 있다.
* 단, UTF-8 등에서는 **한 문자 = 한 byte가 아닐 수 있으므로** 실제 Unicode 처리에서는 인코딩 방식도 고려해야 한다.


## 0001. Two Sum (26.8.16)

### 유형

* Array
* Hash Table
* Complement(补数)

### 처음 풀이

* 현재 원소 `nums[i]`에 대해 `target - nums[i]`인 값을 찾는 방식으로 접근
* `unordered_map<int, int>`에 **숫자 → 인덱스** 형태로 이전에 확인한 원소를 저장
* 필요한 값이 Hash Map에 존재하면 두 인덱스를 반환
* 존재하지 않으면 현재 숫자와 인덱스를 Hash Map에 저장

### 개선점

* Brute Force로 모든 두 숫자 조합을 확인하면 시간복잡도가 `O(n²)`이다.
* `target - nums[i]`를 **이전에 본 적이 있는지** Hash Map에서 확인하면 평균 `O(1)`에 탐색할 수 있다.
* 따라서 전체 시간복잡도를 평균 `O(n)`으로 줄일 수 있다.
* 현재 값을 Hash Map에 **먼저 저장하지 않고, 필요한 값을 먼저 검색한 뒤 저장**해야 한다.

  * `[3,3]`, `target = 6` 같은 경우 같은 원소를 두 번 사용하는 문제를 방지할 수 있다.
* `find()`의 결과를 iterator(迭代器)로 저장하면 다시 `hash[key]`로 접근할 필요가 없다.

```cpp
auto it = hashtable.find(target - nums[i]);

if (it != hashtable.end()) {
    return {it->second, i};
}
```

* `it->first`는 Hash Map의 **key**, `it->second`는 **value**를 의미한다.
* 이 문제에서는 `value`에 인덱스를 저장했으므로 `it->second`가 이전 원소의 인덱스이다.

### 배운 점

* **Two Sum(两数之和)** → 두 원소의 합이 `target`이 되는 두 원소의 인덱스를 찾는 문제
* 현재 값이 `nums[i]`라면 필요한 값은 `target - nums[i]`이다.
* 핵심 패턴은 **이전에 본 데이터 중 특정 값이 존재하는지 빠르게 확인해야 할 때 Hash Map을 고려하는 것**이다.
* `unordered_map`의 `find(key)`는 해당 key를 찾으면 그 원소를 가리키는 iterator를 반환한다.
* 찾지 못하면 `unordered_map.end()`를 반환한다.
* iterator는 `unordered_map` 자체가 아니라 **컨테이너 내부의 특정 원소를 가리키는 객체**이다.
* `it->first` → key
* `it->second` → value
* Hash Map의 탐색·삽입은 평균 `O(1)`이므로 Two Sum을 평균 `O(n)`에 해결할 수 있다.
* **`Two Sum = Hash Map`을 외우는 것이 아니라**, `O(n)` 탐색을 반복하고 있다면 Hash Map의 빠른 lookup(查找)으로 줄일 수 있는지 생각한다.
* 가능한 key 범위가 작고 고정되어 있다면 배열을 고려하고, 이 문제처럼 값의 범위가 크고 다양하다면 Hash Map이 자연스럽다.

## 0202. Happy Number (26.8.17)

### 유형

* Hash Table
* Math
* Cycle Detection(循环检测)

### 처음 풀이

* 현재 숫자 `n`의 각 자리 숫자를 분리하여 **각 자리의 제곱합**을 계산
* 계산된 `sum`을 `vector<int>`에 저장하여 이전에 등장한 값들을 기록
* 새로운 `sum`이 `vector`에 이미 존재하는지 순차 탐색
* 이미 존재한다면 같은 상태가 다시 등장한 것이므로 **Cycle(循环)** 이 발생했다고 판단하여 `false` 반환
* `n == 1`이 되면 Happy Number이므로 `true` 반환

### 개선점

* 내 풀이는 특정 값이 존재하는지 확인하려면 처음부터 순차적으로 탐색해야 하므로 탐색이 `O(k)`이다.
* 이 문제에서는 이전 값들의 **순서나 등장 횟수는 중요하지 않고, 이미 등장했는지만 판단**하면 된다.
* 따라서 `unordered_set<int>`을 사용하면 문제의 목적에 더 잘 맞는다.
* `unordered_set`은 내부적으로 Hash Table(哈希表)을 사용하므로 `count()`, `find()`를 통한 탐색이 평균 `O(1)`이다.
* `unordered_set`은 중복된 값을 저장하지 않으므로 `count(sum)`의 결과는 `0` 또는 `1`이다.
* C++20 이상에서는 의미가 더 직접적인 `contains()`도 사용할 수 있다.

### 배운 점

* **Happy Number(快乐数)** → 각 자리 숫자의 제곱합으로 계속 상태를 변경하면서 최종적으로 `1`에 도달하는지 확인하는 문제
* 각 자리 숫자는 `% 10`으로 얻고 `/ 10`으로 제거할 수 있다.
* 이 문제의 핵심은 단순한 숫자 계산이 아니라 **같은 상태가 다시 등장하는지 탐지하는 것**이다.
* 같은 `sum`이 다시 등장하면 이후 과정도 이전과 동일하게 반복되므로 `1`에 도달하지 못하는 Cycle이 존재한다.
* **이전에 등장한 값인지 빠르게 확인해야 하고, 추가 정보가 필요 없다면 `unordered_set`을 고려한다.**
* `unordered_set`과 `unordered_map`은 둘 다 내부적으로 Hash Table을 사용한다.
* `unordered_set` → 값(key)만 저장
* `unordered_map` → key와 value를 함께 저장
* `set` / `map` → 일반적으로 균형 이진 탐색 트리 기반, 탐색 `O(log n)`
* `unordered_set` / `unordered_map` → Hash Table 기반, 탐색 평균 `O(1)`, 최악 `O(n)`
* Hash Table은 key에 Hash Function(哈希函数)을 적용해 Hash Value를 얻고, 이를 이용해 확인할 Bucket(桶)을 결정한다.
* `int` 같은 기본 타입은 C++ 표준 라이브러리가 `std::hash<int>`를 제공하므로 직접 Hash Function을 만들 필요가 없다.
* 서로 다른 key가 같은 Bucket으로 들어가는 것을 Hash Collision(哈希冲突)이라고 한다.
* **`Happy Number = unordered_set`을 외우는 것이 아니라**, 상태가 반복적으로 변하는 문제에서 **“이 상태가 이전에 등장했는가?”**를 계속 확인한다면 Set을 이용한 Cycle Detection을 생각한다.

### 방법 2: Fast & Slow Pointer

* 숫자의 변화 과정을 **암시적 연결 리스트(隐式链表)** 로 생각한다.
* `getNext(n)`을 현재 숫자에서 **다음 노드로 이동하는 것**으로 볼 수 있다.

```text
2 → 4 → 16 → 37 → 58 → 89 → ...
    ↑                         ↓
    └────── cycle ────────────┘
```

* Floyd Cycle Detection(快慢指针)을 사용한다.

  * `slow` → 한 번에 1번 `getNext()`
  * `fast` → 한 번에 2번 `getNext()`
* `fast == 1`이 되면 Happy Number이므로 `true`
* `slow == fast`가 되면 Cycle이 존재하므로 `false`

```cpp
slow = getNext(slow);
fast = getNext(getNext(fast));
```

### 장점

* 이전 숫자들을 저장할 필요가 없다.
* `unordered_set`을 사용하지 않으므로 **추가 공간복잡도 O(1)**
* 핵심 패턴은 **다음 상태가 하나로 결정되는 구조에서 Cycle을 찾을 때 Fast & Slow Pointer를 고려하는 것**
* Happy Number에서는 보조 풀이에 가깝고, **Linked List Cycle 문제에서 더 중요하게 사용되는 패턴**이다.


## 0383. Ransom Note (26.8.18)

### 유형

* Hash Table
* String
* Counting(计数)

### 처음 풀이

* `unordered_map<char, int>`을 사용하여 `magazine`의 각 문자 등장 횟수를 저장
* `ransomNote`를 순회하면서 필요한 문자의 개수를 하나씩 감소
* 특정 문자의 개수가 `0`보다 작아지면 `magazine`의 문자가 부족하다는 의미이므로 `false` 반환
* 끝까지 문제가 없으면 `true` 반환

### 개선점

* 문제에서 문자가 **소문자 영어 알파벳 `a~z` 26개로 제한**되어 있다.
* 따라서 범용적인 `unordered_map`보다 `vector<int>(26)` 또는 `int cnt[26]`을 사용하는 것이 더 단순하고 효율적이다.
* `c - 'a'`를 이용하면 `'a' → 0`, `'b' → 1`, ..., `'z' → 25`로 변환하여 배열의 인덱스로 사용할 수 있다.
* `ransomNote.size() > magazine.size()`라면 애초에 만들 수 없으므로 바로 `false`를 반환할 수도 있다.

### 배운 점

* 이 문제의 핵심은 문자의 순서가 아니라 **각 문자의 등장 횟수(字符出现次数)** 를 비교하는 것이다.
* `unordered_map[key]`에서 key가 존재하지 않으면 value가 기본값으로 생성된다. `int`의 경우 `0`이다.
* 따라서 `countMgz[c]++`는 key가 없다면 `0`으로 생성한 후 `1` 증가시킨다.
* 문자 범위가 `a~z`처럼 작고 고정되어 있다면 Hash Table보다 **고정 크기 배열을 이용한 Counting**을 고려한다.
* `cnt[c - 'a']--` 후 값이 음수가 되었다면 필요한 문자 수가 보유한 문자 수보다 많다는 의미이다.
* 시간복잡도는 `O(m+n)`, 추가 공간은 알파벳 26개로 고정되어 사실상 `O(1)`이다.
* **`Ransom Note = 배열`을 외우는 것이 아니라**, 제한된 범위의 값에 대해 등장 횟수를 세는 문제라면 **배열/Hash Table을 이용한 Frequency Counting(频率统计)** 을 생각한다. 


## 0205. Isomorphic Strings (26.8.19)

### 유형

* Hash Table
* String
* Mapping(映射)
* Bijection(双射)

### 처음 풀이

* `unordered_map<char, char>`을 사용하여 `s`의 문자가 `t`의 어떤 문자에 대응하는지 저장하려고 했다.
* `s`를 순회하면서 이미 매핑된 문자가 현재 `t[i]`와 다르면 `false`를 반환하도록 했다.
* 서로 다른 `s`의 문자가 같은 `t`의 문자에 매핑되는 경우도 검사하기 위해 Hash Table의 value를 직접 순회하려고 했다.
* 하지만 `unordered_map`에는 `values()`와 같은 방식이 없으며, 매번 전체 value를 탐색하는 방법도 불필요하다.

### 개선점

* 이 문제의 핵심 조건은 `s`와 `t`의 문자 사이에 **일대일 대응(一一对应), 즉 Bijection(双射)** 이 존재해야 한다는 것이다.
* 따라서 하나의 Hash Table만 탐색하는 대신 **두 개의 Hash Table**을 사용하면 간단하게 검사할 수 있다.
* `s2t`는 `s → t`의 매핑을 저장하여 **하나의 `s` 문자가 서로 다른 `t` 문자에 대응하는 경우**를 검사한다.
* `t2s`는 `t → s`의 매핑을 저장하여 **서로 다른 `s` 문자가 같은 `t` 문자에 대응하는 경우**를 검사한다.
* 두 방향 모두 기존 매핑과 충돌하지 않을 때만 현재 문자 관계를 저장한다.

```cpp
if (s2t.count(s[i]) && s2t[s[i]] != t[i]) return false;
if (t2s.count(t[i]) && t2s[t[i]] != s[i]) return false;

s2t[s[i]] = t[i];
t2s[t[i]] = s[i];
```

### 배운 점

* 이 문제는 단순히 문자의 등장 횟수를 비교하는 문제가 아니라 **문자 사이의 매핑 관계(映射关系)** 가 일관적인지 확인하는 문제이다.
* `s → t`만 검사하면 하나의 `s` 문자가 여러 `t` 문자로 가는 **일대다(一对多)** 관계는 막을 수 있지만, 서로 다른 `s` 문자가 하나의 `t` 문자로 가는 **다대일(多对一)** 관계를 막을 수 없다.
* 따라서 `s → t`와 `t → s`를 모두 관리하여 **일대일 대응(一一对应)** 을 보장해야 한다.
* 예를 들어 `s = "foo"`, `t = "bar"`에서는 `o → a`, `o → r`이라는 충돌이 발생하므로 `s2t` 검사에서 `false`가 된다.
* `s = "ab"`, `t = "aa"`에서는 `a → a`, `b → a`처럼 서로 다른 두 문자가 같은 문자에 대응하므로 `t2s` 검사에서 `false`가 된다.
* `unordered_map.count(key)`를 이용하면 해당 key에 기존 매핑이 존재하는지 확인할 수 있다.
* 문자열을 한 번 순회하고 `unordered_map`의 조회·삽입이 평균 `O(1)`이므로 시간복잡도는 **`O(n)`** 이다.
* 공간복잡도는 문자 집합을 `Σ`라고 할 때 **`O(|Σ|)`** 이며, 이 문제에서는 ASCII 문자로 제한되어 있다.
* **`Isomorphic Strings = Hash Map 2개`를 외우는 것이 아니라**, 두 집합의 원소 사이에 **1:1 Mapping / Bijection(双射)** 을 확인해야 하는 문제라면 **정방향 매핑 + 역방향 매핑**을 생각한다.