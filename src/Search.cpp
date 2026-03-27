#include "../header/Search.hpp"
#include "../header/Puzzle.hpp"
#include "../header/Heuristics.hpp"
#include "../header/globals.hpp"
#include <queue>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <algorithm>

using namespace::std;

// input validation helper
char getYesNo(const std::string &prompt) {
    char input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // normalize input to lowercase
        input = std::tolower(input);

        if (input == 'y' || input == 'n') {
            return input;
        } else {
            std::cout << "INVALID INPUT. Enter 'y' or 'n'.\n";
        }
    }
}

// main search algorithm
SearchResult runSearch(const vector<vector<int>>& puzzle_, int algorithm, bool printSteps, bool printSolution) {
    vector<string> algs = {
        "UNIFORM COST SEARCH",
        "the MISPLACED TILE HEURISTIC",
        "the MANHATTAN DISTANCE HEURISTIC"
    };
    priority_queue<Node> pq;
    unordered_map<string, int> bestGn;
    unordered_map<string, string> parentMap; // child state string -> parent state string

    // initial state/node, establish gn and hn
    Node init;
    init.puzzle = puzzle_;
    init.gn = 0;

    if(algorithm == 1) init.hn = uniformHeuristic(puzzle_);
    else if(algorithm == 2) init.hn = misplacedHeuristic(puzzle_);
    else if(algorithm == 3) init.hn = manhattanHeuristic(puzzle_);
    else return SearchResult{"ERROR", -1, -1, -1, false};

    string initStr = puzzleToString(puzzle_);
    pq.push(init);
    parentMap[initStr] = "";

    int numExpanded = 0;
    int queueSize = 1;

    // start loop
    while(!pq.empty()) {
        queueSize = max(queueSize, (int)pq.size());
        Node curr = pq.top();
        pq.pop();

        // print only for algorithms 1-3
        if(printSteps) {
            cout << "The best state to expand with g(n) = " << curr.gn 
                << " and h(n) = " << curr.hn << " is:\n";
            displayPuzzle(curr.puzzle, 2);
            cout << "Expanding this node...\n\n";
        }

        // convert 2D puzzle to string for hashing
        string s = puzzleToString(curr.puzzle);
        
        // skip if we already reached this state at a lower cost
        if(bestGn.find(s) != bestGn.end() && bestGn[s] <= curr.gn) continue;
        bestGn[s] = curr.gn;

        // check if puzzle is solved
        if (curr.puzzle == solution) {
            // reconstruct solution path by following parentMap back to root
            vector<vector<vector<int>>> path;
            string k = s;
            while (!k.empty()) {
                // rebuild 2D state from 9-char string
                vector<vector<int>> state(3, vector<int>(3));
                for (int i = 0; i < 9; i++)
                    state[i / 3][i % 3] = k[i] - '0';
                path.push_back(state);
                k = parentMap[k];
            }
            reverse(path.begin(), path.end());
 
            // print solution path if requested
            if (printSolution) {
                border();
                cout << "====== SOLUTION PATH ======\n\n";
                for (int i = 0; i < (int)path.size(); i++) {
                    cout << "Step " << i << ":\n";
                    displayPuzzle(path[i], 1);
                }
            }
 
            return SearchResult{algs[algorithm - 1], curr.gn, numExpanded, queueSize, true, path};
        }
        
        // continue expanding
        numExpanded++;
        vector<vector<vector<int>>> children = expand(curr.puzzle);

        // loop over every child
        for(auto &childPuzzle : children) {
            int newGn = curr.gn + 1;
            string st = puzzleToString(childPuzzle);
            if(bestGn.count(st) && bestGn[st] <= newGn) continue;

            // record parent only if this is the first/better time we see this state
            if (!parentMap.count(st)) {
                parentMap[st] = s;
            }

            Node child;
            child.puzzle = childPuzzle;
            child.gn = newGn;

            if(algorithm == 1) child.hn = uniformHeuristic(childPuzzle);
            else if(algorithm == 2) child.hn = misplacedHeuristic(childPuzzle);
            else if(algorithm == 3) child.hn = manhattanHeuristic(childPuzzle);

            pq.push(child);
        }
    }
    // no solution found
    return SearchResult{algs[algorithm-1], -1, numExpanded, queueSize, false, {}};
}

// general search interface
void generalSearch(const vector<vector<int>>& puzzle_, int algorithm, bool runAll) {
    // run all algorithms and print results
    if(runAll) {
        vector<SearchResult> results;
        for(int alg = 1; alg <= 3; alg++) {
            auto res = runSearch(puzzle_, alg, false, false);
            results.push_back(res);
        }

        cout << "====== SEARCH RESULTS ======\n";
        for(auto &res : results) {
            cout << "Goal state reached! Used " << res.name << ":\n";
            if(res.success) {
                cout << "  Solution Depth: " << res.solutionDepth << "\n"
                     << "  Nodes Expanded: " << res.nodesExpanded << "\n"
                     << "  Max Queue Size: " << res.maxQueueSize << "\n";
            } 
            else {
                cout << "  FAILED TO FIND SOLUTION!\n";
            }
            cout << endl;
        }
        return;
    }

    // run one algorithm and print results
    char choice, choice2;
    choice = getYesNo("Print intermediate steps? (y/n): ");
    choice2 = getYesNo("Print solution path? (y/n): ");

    bool printSteps = (choice == 'y');
    bool printSolution = (choice2 == 'y');
    SearchResult res = runSearch(puzzle_, algorithm, printSteps, printSolution);

    border();
    cout << "====== SEARCH RESULTS ======\n";
    cout << "Goal state reached! Used " << res.name << ":\n";
    if(res.success) {
        cout << "  Solution Depth: " << res.solutionDepth << "\n"
             << "  Nodes Expanded: " << res.nodesExpanded << "\n"
             << "  Max Queue Size: " << res.maxQueueSize << "\n";
    } 
    else {
        cout << "  FAILED TO FIND SOLUTION!\n";
    }
    cout << endl;
}