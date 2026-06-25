# ♟️ Chess Engine (Terminal-Based C++ Project)

A terminal-based Chess Engine built from scratch in C++ focusing on game logic, board representation, and step-by-step system design.

This project is being developed as an evolving system rather than a finished game — starting from core board mechanics and gradually expanding toward full chess rule validation and AI-based gameplay.

---

## 🎯 Project Objective

The main goal of this project is to understand and implement how a chess engine works internally by building it incrementally.

Instead of focusing on graphics or UI, the emphasis is on:

- Board state management
- Move input parsing
- Coordinate conversion logic
- Object-oriented design principles
- Step-by-step rule enforcement

---

## 🧩 Current Features

- Chess board representation using array of 64-bit integers
- Letter-based visual board display in terminal
- Standard chess initial setup (all pieces placed correctly)
- Coordinate system conversion (e.g., `e2 → matrix indices`)
- Basic move input system (`from → to`)
- Turn-based structure (White / Black alternating)
- Simple piece ownership validation

---

## 🏗️ Architecture Overview

The project is structured around a core `ChessBoard` class which manages:

- Board state (`array<uint64_t, pieceCount>`)
- Piece initialization
- Move execution
- Board display logic

Supporting logic includes:
- Input validation functions
- Coordinate mapping utilities

The design follows a modular approach where future features like move validation, check detection, and AI will be added as separate logical layers.

---

## 🚀 Planned Features (Roadmap)

The project is actively evolving. Upcoming improvements include:

- Piece-specific move validation (Pawn, Rook, Knight, Bishop, Queen, King)
- Path obstruction logic for sliding pieces
- Check and checkmate detection
- Stalemate and draw conditions
- Move history tracking and undo system
- Special moves:
    - Castling
    - En passant
    - Pawn promotion
- Basic AI opponent using Minimax algorithm
- Alpha-beta pruning optimization for AI efficiency
- Optional graphical interface in later stage

---

## 🛠️ Tech Stack

- C++
- STL (Bitboards, Strings, Utility)
- CMake
- Terminal-based UI (Capital & Small letters)

---

## 📝 Author
**Rupam Sahoo**

---

## ▶️ How to Run

### Compile
```bash
g++ main.cpp -o chess
