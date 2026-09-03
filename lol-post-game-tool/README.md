# LoL Interactive Post-Game Review Tool

## Overview

A C-based CLI tool for analyzing League of Legends match events and identifying critical moments.

The project is currently **paused**. It may be resumed later for fun.

## Current Features

* Parse match events from a text file
* Dynamic event storage
* Basic match statistics
* Time-range event queries
* Critical Moment detection
* Binary event serialization/deserialization
* Enum, fixed-width integer, bit flag, and function pointer usage

## Project Structure

```text
lol-post-game-tool/

├── main.c
├── event.c / event.h
├── analysis.c / analysis.h
├── output.c / output.h
├── match.txt
└── README.md
```

## Build & Run

```bash
gcc -Wall -Wextra -pedantic -o event_engine *.c
./event_engine match.txt
```

## Status

* V0.1 — C-based Event Engine ✅
* V0.2 — Improved Match Analysis ⏸️ Paused
* V0.3 — Real Match Data Integration
* V0.4 — Replay Control
* V0.5 — Post-Game Review UI
* V0.6 — Natural-Language Queries
* V1.0 — Interactive Post-Game Review

Further development is paused because LLM forcing newly learned concepts into the project was not an effective learning approach.