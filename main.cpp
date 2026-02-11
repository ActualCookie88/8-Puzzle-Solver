#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

vector<vector<int>> solution = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

/* ////////////////////////////////////////////////////////////////////

HEURISTICS

//////////////////////////////////////////////////////////////////// */

// h(n) = 0
int uniformHeuristic(const vector<vector<int>>& puzzle) {
    return 0;
}

// h(n) = # misplaced tiles
int misplacedHeuristic(const vector<vector<int>>& puzzle) {
    int count = 0;

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(puzzle[i][j] != solution[i][j] && puzzle[i][j] != 0) { // ignore empty
                count++;
            }
        }
    }
    return count;
}

// h(n) = sum of tile distances
int manhattanHeuristic(const vector<vector<int>>& puzzle) {
    unordered_map<int, pair<int,int>> solPositions = { // each individual positions
        {1,{0,0}}, {2,{0,1}}, {3,{0,2}},
        {4,{1,0}}, {5,{1,1}}, {6,{1,2}},
        {7,{2,0}}, {8,{2,1}}
    };
    int dist = 0;

    for(int i = 0; i < 3; i++) { 
        for(int j = 0; j < 3; j++) {
            int position = puzzle[i][j];
            if(position != 0) { // ignore empty
                dist += abs(i - solPositions[position].first) + abs(j - solPositions[position].second);
            }
        }
    }

    return dist;
}

/* ////////////////////////////////////////////////////////////////////

HELPERS

//////////////////////////////////////////////////////////////////// */

// prints puzzle 
void displayPuzzle(const vector<vector<int>>& puzzle) {
    cout << "[" << puzzle[0][0] << " " << puzzle[0][1] << " " << puzzle[0][2] << "]" << endl <<
            "[" << puzzle[1][0] << " " << puzzle[1][1] << " " << puzzle[1][2] << "]" << endl <<
            "[" << puzzle[2][0] << " " << puzzle[2][1] << " " << puzzle[2][2] << "]" << endl << endl;
}

void border() {
    cout << endl;
    cout << "//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    cout << endl;
}

int selectOptionHelper(int min, int max) {
    string input;
    int number;
    while(true) {
        cin >> input;
        cout << endl;

        if(input == "CANCEL") {
            return -1;
        }

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

/* ////////////////////////////////////////////////////////////////////

TEST BENCH FOR ME

//////////////////////////////////////////////////////////////////// */

void testbench() {
    border();
    cout << "WELCOME TO TEST BENCH" << endl << endl;
    vector<vector<int>> puzzle(3, vector<int>(3));

    puzzle = {
            {8, 6, 7},
            {2, 5, 4},
            {3, 0, 1}
    };
    /* Test sets to use
        {3, 2, 8},
        {4, 5, 6},
        {7, 1, 0}
        h(n) = 0, 3, 8

        {8, 6, 7},
        {2, 5, 4},
        {3, 0, 1}
        h(n) = 0, 8, 21
    */

    cout << "Selected puzzle: " << endl;
    displayPuzzle(puzzle);
    cout << "Solution puzzle: " << endl;
    displayPuzzle(solution);

    cout << "Testing Heuristics..." << endl;

    cout << "Uniform Cost Heuristic: " << uniformHeuristic(puzzle) << endl;

    cout << "Misplaced Tile Heuristic: " << misplacedHeuristic(puzzle) << endl;

    cout << "Manhattan Distance Heuristic: " << manhattanHeuristic(puzzle) << endl;

    
}

/* ////////////////////////////////////////////////////////////////////

MAIN

//////////////////////////////////////////////////////////////////// */

int main() {
    border();
    cout << "8-Puzzle Solver" << endl << endl;
    
    // initial user prompt
    cout << "SELECT OPTION: " << endl <<
            "(1) Default Puzzle" << endl <<
            "(2) Custom Puzzle" << endl;
    int choice = selectOptionHelper(1, 3);
    vector<vector<int>> puzzle(3, vector<int>(3));

    if(choice == 1) { // default puzzle
        puzzle = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 0, 8}
        };
    }
    else if(choice == 2) { // custom puzzle
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                cout << "Enter tile Number (Row " << i + 1 << ", Column " << j + 1 << ")" << ": ";
                cin >> puzzle[i][j];
            }
        }
    }
    else if(choice == 3) { // for me
        testbench(); 
        return 0;
    }
    else {
        cout << "ERROR";
    }

    border();

    cout << "Selected puzzle: " << endl;
    displayPuzzle(puzzle);

    // algorithm choice prompt
    cout << "SELECT ALGORITHM: " << endl <<
            "(1) Uniform Cost Search" << endl <<
            "(2) Misplaced Tile Heuristic (A*)" << endl <<
            "(3) Manhattan Distance Heuristic (A*)" << endl;
    choice = selectOptionHelper(1, 3);

    if(choice == 1) { // Uniform Cost Search
        
    }
    else if(choice == 2) { // Misplaced Tile Heuristic (A*)
        
    }
    else if(choice == 3) { // Manhattan Distance Heuristic (A*)

    }
    else {
        cout << "ERROR";
    }

    return 0;
}

