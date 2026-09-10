# bomb-lab-review

## Phase 1

```text
(gdb) file ./bomb

Reading symbols from ./bomb...

(gdb) disas phase_1

Dump of assembler code for function phase_1:
   0x0000000000400ee0 <+0>:  sub    $0x8,%rsp
   0x0000000000400ee4 <+4>:  mov    $0x402400,%esi
   0x0000000000400ee9 <+9>:  call   0x401338 <strings_not_equal>
   0x0000000000400eee <+14>: test   %eax,%eax
   0x0000000000400ef0 <+16>: je     0x400ef7 <phase_1+23>
   0x0000000000400ef2 <+18>: call   0x40143a <explode_bomb>
   0x0000000000400ef7 <+23>: add   $0x8,%rsp
   0x0000000000400efb <+27>: ret
End of assembler dump.
```

* `sub $0x8, %rsp`는 Stack Alignment(栈对齐)를 맞추기 위해 `%rsp`를 이동시킨다.
* `%rsp`는 Stack Pointer(栈指针)이고, `%esi`는 함수의 두 번째 인자를 전달하는 Register(寄存器)이다.
* `call strings_not_equal`은 사용자 입력과 `0x402400`에 저장된 문자열을 비교한다.
* 함수의 반환값은 `%eax`에 저장된다.
* `test %eax, %eax`는 `%eax`가 0인지 확인하기 위해 Condition Code(条件码)를 설정한다.
* `%eax == 0`이면 `je`를 통해 Phase 1을 통과한다.
* `%eax != 0`이면 `explode_bomb`이 호출된다.
* 마지막에 `add $0x8, %rsp`로 이동시켰던 %rsp를 원래대로 복구한다.
* `x/s 0x402400`으로 해당 주소의 문자열을 확인할 수 있다.
* Phase 1의 정답은 `"Border relations with Canada have never been better."`이다.


## Phase 2

```text
Dump of assembler code for function phase_2:
   0x0000000000400efc <+0>:     push   %rbp
   0x0000000000400efd <+1>:     push   %rbx
   0x0000000000400efe <+2>:     sub    $0x28,%rsp
   0x0000000000400f02 <+6>:     mov    %rsp,%rsi
   0x0000000000400f05 <+9>:     call   0x40145c <read_six_numbers>
   0x0000000000400f0a <+14>:    cmpl   $0x1,(%rsp)
   0x0000000000400f0e <+18>:    je     0x400f30 <phase_2+52>
   0x0000000000400f10 <+20>:    call   0x40143a <explode_bomb>
   0x0000000000400f15 <+25>:    jmp    0x400f30 <phase_2+52>
   0x0000000000400f17 <+27>:    mov    -0x4(%rbx),%eax
   0x0000000000400f1a <+30>:    add    %eax,%eax
   0x0000000000400f1c <+32>:    cmp    %eax,(%rbx)
   0x0000000000400f1e <+34>:    je     0x400f25 <phase_2+41>
   0x0000000000400f20 <+36>:    call   0x40143a <explode_bomb>
   0x0000000000400f25 <+41>:    add    $0x4,%rbx
   0x0000000000400f29 <+45>:    cmp    %rbp,%rbx
   0x0000000000400f2c <+48>:    jne    0x400f17 <phase_2+27>
   0x0000000000400f2e <+50>:    jmp    0x400f3c <phase_2+64>
   0x0000000000400f30 <+52>:    lea    0x4(%rsp),%rbx
   0x0000000000400f35 <+57>:    lea    0x18(%rsp),%rbp
   0x0000000000400f3a <+62>:    jmp    0x400f17 <phase_2+27>
   0x0000000000400f3c <+64>:    add    $0x28,%rsp
   0x0000000000400f40 <+68>:    pop    %rbx
   0x0000000000400f41 <+69>:    pop    %rbp
   0x0000000000400f42 <+70>:    ret
End of assembler dump.
```

