# Chapter 6. Memory Hierarchy

### 1. Storage Technologies

- SRAM (Static RAM): 빠르고 비싸며 Refresh가 필요 없음. 주로 CPU Cache에 사용.
- DRAM (Dynamic RAM): SRAM보다 느리고 저렴하며 Refresh가 필요함. 주로 Main Memory에 사용.
- SRAM과 DRAM은 모두 Volatile Memory(휘발성 메모리).
- SDRAM: 클럭과 동기화하여 동작.
- DDR SDRAM: 클럭의 상승·하강 에지 모두에서 데이터 전송.
- Flash Memory: 전원이 꺼져도 데이터가 유지되는 Nonvolatile Memory.

### 2. DRAM Organization

- DRAM은 Row(행) 와 Column(열) 구조로 데이터를 저장.
- RAS: Row 선택 → Row Buffer에 데이터 저장.
- CAS: Column 선택 → 원하는 데이터 접근.
- 여러 DRAM 칩을 병렬로 연결하여 데이터 폭을 확장할 수 있음.

### 3. HDD (Hard Disk Drive)

- Platter: 데이터를 저장하는 원판.
- Track: 원판 표면의 동심원.
- Sector: Track을 나눈 저장 구역.
- Cylinder: 여러 표면에서 같은 반지름에 위치한 Track들의 집합.
- Logical Block: 복잡한 물리적 저장 위치를 숨기는 추상화.

HDD 접근 시간:

\\[ T\_{\text{access}}=T\_{\text{seek}}+T\_{\text{rotation}}+T\_{\text{transfer}} \\]

- Seek Time: 헤드가 목표 Track으로 이동하는 시간.
- Rotational Latency: 목표 Sector가 헤드 아래에 올 때까지 기다리는 시간.
- Transfer Time: 실제 데이터를 전송하는 시간.

평균 회전 지연시간:

\\[ T\_{\text{avg rotation}}=\frac12\times\frac{60}{RPM} \\]

HDD는 데이터 전송 자체보다 Seek와 Rotation에 많은 시간이 소요될 수 있음.

### 4. DMA & SSD

- DMA (Direct Memory Access): CPU가 데이터를 직접 하나씩 복사하지 않고 장치가 메인 메모리와 데이터를 전송하는 방식.
- Interrupt: I/O 작업 완료 등을 CPU에 알리는 신호.
- SSD (Solid State Disk): Flash Memory 기반 저장 장치로 기계적 Seek와 회전 지연이 없음.
- Flash Memory는 Page 단위 읽기·쓰기, Block 단위 지우기를 수행.
- FTL (Flash Translation Layer): 논리 블록과 물리적 Flash 저장 위치를 대응시킴.
- Wear Leveling: 특정 Flash Block에 지우기 작업이 집중되지 않도록 분산.

### 5. CPU-Memory Gap

- CPU의 처리 속도와 메모리 접근 속도 사이의 성능 격차.
- CPU가 빨라도 메모리에서 데이터를 가져오는 시간이 길면 프로그램 성능이 제한될 수 있음.
- 이를 완화하기 위해 Memory Hierarchy와 Caching을 활용.

### 6. Locality (지역성)

- Temporal Locality (시간 지역성): 최근 접근한 데이터를 다시 사용할 가능성이 높음.
- Spatial Locality (공간 지역성): 최근 접근한 주소 주변의 데이터에 접근할 가능성이 높음.
- C 배열은 메모리에 연속적으로 저장되므로 순차 접근 시 공간 지역성이 좋음.
- C의 2차원 배열은 Row-Major Order로 저장되므로 일반적으로 행 우선 순회가 유리함.

### 7. Memory Hierarchy & Cache

메모리 계층 구조:

```
Registers
    ↓
L1 Cache (SRAM)
    ↓
L2 Cache (SRAM)
    ↓
Main Memory (DRAM)
    ↓
Local Disk
    ↓
Remote Storage
```

- 위쪽 계층일수록 빠르고 작으며 바이트당 비용이 높음.
- Cache: 느린 하위 계층의 데이터 일부를 빠른 상위 계층에 보관하는 저장 장치.
- 데이터는 Block 단위로 캐시에 복사됨.
- Cache Hit: 요청한 데이터가 캐시에 존재.
- Cache Miss: 요청한 데이터가 캐시에 없어 하위 계층에서 가져와야 함.
- Placement Policy: 가져온 블록을 캐시의 어디에 배치할지 결정.
- Replacement Policy: 캐시 공간이 부족할 때 제거할 블록을 결정.

