맞아. **그렇게 분리하는 게 더 깔끔해.**

루트 `README.md`는 프로젝트 전체를 설명하는 문서니까 **현재 V0.1의 구체적인 탐지 규칙과 한계까지 길게 적을 필요는 없어.** 그런 내용은 `V0.1` 문서에서 관리하는 게 맞아.

### 프로젝트 `README.md`

남길 내용은 이 정도면 충분해.

````markdown
# LoL Interactive Post-Game Review Tool

## Overview

LoL Interactive Post-Game Review Tool is a project for analyzing League of Legends matches and helping players quickly review the moments that influenced the outcome of a game.

The long-term goal is to build an interactive post-game review system that can detect critical moments, explain win/loss factors, connect important events to replay timestamps, and support natural-language queries.

The current version, **V0.1**, implements a C-based CLI event engine that reads simulated match events from a file, stores them dynamically, calculates basic statistics, and detects simple critical moments.

## Current Features

- Parse match events from a text file
- Store events using dynamic memory allocation
- Calculate basic team statistics
- Query events within a specific time range
- Detect simple Critical Moments
- Accept match files through command-line arguments

## Project Structure

```text
lol-post-game-tool/
├── lolreview.c
├── match.txt
└── README.md
````

## Build & Run

Compile:

```bash
gcc -Wall -Wextra -pedantic -o lolreview lolreview.c
```

Run:

```bash
./lolreview match.txt
```

If no match file is provided:

```text
Usage: ./lolreview <match_file>
```

If the file cannot be opened:

```text
Failed to open file: abc.txt
```

## Example Output

```text
=== Match Summary ===
Kills: 3 - 2
Dragons: 1 - 2

=== Events 25:00 ~ 26:00 ===
25:11 RED DEATH
25:20 RED DEATH
25:32 BLUE BARON

Critical Moment #1
25:11 ~ 25:32
2 Deaths -> BARON Lost
```

## Roadmap

V0.1 — C-based Event Engine
V0.2 — Improved Match Analysis
V0.3 — Real Match Data Integration
V0.4 — Replay Control
V0.5 — Post-Game Review UI
V0.6 — Natural-Language Queries
V1.0 — Interactive Post-Game Review