* `push %rbp`, `push %rbx` → 함수에서 사용할 `%rbp`, `%rbx`의 기존 값을 Stack에 저장한다.
* `sub $0x28, %rsp` → Stack에 40바이트 공간을 확보한다.
* `mov %rsp, %rsi` → 숫자 6개를 저장할 Stack 공간의 주소를 `read_six_numbers`의 두 번째 인자로 전달한다.
* `read_six_numbers` → `sscanf`를 이용해 정수 6개를 Stack에 저장한다.
* `cmpl $0x1, (%rsp)` → 첫 번째 숫자가 `1`인지 검사한다.
* `lea 0x4(%rsp), %rbx` → `%rbx`를 두 번째 숫자의 주소로 설정한다.
* `lea 0x18(%rsp), %rbp` → `%rbp`를 6개 숫자 다음의 끝 주소로 설정한다.
* `mov -0x4(%rbx), %eax` → 이전 숫자의 값을 `%eax`로 가져온다.
* `add %eax, %eax` → 이전 숫자를 2배로 만든다.
* `cmp %eax, (%rbx)` → 현재 숫자와 이전 숫자 × 2를 비교한다.
  * `%eax` → **레지스터에 들어있는 값**
  * `(%rbx)` → **`%rbx`가 가지고 있는 주소로 가서 그 메모리에 저장된 값**
* `add $0x4, %rbx` → `%rbx`를 다음 숫자의 주소로 4바이트 이동한다.
* `cmp %rbp, %rbx` → 현재 주소가 끝 주소에 도달했는지 확인한다.
* `jne 0x400f17` → 아직 끝나지 않았다면 다음 숫자를 검사한다.
* `jmp 0x400f3c` → 모든 검사가 끝나면 Phase 2를 종료한다.
* 최종 조건은 **`1 2 4 8 16 32`**이다.


## Phase 3

```text
Dump of assembler code for function phase_3:
   0x0000000000400f43 <+0>:     sub    $0x18,%rsp
   0x0000000000400f47 <+4>:     lea    0xc(%rsp),%rcx // 두번째 인자
   0x0000000000400f4c <+9>:     lea    0x8(%rsp),%rdx // 첫번째 인자
   0x0000000000400f51 <+14>:    mov    $0x4025cf,%esi //  "%d %d"
   0x0000000000400f56 <+19>:    mov    $0x0,%eax
   0x0000000000400f5b <+24>:    call   0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000400f60 <+29>:    cmp    $0x1,%eax // 인자 개수
   0x0000000000400f63 <+32>:    jg     0x400f6a <phase_3+39>
   0x0000000000400f65 <+34>:    call   0x40143a <explode_bomb>
   0x0000000000400f6a <+39>:    cmpl   $0x7,0x8(%rsp) // 첫번째 인자
   0x0000000000400f6f <+44>:    ja     0x400fad <phase_3+106>
   0x0000000000400f71 <+46>:    mov    0x8(%rsp),%eax
   0x0000000000400f75 <+50>:    jmp    *0x402470(,%rax,8)
   0x0000000000400f7c <+57>:    mov    $0xcf,%eax
   0x0000000000400f81 <+62>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f83 <+64>:    mov    $0x2c3,%eax
   0x0000000000400f88 <+69>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f8a <+71>:    mov    $0x100,%eax
   0x0000000000400f8f <+76>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f91 <+78>:    mov    $0x185,%eax
   0x0000000000400f96 <+83>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f98 <+85>:    mov    $0xce,%eax
   0x0000000000400f9d <+90>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f9f <+92>:    mov    $0x2aa,%eax
   0x0000000000400fa4 <+97>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400fa6 <+99>:    mov    $0x147,%eax
   0x0000000000400fab <+104>:   jmp    0x400fbe <phase_3+123>
   0x0000000000400fad <+106>:   call   0x40143a <explode_bomb>
   0x0000000000400fb2 <+111>:   mov    $0x0,%eax
   0x0000000000400fb7 <+116>:   jmp    0x400fbe <phase_3+123>
   0x0000000000400fb9 <+118>:   mov    $0x137,%eax
   0x0000000000400fbe <+123>:   cmp    0xc(%rsp),%eax
   0x0000000000400fc2 <+127>:   je     0x400fc9 <phase_3+134>
   0x0000000000400fc4 <+129>:   call   0x40143a <explode_bomb>
   0x0000000000400fc9 <+134>:   add    $0x18,%rsp
   0x0000000000400fcd <+138>:   ret
End of assembler dump.
```