### 8. Types of Cache Misses

| 유형            | 발생 원인                     |
| ------------- | ------------------------- |
| Cold Miss     | 블록에 처음 접근하여 캐시에 없음        |
| Conflict Miss | 여러 블록이 같은 캐시 위치에 배치되어 충돌  |
| Capacity Miss | Working Set이 캐시 전체 용량보다 큼 |

### 핵심 정리

> CPU와 메모리·저장 장치 사이의 성능 격차를 완화하기 위해 Memory Hierarchy를 사용하며, 프로그램의 Temporal Locality와 Spatial Locality를 활용하는 Caching을 통해 느린 메모리 접근 횟수를 줄인다.

### 9. Cache Memories

### 10. Cache Organization

- Cache 구조: Set, Line, Valid Bit, Tag, Data Block으로 구성.
- Cache 용량: \\(C=S\times E\times B\\)
  - \\(S\\): Set 개수, \\(E\\): Set당 Line 개수, \\(B\\): Block 크기
- 주소 분해: Tag + Set Index + Block Offset
- Cache Hit: 요청한 데이터가 캐시에 존재.
- Cache Miss: 데이터가 없어 하위 메모리에서 가져와야 함.

### 11. Cache Mapping & Write Policies

- Direct-Mapped: Set당 Line 1개. 구조가 단순하지만 Conflict Miss 발생 가능.
- Set-Associative: Set당 여러 Line을 두어 충돌 완화.
- Replacement Policy: LRU, Random 등.
- Write Policy:
  - Write-Through: 캐시와 하위 메모리에 기록.
  - Write-Back: 수정된 데이터를 교체할 때 하위 메모리에 기록.
  - Write-Allocate / No-Write-Allocate: Write Miss 발생 시 블록을 캐시에 가져올지 결정.

### 12. Cache Performance

- Miss Rate: 전체 접근 중 Cache Miss가 발생한 비율.
- Hit Time: Cache Hit 처리 시간.
- Miss Penalty: Cache Miss로 발생하는 추가 시간.

\\[ AMAT=Hit\ Time+Miss\ Rate\times Miss\ Penalty \\]

### 13. The Memory Mountain

- Working Set Size: 프로그램이 반복적으로 접근하는 데이터 영역 크기.
- Stride: 연속된 메모리 접근 사이의 인덱스 간격.
- Spatial Locality: 작은 Stride로 인접 데이터를 접근할수록 유리.
- Temporal Locality: Working Set이 캐시에 들어가고 데이터를 반복 사용하면 유리.
- Read Throughput: 단위 시간당 읽은 데이터 양(MB/s).

### 14. Loop Reordering

- C의 2차원 배열은 Row-Major Order로 저장.
- 반복문 순서를 변경해 연속된 메모리 주소에 접근하도록 최적화.
- 강의자료의 단순화된 모델 기준:

| Loop Order | Cache Misses / Iteration |
| ---------- | ------------------------ |
| ijk / jik  | 1.25                     |
| kij / ikj  | 0.5                      |
| jki / kji  | 2.0                      |

### 15. Matrix Blocking

- 행렬을 작은 \\(B\times B\\) 타일로 나누어 계산.
- 작은 데이터 영역을 캐시에 유지하며 반복 사용하여 Temporal Locality 개선.
- 강의자료의 Cache Miss 근삿값:

\\[ \begin{aligned} \text{No Blocking} &\approx \frac98n^3\\\ \text{Blocking} &\approx \frac{n^3}{4B} \end{aligned} \\]

\- 타일 크기 조건: \\(3B^2\<C\\) (여기서 \\(C\\)는 캐시 용량을 원소 개수로 표현한 값).

### 핵심 정리

> Cache 성능은 하드웨어 구조뿐 아니라 프로그램의 메모리 접근 패턴에 크게 영향을 받는다. Loop Reordering으로 Spatial Locality를 개선하고, Matrix Blocking으로 Temporal Locality를 활용하면 Cache Miss를 줄여 프로그램 성능을 향상시킬 수 있다.