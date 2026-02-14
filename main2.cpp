#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

vector<vector<int>> solution = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};
unordered_map<int, pair<int,int>> solPositions = { // each individual position
    {1,{0,0}}, {2,{0,1}}, {3,{0,2}},
    {4,{1,0}}, {5,{1,1}}, {6,{1,2}},
    {7,{2,0}}, {8,{2,1}}
};

vector<vector<vector<int>>> depths = {
{   // depth 0
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
}, { // depth 2
    {1, 2, 3},
    {4, 5, 6},
    {0, 7, 8}
}, { // depth 4
    {1, 2, 3},
    {5, 0, 6},
    {4, 7, 8}
}, { // depth 8
    {1, 3, 6},
    {5, 0, 2},
    {4, 7, 8}
}, { // depth 12
    {1, 3, 6},
    {5, 0, 7},
    {4, 8, 2}
}, { // depth 16
    {1, 6, 7},
    {5, 0, 3},
    {4, 8, 2}
}, { // depth 20
    {7, 1, 2},
    {4, 8, 5},
    {6, 3, 0}
}, { // depth 24
    {0, 7, 2},
    {4, 6, 1},
    {3, 5, 8}
},
};


// node structure for states
struct Node {
    vector<vector<int>> puzzle;
    int gn; // cost of start -> finish (or current)
    int hn; // heuristic cost

    int fn() const { // A* : f = g + h
        return gn + hn;
    } 

    bool operator<(const Node& other) const{ // for min-heap behavior
        return fn() > other.fn();  
    }
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
    int dist = 0;

    // loop thru each position
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
void displayPuzzle(const vector<vector<int>>& puzzle, int type) {
    if(type == 1) {
        cout << "[" << puzzle[0][0] << " " << puzzle[0][1] << " " << puzzle[0][2] << "]" << endl <<
            "[" << puzzle[1][0] << " " << puzzle[1][1] << " " << puzzle[1][2] << "]" << endl <<
            "[" << puzzle[2][0] << " " << puzzle[2][1] << " " << puzzle[2][2] << "]" << endl << endl;
    }
    else if(type == 2) {
        cout << "   [" << puzzle[0][0] << " " << puzzle[0][1] << " " << puzzle[0][2] << "]" << endl <<
            "   [" << puzzle[1][0] << " " << puzzle[1][1] << " " << puzzle[1][2] << "]" << endl <<
            "   [" << puzzle[2][0] << " " << puzzle[2][1] << " " << puzzle[2][2] << "]" << endl << endl;
    }
    
}

// for terminal display purposes
void border() {
    cout << endl;
    cout << "//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    cout << endl;
}

// user input checker
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

// generate all possible states
vector<vector<vector<int>>> expand(const vector<vector<int>>& puzzle) {
    vector<vector<vector<int>>> children; // list of 2D puzzles 
    int emptyRow = -1;
    int emptyCol = -1;

    // coordinates for empty space
    bool found = false;
    for(int row = 0; row < 3; row++) { 
        for(int col = 0; col < 3; col++) {
            if(puzzle[row][col] == 0) {
                emptyRow = row;
                emptyCol = col;
                found = true;
            }
        }
    }

    // directions = {up, left, down, right}
    int delRow[] = {-1, 0, 1, 0};
    int delCol[] = {0, -1, 0, 1};

    // check for possible moves
    for(int k = 0; k < 4; k++) {
        int row = emptyRow + delRow[k];
        int col = emptyCol + delCol[k];

        // within puzzle bounds
        if(row >= 0 && row < 3 && col >= 0 && col < 3) {
            vector<vector<int>> newPuzzle = puzzle;

            // move tile to blank (swap empty and adjacent tile)
            swap(newPuzzle[emptyRow][emptyCol], newPuzzle[row][col]);

            // append new state
            children.push_back(newPuzzle);
        }
    }

    return children;
}

/* ////////////////////////////////////////////////////////////////////

SOLVER

//////////////////////////////////////////////////////////////////// */

void generalSearch(const vector<vector<int>>& puzzle_, int algorithm) {
    // establish pqueue and visited set
    priority_queue<Node> pq;
    unordered_map<string, int> bestGn;
    vector<string> algs = {"UNIFORM COST SEARCH", "MISPLACED TILE HEURISTIC", "MANHATTAN DISTANCE HEURISTIC"};

    // initial state/node, establish gn and hn
    Node init;
    init.puzzle = puzzle_;
    init.gn = 0;

    if(algorithm == 1) {
        init.hn = uniformHeuristic(puzzle_);
    }
    else if(algorithm == 2) {
        init.hn = misplacedHeuristic(puzzle_);
    } 
    else if(algorithm == 3) {
        init.hn = manhattanHeuristic(puzzle_);
    }
    else {
        cout << "ERROR";
        return;
    }

    pq.push(init);

    int numExpanded = 0;
    int queueSize = 1;

    // start loop
    while(!pq.empty()) {
        int pqsize = pq.size();
        queueSize = max(queueSize, pqsize);

        Node curr = pq.top();
        pq.pop();

        cout << "Expanding best node: g(n)=" << curr.gn << 
                "   h(n)=" << curr.hn << 
                "   f(n)=" << curr.fn() << endl;
        displayPuzzle(curr.puzzle, 1);

        // convert 2D puzzle to string for hashing
        string s;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                s += to_string(curr.puzzle[i][j]);
            }
        }
        