* `sub $0x18, %rsp` → Stack에 24바이트 공간을 확보한다.
* `lea 0xc(%rsp), %rcx` → `%rcx`에 두 번째 정수를 저장할 주소를 전달한다.
* `lea 0x8(%rsp), %rdx` → `%rdx`에 첫 번째 정수를 저장할 주소를 전달한다.
* `mov $0x4025cf, %esi` → `sscanf`의 format string(格式字符串) `"%d %d"`를 전달한다. 0x4025cf: "%d %d"
* `call __isoc99_sscanf@plt` → 입력을 두 개의 정수로 parsing(解析)한다.
* `cmp $0x1, %eax` → `sscanf`의 반환값을 `1`과 비교한다.
* `jg` → 변환된 입력 항목이 **2개 이상**이면 통과한다.
* `cmpl $0x7, 0x8(%rsp)` → 첫 번째 입력값과 `7`을 비교한다.
* `ja` → 첫 번째 입력값이 **Unsigned(无符号) 기준 7보다 크면** 폭탄이 폭발한다.
* `mov 0x8(%rsp), %eax` → 첫 번째 입력값을 `%eax`에 넣는다.
* `jmp *0x402470(,%rax,8)` → 첫 번째 입력값을 인덱스로 사용해 **Jump Table(跳转表)**에서 해당 case로 이동한다.

```text
x86의 메모리 주소 표현은:

D(Rb, Ri, S)
= D + Rb + Ri × S
```

여기서는:

```text
D  = 0x402470
Rb = 없음 → 0
Ri = %rax
S  = 8
```

따라서:

```text
0x402470 + %rax × 8 → **Jump Table(跳转表)의 항목 주소**
```

실제 Jump Table:

```text
(gdb) x/8gx 0x402470
0x402470:       0x0000000000400f7c      0x0000000000400fb9
0x402480:       0x0000000000400f83      0x0000000000400f8a
0x402490:       0x0000000000400f91      0x0000000000400f98
0x4024a0:       0x0000000000400f9f      0x0000000000400fa6
```

각 항목은 **8바이트 크기의 점프 대상 주소**이다.

```asm
jmp *0x402470(,%rax,8)
```

여기서 `*`는 **계산된 메모리 주소에 직접 점프하는 것이 아니라, 해당 메모리에 저장된 값을 주소로 사용하여 점프한다**는 의미이다.

* 첫 번째 입력값에 따라 `%eax`에 미리 정해진 값이 들어간다. `0` → `207`... `7` → `327`
* `cmp 0xc(%rsp), %eax` → 두 번째 입력값과 case에서 설정된 `%eax`를 비교한다.
* `je` → 두 값이 같으면 Phase 3 통과한다.


## Phase 4

```text
Dump of assembler code for function phase_4:
   0x000000000040100c <+0>:     sub    $0x18,%rsp
   0x0000000000401010 <+4>:     lea    0xc(%rsp),%rcx
   0x0000000000401015 <+9>:     lea    0x8(%rsp),%rdx
   0x000000000040101a <+14>:    mov    $0x4025cf,%esi // "%d %d"
   0x000000000040101f <+19>:    mov    $0x0,%eax
   0x0000000000401024 <+24>:    call   0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000401029 <+29>:    cmp    $0x2,%eax // input num == 2?
   0x000000000040102c <+32>:    jne    0x401035 <phase_4+41>
   0x000000000040102e <+34>:    cmpl   $0xe,0x8(%rsp)
   0x0000000000401033 <+39>:    jbe    0x40103a <phase_4+46> // 0x8(%rsp) <= 0xe
   0x0000000000401035 <+41>:    call   0x40143a <explode_bomb>
   0x000000000040103a <+46>:    mov    $0xe,%edx // 다음 함수의 세 번째 인자 = 0xe
   0x000000000040103f <+51>:    mov    $0x0,%esi // 다음 함수의 두 번째 인자 = 0
   0x0000000000401044 <+56>:    mov    0x8(%rsp),%edi // 다음 함수의 첫 번째 인자 = 첫 번째 입력값   0x0000000000401048 <+60>:    all   0x400fce <func4>
   0x000000000040104d <+65>:    test   %eax,%eax
   0x000000000040104f <+67>:    jne    0x401058 <phase_4+76>
   0x0000000000401051 <+69>:    cmpl   $0x0,0xc(%rsp)
   0x0000000000401056 <+74>:    je     0x40105d <phase_4+81>
   0x0000000000401058 <+76>:    call   0x40143a <explode_bocmb>
   0x000000000040105d <+81>:    add    $0x18,%rsp
   0x0000000000401061 <+85>:    ret
End of assembler dump.
```

