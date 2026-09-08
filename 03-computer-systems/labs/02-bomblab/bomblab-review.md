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