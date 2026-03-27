#include "header/Search.hpp"
#include "header/Puzzle.hpp"
#include "header/Heuristics.hpp"
#include "header/globals.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

/* ////////////////////////////////////////////////////////////////////

HELPERS

//////////////////////////////////////////////////////////////////// */

// user input checker
int selectOptionHelper(int min, int max) {
    string input;
    int number;
    while(true) {
        cin >> input;
        try {
            number = stoi(input); // if fails, throws exception to catch
        } catch(...) {
            cout << "INVALID OPTION. TRY AGAIN: ";
            continue;
        }
        if(number >= min && number <= max) {
            return number;
        } else {
            cout << "INVALID OPTION. TRY AGAIN: ";
        }
    }
}

// depth input checker
vector<vector<int>> depthSelectHelper(int choice) {
    vector<int> depthVals = {0,2,4,6,8,10,12,14,16,18,20,22,24};

    while(true) {
        auto it = find(depthVals.begin(), depthVals.end(), choice);
        if(it != depthVals.end()){
            int index = distance(depthVals.begin(), it);
            return depths[index];
        } else {
            cout << "INVALID DEPTH. Enter an even number between 0 and 24: ";
            choice = selectOptionHelper(0, 24); // reuse input checker
        }
    }
}

// handles custom puzzle input
void inputCustomPuzzle(vector<vector<int>>& puzzle) {
    unordered_set<int> seen;
    cout << "Enter your custom puzzle row by row (use 0 for empty, numbers 1-8, separated by spaces, no duplicates):\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for(int i = 0; i < 3; i++) {
        while(true) {
            cout << "Enter row " << i+1 << ": ";
            string line;
            getline(cin, line);
            stringstream ss(line);
            vector<int> row;
            int tile;
            bool valid = true;

            while(ss >> tile) {
                // check valid number
                if(tile < 0 || tile > 8) {
                    cout << "INVALID: Numbers must be between 0 and 8.\n";
                    valid = false;
                    break;
                }
                // check duplicates
                if(seen.count(tile)) {
                    cout << "INVALID: Tile " << tile << " already used.\n";
                    valid = false;
                    break;
                }
                row.push_back(tile);
            }

            if(!valid) continue;

            // check row length
            if(row.size() != 3) {
                cout << "INVALID: Must enter exactly 3 numbers per row.\n";
                continue;
            }

            // all good, add tiles
            for(int j = 0; j < 3; j++) {
                puzzle[i][j] = row[j];
                seen.insert(row[j]);
            }
            break;
        }
    }
}

/* ////////////////////////////////////////////////////////////////////

MAIN

//////////////////////////////////////////////////////////////////// */

int main() {
    std::srand(std::time(nullptr));
    border();
    cout << "8-Puzzle Solver\n\n";
    
    // initial user prompt
    cout << "SELECT OPTION: \n" <<
            "(1) Default Puzzle\n" <<
            "(2) Custom Puzzle\n" <<
            "(3) Random Puzzle\n";
    int choice = selectOptionHelper(1, 3);
    vector<vector<int>> puzzle(3, vector<int>(3));

    if(choice == 1) { // default puzzle
        cout << "\nENTER DEPTH OF PUZZLE (0-24, even numbers only): ";
        puzzle = depthSelectHelper(selectOptionHelper(0, 24));
    }
    else if(choice == 2) { // custom puzzle
        inputCustomPuzzle(puzzle);
    }
    else { // random puzzle
        puzzle = generateRandomPuzzle();
    }

    if(!isSolvable(puzzle)) { // safety check
        cout << "\nThis puzzle is impossible to solve!\n";
        return 0;
    }

    border();

    cout << "Selected puzzle:\n";
    displayPuzzle(puzzle, 1);

    // algorithm choice prompt
    cout << "SELECT ALGORITHM: \n" <<
            "(1) Uniform Cost Search\n" <<
            "(2) Misplaced Tile Heuristic (A*)\n" <<
            "(3) Manhattan Distance Heuristic (A*)\n" <<
            "(4) ALL OF THEM!\n";
    choice = selectOptionHelper(1, 4);
    
    border();

    auto start = chrono::high_resolution_clock::now();

    if(choice == 4) {
        generalSearch(puzzle, choice, true);
    }
    else {
        generalSearch(puzzle, choice, false);
    }
    
    auto end = chrono::high_resolution_clock::now();

    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << fixed;
    if(duration_ms < 1000) {
        cout << "Execution Time: " << duration_ms << " miliseconds\n";
    } else {
        double duration_s = duration_ms / 1000.0;
        cout << fixed << setprecision(2);
        cout << "Execution Time: " << duration_s << " seconds\n";
    }

    return 0;
}