```text
(gdb) disas 0x400fce
Dump of assembler code for function func4:
   0x0000000000400fce <+0>:     sub    $0x8,%rsp
   0x0000000000400fd2 <+4>:     mov    %edx,%eax // %eax == 0xe
   0x0000000000400fd4 <+6>:     sub    %esi,%eax // %eax == 0xe
   0x0000000000400fd6 <+8>:     mov    %eax,%ecx // %ecx == 0xe
   0x0000000000400fd8 <+10>:    shr    $0x1f,%ecx // %ecx >>= 31 == 0
   0x0000000000400fdb <+13>:    add    %ecx,%eax // %eax == 0xe
   0x0000000000400fdd <+15>:    sar    $1,%eax // %eax >>= 1 == 0x7
   0x0000000000400fdf <+17>:    lea    (%rax,%rsi,1),%ecx // %ecx == 0x7
   0x0000000000400fe2 <+20>:    cmp    %edi,%ecx // 첫번째 입력 vs 0x7
   0x0000000000400fe4 <+22>:    jle    0x400ff2 <func4+36> // 0x7 <= 첫번째
   0x0000000000400fe6 <+24>:    lea    -0x1(%rcx),%edx
   0x0000000000400fe9 <+27>:    call   0x400fce <func4>
   0x0000000000400fee <+32>:    add    %eax,%eax
   0x0000000000400ff0 <+34>:    jmp    0x401007 <func4+57>
   0x0000000000400ff2 <+36>:    mov    $0x0,%eax // %eax = 0 <- 반환값
   0x0000000000400ff7 <+41>:    cmp    %edi,%ecx
   0x0000000000400ff9 <+43>:    jge    0x401007 <func4+57> // 0x7 >= 첫번째
   0x0000000000400ffb <+45>:    lea    0x1(%rcx),%esi // if < 7, esi = (%rcx) + 1
   0x0000000000400ffe <+48>:    call   0x400fce <func4>
   0x0000000000401003 <+53>:    lea    0x1(%rax,%rax,1),%eax
   0x0000000000401007 <+57>:    add    $0x8,%rsp
   0x000000000040100b <+61>:    ret
End of assembler dump.
```

* `mov $0xe, %edx` → `func4`의 세 번째 인자로 `14`를 전달한다.

* `mov $0x0, %esi` → `func4`의 두 번째 인자로 `0`을 전달한다.

* `mov 0x8(%rsp), %edi` → 첫 번째 입력값을 `func4`의 첫 번째 인자로 전달한다.

* `call 0x400fce <func4>` → `func4(first_input, 0, 14)`를 호출한다.

* `lea (%rax,%rsi,1), %ecx` → `lower + (upper - lower) / 2`를 계산하여 중앙값을 구한다.

* `add %eax, %eax` → 왼쪽 탐색 결과를 `2 × result`로 변환한다.

* `lea 0x1(%rax,%rax,1), %eax` → 오른쪽 탐색 결과를 `2 × result + 1`로 변환한다.

* `test %eax, %eax` → `func4`의 반환값이 `0`인지 확인한다.

* `cmpl $0x0, 0xc(%rsp)` → 두 번째 입력값과 `0`을 비교한다.

* `je` → 두 번째 입력값이 `0`이면 Phase 4를 통과한다.

* `func4(first_input, 0, 14)`의 반환값이 `0`이 되어야 한다.

