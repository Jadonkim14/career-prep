# Computer Systems

> Started: September 3, 2026
> Resource: CS:APP 3e + CMU 15-213

## Goal

Understand how C programs are compiled, executed, and managed by the computer system, connecting **C → Machine → Memory → OS**.

## Topics

* Data Representation
* Machine-Level Programming
* Processor Architecture
* Linking
* Memory Hierarchy / Cache
* Virtual Memory
* Dynamic Memory Allocation
* Processes / Signals
* System-Level I/O
* Networking
* Concurrency / Synchronization

## 15-213 Roadmap

### Phase 1 — Computer Systems & Data Representation

Ch.1–2

* Computer System Overview
* Bits / Bytes
* Integer Representation
* Floating Point
* Endianness
* Data Representation

### Phase 2 — Machine-Level Programming

Ch.3

* x86-64 Assembly
* Registers
* Memory Access
* Control Flow
* Functions / Calling Convention
* Stack / Stack Frame
* Arrays / Pointers
* Struct / Union
* Buffer Overflow
* GDB

→ **Data Lab / Bomb Lab**

### Phase 3 — Processor Architecture & Linking

Ch.4 + Ch.7

* ISA
* Datapath / Control
* Pipelining
* Pipeline Hazards
* Object Files
* Symbols
* Relocation
* Static / Dynamic Linking
* Shared Libraries

→ Machine / Build experiments

### Phase 4 — Memory System

Ch.6 + Ch.9

* Memory Hierarchy
* Locality
* Cache
* Cache Organization
* Virtual Address
* Page Table
* TLB
* Page Fault
* Memory Protection
* Heap / Allocator
* `malloc` / `free`

→ **Cache Lab / Malloc Lab**

### Phase 5 — Processes & System I/O

Ch.8 + Ch.10

* Process
* User / Kernel Mode
* Context Switching
* `fork` / `exec`
* Signals
* File Descriptor
* Unix I/O
* System Calls
* Files / Directories
* I/O Redirection

→ **Shell Lab**

### Phase 6 — Performance & Networking

Ch.5 + Ch.11

* Code Optimization
* CPU Performance
* Instruction-Level Parallelism
* Network / Protocol
* IP / TCP
* Sockets
* Client / Server
* HTTP

→ **Proxy Lab (optional)**

### Phase 7 — Concurrent Programming

Ch.12

* Processes / Threads
* Shared Memory
* Synchronization
* Semaphore
* Race Condition
* Thread Safety
* Deadlock
* Thread-Level Parallelism

→ Concurrency experiments

## Labs

**Core**

* Data Lab
* Bomb Lab
* Cache Lab
* Malloc Lab
* Shell Lab

**Recommended**

* Attack Lab
* SFS Lab

**Optional**

* Proxy Lab

## Key Takeaway

Focus on understanding how a **C program becomes machine code and executes through CPU, memory, and OS**, connecting the concepts into one system.