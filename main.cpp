#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

/* ////////////////////////////////////////////////////////////////////

SCAN FILE FOR FEATURES/INSTANCES

//////////////////////////////////////////////////////////////////// */

vector<vector<double>> dataSet;
int numFeatures = 0;

// scan file and load data set + number of features
bool scanData(string fileName) {
    ifstream inputFile(fileName);

    // error handling
    if (!inputFile.is_open()) {
        cout << "Error: could not open file.\n";
        return false;
    }

    string line;

    // scan file
    while(getline(inputFile, line)) {
        stringstream ss(line);
        vector<double> instance;
        double value;

        // scan row
        while(ss >> value) {
            instance.push_back(value);
        }

        if(!instance.empty()) {
            dataSet.push_back(instance);
        }
    }

    inputFile.close();

    if(!dataSet.empty()) {
        numFeatures = dataSet[0].size() - 1; // number of columns - class #
    }

    return true;
}

/* ////////////////////////////////////////////////////////////////////

HELPERS

//////////////////////////////////////////////////////////////////// */

// user input checker
int selectOptionHelper(int min, int max) {
    string input;
    int number;
    while(true) {
        cin >> input;
        cout << endl;
        try {
            number = stoi(input); 
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

// Euclidean Distance formula helper
double euclideanDistance(int row1, int row2, const vector<int>& featureSet) {
    double sum = 0.0;

    if(featureSet.empty()) {
        return numeric_limits<double>::infinity();
    }

    // compute sum of squared differences between two points
    for(int feature : featureSet) {
        double difference = dataSet[row1][feature] - dataSet[row2][feature];
        sum += difference * difference;
    }
    // square root of sum above
    return sqrt(sum);
}

// Leave one out cross validation
double leaveOneOut(const vector<int>& featureSet) {
    if(featureSet.empty()) {
        return 0;
    }

    int numCorrectlyClassified = 0;

    // outer loop for each instance
    for(int i = 0; i < dataSet.size(); i++) {
        double labelObjectToClassify = (int)dataSet[i][0];

        double nearestNeighborDist = numeric_limits<double>::infinity();
        int nearestNeighborLoc = -1;
        
        // innter loop to comapare to other instances
        for(int k = 0; k < dataSet.size(); k++) {
            if(k != i) { // dont compare to self
                double distance = euclideanDistance(i, k, featureSet);

                // update nearest neighbor IF closer
                if(distance <= nearestNeighborDist) {
                    nearestNeighborDist = distance;
                    nearestNeighborLoc = k;
                }
            }
        }

        double neighborLabel = (int)dataSet[nearestNeighborLoc][0];
        if(labelObjectToClassify == neighborLabel) numCorrectlyClassified++; // amount of correct classifications
    }
    // accuracy calculation
    return (double)numCorrectlyClassified / dataSet.size();
}

// display purposes
void printSet(const vector<int>& featureSet) {
    cout << "{";
    for(int i = 0; i < featureSet.size(); i++) {
        cout << featureSet[i];
        if(i != featureSet.size() - 1) {
            cout << ",";
        }
    }
    cout << "}";
}

/* ////////////////////////////////////////////////////////////////////

ALGORITHMS

//////////////////////////////////////////////////////////////////// */

// Select one feature at each level
void forwardSelection() {
    vector<int> currFeatureSet;
    vector<int> bestSet;
    double bestAccuracy = 0.0;
    
    // outer loop for walking down search tree
    for(int level = 1; level <= numFeatures; level++) {
        cout << "On level " << level << " of the search tree\n";
        int featureToAdd = -1;
        double bestAccuracySoFar = 0.0;

        // inner loop for considering selecting each feature
        for(int k = 1; k <= numFeatures; k++) {
            if(find(currFeatureSet.begin(), currFeatureSet.end(), k) != currFeatureSet.end()) continue; // skip if k already in set

            // test accuracy of selecting feature temporarily
            vector<int> temp = currFeatureSet;
            temp.push_back(k);

            double accuracy = leaveOneOut(temp);

            cout << "Using feature(s) ";
            printSet(temp);
            cout << " accuracy is " << accuracy * 100 << "%\n";

            // update accuracy and best feature
            if(accuracy > bestAccuracySoFar) {
                bestAccuracySoFar = accuracy;
                featureToAdd = k;
            }
        }
        // adds best feature to set
        currFeatureSet.push_back(featureToAdd);
        
        cout << "Feature set ";
        printSet(currFeatureSet);
        cout << " was best, accuracy is " << bestAccuracySoFar * 100 << "%\n\n";

        // update best overall
        if(bestAccuracySoFar > bestAccuracy) {
            bestAccuracy = bestAccuracySoFar;
            bestSet = currFeatureSet;
        }
    }
    cout << "Finished search. Best feature subset is ";
    printSet(bestSet);
    cout << " with accuracy " << bestAccuracy * 100 << "%\n";
}

// Remove one feature at each level
void backwardElimination() {
    vector<int> currFeatureSet;
    vector<int> bestSet;
    double bestAccuracy = 0.0;

    // start with all features
    for(int i = 1; i <= numFeatures; i++) {
        currFeatureSet.push_back(i);
    }

    bestAccuracy = leaveOneOut(currFeatureSet);
    bestSet = currFeatureSet;

    // outer loop for walking up search tree
    for(int level = numFeatures; level > 1; level--) {
        cout << "On level " << level << " of the search tree\n";
        int featureToRemove = -1;
        double bestAccuracySoFar = 0.0;

        // inner loop for considering removing each feature
        for(int k = 0; k < currFeatureSet.size(); k++) {

            // test accuracy of removing feature temporarily
            vector<int> temp = currFeatureSet;
            temp.erase(temp.begin() + k);

            double accuracy = leaveOneOut(temp);

            cout << "Using feature(s) ";
            printSet(temp);
            cout << " accuracy is " << accuracy * 100 << "%\n";

            // update best accuracy and set
            if(accuracy > bestAccuracySoFar) {
                bestAccuracySoFar = accuracy;
                featureToRemove = currFeatureSet[k];
            }
        }
        // remove best feature from set
        currFeatureSet.erase(remove(currFeatureSet.begin(), currFeatureSet.end(),featureToRemove),currFeatureSet.end());

        cout << "Feature set ";
        printSet(currFeatureSet);
        cout << " was best, accuracy is " << bestAccuracySoFar * 100 << "%\n\n";

        // update best overall
        if(bestAccuracySoFar > bestAccuracy) {
            bestAccuracy = bestAccuracySoFar;
            bestSet = currFeatureSet;
        }
    }

    cout << "Finished search. Best feature subset is ";
    printSet(bestSet);
    cout << " with accuracy " << bestAccuracy * 100 << "%\n";
}

/* ////////////////////////////////////////////////////////////////////

MAIN

//////////////////////////////////////////////////////////////////// */

int main() {
    cout << "Feature Selection Algorithm" << endl << endl;

    string fileName;
    
    cout << "Enter dataset filename: ";
    cin >> fileName;
    cout << endl;

    if(!scanData(fileName)) {
        return 1;
    }
        
    cout << "This dataset has " << numFeatures << " features with " << dataSet.size() << " instances.\n";

    cout << "Choose algorithm: \n"
         << "(1) Forward Selection\n"
         << "(2) Backward Elimination\n";
    int choice = selectOptionHelper(1,2);
    
    if(choice == 1) {
        forwardSelection();
    }
    else {
        backwardElimination();
    }

    return 0;
}