* 가능한 첫 번째 입력값: 0, 1, 3, 7

* 두 번째 입력값: 0

* 따라서 가능한 정답은: (0, 0), (1, 0), (3, 0), (7, 0)

### 주요 x86-64 레지스터

### 함수 호출

```text
%rdi → 1번째 인자
%rsi → 2번째 인자
%rdx → 3번째 인자
%rcx → 4번째 인자
```

### 함수 반환

```text
%rax / %eax → 반환값
```

### 실행 및 Stack

```text
%rsp → Stack 위치
%rip → 실행 위치
```

### 암기

```text
인자: RDI → RSI → RDX → RCX
반환: RAX
Stack: RSP
실행: RIP
```


## Phase 5

```text
Dump of assembler code for function phase_5:
   0x0000000000401062 <+0>:     push   %rbx
   0x0000000000401063 <+1>:     sub    $0x20,%rsp
   0x0000000000401067 <+5>:     mov    %rdi,%rbx // %rbx = first input string address
   0x000000000040106a <+8>:     mov    %fs:0x28,%rax
   0x0000000000401073 <+17>:    mov    %rax,0x18(%rsp)
   0x0000000000401078 <+22>:    xor    %eax,%eax // %eax = 0
   0x000000000040107a <+24>:    call   0x40131b <string_length>
   0x000000000040107f <+29>:    cmp    $0x6,%eax // len == 6?
   0x0000000000401082 <+32>:    je     0x4010d2 <phase_5+112>
   0x0000000000401084 <+34>:    call   0x40143a <explode_bomb>
   0x0000000000401089 <+39>:    jmp    0x4010d2 <phase_5+112>
   0x000000000040108b <+41>:    movzbl (%rbx,%rax,1),%ecx // %ecx = input[rax] (zero-extended)
   0x000000000040108f <+45>:    mov    %cl,(%rsp) // stack에 %cl 1바이트 저장
   0x0000000000401092 <+48>:    mov    (%rsp),%rdx // rsp부터 8바이트를 %rdx로 읽음
   0x0000000000401096 <+52>:    and    $0xf,%edx // 하위 4비트만 저장
   0x0000000000401099 <+55>:    movzbl 0x4024b0(%rdx),%edx // %edx = (char)
   0x00000000004010a0 <+62>:    mov    %dl,0x10(%rsp,%rax,1)
   0x00000000004010a4 <+66>:    add    $0x1,%rax // %rax += 1
   0x00000000004010a8 <+70>:    cmp    $0x6,%rax
   0x00000000004010ac <+74>:    jne    0x40108b <phase_5+41>
   0x00000000004010ae <+76>:    movb   $0x0,0x16(%rsp) // 1바이트 0을 저장
   0x00000000004010b3 <+81>:    mov    $0x40245e,%esi // 두번째 인자: 주소값
   0x00000000004010b8 <+86>:    lea    0x10(%rsp),%rdi // %rdi = %rsp + 0x10
   0x00000000004010bd <+91>:    call   0x401338 <strings_not_equal>
   0x00000000004010c2 <+96>:    test   %eax,%eax
   0x00000000004010c4 <+98>:    je     0x4010d9 <phase_5+119>
   0x00000000004010c6 <+100>:   call   0x40143a <explode_bomb>
   0x00000000004010cb <+105>:   nopl   0x0(%rax,%rax,1)
   0x00000000004010d0 <+110>:   jmp    0x4010d9 <phase_5+119>
   0x00000000004010d2 <+112>:   mov    $0x0,%eax
   0x00000000004010d7 <+117>:   jmp    0x40108b <phase_5+41>
   0x00000000004010d9 <+119>:   mov    0x18(%rsp),%rax
   0x00000000004010de <+124>:   xor    %fs:0x28,%rax
   0x00000000004010e7 <+133>:   je     0x4010ee <phase_5+140>
   0x00000000004010e9 <+135>:   call   0x400b30 <__stack_chk_fail@plt>
   0x00000000004010ee <+140>:   add    $0x20,%rsp
   0x00000000004010f2 <+144>:   pop    %rbx
   0x00000000004010f3 <+145>:   ret
End of assembler dump.

(gdb) x/s 0x4024b0
0x4024b0 <array.3449>:  "maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?"

(gdb) x/s 0x40245e
0x40245e:       "flyers"

```


