# Chapter 2 — Data Representation

## Chapter 2.1 — Integer Data Types (26.09.05)

### 1. Bits & Bytes

* Bit: `0` 또는 `1`
* Byte: `8 bits`
* Hex: 1자리 = 4 bits

### 2. Integer Representation

#### Unsigned

$$
0 \sim 2^w-1
$$

#### Two's Complement

$$
-2^{w-1}\sim2^{w-1}-1
$$

* 같은 bit pattern도 signed / unsigned에 따라 다르게 해석됨.

### 3. Integer Arithmetic

* `<< k` → `× 2^k`
* unsigned `>> k` → `÷ 2^k`
* unsigned overflow → modulo `2^w`
* signed overflow → **Undefined Behavior**

### 4. Signed / Unsigned Conversion

signed와 unsigned가 섞이면 signed 값이 unsigned로 변환될 수 있다.

```c
-1 < 1U  // false
```

### 5. Memory

* 메모리는 **byte 단위**로 addressable.
* Pointer(指针)는 memory address를 저장한다.
* Word는 여러 byte를 묶은 기본 데이터 단위.

### 6. Endianness

Multi-byte value의 byte 저장 순서.

```text
Big Endian    → MSB가 낮은 주소
Little Endian → LSB가 낮은 주소
```

x86 → Little Endian

### 7. C String

```c
char str[] = "abc";
```

```text
'a' 'b' 'c' '\0'
```

C string은 **null-terminated char array**이다.

---

###### 질문·헷갈린 내용

###### 32-bit 주소 → 왜 4GB인가?

* 주소 하나가 **1 byte**를 가리킴.
* 32-bit 주소 → `2^32`개의 주소 표현 가능.
* 따라서:

$$
2^{32}\text{ bytes}=4\text{ GiB}
$$

###### signed overflow가 UB인 이유

**signed overflow가 발생하지 않는다는 전제**로 코드를 최적화할 수 있다.

예:

```c
x + 1 > x
```

* 정상적인 signed integer 범위에서는 x + 1이 x보다 크므로 컴파일러는 이를 무조건 참으로 판단 가능

---

### 핵심 정리

> C의 자료형은 추상화이지만, 실제 컴퓨터에서는 결국 **bit → byte → memory**로 표현되고 처리된다.

