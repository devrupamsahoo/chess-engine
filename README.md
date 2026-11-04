# ♟️ Chess Engine (Terminal-Based C++ Project)

A terminal chess game written in C++ with a bitboard-based board representation and separate modules for move validation, attack detection, and board display.

## 🎯 Project Objective

This project explores how chess rules and board state can be represented and implemented in C++. It currently provides a playable terminal game with move history and FEN position support.

## 🧩 Current Features

- Board representation using 12 piece bitboards and cached occupancy bitboards
- Coordinate-based move input, such as `e2e4`
- Move validation for pawns, knights, bishops, rooks, queens, and kings
- Sliding-piece path checks and protection against moves that leave the moving side's king in check
- Castling, en passant, and pawn promotion
- Check detection
- Undo and redo for moves, including special moves
- FEN serialization and position loading
- FEN snapshots for each position in the current match, with export to a text file
- Game reset and quit commands

## 🎮 Playing the Game

Enter moves using the starting and ending coordinates, for example:

```text
e2e4
```

For promotion, append `q`, `r`, `b`, or `n` to choose a queen, rook, bishop, or knight. For example, `e7e8n` promotes to a knight. If the promotion letter is omitted, the pawn promotes to a queen. Move input and commands are converted to lowercase, so uppercase input is accepted too.

The game displays the available commands while running:

| Input | Action |
| --- | --- |
| `u` | Undo the last move |
| `r` | Redo the last undone move |
| `x` | Restart from the standard starting position |
| `f` | Load a position from a FEN string |
| `e` | Export the current match's FEN snapshots to `match_fens.txt` |
| `q` | Quit the game |

When loading a position, enter its complete six-field FEN string at the prompt. A successful load starts a new match history from that position. Export writes the recorded positions in order, one FEN string per line, to `match_fens.txt` in the program's current working directory.

## 🏗️ Architecture Overview

`ChessBoard` stores the piece bitboards, occupancy, side to move, castling rights, en passant target, move counters, and move history. `MoveValidator` coordinates piece-specific movement checks, path checks, and king-safety checks. `AttackValidator` uses precomputed attack tables for leapers and pawns, and ray scans for sliding attacks. `CastlingValidator` checks castling rights, required pieces, clear paths, and attacked squares. `FEN.cpp` implements board serialization and FEN loading, while `DisplayBoard` handles terminal board rendering. `GameManager` handles player input, commands, turns, and match FEN snapshots.

## 🚀 Planned Features

- Checkmate, stalemate, and draw detection
- A graphical interface with visually distinct chess pieces
- A basic AI opponent using minimax, followed by alpha-beta pruning

## 🛠️ Tech Stack

- C++20
- CMake
- Standard Library containers and utilities
- Bitboards for board representation

## 📝 Author

**Rupam Sahoo**

## ▶️ How to Build and Run

From the project directory, configure and build with CMake:

```bash
cmake -S . -B build
cmake --build build
```

Run the `chess_engine` executable produced in the `build` directory. Its exact path depends on the operating system and build configuration.
