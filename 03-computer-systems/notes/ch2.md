# Chapter 2. Data Representation

## 2.1 Bits and Bytes ~ 2.3 Integers

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

## 2.4 Floating Point

### 1. Binary Fraction

* 소수도 2진수로 표현 가능.
* 각 자리의 값은 \(2^{-1}, 2^{-2}, 2^{-3}, \cdots\)
* 모든 10진 소수가 2진수로 **정확하게 표현되는 것은 아님.**

예:

$$
0.1_{10}=0.0001100110011\cdots_2
$$

---

### 2. IEEE 754

Single Precision → **32 bits**

```text
Sign | Exponent | Fraction
 1   |    8     |    23
```

정규화된 값:

$$
(-1)^s\times M\times2^E
$$

* **Sign(符号)** → 양수 / 음수
* **Exponent(指数)** → 지수
* **Fraction(小数部分)** → 가수의 소수 부분

Normalized:

$$
M=1.xxx_2
$$

* 앞의 `1`은 **implicit(隐藏的1)**으로 저장하지 않음.
* 실제 지수:

$$
E=Exp-Bias
$$

Single Precision:

$$
Bias=127
$$

---

### 3. Denormalized

```text
exp = 00000000
frac ≠ 0
```

* `1`을 implicit하게 붙이지 않음.
* \(M=0.xxx_2\)
* 매우 작은 값을 표현하기 위해 사용.
* **Underflow(下溢)** 구간을 0과 최소 normalized 값 사이에서 부드럽게 연결.

최소 positive denormal:

$$
2^{-149}
$$

최소 positive normalized:

$$
2^{-126}
$$

---

### 4. Special Values

```text
exp = 00000000
├─ frac = 0 → ±0
└─ frac ≠ 0 → Denormalized

exp = 11111111
├─ frac = 0 → ±∞
└─ frac ≠ 0 → NaN
```

* `∞` → **Overflow(上溢)** 등에서 발생.
* `NaN` → 유효하지 않은 연산 결과를 나타냄.

---

### 5. Rounding

저장할 수 있는 비트보다 결과가 길면 **Rounding(舍入)** 필요.

기본 방식:

**Round-to-even(偶数舍入)**

```text
버릴 부분 < 1/2 → 버림
버릴 부분 > 1/2 → 올림
버릴 부분 = 1/2 → 마지막 비트를 even으로
```

예:

```text
1.101 | 100...
```

정확히 절반이고 마지막 bit가 `1`이면:

```text
1.101 → 1.110
```

---

### 6. Floating-Point Arithmetic

**Addition**

1. Exponent를 맞춤
2. Significand를 더함
3. Normalization
4. Rounding

**Multiplication**

1. Significand를 곱함
2. Exponent를 더함
3. Normalization
4. Rounding

---

### 7. Floating Point in C

```c
float
double
```

* `float` → Single Precision
* `double` → Double Precision
* 10진 소수가 정확하게 표현되지 않을 수 있음.
* 따라서 Floating Point 계산에는 **Rounding Error(舍入误差)**가 발생할 수 있음.
* `==` 비교를 무조건 신뢰하면 안 됨.
* 계산 순서에 따라 결과가 달라질 수 있음 (**Non-associativity, 非结合性**).

---

### 8. Conversion

**Integer → Floating Point**

* 충분한 정밀도가 있으면 정확하게 표현 가능.
* 큰 정수는 `float`의 정밀도를 초과하여 값이 변할 수 있음.

**Float → Double**

* `float` 값을 정확하게 표현 가능.
* 일반적으로 정밀도 손실 없음.

**Double → Float**

* 정밀도 손실 가능.
* 따라서 원래 `double` 값과 달라질 수 있음.

---

### 핵심 정리

> Floating Point는 실수를 그대로 저장하는 것이 아니라 **제한된 bit로 근사하여 표현**한다.
> 따라서 **IEEE 754의 bit 구조, 정규화, 반올림, 표현 범위**를 이해해야 실제 컴퓨터에서 실수가 어떻게 처리되는지 이해할 수 있다. 