        // if current state not visited yet, expand
        if(bestGn.count(s))
            continue;

        bestGn[s] = curr.gn;

        // check if puzzle is solved
        if(curr.puzzle == solution) {
            cout << "PUZZLE SOLVED USING " << algs[algorithm-1] << "!" << endl <<
                    "Solution Depth: " << curr.gn << endl <<
                    "Nodes Expanded: " << numExpanded << endl <<
                    "Max Queue size: " << queueSize << endl;
            return;
        }
        
        // continue expanding
        numExpanded++;
        
        vector<vector<vector<int>>> children = expand(curr.puzzle);

        // loop over every state
        for(int i = 0; i < children.size(); i++) {
            vector<vector<int>> childPuzzle = children[i];

            int newGn = curr.gn + 1;

            // convert 2D puzzle to string for hashing
            string st;
            for(int j = 0; j < 3; j++) {
                for(int k = 0; k < 3; k++) {
                    st += to_string(childPuzzle[j][k]);
                }
            }

            // A*, keep best path
            if(bestGn.count(st))
                continue;
            // child state/node, establish gn and hn
            Node child;
            child.puzzle = childPuzzle;
            child.gn = newGn;

            if(algorithm == 1) {
                child.hn = uniformHeuristic(childPuzzle);
            }
            else if(algorithm == 2) {
                child.hn = misplacedHeuristic(childPuzzle);
            } 
            else if(algorithm == 3) {
                child.hn = manhattanHeuristic(childPuzzle);
            }
            else {
                cout << "ERROR";
                return;
            }
            pq.push(child);
        }
        //cout << "------------------------------------------" << endl;
    }
    // all states have been visited
    cout << endl;
    cout << "FAILED TO FIND SOLUTION!" << endl;
}

/* ////////////////////////////////////////////////////////////////////

TEST BENCH FOR ME

//////////////////////////////////////////////////////////////////// */

void testbench() {
    border();
    cout << "WELCOME TO TEST BENCH" << endl << endl;

    vector<vector<vector<int>>> depthMap = depths;
    vector<int> depthVals = {0, 2, 4, 8, 12, 16, 20, 24};

    while(true) {
        cout << endl;
        cout << "Testing depths..." << endl;
        cout << "ENTER -1 TO CANCEL" << endl;
        cout << "Select Depth (0, 2, 4, 8, 12, 16, 20, 24): ";
        int choice = selectOptionHelper(-1, 24);
        if(choice == -1) break;

        // find index of selected depth
        auto it = find(depthVals.begin(), depthVals.end(), choice);
        if(it == depthVals.end()) {
            cout << "Invalid depth selection." << endl;
            continue;
        }
        int index = distance(depthVals.begin(), it);

        if(choice <= 8) {
            // for easy depths, run all algorithms
            cout << "Depth " << choice << ":" << endl;
            generalSearch(depthMap[index], 1);
            generalSearch(depthMap[index], 2);
            generalSearch(depthMap[index], 3);
        }
        else {
            // for hard depths, let user select algorithm
            cout << "SELECT ALGORITHM: " << endl
                 << "(1) Uniform Cost Search" << endl
                 << "(2) Misplaced Tile Heuristic (A*)" << endl
                 << "(3) Manhattan Distance Heuristic (A*)";
            int choice1 = selectOptionHelper(1, 3);
            cout << "Depth " << choice << ":" << endl;
            generalSearch(depthMap[index], choice1);
        }
    }
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
    displayPuzzle(puzzle, 1);

    // algorithm choice prompt
    cout << "SELECT ALGORITHM: " << endl <<
            "(1) Uniform Cost Search" << endl <<
            "(2) Misplaced Tile Heuristic (A*)" << endl <<
            "(3) Manhattan Distance Heuristic (A*)" << endl;
    choice = selectOptionHelper(1, 3);
    
    border();

    generalSearch(puzzle, choice);

    return 0;
}

