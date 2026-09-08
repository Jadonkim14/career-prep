# data-lab-review

## tmin

* `int`가 32비트라면 TMin은 `-2^31`이다.

* `1 << 31`은 `1000...0000`이 되어 TMin의 비트 패턴을 만든다.

* Data Lab에서는 `tmin` 문제의 제한에 맞춰 `1 << 31`을 사용할 수 있다.

## istmax

* `TMax = 0111...1111`이고 `~TMax = 1000...0000`이다.

* `TMax + 1`과 `~TMax`가 같은 비트 패턴이라는 성질을 이용할 수 있다.

* `!((x + 1) ^ (~x))`만 사용하면 `TMax`뿐만 아니라 `-1`도 1이 된다.

* `x = -1`이면 `x + 1 = 0`이다.

* `!!(x + 1)`은 `x = -1`일 때 0, 그 외에는 1이 된다.

* `-1 + 1 = 0`은 signed overflow가 아니다. 자료형의 표현 범위 안에 있기 때문이다.

## alloddbits

* 홀수 번째 비트(bit 1, 3, 5, ..., 31)가 모두 1인지 확인하는 문제다.

* `0xAA = 10101010`을 이용해 `10101010 10101010 10101010 10101010` mask를 만들 수 있다.

* `x & mask`를 이용하면 관심 있는 홀수 비트만 추출할 수 있다.

* `(x & mask) ^ mask`가 0이면 관심 있는 모든 비트가 1이다.

* `!(x ^ mask)`가 아니라 `!((x & mask) ^ mask)`를 사용해야 짝수 비트의 값은 무시할 수 있다.

## isasciidigit

* ASCII 숫자 `'0' ~ '9'`의 범위는 `0x30 ~ 0x39`이다.

* 범위 검사는 `x - 48 >= 0`과 `x - 58 < 0`으로 생각할 수 있다.

* `x - 48`이 음수가 아니고 `x - 58`이 음수이면 `0x30 ~ 0x39` 범위다.

## conditional

* `!!x`는 `x == 0`이면 0, `x != 0`이면 1로 만든다.

* `0/1`을 이용해 `0x00000000` 또는 `0xFFFFFFFF` 형태의 mask를 만들 수 있다.

* mask를 이용하면 `&`와 `|`로 `y`와 `z` 중 하나를 선택할 수 있다.

* `~0 + 1 = -1 + 1 = 0`이며, 이는 signed overflow가 아니다.

## islessorequal

* 부호가 다를 때 뺄셈을 하면 오버플로우 생길 수 있다.
* 그래서 부호가 같을 떄랑 다를 때로 나눠서 판단해야 한다.
* `sign_diff`로 `x`와 `y`의 부호가 다른지 확인한다.
* 부호가 다르면 `x`가 음수인지 확인하여 `x <= y`를 판단한다.
* 부호가 같으면 `diff = x - y`를 이용한다.
* `(diff >> 31) | !diff`는 `diff < 0` 또는 `diff == 0`인지 확인하여 `diff <= 0`을 판단한다.
* `diff >> 31`은 `diff`가 음수이면 `0xFFFFFFFF`, 양수이면 `0`이 된다.

## floatscale2

* IEEE 754 단정밀도 `float`는 `sign | exponent | fraction`으로 구성된다.
* `exp == 0`이면 `fraction == 0`이면 `0`, `0`이 아니면 subnormal(非规格化数)이므로 `frac <<= 1`로 2배를 표현한다.
* `exp == 255`이면 `fraction == 0`이면 Infinity, `0`이 아니면 NaN이므로 그대로 둔다.
* `exp`가 `1~254`이면 normal(规格化数)이므로 `exp += 1`로 2배를 표현한다.
* 마지막에 `sign`, `exp`, `frac`을 다시 합쳐 원래와 같은 비트 형태로 반환한다.

## floatfloat2int

* 저장된 exponent에서 bias `127`을 빼서 실제 지수 `exp`를 구한다.
* normal(规格化数)은 생략된 `1`이 있으므로 `frac | (1 << 23)`으로 **implicit 1**을 복원한다.
* `mantissa`는 `1.fraction × 2^23` 형태이므로 실제 정수값은 `mantissa × 2^(exp - 23)`으로 구한다.
* `exp < 0`이면 정수 부분이 없으므로 `0`을 반환한다.
* `exp > 30`이면 `int` 범위를 벗어나므로 `0x80000000u`를 반환한다.
* `exp >= 23`이면 `mantissa << (exp - 23)`으로 정수 위치까지 이동한다.
* `exp < 23`이면 `mantissa >> (23 - exp)`으로 소수 부분을 버린다.
* `sign == 1`이면 음수로 만들고, `sign == 0`이면 그대로 반환한다.

### IEEE 754 Float 기본 개념

* `float`는 `sign 1비트 + exponent 8비트 + fraction 23비트`로 구성된다.
* `sign`: 부호를 결정한다.
* `exponent`: bias(偏置값) `127`을 사용하며, 실제 지수는 `exp - 127`이다.
* `fraction`: `1.fraction`의 소수 부분을 저장하며, float의 정밀도(精度)를 결정한다.
* `exp == 0`: `fraction == 0`이면 `0`, 아니면 subnormal(非规格化数)이다.
* `exp == 255`: `fraction == 0`이면 Infinity, 아니면 NaN이다.
* `exp == 1~254`: normal(规格化数)이며 실제 값은 `(-1)^sign × 1.fraction × 2^(exp-127)`이다.
* `fraction`이 23비트라 모든 실수를 정확히 표현할 수 없으며, 표현할 수 없는 값은 rounding(舍入)된다.

## floatpower2

* `2^x`는 항상 양수이므로 `sign = 0`, normal에서는 `fraction = 0`이다.
* `x > 127`이면 `float` 범위를 초과하므로 `+Infinity`를 반환한다.
* `-126 ≤ x ≤ 127`이면 normal(规格化数)이며 `exp = x + 127`로 bias를 적용한다.
* exponent를 float의 exponent 영역에 넣기 위해 `(x + 127) << 23`을 사용한다.
* `-149 ≤ x < -126`이면 subnormal(非规格化数)이며 `fraction = 1 << (x + 149)`로 표현한다.
* `x < -149`이면 너무 작아서 subnormal으로도 표현할 수 없으므로 `0`을 반환한다.