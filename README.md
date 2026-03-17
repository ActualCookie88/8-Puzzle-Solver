# 8-Puzzle Solver

Project completed as part of an algorithms course. 
Focus: Implementing search algorithms for solving the 8-puzzle.

This project implements an 8-puzzle solver in C++ using:

- Uniform Cost Search (UCS)
- Misplaced Tile Heuristic (A*)
- Manhattan Distance Heuristic (A*)
  
## Prerequisites

- C++17 compiler (GCC, Clang, or MSVC)
- CMake (version 3.15+)
- Ninja (optional, recommended for fast builds)

## Features
- Default puzzles of various depths
- Custom puzzles with input validation
- Node expansion statistics (nodes expanded, max queue size)
- Modular, readable code structure

## Building the Project
1. Clone the repository
```bash
git clone https://github.com/ActualCookie88/8_Puzzle_Solver.git
```
2. Create a build directory
```bash
mkdir build
cd build
```
3. Configure CMake to use Ninja
```bash
cmake -G Ninja ..
```
This generates all build files inside the build/ directory.

4. Build the project
```bash
ninja
```

## Running the Program
```bash
./puzzle        # On Linux/macOS
puzzle.exe      # On Windows
```

## Report
For detailed results and analysis, see project report:
[View Project Report](8-Puzzle-ProjectReport.pdf)
