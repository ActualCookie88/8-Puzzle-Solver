# 8-Puzzle Solver

By Luke Matsunaga

Project completed as part of an Intro to Artificial Intelligence course. 

Focus: Implementing search algorithms for solving the 8-puzzle.

This project implements an 8-puzzle solver in C++ using classical AI search techniques:

- Uniform Cost Search (UCS)
- Misplaced Tile Heuristic (A*)
- Manhattan Distance Heuristic (A*)
  
## Prerequisites
- C++17 compiler (GCC, Clang, or MSVC)
- CMake (version 3.15+)
- Ninja (optional, recommended for fast builds)

## Features
- Predefined puzzles of varying difficulty  
- Custom puzzle input with validation  
- Performance metrics:
  - Nodes expanded  
  - Maximum queue size  
- Comparison of search strategies and heuristics  
- Modular and readable C++ implementation  

## Concepts Covered
- State-space representation  
- Informed vs uninformed search  
- Heuristic design and evaluation  
- Optimal pathfinding and cost analysis

## Interactive Demo
Below is a link to an interactive visual demo of the 8-puzzle.
[Link here](https://ActualCookie88.github.io/8-Puzzle-Solver/docs/visualizer.html)

## Building the Project
1. Clone the repo
```bash
git clone https://github.com/ActualCookie88/8-Puzzle-Solver.git
```
2. Create a build directory
```bash
mkdir build
cd build/
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
./8puzzle        # On Linux/macOS
8puzzle.exe      # On Windows
```

You will be prompted to:

Choose the puzzle type and depth or provide your own (via input)
Choose the algorithm (UCS, Misplaced Tile, Manhattan Distance)
Choose to visualize intermediate steps or solution path

## Output
This program displays:

- Solution path from initial state to goal  
- Total number of moves (solution cost)  
- Number of nodes expanded during the search  
- Maximum queue size reached
- Program runtime

## Report
For detailed results and analysis, see project report:
[View Project Report](docs/8-Puzzle-ProjectReport.pdf)
