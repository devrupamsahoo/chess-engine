# Chess Engine

A C++20 chess application with a shared chess core and two frontends: a terminal interface and an optional Qt desktop GUI. The core owns the board state, validates and applies moves, and manages the current game session. Frontends handle user input and display the session state.

## Current Features

- Board representation using 12 piece bitboards and cached occupancy bitboards
- Legal move validation for pawns, knights, bishops, rooks, queens, and kings
- King-safety checks, castling, en passant, and pawn promotion
- Undo and redo, including special moves
- FEN serialization, position loading, and per-match FEN history
- Terminal and Qt desktop interfaces using the shared `GameSession`
- Desktop board with piece graphics, coordinate labels, click-to-move and drag-and-drop input
- Desktop promotion choice, check indicator, captured-piece display, and controls to undo, redo, reset, load FEN, copy the current FEN, and export FEN history

The game does not yet announce checkmate, stalemate, or draws. These are planned after move generation is implemented.

## Terminal Interface

Enter moves using the starting and ending coordinates, for example:

```text
e2e4
```

For promotion, append `q`, `r`, `b`, or `n` to choose a queen, rook, bishop, or knight. For example, `e7e8n` promotes to a knight. If the promotion letter is omitted, the pawn promotes to a queen. Move input and commands are converted to lowercase, so uppercase input is accepted too.

| Input | Action |
| --- | --- |
| `u` | Undo the last move |
| `r` | Redo the last undone move |
| `x` | Restart from the standard starting position |
| `f` | Load a position from a FEN string |
| `e` | Export the match's FEN snapshots to `match_fens.txt` |
| `q` | Quit the game |

When loading a position, enter its complete six-field FEN string at the prompt. A successful load starts a new match history from that position. The terminal export writes one FEN position per line to `match_fens.txt` in the project directory.

## Qt Desktop Interface

The desktop interface is optional and requires Qt 6 Widgets and Qt 6 Svg development packages to be installed and discoverable by CMake. It provides a graphical board and controls for promotion, undo, redo, reset, FEN loading, copying the current FEN, and exporting match FEN history. The export dialog lets you choose the destination file.

## Project Structure

- `include/chess/` contains public core, move-validation, history, and notation headers
- `src/` contains the shared chess core implementation
- `applications/terminal/` contains the terminal frontend
- `applications/desktop/` contains the Qt desktop frontend
- `assets/` contains desktop piece graphics and Qt resource definitions

`ChessBoard` stores the piece bitboards, occupancy, side to move, castling rights, en passant target, move counters, and move history. `GameSession` provides the frontend-facing interface for making moves, undoing and redoing moves, loading FEN positions, and reading the current game state. The move-validation modules check piece movement, paths, attacks, castling, and king safety. `FEN` handles position serialization and loading.

## Planned Features

- Move generation
- Checkmate, stalemate, and draw detection after move generation
- A basic AI opponent using minimax, followed by alpha-beta pruning

## Build and Run

### Terminal application

Configure and build the terminal application from the project directory:

```bash
cmake -S . -B build
cmake --build build --target chess_engine
```

Run the `chess_engine` executable from the build directory. Its exact location depends on the operating system and build configuration. For a single-configuration build on Linux or macOS, run:

```bash
./build/chess_engine
```

### Qt desktop application

Configure a build with the desktop target enabled, then build it:

```bash
cmake -S . -B build-qt -DBUILD_QT_GUI=ON
cmake --build build-qt --target chess_desktop
```

Launch the resulting `chess_desktop` executable from the build directory. Its exact path depends on the operating system and build configuration.

## Tech Stack

- C++20
- CMake
- Qt 6 Widgets and Svg for the optional desktop frontend
- Standard Library containers and utilities
- Bitboards for board representation

## Author

Rupam Sahoo