* push %rbx 는 
  sub $8, %rsp // 8바이트
  mov %rbx, (%rsp) 와 같은 동작

* **Segment Register(세그먼트 레지스터)** → 특정 메모리 영역에 접근할 때 기준으로 사용할 수 있는 특수 레지스터. x86-64 Linux에서는 `%fs`가 주로 현재 Thread의 TLS에 접근하는 데 사용된다.

* **TLS(Thread Local Storage)** → 각 Thread가 독립적으로 가지는 전용 데이터 저장 영역.

* `mov %fs:0x28, %rax` → TLS의 `FS base + 0x28` 위치에 있는 값을 `%rax`에 저장한다. Linux x86-64에서는 Stack Canary(실행 중 스택 메모리가 덮어써졌는지 감지하는 보안용 감시 값)를 읽어오는 전형적인 패턴이다.

* xor %eax,%eax → %eax를 0으로 초기화하는 전형적인 어셈블리 패턴.

* xor reg, reg → 효율적인 0 초기화 + Flags 변경
  mov $0, reg → 0 초기화 + 기존 Flags 보존
  => 뒤에서 기존 Condition Code를 계속 사용해야 한다면 mov

* `movzbl (%rbx,%rax,1), %ecx`
  * `mov` → **Move**: 데이터를 이동한다.
  * `z` → **Zero Extend**: 남는 상위 비트를 `0`으로 채운다.
  * `b` → **Byte (8-bit)**: Source에서 1바이트를 읽는다.
  * `l` → **Long (32-bit)**: 32비트로 확장한다.
  * `(%rbx,%rax,1)` → `%rbx + %rax × 1`로 주소를 계산한다.
  * `%ecx` → 읽은 1바이트를 **Zero Extension(零扩展)**하여 32비트로 저장한다.

* %rcx  → 64비트
  %ecx  → 하위 32비트
  %cx   → 하위 16비트
  %cl   → 하위 8비트

* `test %eax, %eax` → `%eax & %eax`를 수행해 **Flag만 설정하고 결과는 저장하지 않는다.** 같은 값을 AND하므로 `%eax`가 `0`인지 검사할 때 주로 사용. `%eax`가 `0`이면 ZF = 1.

* `nopl 0x0(%rax,%rax,1)` → **NOP(No Operation)** 명령어로, 실질적인 연산을 수행하지 않는다.
  * 레지스터와 메모리의 값을 변경하지 않는다.
  * 여기서는 주로 **명령어 주소 정렬(Alignment) 및 Padding**을 위해 삽입된 것으로 볼 수 있다.

* `mov 0x18(%rsp), %rax` → Stack에 저장해둔 **Stack Canary**를 가져온다.
* `xor %fs:0x28, %rax` → 원본 Canary와 비교한다. 같으면 결과가 `0` → `ZF = 1`.
* `je` → Canary가 같으면 정상 종료한다.
* `__stack_chk_fail` → Canary가 다르면 Stack 손상으로 판단한다.
* `add $0x20, %rsp` → 확보했던 32바이트 Stack 공간을 반환한다.
* `pop %rbx` → Stack에 저장했던 기존 `%rbx`를 복구한다.

* 정답:
* 입력은 **6글자**여야 한다.
* 각 입력 문자의 ASCII 값에 `& 0xF` → **하위 4비트만 추출**한다.
* 추출값을 `"maduiersnfotvbyl"`의 인덱스로 사용한다.
* 변환된 6글자가 `"flyers"`와 같으면 통과한다.

`flyers`에 필요한 인덱스:

`f → 9`, `l → F`, `y → E`, `e → 5`, `r → 6`, `s → 7`

→ 필요한 하위 4비트: `9 F E 5 6 7`

예: `ionefg` → `9FE567` → `flyers`

* 하위 4비트만 비교하므로 가능한 입력은 여러 개다.